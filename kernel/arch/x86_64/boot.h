#ifndef _BOOT_H
#define _BOOT_H

/* Known addresses */
#define KERNEL_PHYSICAL_START (0x0000000000400000)
#define KERNEL_VIRTUAL_START  (0xFFFFFFFF80400000)

/* We use multiboot 2 on x86_64 */
#define MULTIBOOT2_HEADER_MAGIC 0xe85250d6

#define MULTIBOOT_ARCHITECTURE_I386  0

#define MULTIBOOT_HEADER_TAG_END 0

/* GDT Utilities */
#define GDT_FLAG_FOUR_KILOBYTE_GRANULARITY (1 << 3)
#define GDT_FLAG_32BIT_PROTECTED_MODE (1 << 2)
#define GDT_FLAG_64BIT_MODE (1 << 1)

#define GDT_ACCESS_PRESENT (1 << 7)
#define GDT_ACCESS_PRIVILEGE_RING0 (0x0 << 5)
#define GDT_ACCESS_PRIVILEGE_RING1 (0x1 << 5)
#define GDT_ACCESS_PRIVILEGE_RING2 (0x2 << 5)
#define GDT_ACCESS_PRIVILEGE_RING3 (0x3 << 5)
#define GDT_ACCESS_EXECUTABLE (1 << 3)
#define GDT_ACCESS_DIRECTION_DOWN (1 << 2)
#define GDT_ACCESS_READABLE_WRITABLE (1 << 1)

#define DECLARE_GDT_ENTRY(base, limit, flags, access)                          \
  (                                                                            \
    (((((base)) >> 24) & 0xFF) << 56) |                                        \
    ((((flags)) & 0xF) << 52) |                                                \
    (((((limit)) >> 16) & 0xF) << 48) |                                        \
    (((((access) | (1 << 4))) & 0xFF) << 40) |                                 \
    ((((base)) & 0xFFF) << 16) |                                               \
    (((limit)) & 0xFFFF)                                                       \
  )

#define GDT_FIRST_ENTRY 0

#define GDT_KERNEL_ENTRY                                                       \
  DECLARE_GDT_ENTRY(0, 0,                                                      \
                    GDT_FLAG_64BIT_MODE,                                       \
                    GDT_ACCESS_PRESENT |                                       \
                    GDT_ACCESS_PRIVILEGE_RING0 |                               \
                    GDT_ACCESS_EXECUTABLE)                                     \

#define GDT_TABLE_ALIGNMENT 4
#define GDT_TABLE_SIZE 0x800

/* MMU Utilities */

#define MMU_PRESENT (1 << 0)
#define MMU_WRITABLE (1 << 1)
#define MMU_PD_2MiB (1 << 7)

/* CR Utilities */

/* PM-Mode | FPU | Paging */
#define CR0_REG ((1 << 0) | (1 << 4) | (1 << 31))

/* PAE */
#define CR4_REG (1 << 5)

/* MSR Utilities */

#define MSR_EFER (0xC0000080)
/* Long Mode */
#define MSR_EFER_LM (1 << 8)

#endif
