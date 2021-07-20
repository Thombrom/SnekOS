;
;   Bootloader for OS system
;
;   Load sectors into memory
;   for further execution
;
;   Switch to 32bit protected
;   mode and hand over controll
;   to kernel
;

[bits 16]
[org 0x7c00]                ; Bootloader always loaded at
                            ; this specific adress
PROGRAM_SPACE equ 0x9000
SECTOR_LOAD_NUM equ 25

    ; Load further sectors
    ; into memory
    mov [BOOT_DRIVE], dl    ; BIOS stores boot drive in DL
                            ; and we remember this for later

    mov bp, 0x8000          ; Set stack to save location
    mov sp, bp

    mov bx, PROGRAM_SPACE   ; Load sectors to 0x9000
    mov dh, SECTOR_LOAD_NUM ; from boot disk
    mov dl, [BOOT_DRIVE]
    call disk_load

    mov bx, boot_msg
    call print_str

    jmp PROGRAM_SPACE

BOOT_DRIVE:
    db 0

boot_msg:
    db "Booted in 16 bit mode", 0

%include "src/bootloader/print_str.asm"
%include "src/bootloader/print_hex.asm"
%include "src/bootloader/disk_load.asm"

; Bootsector end bits
; which must be 0xaa55
times 510-($-$$) db 0
dw 0xaa55

