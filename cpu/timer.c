#include "../drivers/screen.h"
#include "../drivers/port.h"
#include "../util/type.h"
#include "timer.h"
#include "isr.h"

u32 tick = 0;

static void timer_callback(registers_t r){

  tick++;
  char tick_str[256];
  itoa(tick, tick_str);
  kprint(tick_str);
  kprint(" ");

}

void init_timer(u32 freq){

  // Installerer interrupten
  register_interrupt_handler(IRQ0, timer_callback);

  // PIT-verdien med en 1193180 hz klokke
  u32 divisor = 1193180 / freq;
  u8 lower = (u8)(divisor & 0xff);
  u8 upper = (u8)((divisor >> 8) & 0xff);

  // Sender kommandoen
  port_byte_out(0x43, 0x36);
  port_byte_out(0x40, lower);
  port_byte_out(0x40, upper);

}