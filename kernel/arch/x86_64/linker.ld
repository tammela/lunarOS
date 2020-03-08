ENTRY(_start)

SECTIONS
{
   . = 4K;
  _kernel_physical_start = .;

  .boottext ALIGN(4K) :
    {
      arch/x86_64/boot.o (.multiboot)
      arch/x86_64/boot.o (.text)
    }
  .bootrodata ALIGN(4K) :
    {
      arch/x86_64/boot.o (.rodata)
    }
  .bootdata ALIGN(4K) :
    {
      arch/x86_64/boot.o (.data)
    }
  .bootbss ALIGN(4K) :
    {
      arch/x86_64/boot.o (.bss)
      arch/x86_64/boot.o (COMMON)
    }

  _boot_end = .;

  . += 0xFFFF800000000000;
  _kernel_virtual_start = .;
  .text : AT(_boot_end)
  {
    *(.text)
  }

  .rodata ALIGN (4K) : AT ( (LOADADDR (.text) + SIZEOF (.text)) )
  {
    *(.rodata)
  }

  .data ALIGN (4K) : AT ( (LOADADDR (.rodata) + SIZEOF (.rodata)) )
  {
    *(.data)
  }

  .bss ALIGN (4K) : AT ( (LOADADDR (.data) + SIZEOF (.data)) )
  {
    *(COMMON)
    *(.bss)
  }

  _kernel_virtual_end = .;

  _kernel_physical_end = (LOADADDR (.bss) + SIZEOF (.bss) + 0xFFF) & 0xFFFFFFFFFFFFF000;
}
