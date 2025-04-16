#include "mem.h"

/* Kopierer n antall bytes fra src til dst */
void memcpy(char *src, char *dst, int n){

    int bytes = 0;
    while(bytes < n){
      *(dst + bytes) = *(src + bytes);
      bytes++;
    }
  
  }