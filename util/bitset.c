#include "bitset.h"
#include "type.h"


/* Setter en bit på en indeks */
void set_bit(u32 bit_set[], u32 index){

  bit_set[index/32] |= (0x1 << index%32);

}

/* Returnerer bitten på en indeks */
int test_bit(u32 bit_set[], u32 index){

  return (bit_set[index/32] & (0x1 << index%32)) != 0;

}

/* Inverterer en bit på en indeks */
void toggle_bit(u32 bit_set[], u32 index){

  bit_set[index/32] ^= (0x1 << index%32);

}

/* Klargjører en bit på en indeks */
void clear_bit(u32 bit_set[], u32 index){

  bit_set[index/32] &= ~(0x1 << index%32);

}

/* Printer ut hele bitsettet * Midlertidig så printer den INTene baklengs (første int er nederst) * */
void bit_set_dump(u32 bit_set[], int size){

  int i = 32*size-1;
  while(i > -1){

    int bit = test_bit(bit_set, i);
    if(bit == 1) kprint("1");
    else kprint("0");

    if(i%32 == 0) kprint("\n");
    else if(i%8==0) kprint(" ");

    i--;

  }

}