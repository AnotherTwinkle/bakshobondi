#include "fpu.h"

void FPU_INIT() {
    unsigned long t;

    // Load CR0 into eax
    asm volatile(
        "clts\n"
        "mov %%cr0, %%eax\n"
        "mov %%eax, %0\n"
        : "=r"(t)        // output to C variable
        :
        : "eax"
    );

    t &= ~(1 << 2);
    t |=  (1 << 1);

    // Write back to CR0
    asm volatile(
        "mov %0, %%eax\n"
        "mov %%eax, %%cr0\n"
        :
        : "r"(t)
        : "eax"
    );

    // Load CR4
    asm volatile(
        "mov %%cr4, %%eax\n"
        "mov %%eax, %0\n"
        : "=r"(t)
        :
        : "eax"
    );

    t |= 3 << 9;

    // Write back to CR4
    asm volatile(
        "mov %0, %%eax\n"
        "mov %%eax, %%cr4\n"
        :
        : "r"(t)
        : "eax"
    );

    // Initialize FPU
    asm volatile("fninit");
}
