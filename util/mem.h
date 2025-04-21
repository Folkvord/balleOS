#ifndef MEM_H
#define MEM_H

void memcpy(u8 *src, u8 *dst, u32 n);
void memset(u8 *start_addr, u8 val, u32 len);
int malloc(u32 size);
int bolloc(u32 size);

#endif