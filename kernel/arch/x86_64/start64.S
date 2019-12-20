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

   /* Unmap identity mappings */
   movq $0, pml4 + 0

   /* Reload cr3 and flush TLB */
   movq %cr3, %rax
   movq %rax, %cr3

   /* Clear segment selectors */
   movw $0, %ax
   movw %ax, %ds
   movw %ax, %es
   movw %ax, %fs
   movw %ax, %gs
   movw %ax, %ss

   /* Reserved physical pages */
   movq $_kernel_physical_end, %rdx

   /* Multiboot info to higher half */
   addq %r11, %rsi

   /*
   ** %rdi - multiboot magic
   ** %rsi - multiboot address
   ** %rdx - reserved physical pages offset
   */
   call main

   cli
   hlt
1:
   jmp 1b
