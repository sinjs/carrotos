#include <stdio.h>
#include <string.h>

#include "cpu/gdt.h"
#include "kernel/logging.h"

gdt_t gdt;

extern void gdt_flush(u32);

void gdt_setup() {
    // null segment
    gdt_entry(0, 0, 0, 0, 0, "NULL");

    // kernel code segment
    gdt_entry(1, 0, 0xffffffff, PRESENT | EXECUTABLE | READWRITE, FLAGS, "KCODE");

    // kernel data segment
    gdt_entry(2, 0, 0xffffffff, PRESENT | READWRITE, FLAGS, "KDATA");

    // user code segment
    gdt_entry(3, 0, 0xffffffff, PRESENT | EXECUTABLE | READWRITE | USER, FLAGS, "UCODE");

    // user data segment
    gdt_entry(4, 0, 0xffffffff, PRESENT | READWRITE | USER, FLAGS, "UDATA");

    // tss segment
    gdt_tss_entry(5, 0x10, 0x0);

    gdt.descriptor.offset = (u32) & gdt.entries[0];
    gdt.descriptor.size = (sizeof(gdt_entry_t) * GDT_ENTRY_COUNT) - 1;

    gdt_flush((u32) & gdt.descriptor);
}

void set_kernel_stack(u32 stack) {
    gdt.tss.esp0 = stack;
}

/* --- gdt entry setup ------------------------------------------------------ */

void gdt_entry(int index, u32 base, u32 limit, u8 access, u8 flags, string hint) {
    debug("GDT[%d:%s]\t = { BASE=%x, LIMIT=%x, ACCESS=%b, FLAGS=%b }", index, hint, base, limit, access, flags);

    gdt_entry_t *entry = &gdt.entries[index];
    entry->acces = access;
    entry->flags = flags;

    entry->base0_15 = base & 0xffff;
    entry->base16_23 = (base >> 16) & 0xff;
    entry->base24_31 = (base >> 24) & 0xff;

    entry->limit0_15 = limit & 0xffff;
    entry->limit16_19 = (limit >> 16) & 0x0f;
}

void gdt_tss_entry(int index, u16 ss0, u32 esp0) {
    gdt_entry(index, (u32) & gdt.tss, sizeof(tss_t), PRESENT | EXECUTABLE | ACCESSED, TSS_FLAGS, "TSS");
    memset(&gdt.tss, 0, sizeof(tss_t));

    tss_t *tss = &gdt.tss;

    tss->ss0 = ss0;
    tss->esp0 = esp0;

    tss->cs = 0x0b;
    tss->ss = 0x13;
    tss->ds = 0x13;
    tss->es = 0x13;
    tss->fs = 0x13;
    tss->gs = 0x13;
}

/* --- debug ---------------------------------------------------------------- */

void gdt_dump() {
    puts("\n&fGlobal descriptor table dump:\n&7");
    for (u32 i = 0; i < GDT_ENTRY_COUNT; i++) {
        puts("\t");
        gdt_entry_dump(i);
    }
    puts("\n&f");
}

void gdt_entry_dump(int index) {
    gdt_entry_t *entry = &gdt.entries[index];
    printf("%d: base=%x limite=%x access=%x flags=%x\n",
           index,
           entry->base0_15 | entry->base16_23 << 16 | entry->base24_31 << 24,
           entry->limit0_15 | entry->limit16_19 << 16,
           entry->acces, entry->flags);
}
