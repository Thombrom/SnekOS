;
;   Print string routine
;       - bx: point to null-terminated string
;
print_str:
    pusha

print_string_start_:
    mov cx, [bx]
    cmp cl, 0x00
    je print_str_end_

    mov al, [bx]
    call print_char
    add bx, 1
    jmp print_string_start_

print_str_end_:
    popa
    ret

print_char:
    mov ah, 0x0e
    int 0x10
    ret
