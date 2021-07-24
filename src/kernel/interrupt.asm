[bits 64]

[extern _idt]
idt_descriptor:
    dw 4095
    dq _idt

GLOBAL isr1            ; So we can access it from the kernel
[extern isr1_handler]
isr1:
    push rax
    push rcx
    push rdx
    push r8
    push r9
    push r10
    push r11

    call isr1_handler

    pop r11
    pop r10
    pop r9
    pop r8
    pop rdx
    pop rdx
    pop rax

    iretq               ; 64 bit

GLOBAL idt_load_asm
idt_load_asm:
    lidt[idt_descriptor]
    sti
    ret


