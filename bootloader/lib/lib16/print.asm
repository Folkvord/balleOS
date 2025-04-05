; Printer ut en string
; Tar input gjennom registeret: bx
str_print16:
    pusha
    mov ah, 0x0E

str_print_loop16:
    mov al, [bx]
    or al, al
    jz str_print_end16
    int 0x10
    add bx, 1
    jmp str_print_loop16

str_print_end16:
    call new_line_print16
    popa
    ret


; Printer ut en char
; Tar input gjennom registeret: al
char_print16:
    push ax
    mov ah, 0x0E
    int 0x10
    pop ax
    ret

; Printer en new-line
new_line_print16:
    pusha
    mov ah, 0x0E
    mov al, 0x0A
    int 0x10
    mov al, 0x0D
    int 0x10
    popa
    ret

; Printer ut et hex-tall
; Tar input gjennom registeret: dx
print_hex16:
    pusha
    mov cx, 0   ; Index

hex_loop16:
    cmp cx, 4   ; Lar loopen gå 4 ganger da dette er en 16-bit bootloader
    je hex_end16
    mov ax, dx
    and ax, 0x000F
    add ax, 0x30
    cmp al, 0x39
    jle hex_next16
    add al, 7
hex_next16:
    mov bx, HEX_OUT + 5
    sub bx, cx
    mov [bx], al
    ror dx, 4
    add cx, 1
    jmp hex_loop16

hex_end16:
    mov bx, HEX_OUT
    call str_print16
    popa
    ret

clear_screen16:
    push ax
    mov ah, 0x0
    mov al, 0x3
    int 0x10
    pop ax
    ret

HEX_OUT: db '0x0000', 0