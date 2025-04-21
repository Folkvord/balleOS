#ifndef PAGE_H
#define PAGE_H

#include "../util/type.h"

void init_page_directory();
void init_page_table(u32* table);
void activate_paging();

#endif