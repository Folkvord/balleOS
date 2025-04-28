#include "../util/type.h"
#include "../util/util.h"
#include "../util/string.h"

/* NOTER:
 * Dette systemet håndterer ikke 64-bit variabler 
 * DVS. at lengdene kan regnes feil:
 * Programmet leser bare de nedre 32 bitsene.
 * Fin ting for deg å fikse :) 
 */

#define SMAP_ENTRY_COUNT  0x8000
#define SMAP_ENTRY_ADDR   0x8004
#define SMAP_ENTRY_SIZE   24
#define MAX_SMAP_ENTRIES  32

#define USABLE_MEM    1
#define RESERVED_MEM  2
#define RECLAIMABLE   3
#define ACPI_NVS      4
#define BAD_MEM       5

typedef struct SMAP_entry{
  u32 base_lower;
  u32 base_upper;
  u32 length_lower;
  u32 length_upper;
  u32 type;
  u32 ACPI;
}__attribute__((packed)) SMAP_entry;

SMAP_entry SMAP_entries[MAX_SMAP_ENTRIES];
u32 ram_in_bytes = (u32)-1;

/* Leser SMAP-oppføringene bootloaderen lagret */
void read_SMAP(){

  int smap_entry_count = *((u16*) SMAP_ENTRY_COUNT);

  char str[10];
  itoa(smap_entry_count, str);
  
  int i = 0;
  int offset = 0;
  while(i < smap_entry_count){

    SMAP_entries[i].base_lower = *((u32*)(SMAP_ENTRY_ADDR + offset));
    SMAP_entries[i].base_upper = *((u32*)(SMAP_ENTRY_ADDR + offset + 4));
    SMAP_entries[i].length_lower = *((u32*)(SMAP_ENTRY_ADDR + offset + 8));
    SMAP_entries[i].length_upper = *((u32*)(SMAP_ENTRY_ADDR + offset + 12));
    SMAP_entries[i].type = *((u32*)(SMAP_ENTRY_ADDR + offset + 16));
    SMAP_entries[i].ACPI = *((u32*)(SMAP_ENTRY_ADDR + offset + 20));

    offset += SMAP_ENTRY_SIZE;
    i++;
  }
 
}

/* Printer ut informasjon om SMAP-oppføringene */
void SMAP_dump(){

  int smap_entry_count = *((u16*) SMAP_ENTRY_COUNT);
  char buffer[10];
  int i = 0;
  while(i < smap_entry_count){

    itoa(i+1, buffer);
    kprint("SMAP ENTRY: ");
    kprint(buffer);
    kprint("\n");

    kprint("BASE:   ");
    hex32_str(SMAP_entries[i].base_upper, buffer);
    kprint(buffer);
    kprint(" ");
    hex32_str(SMAP_entries[i].base_lower, buffer);
    kprint(buffer);
    kprint("\n");

    kprint("LENGTH: ");
    hex32_str(SMAP_entries[i].length_upper, buffer);
    kprint(buffer);
    kprint(" ");
    hex32_str(SMAP_entries[i].length_lower, buffer);
    kprint(buffer);
    kprint("\n");

    kprint("TYPE:   ");
    hex32_str(SMAP_entries[i].type, buffer);
    kprint(buffer);
    kprint("\n");

    kprint("ACPI:   ");
    hex32_str(SMAP_entries[i].ACPI, buffer);
    kprint(buffer);
    kprint("\n");

    kprint("\n");
    purge(buffer);
    i++;
    
  }

}

/* Returnerer antall ledige bytes i RAM */
int get_available_ram_bytes(){

  if(ram_in_bytes != (u32)-1) return ram_in_bytes;

  int smap_entry_count = *((u16*) SMAP_ENTRY_COUNT);
  int available_mem = 0;

  int i = 0;
  while(i < smap_entry_count){

    int smap_type = SMAP_entries[i].type;

    // Sjekker tilfeller hvor vi ikke kan bruke minnet
    // Kan være fint å evt logge
    if(smap_type == RESERVED_MEM){
      // Reservert
    }
    else if(smap_type == RECLAIMABLE){
      // ACPI reclaimable
    }
    else if(smap_type == ACPI_NVS){
      // ACPI NVS memory
    }
    else if(smap_type == BAD_MEM){
      // Ødelagt minne
    }
    else{
      // Ellers er minne brukbart
      available_mem += SMAP_entries[i].length_lower;
    }

    i++;

  }

  ram_in_bytes = available_mem;
  return available_mem;

}