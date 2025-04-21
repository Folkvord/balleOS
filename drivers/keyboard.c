#include "../drivers/screen.h"
#include "../kernel/kernel.h"
#include "../drivers/port.h"
#include "../util/string.h"
#include "../util/type.h"
#include "../util/util.h"
#include "../cpu/isr.h"
#include "keyboard.h"

#define KEYS_DEFINED 71
#define BACKSPACE   0x0e
#define ESC         0x01
#define ENTER       0x1c
#define SHIFT_P     0x2a
#define SHIFT_R     0x8a
#define CAPS        0x3a
#define F1          0x3b
#define F2          0x3c
#define F3          0x3d
#define F4          0x3e
#define F5          0x3f
#define F6          0x40
#define F7          0x41
#define F8          0x42
#define F9          0x43
#define F10         0x44
#define F11         0x57
#define F12         0x58

int caps_on = 0;
int shift_pressed = 0;

/* Inputbuffer */
unsigned char in_buffer[256];

/* Tegn sortert etter scankodene til det norske tastatur */
const char normal_keys[] = {
  0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '\\', '\b',         // Reserved, ESC, 1... -> back
'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', '^', '\n',               // tab, q... -> å, enter
  0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'o', 'e', '|',  0,                 // lctrl, a... -> æ, lshift
'\'', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-',  0,  '*',  0, ' ', 0,         // ', z... -> -, rshift, lmenu, caps
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                                                  // F-knappene
};

const char shift_keys[128] = {
  0,   27, '!', '"', '#', '$', '%', '&', '/', '(', ')', '=', '?', '`', '\b', 
'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', '^', '\n',
  0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'O', 'E', '|',  0,
'*', 'Z', 'X', 'C', 'V', 'B', 'N', 'm', ';', ':', '_',  0,  '*',  0, 0, ' ', 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


void keyboard_callback(registers r){
  
  u8 scancode = port_byte_in(0x60);
  if(scancode >= KEYS_DEFINED) return;

  // Sjekk spessielle knapper
  if(scancode == SHIFT_P){
    shift_pressed = 1;
  }
  else if(scancode == SHIFT_R){
    shift_pressed = 0;
  }
  else if(scancode == CAPS){
    caps_on = !caps_on;
  }
  else if(scancode == BACKSPACE){
    pop(in_buffer);
    char* str = "\b";   // Midlertidig
    kprint(str);        // 
    return;
  }
  else if(scancode == ESC){
    // Gjør noe ig
  }
  else if(scancode == ENTER){
    get_input(in_buffer);
    purge(in_buffer);
    return;
  }

  if(scancode & 0x80) return;

  char key = shift_pressed ? shift_keys[scancode] : normal_keys[scancode];
  if(key != 0){
    append(in_buffer, key);
    char str[2];    //
    str[0] = key;   //  Midlertidig for å se hva som skrives
    str[1] = '\0';  //
    kprint(str);    //
  }


}

void init_keyboard(){

  register_interrupt_handler(IRQ1, keyboard_callback);

}
