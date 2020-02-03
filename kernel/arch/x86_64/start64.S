#include "boot.h"

.code64

.section .text
.global _start64
.type _start64, @function
_start64:
   /* Higher half linear offset */
   movq $0xFFFF800000000000, %r11

   /* Higher half virtual stack */
   addq %r11, %rsp

   /* Reuse tables */
   movq $pdpte, %rax
   orq $(MMU_PRESENT | MMU_WRITABLE), %rax
   movq %rax, pml4 + (257 * 8)

   movq $pde, %rax
   orq $(MMU_PRESENT | MMU_WRITABLE), %rax
   movq %rax, pdpte + 0

   movq $pte, %rax
   orq $(MMU_PRESENT | MMU_WRITABLE), %rax
   movq %rax, pde + 0

   /* Unmap identity mappings */
   movq $0, pml4 + 0
   movq $1, %rcx
L2:
   movq $0, pde(, %rcx, 8)
   inc %rcx
   cmp $512, %rcx
   jne L2

   /* flush TLB */
   movq %cr3, %rax
   movq %rax, %cr3

   /* Clear segment selectors */
   movw $0, %ax
   movw %ax, %ds
   movw %ax, %es
   movw %ax, %fs
   movw %ax, %gs
   movw %ax, %ss

   /* Reserved physical pages offset */
   movq $_kernel_physical_end, %rdx

   /* 2MiB virtually mapped area */
   movq $pte, %rcx
   addq %r11, %rcx

   /* Multiboot info to higher half */
   addq %r11, %rsi

   /*
   ** %rdi - multiboot magic
   ** %rsi - multiboot address
   ** %rdx - reserved physical pages offset
   ** %rcx - reserved virtually mapped area
   */
   call main

   cli
   hlt
1:
   jmp 1b
