;
;   For detecting the pressence of
;   CPUID and Long mode support
;

cpuid_detect:
    pushfd                  ; Push flags to stack
    pop eax                 ; and retrieve in eax

    mov ecx, eax            ; Store tmp value in ecx for later comparison
    xor eax, 1 << 21        ; And flip eax 21 bit

    push eax                ; which we insert to the flags
    popfd                   ; register back

    pushfd
    pop eax                 ; Check if the bit remained flipped

    push ecx                ; Restore flags value from
    popfd                   ; before by ecx tmp storage

    xor eax, ecx            ; Compare if they are the same
    jz cpuid_none           ; and if they are, it was flipped back
                            ; and we should HALT
    ret

long_mode_detect:
    mov eax, 0x80000001
    cpuid                   ; Run CPUID
    test edx, 1 << 29       ; Check if edx flipped

    jz long_mode_none       ; If bit not flipped, we do
    ret                     ; not have long mode

cpuid_none:
    mov ebx, cpuid_none_msg
    call print_str_32

    jmp $                   ; We halt

long_mode_none:
    mov ebx, long_mode_none_msg
    call print_str_32

    jmp $                   ; We halt

cpuid_none_msg:
    db "No CPUID detected", 0

long_mode_none_msg:
    db "No long mode detected", 0
