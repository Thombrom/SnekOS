[org 0x9000]

; Make switch to 32-bit protected
protected_mode_enter:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]

    mov eax, cr0            ; Fetch cr0 register
    or eax, 0x1             ; set first bit
    mov cr0, eax            ; and store it back

    ; Perform far jump to protected mode
    jmp CODE_SEG:protected_mode_init

%include "src/bootloader/GDT.asm"

[bits 32]

protected_mode_init:

    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; Update stack so it at the top
    mov esp, ebp            ; of the free space

protected_mode_begin:

    mov ebx, protected_mode_message
    call print_str_32

    call cpuid_detect       ; Detect cpuid
    call long_mode_detect   ; Detect long mode

    jmp $

%include "src/bootloader/print_str_32bit.asm"
%include "src/bootloader/CPUID.asm"

protected_mode_message:
    db "Entered 32bit protected mode", 0

