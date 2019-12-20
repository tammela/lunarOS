# Makefile for compiling the LunarOS Kernel

CFLAGS+= -flto

OUTPUT:= lunar.kernel

# == END OF USER SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE =======

INCLUDEDIR:= include/
ARCHDIR:= arch/$(ARCH)
KERNDIR:= kernel
LIBDIR:= lib

# Defines $(OBJS) for the whole tree!
include $(ARCHDIR)/Kbuild
include $(KERNDIR)/Kbuild
include $(LIBDIR)/Kbuild

FOPS:= -mcmodel=large -ffreestanding -mno-red-zone -fno-builtin -fno-pie -no-pie -mgeneral-regs-only
CFLAGS:= $(CFLAGS) $(FOPS) -nostdlib
ASFLAGS:= $(CFLAGS) $(ASFLAGS)
CPPFLAGS:= $(CPPFLAGS) -I$(INCLUDEDIR) -D_KERNEL -D__$(ARCH)__
LDFLAGS:= $(LDFLAGS) -Wl,--build-id=none
LIBS:= $(LIBS)

all: $(OUTPUT)

$(OUTPUT): $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(ARCHDIR)/linker.ld -o $@ $(OBJS) $(LIBS)
	$(call ARCH_CHECKS, $@)
.c.o:
	$(CC) -std=gnu99 $(CFLAGS) $(CFLAGS-$@) $(CPPFLAGS) $(LDFLAGS) -MD -c $< -o $@ $(LIBS)

.S.o:
	$(CC) $(ASFLAGS) $(ASFLAGS-$@) $(CPPFLAGS) $(LDFLAGS) -MD -c $< -o $@ $(LIBS)

clean:
	@rm -f lunar.kernel
	rm -f $(OBJS) $(OBJS:.o=.d)

-include $(OBJS:.o=.d)

.PHONY: clean
.SUFFIXES: .o .c .S

# (end of Makefile)
