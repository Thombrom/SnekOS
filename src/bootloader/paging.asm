;
;   For setting up paging
;   Currently identity paging
;

PAGE_TABLE_ENTRY equ 0x1000

paging_identity_setup:
    mov edi, PAGE_TABLE_ENTRY       ; Set location of our paging
    mov cr3, edi                    ; by moving it into the control register

    mov dword [edi], 0x2003         ; Pointer to second table
    add edi, 0x1000

    mov dword [edi], 0x3003
    add edi, 0x1000

    mov dword [edi], 0x4003
    add edi, 0x1000

    ; At our last table, we map 512 tables
    mov ebx, 0x00000003             ; Flags, link above
    mov ecx, 512                    ; Loop 512 times
    .set_entry:
        mov dword [edi], ebx
        add ebx, 0x1000
        add edi, 8
        loop .set_entry

    mov eax, cr4                    ; Set 5th bit of controll register
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080             ; Set long mode bit
    rdmsr                           ; in the model specific
    or eax, 1 << 8                  ; register
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret
