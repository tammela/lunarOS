#include <builtin/cpuid.h>
#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/compiler.h>
#include <lunaros/cpu.h>
#include <lunaros/kernel.h>
#include <lunaros/printf.h>
#include <lunaros/x86.h>

#include <arch/x86/msr.h>
#include <arch/x86/mtrr.h>

#define CPUID_MIN_EXT_LEAF (0x80000008)

static __inline int cpuid_maxext(void) {
   uint32_t max = __get_cpuid_max(0x80000000, NULL); /* max ext leaf */
   return max >= CPUID_MIN_EXT_LEAF;
}

static __inline uint32_t cpuid_extaddress(void) {
   uint32_t ebx, ecx, edx;
   uint32_t eax = 0;
   __get_cpuid(0x80000008, &eax, &ebx, &ecx, &edx);
   return eax;
}

static __inline int cpuid_hasapic(void) {
   uint32_t eax, ebx, ecx, edx;
   if (__get_cpuid(0x01, &eax, &ebx, &ecx, &edx))
      return edx & (1 << 9);
   return 0;
}

static __inline int cpuid_hasmtrr(void) {
   uint32_t ebx, ecx, edx;
   uint32_t eax = 1;
   if (__get_cpuid(0x01, &eax, &ebx, &ecx, &edx))
      return edx & (1 << 12);
   return 0;
}

/*
** Check for the PAT extension. A CPU without PAT is not supported.
** PAT is used in conjunction with MTRRs for memory typing.
*/
static __inline int cpuid_haspat(void) {
   uint32_t ebx, ecx, edx;
   uint32_t eax = 1;
   if (__get_cpuid(0x01, &eax, &ebx, &ecx, &edx))
      return edx & (1 << 16);
   return 0;
}

static __inline void cpu_brand(void) {
   uint32_t eax, ebx, ecx, edx;
   if (__get_cpuid(0x0, &eax, &ebx, &ecx, &edx))
      printf("%s%s%s\n", (char *)&ebx, (char *)&edx, (char *)&ecx);
   else
      panic("CPUID not supported");
}

void cpu_getinfo(struct cpu *cpu) {
   uint32_t ret = cpuid_extaddress();
   cpu->pmax = ret & 0xFF;
   cpu->lmax = (ret & 0xFF00) >> 8;
}

/*
** Setup the MTRRs to WB and let PAT do the memory typing.
*/
void cpu_setupmem(void) {
   /* Also disabling fixed range MTRRs */
   wrmsr(IA32_MTRR_DEF_TYPE, MTRR_ENABLE_WB);
   /* Clear all variable ranges */
   wrmsr(IA32_MTRR_PHYBASE0, 0);
   wrmsr(IA32_MTRR_PHYSMASK0, 0);
   wrmsr(IA32_MTRR_PHYBASE1, 0);
   wrmsr(IA32_MTRR_PHYSMASK1, 0);
   wrmsr(IA32_MTRR_PHYBASE2, 0);
   wrmsr(IA32_MTRR_PHYSMASK2, 0);
   wrmsr(IA32_MTRR_PHYBASE3, 0);
   wrmsr(IA32_MTRR_PHYSMASK3, 0);
   wrmsr(IA32_MTRR_PHYBASE4, 0);
   wrmsr(IA32_MTRR_PHYSMASK4, 0);
   wrmsr(IA32_MTRR_PHYBASE5, 0);
   wrmsr(IA32_MTRR_PHYSMASK5, 0);
   wrmsr(IA32_MTRR_PHYBASE6, 0);
   wrmsr(IA32_MTRR_PHYSMASK6, 0);
   wrmsr(IA32_MTRR_PHYBASE7, 0);
   wrmsr(IA32_MTRR_PHYSMASK7, 0);
   wrmsr(IA32_MTRR_PHYBASE8, 0);
   wrmsr(IA32_MTRR_PHYSMASK8, 0);
   wrmsr(IA32_MTRR_PHYBASE9, 0);
   wrmsr(IA32_MTRR_PHYSMASK9, 0);
   /*
   ** Intel documentation says that if no match occurs
   ** the default type is used. We force this behavior
   ** while keeping the MTRRs active.
   */
}

/*
** Can't boot without these...
*/
void cpu_checkfeatures(void) {
   cpu_brand();
   if (unlikely(!cpuid_maxext()))
      panic("Extended cpuid information lacks crucial leafs");
   if (unlikely(!cpuid_hasapic()))
      panic("CPU doesn't support APIC");
   if (unlikely(!cpuid_hasmtrr()))
      panic("CPU doesn't support MTRRs");
   if (unlikely(!cpuid_haspat()))
      panic("CPU doesn't support Page Attribute Table (PAT)");
}
