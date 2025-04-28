#ifndef STRING_H
#define STRING_H

int strlen(char str[]);
int strcmp(char str1[], char str2[]);
int strcpy(char str[], char* buffer);
char toupper(char c);
char tolower(char c);
void append(char buffer[], char c);
void pop(char buffer[]);
void purge(char buffer[]);

#endif