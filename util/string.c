#include "string.h"

#define OK 0
#define ERROR 1

/* Returnerer lengden på en string, eksluderende nullterminatoren */
int strlen(char str[]){
  int i = 0;
  while(str[i] != '\0') i++;
  return i;
}

/* Sammenligner to stringer med hverandre. Om de er like returnerer den 0, hvis ikke: 1 */
int strcmp(char str1[], char str2[]){
  int i = 0;
  while(1){
    if(str1[i] != str2[i]) return ERROR;
    if(str1[i] == '\0' && str2[i] == '\0') return OK;
    i++;
  }
}

/* Kopierer en string inn til en annen. Returnerer dersom stringen er større enn bufferet */
int strcpy(char str[], char* buffer){
  if(strlen(str) >= sizeof(buffer)) return ERROR;
  int i = 0;
  while(str[i] != '\0'){
    buffer[i] = str[i];
    i++;
  }
  str[i] = '\0';
  return OK;
}

/* Tar en liten bokstav og returnerer den store versjonen av den.
  Hvis noe annet enn en bokstav sendes, retrurneres det tegnet. */
  char toupper(char c){
    if(c < 'a' || c > 'z') return c;
    return (c - 32);
  }
  
  /* Tar en stor bokstav og returnerer den lille versjonen av den
    Hvis noe annet enn en bokstav sendes, retrurneres det tegnet. */
  char tolower(char c){
    if(c < 'A' || c > 'Z') return c;
    return (c + 32);
  }
  

/* Legger til et tegn i et buffer */
void append(char str[], char c){
  int len = strlen(str);
  str[len] = c;
  str[len+1] = '\0';
}

/* Fjerner det siste tegnet i et buffer */
void pop(char str[]){
  int len = strlen(str);
  str[len-1] = '\0';
}

/* Klargjører et buffer */
void purge(char str[]){
  int len = strlen(str), i = 0;
  while(i < len){
    str[i] = '\0';
    i++;
  }
}