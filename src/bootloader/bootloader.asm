;
;   Bootloader for OS system
;
;   Load sectors into memory
;   for further execution
;

[org 0x7c00]                ; Bootloader always loaded at
                            ; this specific adress
    mov [BOOT_DRIVE], dl    ; BIOS stores boot drive in DL
                            ; and we remember this for later

    mov bp, 0x8000          ; Set stack to save location
    mov sp, bp

    mov bx, 0x9000          ; Load 5 sectors to 0x9000
    mov dh, 5               ; from boot disk
    mov dl, [BOOT_DRIVE]
    call disk_load

    jmp $

BOOT_DRIVE:
    db 0

boot_msg:
    db "Booting SnekOS", 0

; Nessecary includes when booting
; which are included in the bootsector
%include "src/bootloader/print_str.asm"
%include "src/bootloader/print_hex.asm"
%include "src/bootloader/disk_load.asm"

times 510-($-$$) db 0
dw 0xaa55

; Beyond bootsector


