#ifndef SCREEN_DRIVER
#define SCREEN_DRIVER

#define VIDEO_ADDRESS 0xb8000
#define VIDEO_WIDTH  80
#define VIDEO_HEIGHT 25
#define SCREEN_SIZE VIDEO_HEIGHT*VIDEO_WIDTH

/* Registere */
#define SCREEN_CTRL_REG 0x3d4
#define SCREEN_DATA_REG 0x3d5

/* Farger */
#define FG_WHITE         0x0f
#define BG_WHITE         0xf0
#define FG_YELLOW        0x0e
#define BG_YELLOW        0xe0
#define FG_LIGHT_MAGENTA 0x0d
#define BG_LIGHT_MAGENTA 0xd0
#define FG_LIGHT_RED     0x0c
#define BG_LIGHT_RED     0xc0
#define FG_LIGHT_CYAN    0x0b
#define BG_LIGHT_CYAN    0xb0
#define FG_LIGHT_GREEN   0x0a
#define BG_LIGHT_GREEN   0xa0
#define FG_LIGHT_BLUE    0x09
#define BG_LIGHT_BLUE    0x90
#define FG_DARK_GRAY     0x08
#define BG_DARK_GRAY     0x80
#define FG_GRAY          0x07
#define BG_GRAY          0x70
#define FG_BROWN         0x06
#define BG_BROWN         0x60
#define FG_MAGENTA       0x05
#define BG_MAGENTA       0x50
#define FG_RED           0x04
#define BG_RED           0x40
#define FG_CYAN          0x03
#define BG_CYAN          0x30
#define FG_GREEN         0x02
#define BG_GREEN         0x20
#define FG_BLUE          0x01
#define BG_BLUE          0x10
#define FG_BLACK         0x00
#define BG_BLACK         0x00

#define CAHCE_SIZE 100
#define TAB_SIZE 4

#define DEFAULT_ATTRIBUTE (FG_GRAY | BG_BLACK)

void fill_screen(char fill);
void clear_screen();
void kprint_at(char* str, int x, int y);
void kprint(char* str);
void change_fg(char fg);
void change_bg(char bg);
void scroll(int n);
void fill_line(int y);

#endif