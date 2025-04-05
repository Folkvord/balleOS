#include "idt.h"
#include "../util/types.h"

void set_idt_entry(int n, u32 offset){

  idt[n].isr_low = LOWER_16(offset);
  idt[n].kernel_cs = KERNEL_CS;
  idt[n].reserved = 0;
  idt[n].attributes = 0x8e;
  idt[n].isr_high = UPPER_16(offset);

}

void set_idt(){

  idtr_reg.base = (u32) &idt;
  idtr_reg.limit = IDT_ENTRIES * sizeof(idt_entry);
  __asm__ __volatile__("lidtl (%0)" :  : "r" (&idtr_reg));

}