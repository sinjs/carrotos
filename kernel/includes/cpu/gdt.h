#pragma once
#include "types.h"
#include "utils.h"

#define GDT_ENTRY_COUNT 6

#define PRESENT    0b10010000 // Present bit. This must be 1 for all valid selectors.
#define USER       0b01100000 // Privilege, 2 bits. Contains the ring level, 0 = highest (kernel), 3 = lowest (user applications).
#define EXECUTABLE 0b00001000 // Executable bit. If 1 code in this segment can be executed, ie. a code selector. If 0 it is a data selector.
#define READWRITE  0b00000010 // Readable bit for code selectors //Writable bit for data selectors
#define ACCESSED   0b00000001

#define FLAGS      0b1100
#define TSS_FLAGS  0

typedef PACKED(struct)
{
u32	prev_tss;
u32	esp0;
u32	ss0;
u32	esp1;
u32	ss1;
u32	esp2;
u32	ss2;
u32	cr3;
u32	eip;
u32	eflags;
u32	eax;
u32	ecx;
u32	edx;
u32	ebx;
u32	esp;
u32	ebp;
u32	esi;
u32	edi;
u32	es;
u32	cs;
u32	ss;
u32	ds;
u32	fs;
u32	gs;
u32	ldt;
u16	trap;
u16	iomap_base;
} tss_t;

typedef PACKED(struct)
{
u16 size;
u32 offset;
} gdt_descriptor_t;

typedef PACKED(struct)
{
u16 limit0_15;
u16 base0_15;
u8 base16_23;
u8 acces;
u8 limit16_19 : 4;
u8 flags : 4;
u8 base24_31;
} gdt_entry_t;

typedef struct
{
    gdt_entry_t entries[GDT_ENTRY_COUNT];
    gdt_descriptor_t descriptor;
    tss_t tss;
} gdt_t;

void gdt_setup();
void gdt_entry(int index, u32 base, u32 limit, u8 access, u8 flags, string hint);
void gdt_tss_entry(int index, u16 ss0, u32 esp0);

void gdt_dump();
void gdt_entry_dump(int index);
void set_kernel_stack(u32 stack);
