ORG 0x7C00
[bits 16]

KERNEL_LOCATION equ 0x1000

mov [BOOT_DISK], dl

; Reset registere og initier stacken
xor ax, ax
mov ds, ax
mov es, ax
mov bp, 0x9000
mov sp, bp

; Les fra disk
mov bx, KERNEL_LOCATION
mov dh, 0x0f    ; 16 sektorer           <---- SKJER NOE HELT ????, ØK DENNE
mov ah, 0x02
mov al, dh
mov ch, 0x00    ; Syllinder 0
mov dh, 0x00    ; Hode 0
mov cl, 0x02    ; Sektor 2
mov dl, [BOOT_DISK]
int 0x13        ; LEGG TIL FEILSJEKKING!!!
jc error

call clear_screen16

; Hopp over til 32-bit
cli
lgdt [GDT_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG    ; Sett til DATA_SEG for å gi registerene tilgang til RAM-data
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    mov ebp, 0x90000    ; Sett stackpekerne
    mov esp, ebp

    jmp KERNEL_LOCATION

halt:
    jmp $

error:
    mov bx, ERROR_MSG
    call str_print
    jmp halt

%include "gdt.asm"
%include "print.asm"
%include "lib16/print.asm"
%include "lib16/load_disk.asm"

; STRING
BOOT_MSG: db 'HALLO!!! BalleOS ER KLAR TIL A BOOTES', 0
DESC: db 'GLED DEG TIL OPTIMAL YTELSE!!!!', 0
ERROR_MSG: db 'OIDA!!!! :(', 0

BOOT_DISK: db 0

; Skriver bootloader-signaturen
times 510-($-$$) db 0
dw 0xAA55