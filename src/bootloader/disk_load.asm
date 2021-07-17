;
;   Routine to load sectors from
;   the disk to a memory adress
;
;   dh:     Number of sectors to load
;   dl:     Drive number
;   es:bx   Location to read to
;

disk_load:
    push dx

    mov ah, 0x02            ; BOIS read sector function
    mov al, dh              ; Amount of sectors to read
    mov ch, 0x00            ; Select cylinder 0
    mov dh, 0x00            ; Select head 0
    mov cl, 0x02            ; Select second sector (the
                            ; sector after bootsector)

    int 0x13                ; BIOS interrupt
    jc disk_read_error      ; Carry set if out of bounds

    pop dx
    cmp al, dh              ; Have we read correct number of
    jne disk_read_count_error     ; sectors (stored in al)
    ret

disk_read_error:
    mov bx, disk_read_error_msg
    call print_str
    jmp $                           ; Jump infinitely, as we could not
                                    ; read the disk content

disk_read_count_error:
    mov bx, disk_read_count_error_msg
    call print_str
    jmp $

disk_read_error_msg:
    db "Error reading disk", 0

disk_read_count_error_msg:
    db "Error reading number of sectors", 0

