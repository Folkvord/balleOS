#include "ports.h"
#include "../util/types.h"

/* Leser en byte fra I/O-porten lagret i dx og returnerer det */
u8 port_byte_in(u16 port){
  
  u8 result;
  __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
 
  return result;

}

/* Sender en byte til I/O-porten gitt */
void port_byte_out(u16 port, u8 data){

  __asm__("out %%al, %%dx" : : "a"(data), "d"(port));

}

/* Leser et dataord (2 bytes) fra I/O-porten gitt i data */
u16 port_word_in(u16 port){

  u16 result;
  __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));

  return result;

}

/* Sender en dataord til I/O-porten gitt */
void port_word_out(u16 port, u16 data){

  __asm__("out %%al, %%dx" :  : "a"(data), "d"(port));

}