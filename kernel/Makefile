# Makefile for compiling the LunarOS Kernel

# == END OF USER SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE =======

INCLUDEDIR:= include/
ARCHDIR:= arch/$(ARCH)
KERNDIR:= kernel
LIBDIR:= lib

# Defines $(OBJS) for the whole tree!
include $(ARCHDIR)/Kbuild
include $(KERNDIR)/Kbuild
include $(LIBDIR)/Kbuild

FOPS:= -mcmodel=kernel -ffreestanding -mno-red-zone -fno-builtin -fno-pie -no-pie
CFLAGS:= $(CFLAGS) $(FOPS) -nostdlib
ASFLAGS:= $(CFLAGS) $(ASFLAGS)
CPPFLAGS:= $(CPPFLAGS) -I$(INCLUDEDIR) -D_KERNEL
LDFLAGS:= $(LDFLAGS) -Wl,--build-id=none
LIBS:= $(LIBS)

all: lunar.kernel

lunar.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -z max-page-size=0x1000 $(CFLAGS) $(LDFLAGS) -T $(ARCHDIR)/linker.ld -o lunar.kernel $(OBJS) $(LIBS)
	grub-file --is-x86-multiboot2 lunar.kernel

.c.o:
	$(CC) -std=gnu99 $(CFLAGS) $(CFLAGS-$@) $(CPPFLAGS) $(LDFLAGS) -MD -c $< -o $@ $(LIBS)

.S.o:
	$(CC) $(ASFLAGS) $(ASFLAGS-$@) $(CPPFLAGS) $(LDFLAGS) -MD -c $< -o $@ $(LIBS)

clean:
	rm -f lunar.kernel
	rm -f $(OBJS) $(OBJS:.o=.d)

-include $(OBJS:.o=.d)

.PHONY: clean
.SUFFIXES: .o .c .S

# (end of Makefile)
