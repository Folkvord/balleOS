#ifndef PORT_DRIVER
#define PORT_DRIVER

#include "../util/type.h"

u8 port_byte_in(u16 port);
void port_byte_out(u16 port, u8 data);
u16 port_word_in(u16 port);
void port_word_out(u16 port, u16 data);

#endif