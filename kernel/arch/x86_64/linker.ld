ENTRY(_start)

CACHE_ALIGNMENT = 64;

SECTIONS
{
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

  __boot_end = .;
  . += 0xFFFF800000000000;
  __text_start = .;

  .text ALIGN (4K) :
   AT(__boot_end)
  {
    *(.text)
    *(.text.*)
    *(.stub)
  }

  __text_end = .;
  __rodata_start = .;

  .rodata ALIGN (4K) :
   AT ((LOADADDR (.text) + SIZEOF (.text) + 0xFFF) & 0xFFFFFFFFFFFFF000)
  {
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(CACHE_ALIGNMENT);
    __CTOR_LIST__ = .;
    *(.ctors)
    __CTOR_END__ = .;
  }

  __rodata_end = .;
  __data_start = .;

  .data ALIGN (4K) :
   AT ((LOADADDR (.rodata) + SIZEOF (.rodata) + 0xFFF) & 0xFFFFFFFFFFFFF000)
  {
    *(.data)
  }
  .data.cacheline_aligned ALIGN (CACHE_ALIGNMENT) :
   AT ((LOADADDR (.data) + SIZEOF(.data) + 0xFFF) & 0xFFFFFFFFFFFFF000)
  {
    *(.data.cacheline_aligned)
  }
  .data.read_mostly ALIGN (CACHE_ALIGNMENT) :
   AT ((LOADADDR (.data.cacheline_aligned) + SIZEOF(.data.cacheline_aligned) + 0xFFF) & 0xFFFFFFFFFFFFF000)
  {
    *(.data.read_mostly)
  }

  __data_end = .;
  __bss_start = .;

  .bss ALIGN (4K) :
   AT ((LOADADDR (.data.read_mostly) + SIZEOF (.data.read_mostly) + 0xFFF) & 0xFFFFFFFFFFFFF000)
  {
    *(.bss)
    *(.bss.*)
    *(COMMON)
  }

  __bss_end = .;
  __kernel_end = (LOADADDR (.bss) + SIZEOF (.bss) + 0xFFF) & 0xFFFFFFFFFFFFF000;
}
