GDT_start:
    null_descriptor:
        dd 0
        dd 0
    code_descriptor:
        dw 0xFFFF       ; Limit (Sier hvor stort segmentet skal være)
        dw 0            ; Basen (Sier hvor segmentet skal starte)
        db 0            ; Basen
        db 0b10011010   ; Type (Sier hva slags segment dette er) [Present = 1], [Privilege = 00], [Executable = 0]
        db 0b11001111   ; 
        db 0            ; Basen
    data_descriptor:
        dw 0xFFFF       ; Limit
        dw 0            ; Basen
        db 0            ; Basen
        db 0b10010010   ; Type []
        db 0b11001111   
        db 0            ; Basen
GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start      ; Size
    dd GDT_start                ; Start

CODE_SEG equ code_descriptor - GDT_start
DATA_SEG equ data_descriptor - GDT_start