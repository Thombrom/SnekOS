; Make switch to 32-bit protected
[bits 16]
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

    ; Enable A20
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Switch to Long Mode
    call cpuid_detect               ; Detect cpuid
    call long_mode_detect           ; Detect long mode
    call paging_identity_setup      ; Set up identity paging
    call gdt_edit_long_mode         ; Set up GDT for long mode

    jmp CODE_SEG:long_mode_init

%include "src/bootloader/print_str_32bit.asm"
%include "src/bootloader/CPUID.asm"
%include "src/bootloader/paging.asm"

protected_mode_message:
    db "Entered 32bit protected mode", 0

[bits 64]
[extern _start]

%Include "src/kernel/interrupt.asm"

long_mode_init:

    mov ax, DATA_SEG        ; Update segments
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; Update stack so it at the top
    mov esp, ebp            ; of the free space

long_mode_begin:

    mov edi, 0xb8000
    mov rax, 0x1f201f201f201f20
    mov ecx, 500
    rep stosq

    call _start

    jmp $


