#include "../drivers/screen.h"
#include "../drivers/ports.h"
#include "../util/types.h"
#include "../cpu/isr.h"
#include "keyboard.h"

char scancode_to_ascii(u8);

void keyboard_callback(registers r){
  
  u8 scancode = port_byte_in(0x60);
  char c = scancode_to_ascii(scancode);
  char str[2];
  str[0] = c;
  str[1] = '\0';
  kprint(str);
  
}

void init_keyboard(){

  register_interrupt_handler(IRQ1, keyboard_callback);

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

    default:
      
      return 0;  // Ukjent tast / ignorer
}
}
