#include "../drivers/screen.h"
#include "../drivers/ports.h"
#include "../util/types.h"
#include "../util/util.h"
#include "../cpu/isr.h"
#include "keyboard.h"

void handle_scancode(u8);
char scancode_to_ascii(u8);
char scancode_to_big_ascii(u8);
char print_key(u8);
void test_scancode(u8);

int caps_on = 0;
int shift_pressed = 0;

const unsigned char normal_keys[] = {
  0,   0,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '\\', '\b',         // Reserved, ESC, 1... -> back
'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', '^', '\n',               // tab, q... -> å, enter
  0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'o', 'e', '|',  0,                 // lctrl, a... -> æ, lshift
'\'', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-',  0,  '*',  0, ' ', 0,         // ', z... -> -, rshift, lmenu, caps
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                                                  // F-knappene
};

//char caps_keys[128] = {
//                    0,   0,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '\\', '\b',
//                  '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', '¨', '\n',
//                    0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'O', 'E', '|',  0,
//                  '\'', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '-',  0,  '*',  0, ' ', 0,
//                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                                                  
//                  };
//
//char shift_keys[128] = {
//                    0,   0,  '!', '"', '#', '¤', '%', '&', '/', '(', ')', '=', '?', '`', '\b', 
//                  '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', '^', '\n',
//                    0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'O', 'E', '§',  0,
//                   '*', 'Z', 'X', 'C', 'V', 'B', 'N', 'm', ';', ':', '_',  0,  '*',  0, 0, ' ', 0,
//                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//                  };


void keyboard_callback(registers r){
  
  u8 scancode = port_byte_in(0x60);
  handle_scancode(scancode);

}

void init_keyboard(){

  register_interrupt_handler(IRQ1, keyboard_callback);

}

void handle_scancode(u8 scancode){

  if(scancode & 0x80) return;

  char c = normal_keys[scancode];
  char str[2];
  str[0] = c;
  str[1] = '\0';
  kprint(str);
  
}

char scancode_to_ascii(u8 scancode) {
  if(scancode & 0x80) return 0;
  switch (scancode) {
    case 0x01: return 27;        // ESC
    case 0x02: return '1';
    case 0x03: return '2';
    case 0x04: return '3';
    case 0x05: return '4';
    case 0x06: return '5';
    case 0x07: return '6';
    case 0x08: return '7';
    case 0x09: return '8';
    case 0x0A: return '9';
    case 0x0B: return '0';
    case 0x0C: return '+';
    case 0x0D: return '\\';      // Backslash on Norwegian layout
    case 0x0E: return '\b';      // Backspace
    case 0x0F: return '\t';      // Tab
    case 0x10: return 'q';
    case 0x11: return 'w';
    case 0x12: return 'e';
    case 0x13: return 'r';
    case 0x14: return 't';
    case 0x15: return 'y';
    case 0x16: return 'u';
    case 0x17: return 'i';
    case 0x18: return 'o';
    case 0x19: return 'p';
    case 0x1A: return 'a';       // å 
    case 0x1B: return '^';       // Norwegian "dead key"
    case 0x1C: return '\n';      // Enter
    case 0x1E: return 'a';
    case 0x1F: return 's';
    case 0x20: return 'd';
    case 0x21: return 'f';
    case 0x22: return 'g';
    case 0x23: return 'h';
    case 0x24: return 'j';
    case 0x25: return 'k';
    case 0x26: return 'l';
    case 0x27: return 'o';       // ø
    case 0x28: return 'a';       // æ
    case 0x29: return '`';       // Norwegian ¨/backtick
    case 0x2C: return 'z';
    case 0x2D: return 'x';
    case 0x2E: return 'c';
    case 0x2F: return 'v';
    case 0x30: return 'b';
    case 0x31: return 'n';
    case 0x32: return 'm';
    case 0x33: return ',';
    case 0x34: return '.';
    case 0x35: return '-';
    case 0x39: return ' ';       // Spacebar
  
    // Funksjonstaster og spesialtaster (kan utvides med egne koder)
    case 0x3B: return 0;         // F1
    case 0x3C: return 0;         // F2
    case 0x3D: return 0;         // F3
    case 0x3E: return 0;         // F4
    case 0x3F: return 0;         // F5
    case 0x40: return 0;         // F6
    case 0x41: return 0;         // F7
    case 0x42: return 0;         // F8
    case 0x43: return 0;         // F9
    case 0x44: return 0;         // F10
    case 0x57: return 0;         // F11
    case 0x58: return 0;         // F12

    return 0;

  }
}

char scancode_to_big_ascii(u8 scancode) {
  if(scancode & 0x80) return 0;
  switch (scancode) {
    case 0x01: return 27;        // ESC
    case 0x02: return '1';
    case 0x03: return '2';
    case 0x04: return '3';
    case 0x05: return '4';
    case 0x06: return '5';
    case 0x07: return '6';
    case 0x08: return '7';
    case 0x09: return '8';
    case 0x0A: return '9';
    case 0x0B: return '0';
    case 0x0C: return '+';
    case 0x0D: return '\\';      // Backslash on Norwegian layout
    case 0x0E: return '\b';      // Backspace
    case 0x0F: return '\t';      // Tab
    case 0x10: return 'Q';
    case 0x11: return 'W';
    case 0x12: return 'E';
    case 0x13: return 'R';
    case 0x14: return 'T';
    case 0x15: return 'Y';
    case 0x16: return 'U';
    case 0x17: return 'I';
    case 0x18: return 'O';
    case 0x19: return 'P';
    case 0x1A: return 'A';       // å 
    case 0x1B: return '^';       // Norwegian "dead key"
    case 0x1C: return '\n';      // Enter
    case 0x1E: return 'A';
    case 0x1F: return 'S';
    case 0x20: return 'D';
    case 0x21: return 'F';
    case 0x22: return 'G';
    case 0x23: return 'H';
    case 0x24: return 'J';
    case 0x25: return 'K';
    case 0x26: return 'L';
    case 0x27: return 'O';       // ø
    case 0x28: return 'A';       // æ
    case 0x29: return '`';       // Norwegian ¨/backtick
    case 0x2C: return 'Z';
    case 0x2D: return 'X';
    case 0x2E: return 'C';
    case 0x2F: return 'V';
    case 0x30: return 'B';
    case 0x31: return 'N';
    case 0x32: return 'M';
    case 0x33: return ',';
    case 0x34: return '.';
    case 0x35: return '-';
    case 0x39: return ' ';       // Spacebar
  
    // Funksjonstaster og spesialtaster (kan utvides med egne koder)
    case 0x3B: return 0;         // F1
    case 0x3C: return 0;         // F2
    case 0x3D: return 0;         // F3
    case 0x3E: return 0;         // F4
    case 0x3F: return 0;         // F5
    case 0x40: return 0;         // F6
    case 0x41: return 0;         // F7
    case 0x42: return 0;         // F8
    case 0x43: return 0;         // F9
    case 0x44: return 0;         // F10
    case 0x57: return 0;         // F11
    case 0x58: return 0;         // F12

    return 0;

  }
}
