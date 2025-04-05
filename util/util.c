#include "util.h"
#include "../drivers/screen.h"

/* Kopierer n antall bytes fra src til dst */
void memcpy(char *src, char *dst, int n){

  int bytes = 0;
  while(bytes < n){
    *(dst + bytes) = *(src + bytes);
    bytes++;
  }

}

/* Oversetter et heltall (int) til ASCII */
void itoa(int n, char str[]){

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
