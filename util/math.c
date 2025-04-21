#include "math.h"

/* Power: base opphøyd i exp */
int pow(int base, int exp){
  
  if(exp == 0) return 1;

  int n = base;
  int i = 0;
  while(i < exp){
    n *= base;
    i++;
  }
  
  return n;
  
  }  