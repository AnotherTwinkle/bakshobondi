#include "util.h"

#define KERNEL_CODE_SEL 0x08
#define IDT_SIZE 256

struct IDTEntry {
	u16 low_offset; // Low 16 bits of interrupt handler
	u16 sel;        // Kernel code segment selector
	u8  null;
	u8  type;
	u16 high_offset;
} __attribute__((packed));


struct IDTRegister {
	u16 limit; 
	u32 base;    // Pointer to first array
} __attribute__((packed));


static struct {
	struct IDTEntry entries[IDT_SIZE];
	struct IDTRegister reg;
} idt;

void set_idt_entry(int idx, u32 handler);
void IDT_INIT();
