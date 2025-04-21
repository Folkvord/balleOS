#include "util.h"
#include "type.h"
#include "string.h"
#include "../drivers/screen.h"


/* Oversetter et heltall (int) til ASCII */
void itoa(int n, char* str){

  // Sjekker om tallet er negativt
  int sign = n < 0;
  if(sign){
    n = -n;
  }

  // Oversett ett og ett tall
  int i = 0;
  char rev_str[sizeof(str) + sign];    // rev_str er lik størrelsen til str pluss et tegn til om tallet er negativt for å få plass til minustegn
  do{
    rev_str[i++] = (n % 10) + '0';
  } while((n /= 10) > 0);

  // Legg til minustegn hvis tallet var negativt
  if(sign){
    rev_str[i] = '-';
  }

  // Reversjer
  int j = 0, max_index = i+sign;
  while(j < max_index){
    str[j++] = rev_str[max_index - j];
  }

}

/* Oversetter et tall på stringformat til en int */
int atoi(char* str){

  int i = 0;
  int sign = 1;  
  if(str[i] == '-'){
    sign = -1;
    i++;
  }

  int n = 0;
  while(str[i] != '\0'){
    if(str[i] < '0' || str[i] > '9') return 0;
    n = 10 * n + (str[i++] - '0');
  }

  return n * sign;

}

void hex_str(int n, char* store){

  store[0] = '\0';
  append(store, '0');
  append(store, 'x');

  u32 tmp;
  int i = 28;
  int leading_zero = 1;
  while(i > 0){

    tmp = (n >> i) & 0xf;
    if(tmp == 0 && leading_zero){
      i -= 4;
      continue;
    }

    leading_zero = 0;
    if(tmp <= 9){
      append(store, (char) tmp + '0');
    }
    else{
      append(store, (char) tmp - 10 + 'A');
    }

    i -= 4;

  }

  tmp = n & 0xf;
  if(tmp <= 9){
    append(store, (char) tmp + '0');
  }
  else{
    append(store, (char) tmp - 10 + 'A');
  }

}