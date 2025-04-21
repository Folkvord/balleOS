#ifndef STRING_H
#define STRING_H

int strlen(char* str);
char toupper(char c);
char tolower(char c);
int strcmp(char* str1, char* str2);
void append(char* buffer, char c);
void pop(char* buffer);
void purge(char* buffer);

#endif