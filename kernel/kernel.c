#include <stdint.h>
#include "../cpu/isr.h"
#include "../util/util.h"
#include "../drivers/ports.h"
#include "../drivers/screen.h"

void main(){

  isr_install();
  fill_screen(BG_BLUE);
  change_bg(BG_BLUE);
  change_fg(FG_WHITE);

  int i = 0; char str[3];
  while(i < VIDEO_HEIGHT){
    itoa(i+1, str);
    kprint_at(str, 0, i);
    i++;
  }
  
  kprint("BALLEOS");

//  scroll(1);

  __asm__ __volatile__("int $2");

  return;

}