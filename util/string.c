#include "string.h"

/* strlen: Returnerer lengden på en string, eksluderende nullterminatoren */
int strlen(char* str){
  int i = 0;
  while(str[i] != '\0') i++;
  return i;
}

/* toupper: Tar en liten bokstav og returnerer den store versjonen av den.
  Hvis noe annet enn en bokstav sendes, retrurneres det tegnet. */
char toupper(char c){
  if(c < 'a' || c > 'z') return c;
  return (c - 32);
}

/* tolower: Tar en stor bokstav og returnerer den lille versjonen av den
  Hvis noe annet enn en bokstav sendes, retrurneres det tegnet. */
char tolower(char c){
  if(c < 'A' || c > 'Z') return c;
  return (c + 32);
}

/* strcmp: Sammenligner to stringer med hverandre. Om de er like returnerer den 0, hvis ikke: 1 */
int strcmp(char* str1, char* str2){
  int i = 0;
  while(1){
    if(str1[i] != str2[i]) return 1;
    if(str1[i] == '\0' && str2[i] == '\0') return 0;
    i++;
  }
}

/* append: Legger til et tegn i et buffer */
void append(char* str, char c){
  int len = strlen(str);
  str[len] = c;
  str[len+1] = '\0';
}

/* pop: Fjerner det siste tegnet i et buffer */
void pop(char* str){
  int len = strlen(str);
  str[len-1] = '\0';
}

/* purge: Klargjører et buffer */
void purge(char* str){
  int len = strlen(str), i = 0;
  while(i < len){
    str[i] = '\0';
    i++;
  }
}