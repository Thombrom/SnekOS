;
;   For printing hexadecimal values to the
;   screen of the os
;       dx: Value to be printed
;

print_hex:
    pusha

    ; Print 0x initially
    mov al, '0'
    call print_char

    mov al, 'x'
    call print_char

    ; 16 bit print, we have 4 values
    mov cl, dh                  ; We pick up the second half of dl
    shr cl, 4                   ; shift right 4 to get outer value
    mov bx, hex_char_lookup
    add bx, cx                  ; Add value to lookup
    mov al, [bx]                ; Load looked up value
    call print_char

    ; Repeat the above for the third byte
    mov cl, dh
    and cl, 0b00001111          ; Only take the lower half
    mov bx, hex_char_lookup
    add bx, cx
    mov al, [bx]
    call print_char

    ; Repeat the above for the second byte
    mov cl, dl
    shr cl, 4
    mov bx, hex_char_lookup
    add bx, cx
    mov al, [bx]
    call print_char

    ; Repeat the above for the first byte
    mov cl, dl
    and cl, 0b00001111          ; Only take the lower half
    mov bx, hex_char_lookup
    add bx, cx
    mov al, [bx]
    call print_char

    popa
    ret

hex_char_lookup:
    dw "0123456789abcdef"               ; We use this as lookup tacle
                                        ; Could be done with ASCII offsets
                                        ; but this is more readacle

