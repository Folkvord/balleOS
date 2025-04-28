#include "type.h"
#include "mem.h"


/* Kopierer n antall bytes fra src til dst */
void memcpy(u32 *src, u32 *dst, u32 n){

    int bytes = 0;
    while(bytes < n){
      *(dst + bytes) = *(src + bytes);
      bytes++;
    }
  
}

void memset(u32 *start_addr, u32 val, u32 len){

  int bytes = 0;
  while(bytes < len){
    *(start_addr + bytes) = val;
    bytes++;
  }

}

