#include "ports.h"

/* Leser en byte fra I/O-porten lagret i dx og returnerer det */
unsigned char port_byte_in(unsigned short port){
  unsigned char result;

  __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
 
  return result;

}

/* Sender en byte til I/O-porten gitt */
void port_byte_out(unsigned short port, unsigned char data){

  __asm__("out %%al, %%dx" : : "a"(data), "d"(port));

}

/* Leser et dataord (2 bytes) fra I/O-porten gitt i data */
unsigned short port_word_in(unsigned short port){
  unsigned short result;

  __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));

  return result;

}

/* Sender en dataord til I/O-porten gitt */
void port_word_out(unsigned short port, unsigned short data){

  __asm__("out %%al, %%dx" :  : "a"(data), "d"(port));

}