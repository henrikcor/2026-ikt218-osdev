#include "arch/i386/gdt.h"

void main(void) {
    gdt_install();

    volatile char *video = (volatile char *)0xB8000;
    video[0] = 'O';
    video[1] = 0x07;
    video[2] = 'K';
    video[3] = 0x07;

    for (;;) {}
}
