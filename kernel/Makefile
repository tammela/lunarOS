# Makefile for compiling the LunarOS Kernel

# == END OF USER SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE =======

ARCHDIR:= arch/$(ARCH)
KERNDIR:= kernel

# Defines $(OBJS) for the whole tree!
include $(ARCHDIR)/Kbuild
include $(KERNDIR)/Kbuild

CFLAGS:= $(CFLAGS) -mcmodel=kernel -fno-builtin -fno-pie -no-pie -ffreestanding -mno-red-zone
ASFLAGS:= $(CFLAGS) $(ASFLAGS)
CPPFLAGS:= $(CPPFLAGS) -D_KERNEL
LIBS:= $(LIBS) -nostdlib

all: lunar.kernel

lunar.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o lunar.kernel $(CFLAGS) $(OBJS)
	grub-file --is-x86-multiboot2 lunar.kernel

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(CFLAGS) $(CPPFLAGS) $(LIBS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(LIBS)

clean:
	rm -f lunar.kernel
	rm -f $(OBJS) $(OBJS:.o=.d)

-include $(OBJS:.o=.d)

.PHONY: clean
.SUFFIXES: .o .c .S

# (end of Makefile)
