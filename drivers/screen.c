#include "screen.h"
#include "ports.h"


int print_char(char c, char attrib, int offset);
void set_cursor_offset(int offset);
unsigned int get_cursor_offset();
int get_offset_x(int offset);
int get_offset_y(int offset);
int get_offset(int x, int y);
void scroll(int rows);

/*** Private variabler ***/

char current_fg = DEFAULT_ATTRIBUTE & 0x0f;   // Fontfarge
char current_bg = DEFAULT_ATTRIBUTE & 0xf0;   // Bakgrunnsfarge

char line_cache[CAHCE_SIZE][VIDEO_WIDTH*2];


/*********************************************
 * Offentlige kernel API funksjoner          *
 *********************************************/

/* BalleOS sin kprint */
void kprint(char* str){

  kprint_at(str, -1, -1);

}

/* Printer en string fra og med VGA-x og -y */
void kprint_at(char* str, int x, int y){
  
  // Starts offset
  int offset = (x >= 0 | y >= 0) ? get_offset(x, y) : get_cursor_offset();

  int i = 0;
  while(str[i] != '\0'){
    offset = print_char(str[i], current_fg | current_bg, offset);
    i++;
  }

  set_cursor_offset(offset);

}

/* Fyller skjermen med en farge */
void fill_screen(char fill){

  char* vga = (char*)VIDEO_ADDRESS;
  int offset = 0;

  int i = 0;
  while(i < SCREEN_SIZE){

    *(vga + offset) = ' ';
    *(vga + offset+1) = fill;
    offset += 2;
    i++;

  }

}

/* Klargjør skjermen */
void clear_screen(){

  fill_screen(FG_BLACK | BG_BLACK);
  
}

/* Endrer fontfargen */
void change_fg(char fg){
  current_fg = fg;
}

/* Endrer bakgrunnsfargen */
void change_bg(char bg){
  current_bg = bg;
}

/* Skroller n antall rader ned */
void scroll(int n){

  int bytes = VIDEO_WIDTH * 2;
  int lines_scrolled = 0;
  while(lines_scrolled < n){

    int lines_moved = 0;
    
    while(lines_moved < VIDEO_HEIGHT){

      char* src = (char*) (VIDEO_ADDRESS + (VIDEO_WIDTH * 2) * (lines_moved+1));
      char* dst = (char*) (VIDEO_ADDRESS + (VIDEO_WIDTH * 2) * lines_moved);
          
      memcpy(src, dst, bytes);
      lines_moved++;

    }

    int i = 0;
    while(i < VIDEO_WIDTH-1){
      *(char*)(VIDEO_ADDRESS + (VIDEO_WIDTH * VIDEO_HEIGHT * 2)) = ' ';
      *(char*)(VIDEO_ADDRESS + (VIDEO_WIDTH * VIDEO_HEIGHT * 2) + 1) = current_fg | current_bg;
    }

    lines_scrolled++;

  } 

}

/* Fyller en linje med en rødfarge (for debugging) */
void fill_line(int y){

  int x = 0, offset = 0;
  while(x < VIDEO_WIDTH){
    *(char*)(VIDEO_ADDRESS + VIDEO_WIDTH * 2 * y + offset) = ' ';
    *(char*)(VIDEO_ADDRESS + VIDEO_WIDTH * 2 * y + 1 + offset) = BG_RED;
    offset += 2;
    x++;
  }

}


/*********************************************
 * Private kernel funksjoner                 *
 *********************************************/

/* Printer ut en char og oppdaterer offsettet */
int print_char(char c, char attrib, int offset){

  if(offset > SCREEN_SIZE){

  }

  if(c == '\n'){
    int y = get_offset_y(offset);
    offset = get_offset(0, y+1);
  }
  else if(c == '\t'){
    int y = get_offset_y(offset);
    int x = get_offset_x(offset);
    offset = get_offset(x + 4, y);
  }
  else{
    *(char*)(VIDEO_ADDRESS + offset) = c;
    *(char*)(VIDEO_ADDRESS + offset + 1) = attrib;
    offset += 2;
  }

  return offset;

}


 /* Henter offsettet til markøren */
unsigned int get_cursor_offset(){

  port_byte_out(SCREEN_CTRL_REG, 14);
  int offset = port_byte_in(SCREEN_DATA_REG);
  offset = offset << 8;

  port_byte_out(SCREEN_CTRL_REG, 15),
  offset += port_byte_in(SCREEN_DATA_REG);

  return offset * 2;

}

/* Lagrer markørpossisjonen */
void set_cursor_offset(int offset){   

  offset /= 2;
  port_byte_out(SCREEN_CTRL_REG, 14);                       /* Spør om tilgang til byte 14 */
  port_byte_out(SCREEN_DATA_REG, (offset >> 8) & 0xff);   /* Sett verdien til byte 14 til den øvre byten av posisjonen */

  port_byte_out(SCREEN_CTRL_REG, 15);                       /* Spør om tilgang til byte 15 */
  port_byte_out(SCREEN_DATA_REG, offset & 0xff);          /* Sett verdien til byten til den nedre byten */

  return;

}

/* Henter offsettet basert på koordinatene */
int get_offset(int x, int y){
  return 2 * (VIDEO_WIDTH * y + x);
}

/* Henter x-koordinatet til et offset */
int get_offset_x(int offset){
  return (offset - (get_offset_y(offset) * 2 * VIDEO_WIDTH)) / 2;
}

/* Henter y-koordinatet til et offset */
int get_offset_y(int offset){
  return offset / (2 * VIDEO_WIDTH);
}