#include "../util/type.h"

#define PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_SIZE 4096

u32* page_directory = 0x9c000;    // Anner ikke om dette er ledig
u32* first_page_table = 0x9d000;  // Neste 4KiB

/* 
 * Det er viktig å vite hvordan en page-directory og page-tabell er oppbygd.
 * Før du prøver å forstå hva koden under gjør, se på en modell av en tabelloppføring. 
 */

/* Initialiserer page-katalogen *Midlertidig setter den bare den første page-tabellen* */
void init_page_directory(){

  page_directory[0] = ((u32)first_page_table) | 3;

}

/* Initialiserer en tabell *Midlertidig initialiserer den bare den første tabellen* */
void init_page_table(u32* table){

  // Midlertidig
  table = first_page_table;

  // Gå gjennom hver index og legg til en offset (i * PAGE_SIZE) og permisjoner (| 3)
  int i = 0;
  while(i < PAGE_TABLE_ENTRIES){
    table[i] = (i * PAGE_SIZE) | 3;
    i++;
  }

}

/* Aktiverer paging */
void activate_paging(){

  // Laster inn adressen til page-katalogen inn i cr3-registeret
  asm volatile("mov %0 %%cr3" :: "r"(page_directory));

  u32 cr0;  // Laster inn cr0-registeret
  asm volatile("mov %%cr0 %0" : "=r"(cr0));

  cr0 |= 0x80000000;  // Setter den åttende bit til 1 for å aktivere paging
  asm volatile("mov %0 %%cr0" :: "=r"(cr0));

}