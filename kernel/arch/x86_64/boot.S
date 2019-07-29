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

/* 4-level page walkthrough:
 * PML4 -> PDPTR -> PD -> PT -> Page Frame
 */
.section .bss
   .comm pml4, 0x1000, 0x1000
   .comm low_pdptr, 0x1000, 0x1000
   .comm high_pdptr, 0x1000, 0x1000
   .comm high_pd, 0x1000, 0x1000
   .comm low_pd, 0x1000, 0x1000
   .comm tmp_stack, 0x4000, 0x1000

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

/* Linker ENTRY */
.section .text
.global _start
.type _start, @function
_start:
   /* Set up the stack */
   movl $(tmp_stack + 0x4000), %esp

   /* Push the pointer to the Multiboot information structure */
   pushl %ebx
   /* Push the magic value */
   pushl %eax

   /* ==={ */
   /* Populate PML4 */

   movl $low_pdptr, %eax
   /* Supervisor is set by default */
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   /* PML4[0] = %eax */
   movl %eax, pml4 + 0

   movl $high_pdptr, %eax
   /* Supervisor is set by default */
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   /* PML4[511] = %eax */
   movl %eax, pml4 + (511 * 8)

   /* Populate PD */

   movl $low_pd, %eax
   /* Supervisor is set by default */
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   /* LOW_PDPTR[0] = %eax */
   movl %eax, low_pdptr + 0

   movl $high_pd, %eax
   /* Supervisor is set by default */
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   /* HIGH_PDPTR[510] = %eax */
   movl %eax, high_pdptr + (510 * 8) /* ??? */
   /* }=== */

   /* ==={ */
   /* map_kernel loop counters */
   movl $0, %ecx
   movl $_kernel_physical_end, %esi
   /* We will initially map the kernel in 2MiB pages */
   /* %esi = npages */
   shrl $21, %esi
   addl $1, %esi

   /* Does both an identity map and a high map */
map_kernel:
   movl $(1 << 21), %eax
   mul %ecx
   /* Supervisor is set by default */
   orl $(MMU_PRESENT | MMU_WRITABLE | MMU_PD_2MiB), %eax
   movl %eax, low_pd(, %ecx, 8)
   movl %eax, high_pd(, %ecx, 8)

   inc %ecx
   cmp %esi, %ecx
   jne map_kernel
   /* }=== */

   /* ==={ */
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
   /* }=== */

   /* Recover multiboot2 parameters */
   pop %edi
   pop %esi

   /* Load GDT */
   lgdt GDTptr

   /* ljmp into 64-bit */
   /* Kernel is GDT[1] */
   ljmp $(1 * 8), $_start64

.code64

.global _start64
.type _start64, @function
_start64:
   /* Clear segment selectors */
   movw $0, %ax
   movw %ax, %ds
   movw %ax, %es
   movw %ax, %fs
   movw %ax, %gs
   movw %ax, %ss

   call main

   /* Never happens */
   cli
   hlt
1:
   jmp 1b
