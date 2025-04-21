#ifndef IDT_H
#define IDT_H

#include "../util/type.h"

#define IDT_ENTRIES 256
#define KERNEL_CS 0x08

typedef struct{
  u16 isr_low;     // Nedre 16 bits av ISR adressen
  u16 kernel_cs;   // GDT
  u8  reserved;    // Nullpeker
  u8  attributes;  // Typer og attributer
  u16 isr_high;    // Øvre 16 bits av ISR adressen
} __attribute__((packed)) idt_entry;

typedef struct{
  u16 limit;
  u32 base;
} __attribute__((packed)) idtr;

static idtr idtr_reg;
__attribute__((aligned(0x10)))
static idt_entry idt[256];

void set_idt_entry(int n, u32 offset);
void set_idt();

#endif