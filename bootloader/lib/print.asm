str_print:
    pusha
    mov edx, 0xB8000
    mov ah, 0x0F
str_print_loop:
    mov al, [ebx]
    cmp al, 0
    jz str_print_end
    mov [edx], ax
    add edx, 2
    add ebx, 1
    jmp str_print_loop

str_print_end:
    popa
    ret

print_newline:
    mov ah, 0x0F
    mov al, 0x0A
    mov al, 0x0D