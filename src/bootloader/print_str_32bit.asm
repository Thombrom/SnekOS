;
;   Print string routine for 32 bit mode
;       - ebx: point to null-terminated string
;
VIDEO_MEMORY    equ 0xb8000             ; Location for video memory
WHITE_ON_BLACK  equ 0x0f                ; White on black code

print_str_32:
    pusha
    mov edx, VIDEO_MEMORY

print_string_start_32_:
    mov al, [ebx]                       ; We store char in al
    mov ah, WHITE_ON_BLACK              ; and formatting in ah

    cmp al, 0
    je print_str_end_32_

    mov [edx], ax                       ; Move it into position

    add ebx, 1                          ; Advance string base one
    add edx, 2                          ; Advance position two (as there
                                        ; is formatting)
    jmp print_string_start_32_

print_str_end_32_:
    popa
    ret
