; Laster 'dh' sektorer inn fra driver 'dl' inn til bx
load_disk:
    pusha
    push dx
    mov ah, 0x02
    mov al, dh
    mov cl, 0x02
    mov ch, 0x00
    mov dl, 0x00
    int 0x13

    pop dx
    popa
    ret