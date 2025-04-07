#include "isr.h"
#include "idt.h"
#include "../util/util.h"
#include "../util/types.h"
#include "../drivers/ports.h"
#include "../drivers/screen.h"

isr interrupt_handlers[256];

void isr_install(){

  set_idt_entry(0, (u32)isr0);
  set_idt_entry(1, (u32)isr1);
  set_idt_entry(2, (u32)isr2);
  set_idt_entry(3, (u32)isr3);
  set_idt_entry(4, (u32)isr4);
  set_idt_entry(5, (u32)isr5);
  set_idt_entry(6, (u32)isr6);
  set_idt_entry(7, (u32)isr7);
  set_idt_entry(8, (u32)isr8);
  set_idt_entry(9, (u32)isr9);
  set_idt_entry(10, (u32)isr10);
  set_idt_entry(11, (u32)isr11);
  set_idt_entry(12, (u32)isr12);
  set_idt_entry(13, (u32)isr13);
  set_idt_entry(14, (u32)isr14);
  set_idt_entry(15, (u32)isr15);
  set_idt_entry(16, (u32)isr16);
  set_idt_entry(17, (u32)isr17);
  set_idt_entry(18, (u32)isr18);
  set_idt_entry(19, (u32)isr19);
  set_idt_entry(20, (u32)isr20);
  set_idt_entry(21, (u32)isr21);
  set_idt_entry(22, (u32)isr22);
  set_idt_entry(23, (u32)isr23);
  set_idt_entry(24, (u32)isr24);
  set_idt_entry(25, (u32)isr25);
  set_idt_entry(26, (u32)isr26);
  set_idt_entry(27, (u32)isr27);
  set_idt_entry(28, (u32)isr28);
  set_idt_entry(29, (u32)isr29);
  set_idt_entry(30, (u32)isr30);
  set_idt_entry(31, (u32)isr31);

  // Kartlegger PIC-en
  port_byte_out(PIC1_COMMAND, 0x11);
  port_byte_out(PIC2_COMMAND, 0x11);
  port_byte_out(PIC1_DATA, 0x20);
  port_byte_out(PIC2_DATA, 0x28);
  port_byte_out(PIC1_DATA, 0x04);
  port_byte_out(PIC2_DATA, 0x02);
  port_byte_out(PIC1_DATA, 0x01);
  port_byte_out(PIC2_DATA, 0x01);
  port_byte_out(PIC1_DATA, 0x00);
  port_byte_out(PIC2_DATA, 0x00);

  // Installerer IRQ-ene
  set_idt_entry(32, (u32)irq0);
  set_idt_entry(33, (u32)irq1);
  set_idt_entry(34, (u32)irq2);
  set_idt_entry(35, (u32)irq3);
  set_idt_entry(36, (u32)irq4);
  set_idt_entry(37, (u32)irq5);
  set_idt_entry(38, (u32)irq6);
  set_idt_entry(39, (u32)irq7);
  set_idt_entry(40, (u32)irq8);
  set_idt_entry(41, (u32)irq9);
  set_idt_entry(42, (u32)irq10);
  set_idt_entry(43, (u32)irq11);
  set_idt_entry(44, (u32)irq12);
  set_idt_entry(45, (u32)irq13);
  set_idt_entry(46, (u32)irq14);
  set_idt_entry(47, (u32)irq15);

  set_idt();

}

/* Meldinger som vises når interrupten skjer */
char* exception_msg[] = {
  "Deler på null",
  "Debug exception"
  "Ikke maskerbar interrupt",
  "Breakpoint",
  "Into Detected Overflow",
  "Out of Bounds",
  "Invalid Opcode",
  "No Coprocessor",

  "Double Fault",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Segment Not Present",
  "Stack Fault",
  "General Protection Fault",
  "Page Fault",
  "Unknown Interrupt",

  "Coprocessor Fault",
  "Alignment Check",
  "Machine Check",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",

  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved"
};

void isr_handler(registers r){

  kprint("INTERUPT!!!\n");
  char str[3];
  itoa(r.int_no, str);
  kprint("Int: ");
  kprint(str);
  kprint("\n");
  kprint(exception_msg[r.int_no]);
  kprint("\n");

}

void register_interrupt_handler(u8 n, isr handler){
  interrupt_handlers[n] = handler;
}

void irq_handler(registers r){

  // Sender en EOI (End Of Interrupt) til PIC-ene
  if(r.int_no >= 40) port_byte_out(PIC2_COMMAND, 0x20);
  port_byte_out(PIC1_COMMAND, 0x20);

  if(interrupt_handlers[r.int_no] != 0){
    isr handler = interrupt_handlers[r.int_no];
    handler(r);
  }

}