;
;   For GDT layout and specification
;

gdt_start:

gdt_null:               ; Mandatory null descriptor
    dd 0x0
    dd 0x0

gdt_code:
    ; We set base=0x0, limit=0xffffff
    ; Flags1: present=1, privilige=00, descriptortype=1,
    ; code=1, conforming=0, readable=1, accessed=0
    ;       -> 10011010
    ; Flags2: granularity=1, 32bit=1, 64seg=0, AVL=0
    ;       -> 1100

    dw 0xffff       ; Limit         (bits 0-15)
    dw 0x0          ; Base          (bits 0-15)
    db 0x0          ; Base          (bits 16-23)
    db 10011010b    ; Flags1
    db 11001111b    ; Flags2 + Limit(bits 16-19)
    db 0x0          ; Base          (bits 24-31)

gdt_data:
    ; Same as GDT_COde except for code=1
    dw 0xffff       ; Limit         (bits 0-15)
    dw 0x0          ; Base          (bits 0-15)
    db 0x0          ; Base          (bits 16-23)
    db 10010010b    ; Flags1
    db 11001111b    ; Flags2 + Limit(bits 16-19)
    db 0x0          ; Base          (bits 24-31)

gdt_end:
    ; Makes us able to calculate size


gdt_descriptor:
    dw gdt_end - gdt_start - 1      ; Size of GDT
    dd gdt_start                    ; Start adress

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start


