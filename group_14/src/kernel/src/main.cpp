
#include "system.h"
#define GDT_ENTRY_NULL 0
#define GDT_ENTRY_DATA 1
#define GDT_ENTRY_TEXT 2

// Define entry point in asm to prevent C++ mangling
extern "C"{
    void kernel_main();
}

// Function in the code segment
void my_function() {
    printf("Hello World!");
}

// Global variable in the data segment
int my_data_var = 42;

void kernel_main()
{
    struct GDT_Entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct GDT_Pointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct GDT_Entry gdt_entries[] = {
    // Null descriptor
    {
        .limit_low = 0,
        .base_low = 0,
        .base_middle = 0,
        .access = 0,
        .granularity = 0,
        .base_high = 0
    },
    // Data descriptor
    {
        .limit_low = 0xFFFF,
        .base_low = 0,
        .base_middle = 0,
        .access = 0b10011010,  // Set the present, ring level, and data type bits
        .granularity = 0b11001111,  // Set the granularity and segment size bits
        .base_high = 0
    },
    // Text descriptor
    {
        .limit_low = 0xFFFF,
        .base_low = 0,
        .base_middle = 0,
        .access = 0b10001010,  // Set the present, ring level, and code type bits
        .granularity = 0b11001111,  // Set the granularity and segment size bits
        .base_high = 0
    }
};

struct GDT_Pointer gdt_ptr = {
    .limit = sizeof(gdt_entries) - 1,
    .base = (uint32_t)&gdt_entries[0]
};

void gdt_load() {
    asm volatile("lgdt %0" : : "m" (gdt_ptr));
}

gdt_set_entry(1, 0, 0xFFFFFFFF, GDT_ACCESS_RW, GDT_FLAG_SEGMENT | GDT_FLAG_32BIT);
gdt_set_entry(2, 0, 0xFFFFFFFF, GDT_ACCESS_RX, GDT_FLAG_SEGMENT | GDT_FLAG_32BIT);



gdt_load();
my_function();
my_data_var = 123;

for (;;) {}

}
