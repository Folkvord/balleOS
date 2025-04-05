; Tar inn et input fra tastaturet og lagrer det i et buffer på 10 bytes
; Printer også ut stringen til skjerm
str_input:
    pusha
    mov ah, 0
    mov bx, str_buffer
    mov dx, 0

str_input_loop:
    cmp dx, 10
    je str_input_end
    int 0x16
    call char_print16
    mov ah, 0
    mov [bx], al
    add bx, 1
    add dx, 1
    jmp str_input_loop

str_input_end:
    popa
    ret

char_input:
    pusha
    mov ah, 0
    mov bx, char_buffer

char_input_loop:
    int 0x16
    call char_print16
    mov [bx], al
char_input_need_enter:
    mov ah, 0
    int 0x16
    cmp al, 0x0D
    je char_input_end

char_input_end:
    popa
    ret

char_buffer:
    times 1+1 db 0

str_buffer:
    times 10+1 db 0