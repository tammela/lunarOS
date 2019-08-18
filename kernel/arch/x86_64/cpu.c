#include <builtin/cpuid.h>
#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/kernel.h>
#include <lunaros/compiler.h>
#include <lunaros/printf.h>
#include <lunaros/cpu.h>

#define CPUID_MIN_EXT_LEAF (0x80000008)

static __inline int cpuid_hasapic(void) {
   uint32_t eax, ebx, ecx, edx;
   if (__get_cpuid(0x01, &eax, &ebx, &ecx, &edx))
      return edx & (1 << 9);
   return 0;
}

static __inline void brand(void) {
   uint32_t eax, ebx, ecx, edx;
   if (__get_cpuid(0x0, &eax, &ebx, &ecx, &edx))
      printf("%s%s%s\n", (char *)&ebx, (char *)&edx, (char *)&ecx);
   else
      panic("CPUID not supported");
}

static __inline int cpuid_maxext(void) {
   uint32_t max = __get_cpuid_max(0x80000000, NULL); /* max ext leaf */
   return max >= CPUID_MIN_EXT_LEAF;
}

void cpu_dofeatures(void) {
   brand();
   if (unlikely(!cpuid_maxext()))
      panic("Extended cpuid information lacks crucial leafs");
   if (unlikely(!cpuid_hasapic()))
      panic("CPU doesn't support APIC");
}

static __inline uint32_t cpuid_extaddress(void) {
   uint32_t ebx, ecx, edx;
   uint32_t eax = 0;
   __get_cpuid(0x80000008, &eax, &ebx, &ecx, &edx);
   return eax;
}

void cpu_getinfo(struct cpu *cpu) {
   uint32_t ret = cpuid_extaddress();
   cpu->pmax = ret & 0xFF;
   cpu->lmax = (ret & 0xFF00) >> 8;
}
