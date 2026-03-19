[BITS 32]


; Loads the new GDT with lgdt and reloads the CPU segment registers.
; A far jump is used to reload CS, since CS cannot be changed with mov.
; The jump specifies both a code segment selector and an offset.
global gdt_flush
extern gdtr
gdt_flush:
    lgdt [gdtr]

    mov ax, 0x10
    mov ds, ax
    mov es,ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:flush_cs

flush_cs:
    ret                 ; Returns back to C code