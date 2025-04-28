#include "smap.h"
#include "kernel.h"
#include "../cpu/isr.h"
#include "../util/mem.h"
#include "../util/util.h"
#include "../util/type.h"
#include "../util/string.h"
#include "../util/bitset.h"

// Mengder
#define PAGE_DIRECTORY_ENTRIES 0x400
#define PAGE_TABLE_ENTRIES 0x400
#define PAGE_SIZE 0x1000

// Makrofunksjoner
#define TO_INDEX(addr) addr/PAGE_SIZE
#define TO_ADDR(index) index*PAGE_SIZE

// Adresser
#define KERNEL_PD_ADDR       0x9c000
#define BASE_PAGE_TABLE_ADDR 0x9d000
#define KMALLOC_FIRST_ADDR   0x100000

// Flagg
#define PRESENT   0x1
#define READWRITE 0x2
#define USERMODE  0x4
#define ACCESSED  0x32
#define DIRTY     0x64

// PT-modell
typedef struct page_table_t{

  // Pagene
  u32* pages[PAGE_TABLE_ENTRIES];

} page_table_t;

// PD-modell
typedef struct page_directory_t{

  // Pagetabellene
  page_table_t* tables[PAGE_DIRECTORY_ENTRIES];
  // Fysiske pagetabelladresser
  u32* phys_pt_addr[PAGE_DIRECTORY_ENTRIES];
  // PD-ens fysiske adresse
  u32 phys_addr;

} page_directory_t;


void change_page_directory(page_directory_t* pd);
u32 first_frame();
void alloc_frame(u32* page, int is_kernel, int is_writable);
void free_frame(u32* page);
u32* get_page(u32 addr, int make, page_directory_t* pd);
u32 kmalloc(int size, int align, u32* phys);
void handle_page_fault(registers_t regs);

u32 kmalloc_placement_addr = KMALLOC_FIRST_ADDR;  // Adressen der kmalloc allokerer fra
u32* frame_bitset;                                // Bitset over alle frame_bitset i RAM
u32 frame_amount;                                 // Antall frame_bitset

page_directory_t* kernel_page_directory = (page_directory_t*) KERNEL_PD_ADDR;   // Kernelens pagedirectory
page_directory_t* current_active_pd;                                            // Den aktive pagedirectoryen


/* Initialiserer paging */
void init_paging(){

  // Finn ut hvor mange frame_bitset vi kan ha (326393 frame_bitset med 133MB)
  u32 frames = get_available_ram_bytes()/PAGE_SIZE;   // Antall frames (ramstørrelse/framestørrelse)
  frame_bitset = (u32*)kmalloc(frames, 0, 0);         // Allokerer nok plass for framebitsettet
  memset(frame_bitset, 0, frames);                    // Instansierer bitsettet som tomt

  // Initialiser en Page directory
  kernel_page_directory = (page_directory_t*)kmalloc(sizeof(page_directory_t), 1, 0);   // Allokerer plass for kernelens PD
  memset((u32*)kernel_page_directory, 0, sizeof(page_directory_t));                     // Instansierer det allokerte minnet tomt
  current_active_pd = kernel_page_directory;                                            // Setter det aktive PD-et til kernelen sin

  // Identitetmap alle adressene fram til kmalloc sin startsadresse
  int i = 0;
  while(i < kmalloc_placement_addr){
    u32* page = get_page(i, 1, current_active_pd);    // Hent en page på adressen *i*
    alloc_frame(page, 0, 0);                          // Alloker en frame til den pagen
    i += PAGE_SIZE;                                   // Neste pageadresse
  }

  // Registrer pagefaulthandleren med ISR-en
  register_interrupt_handler(IRQ14, &handle_page_fault);

  // Bytt til kernelPD-et
  change_page_directory(kernel_page_directory);

}

/* Bytter PD */ 
void change_page_directory(page_directory_t* pd){

  current_active_pd = pd;

  // Laster inn adressen til page-katalogen inn i cr3-registeret
  asm volatile("mov %0, %%cr3" :: "r"(pd));

  u32 cr0;  // Laster inn cr0-registeret
  asm volatile("mov %%cr0, %0" : "=r"(cr0));

  cr0 |= 0x80000000;  // Setter den åttende bit til 1 for å aktivere paging
  asm volatile("mov %0, %%cr0" :: "r"(cr0));

}

/* Finner den første ledige pagen */
u32 first_frame(){

  int i = 0, j = 0;
  while(i < frame_amount){

    if(frame_bitset[i] == 0){
      i++;
      continue;
    }

    j = 0;
    while(j < 32){
      if(test_bit(frame_bitset, i*32+j)) return i*32+j;
      j++;
    }

    i++;
  }

  return (u32)-1;

}

/* Allokerer en frame til en page */
void alloc_frame(u32* page, int is_kernel, int is_writable){

  // Sjekk om pagen allerede er allokert
  if(*page & 0xfffff000 != 0){
    return; 
  }

  // Finn en frame
  u32 index = first_frame();
  if(index == (u32)-1){
    PANIC("INGEN TILGJENGELIGE FRAMES");  // Ingen frame_bitset ;(
  }

  // Frame secured
  set_bit(frame_bitset, index);
  *page |= PRESENT; 
  if(is_writable) *page |= READWRITE;
  if(is_kernel) *page &= ~USERMODE;
  *page |= (index << 12);

}

/* Frigjør en frame fra en page */
void free_frame(u32* page){
  
  // Sjekk om pagen er uallokert
  u32 page_frame = *page & 0xfffff000;
  if(page_frame == 0x0) return;

  u32 frame_index = page_frame/PAGE_SIZE;
  clear_bit(frame_bitset, frame_index);

  *page &= (0x00000fff);
  
}

/* Primitiv malloc før heapen eksisterer; antar at ingen adresse trengs å bli "kfree()-gjort" */
u32 kmalloc(int size, int align, u32* phys){

  // Juster minneblokken til en page
  if(align == 1 && (kmalloc_placement_addr & 0xfffff000)){
    kmalloc_placement_addr &= 0xfffff000;   // Reset offsettet
    kmalloc_placement_addr += 0x1000;       // Hopp en page fram
  }
  
  if(phys){
    *phys = kmalloc_placement_addr;
  }

  u32 assigned_addr = kmalloc_placement_addr;
  kmalloc_placement_addr += size;
 
  return assigned_addr;

}

u32* get_page(u32 addr, int make, page_directory_t* pd){

  // Hent indeksene i PT-en og PD-en
  u32 pd_index = addr/=0x1000;
  u32 pt_index = addr/1024;

  page_table_t* table = pd->tables[pd_index];
  
  if(table){
    // Eksisterer
    return table->pages[pt_index];
  }
  else if(make){
    // Eksisterer ikke, skap
    u32 table_addr_phys;
    pd->tables[pd_index] = (page_table_t*)kmalloc(sizeof(page_table_t), 1, &table_addr_phys);   // Allokerer en frame for PT-en
    memset((u32*)pd->tables[pd_index], 0, sizeof(page_table_t));                                // Initialiserer tabellen som tom
    pd->phys_pt_addr[pd_index] = (u32*)table_addr_phys;                                         // Logger tabellens fysiske adresse
    return pd->tables[pd_index]->pages[pt_index];                                               // Returnerer adressen til pagen
  }
  else{
    // Eksisterer ikke, ikke skap
    return 0;
  }

}

/* For ISR <3 */
void handle_page_fault(registers_t regs){

  u32 p    = !(regs.err_code & PRESENT);
  u32 rw   = !(regs.err_code & READWRITE);
  u32 us   = !(regs.err_code & USERMODE);
  u32 rsvd = !(regs.err_code & 0x8);
  u32 id   = !(regs.err_code & 0x16);

  u32 faulty_addr;
  asm volatile("mov %%cr2, %0" : "=r" (faulty_addr));

  char dadl_addr[10];
  hex32_str(faulty_addr, dadl_addr);

  kprint("PAGE FAULT!!! (");
  if(p) kprint("PAGE ER IKKE I MINNET)\n");
  if(rw) kprint("PAGE ER IKKE SKRIVBART)\n");
  if(us) kprint("PAGE ER BEGRENSET TIL KERNELEN)\n");
  if(rsvd) kprint("PAGE ER RESERVERT)\n");
  kprint("DADDELVERDIG ADRESSE: ");
  kprint(dadl_addr);
  kprint("\n");

  PANIC("KERNEL PANIC");

}