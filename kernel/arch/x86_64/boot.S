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

.global _physical_end
_physical_end:
   .zero 4

.section .bss
   .comm pml4, 0x1000, 0x1000
   .comm low_pdpte, 0x1000, 0x1000
   .comm high_pdpte, 0x1000, 0x1000
   .comm high_pde, 0x1000, 0x1000
   .comm low_pde, 0x1000, 0x1000
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

.local page_init
.type page_init, @function
page_init:
   pushl   %ebp
   movl    %esp, %ebp
   subl    $8, %esp

   movl $low_pdpte, %eax
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   /* PML4[0] = %eax */
   movl %eax, pml4 + 0

   movl $high_pdpte, %eax
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   /* PML4[511] = %eax */
   movl %eax, pml4 + (511 * 8)

   movl $low_pde, %eax
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   /* LOW_PDPTE[0] = %eax */
   movl %eax, low_pdpte + 0

   movl $high_pde, %eax
   orl $(MMU_PRESENT | MMU_WRITABLE), %eax
   /* HIGH_PDPTE[510] = %eax */
   movl %eax, high_pdpte + (510 * 8)

   leave
   ret

.local mmap_kernel
.type mmap_kernel, @function
mmap_kernel:
   pushl   %ebp
   movl    %esp, %ebp
   subl    $8, %esp

   /* loop counters */
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
   orl $(MMU_PRESENT | MMU_WRITABLE | MMU_PD_2MiB), %eax
   movl %eax, low_pde(, %ecx, 8)
   movl %eax, high_pde(, %ecx, 8)

   inc %ecx
   cmp %esi, %ecx
   jne map_kernel

   leave
   ret

.local cpu_setpaging
.type cpu_setpaging, @function
cpu_setpaging:
   pushl   %ebp
   movl    %esp, %ebp
   subl    $8, %esp

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

   leave
   ret

halt32:
   cli
   hlt
1:
   jmp 1b

/* Linker ENTRY */
.global _start
.type _start, @function
_start:
   movl $(stack + 0x4000), %esp

   /* Save the Multiboot information structure */
   pushl %ebx
   pushl %eax

   /* Check if 64-bit is supported */
   movl $0x80000001, %eax
   cpuid
   and $(1 << 29), %edx

   cmp $0, %edx
   je halt32

   call page_init
   call mmap_kernel
   call cpu_setpaging

   /* Recover multiboot2 parameters */
   pop %edi
   pop %esi


   lgdt GDTptr

   /* jump into 64-bit */
   ljmp $8, $_start64

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

   movq $_kernel_physical_end, %r10
   movq %r10, (_physical_end)

   call main

   cli
   hlt
1:
   jmp 1b
