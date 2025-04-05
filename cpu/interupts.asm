; ISR_handeleren definert i isr.c
[extern isr_handeler]

; Alle ISR-er har denne delen til felles
isr_common_stub:
    ; Lagre CPU tilstand
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Kall C handeleren
    call isr_handeler

    ; Gjenopprett CPU tilstanden
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    sti
    iret

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr9
global isr8
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

; 0: Del på null
isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

; 1: Debug exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

; 2: Ikke maskerbar interupt exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

; 3: Int 3 exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

; 4: INTO exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

; 5: Ut av rekkevidde exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

; 6: Ugyldig opcode
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

; 7: Enhet ikke tilgjenlig
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

; 8: Dobbel fault
isr8:
    cli
    push byte 0
    push byte 8
    jmp isr_common_stub

; 9: Ko-prosessor segment overkjørt
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Ugyldig TSS
isr10:
    cli
    push byte 10
    jmp isr_common_stub

; 11: Segment ikke til stede
isr11:
    cli
    push byte 11
    jmp isr_common_stub

; 12: Stack-segment fault
isr12:
    cli
    push byte 12
    jmp isr_common_stub

; 13: Generel forsvarsfeil
isr13:
    cli
    push byte 13
    jmp isr_common_stub

; 14: Page fault
isr14:
    cli
    push byte 14
    jmp isr_common_stub

; 15: RESERVERT
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: x87 Floating point exception 
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Justeringssjekk
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

; 18: Maskinsjekk
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: SIMD Floating point exception
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Virtualiserings exception
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

; 21: Forsvarskontroll exception
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

; 22 - 27: RESERVERTE
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

; 28: Hypervisor injection exception
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

; 29: VMM Kommunikasjons exception
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

; 30: Sikkerhets exception
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

; 31: RESERVERT
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub