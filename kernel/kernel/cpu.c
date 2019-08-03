#include <builtin/cpuid.h>
#include <std/stddef.h>
#include <std/stdint.h>

#include <lunaros/cpu.h>
#include <lunaros/printf.h>
#include <lunaros/tty.h>

void cpu_init(void) {
   uint32_t eax, ebx, ecx, edx;
   if (__get_cpuid(0x0, &eax, &ebx, &ecx, &edx))
      printf("%s%s%s\n", (char *)&ebx, (char *)&edx, (char *)&ecx);
}
