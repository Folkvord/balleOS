#include "type.h"
#include "mem.h"

/* Kopierer n antall bytes fra src til dst */
void memcpy(u8 *src, u8 *dst, u32 n){

    int bytes = 0;
    while(bytes < n){
      *(dst + bytes) = *(src + bytes);
      bytes++;
    }
  
}

void memset(u8 *start_addr, u8 val, u32 len){

  int bytes = 0;
  while(bytes < len){
    *(start_addr + bytes) = val;
    bytes++;
  }

}

int malloc(u32 size){
  
   

}

int bolloc(u32 size){
  malloc(size);
}