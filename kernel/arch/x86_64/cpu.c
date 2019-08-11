#include <builtin/cpuid.h>
#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/kernel.h>
#include <lunaros/compiler.h>
#include <lunaros/printf.h>
#include <lunaros/cpu.h>

static __inline int has_apic(void) {
   uint32_t eax, ebx, ecx, edx;
   if (__get_cpuid(0x01, &eax, &ebx, &ecx, &edx))
      return edx & (1 << 9);
   return 0;
}

static __inline void brand(void) {
   uint32_t eax, ebx, ecx, edx;
   if (__get_cpuid(0x0, &eax, &ebx, &ecx, &edx))
      printf("%s%s%s\n", (char *)&ebx, (char *)&edx, (char *)&ecx);
}

void cpu_dofeatures(void) {
   brand();
   if (unlikely(!has_apic()))
      panic("CPU doesn't support APIC");
}
