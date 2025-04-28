#ifndef KERNEL
#define KERNEL

#include "../drivers/screen.h"

#define PANIC(msg)                  \
    do{                             \
        kprint("KERNEL PANIKK: ");  \
        kprint(msg);                \
        kprint("\n");               \
        kprint("HALTET SYSTEMET");  \
        asm volatile("cli");        \
        asm volatile("hlt");        \
        while(1);                   \
    } while(0);                     \
    

void get_input(char[]);

#endif