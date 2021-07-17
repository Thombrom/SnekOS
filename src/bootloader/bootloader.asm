;
;   Bootloader for OS system
;

[org 0x7c00]            ; Bootloader always loaded at
                        ; this specific adress
    mov bx, boot_msg
    call print_str

    mov dx, 0x1234
    call print_hex


    jmp $

boot_msg:
    db "Booting SnekOS", 0

%include "src/bootloader/print_str.asm"
%include "src/bootloader/print_hex.asm"

times 510-($-$$) db 0
dw 0xaa55
