; TODO:
; - Legge til exception handeling
; - Mer nøyaktig telling (Om et segment er gyldig)
; - Antall oppføringer lest

BITS 16

ENTRY_COUNT  equ 0x8000       ; Adressen hvor antall oppføringer lagres
MMAP_ENTRY   equ 0x8004       ; Adressen hvor SMAP-oppføringene begynner
ENTRY_SIZE   equ 24           ; Størrelsen på en oppføring

get_mmap:
    xor ebx, ebx
    xor bp, bp
    mov ax, 0
    mov es, ax
    mov di, MMAP_ENTRY
.e820_start:
    mov eax, 0xe820
    mov edx, 0x534d4150
    mov dword [es:di + 20], 1
    mov ecx, ENTRY_SIZE
    int 0x15
    jc short .failed            ; Carry er satt på første interrupt -> ingen oppføringer
    mov edx, 0x534d4150         ; Interrupten thrasher EDX for en eller en annen grunn
    cmp eax, edx
    jne short .failed
    test ebx, ebx
    je short .failed             ; EBX er 0 etter første lesning -> én oppføring; ubrukelig
    jmp short .validate_entry

.e820_loop:
    mov eax, 0xe820
    mov dword [es:di + 20], 1
    mov ecx, ENTRY_SIZE
    int 0x15
    jc short .done              ; Om carry er 0, har vi allerede lest alle oppføringer
    mov edx, 0x534d4150
.validate_entry:
    jcxz .skip                      ; Hopp over oppføringer med lengde 0
    cmp cl, 20                      ; Sjekk om svaret var en 24-byte oppføring
    jbe short .check_entry_content  ; Hvis så hopp over ingorebit-sjekken
    test byte [es:di + 20], 1       ; Ingore-bit satt?
    je short .e820_loop             ; Hvis så, ignorer
.check_entry_content:
    mov ecx, [es:di + 8]    ; Hent den nedre delen av lengden
    or ecx, [es:di + 12]    ; Sjekk om lengden er 0 (OR 32 nedre bits med 32 øvre bits)
    jz short .skip          ; Hvis så, ikke tell med denne oppføringen
    add di, ENTRY_SIZE      ; Inkrementer offsettet
    inc bp                  ; Inkrementer oppføringstelleren
.skip:
    test ebx, ebx           ; Sjekk om ebx er 0 (Ingen oppføringer igjen)
    jne short .e820_loop    ; Hopp til begynnelsen av løkken
    
.done:
    mov ax, 0
    mov ds, ax
    mov [es:ENTRY_COUNT], bp
    clc
    ret
.failed:
    stc
    clc
    ret
