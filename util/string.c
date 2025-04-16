#include "string.h"

int strlen(char* str){
  int i = 0;
  while(str[i] != '\0') i++;
  return i;
}

char toupper(char c){
  if(c < 'a' || c > 'z') return c;
  return (c - 32);
}

char tolower(char c){
  if(c < 'A' || c > 'Z') return c;
  return (c + 32);
}

int strcmp(char* str1, char* str2){
  int i = 0;
  while(1){
    if(str1[i] != str2[i]) return 1;
    if(str1[i] == '\0' && str2[i] == '\0') return 0;
    i++;
  }
}