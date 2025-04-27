#ifndef BITSET_H
#define BITSET_H

#include "type.h"

void set_bit(u32 bit_set[], u32 index);
void clear_bit(u32 bit_set[], u32 index);
void toggle_bit(u32 bit_set[], u32 index);
int test_bit(u32 bit_set[], u32 index);
void bit_set_dump(u32 bit_set[], int size);

#endif