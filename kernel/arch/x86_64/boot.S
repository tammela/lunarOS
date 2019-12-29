#define ASM_FILE
#include <multiboot/multiboot2.h>

#include "boot.h"

.SET HEADER_LENGTH, header_end - header_start
.SET CHECKSUM, -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + HEADER_LENGTH)
.section .multiboot
header_start:
    .long MULTIBOOT2_HEADER_MAGIC
    .long MULTIBOOT_ARCHITECTURE_I386
    .long HEADER_LENGTH
    .long CHECKSUM

    .short MULTIBOOT_HEADER_TAG_END
    .short 0
    .long 8
header_end:

.code32

.section .bss
   .comm pml4, 0x1000, 0x1000
   .comm pdpte2mb, 0x1000, 0x1000
   .comm pde2mb, 0x1000, 0x1000
   .comm pdpte, 0x1000, 0x1000
   .comm pde, 0x1000, 0x1000
   .comm pte, 0x1000, 0x1000
   .comm stack, 0x4000, 0x1000

.data
.align 0x1000
GDTstart:
   .8byte GDT_FIRST_ENTRY
   .8byte GDT_KERNEL_ENTRY
GDTfini:
   .skip (GDT_TABLE_SIZE - (GDTfini - GDTstart))
GDTptr:
   .short GDT_TABLE_SIZE - 1
   .long GDTstart

.section .text

halt32:
   cli
   hlt
1:
   jmp 1b

/* Linker ENTRY */
.global _start
.type _start, @function
_start:
   /* physical stack */
   movl $(stack + 0x4000), %esp

   /* Save Multiboot info */
   pushl %ebx
   pushl %eax

   /* Check if 64-bit is supported */
   movl $0x80000001, %eax
   cpuid
   and $(1 << 29), %edx

   cmp $0, %edx
   je halt32

_boot_page_init:
   movl $pdpte, %eax
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   movl %eax, pml4 + 0

   movl $pdpte2mb, %eax
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   movl %eax, pml4 + (256 * 8)

   movl $pde, %eax
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   movl %eax, pdpte + 0

   movl $pde2mb, %eax
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   movl %eax, pdpte2mb + 0

_boot_mmap_kernel:
   /* loop counters */
   movl $0, %ecx
   movl $_kernel_physical_end, %esi
   /* We will initially map the kernel in 2MiB pages */
   /* %esi = npages */
   shrl $21, %esi
   addl $1, %esi

   /* Does both an identity map and a high map */
L1:
   movl $(1 << 21), %eax
   mul %ecx
   orl $(MMU_PRESENT | MMU_WRITABLE | MMU_PD_2MiB), %eax
   movl %eax, pde(, %ecx, 8)
   movl %eax, pde2mb(, %ecx, 8)

   inc %ecx
   cmp %esi, %ecx
   jne L1

_boot_cpu_setpaging:
   /* Setup pagging */
   movl $pml4, %eax
   movl %eax, %cr3

   /* Activate paging and long mode (must be in this order!) */
   movl $CR4_REG, %eax
   movl %eax, %cr4

   movl $MSR_EFER, %ecx
   rdmsr
   orl $MSR_EFER_LM, %eax
   wrmsr

   movl $CR0_REG, %eax
   movl %eax, %cr0

   /* Recover multiboot2 parameters */
   pop %edi
   pop %esi

   lgdt GDTptr

   /* jump into 64-bit */
   ljmp $8, $_trampoline

.code64

.global _trampoline
.type _trampoline, @function
_trampoline:
   movabs $_start64, %r12
   jmp *%r12
