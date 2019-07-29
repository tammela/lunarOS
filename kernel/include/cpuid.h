#if defined(__GNUC__) || defined(__clang__)
#include <cpuid.h>
#else
#error "Make sure this compiler provides cpuid.h"
#endif
