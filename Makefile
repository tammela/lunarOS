# Makefile for compiling the LunarOS Kernel

# == CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT =======================

ISO=lunarOS.iso

# Your architecture. See PLATS for possible values.
ARCH= none

export CC= gcc
export CFLAGS= -O0 -Wall -Wextra -Werror -g
export ASFLAGS= -Wa,--divide

# == END OF USER SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE =======

export SRC= $(realpath .)

# Convenience platforms targets.
PLATS= x86_64

# Kernel version and release.
V= 0.1
R= $V.1

# Targets start here.
all:	$(ARCH)

$(PLATS):
	@cd kernel/ && $(MAKE)

clean:
	@cd kernel/ && $(MAKE) $@
	@rm -rf isodir

iso: $(ISO)

$(ISO): $(ARCH)
	@mkdir -p isodir/boot/grub
	@cp kernel/lunar.kernel isodir/boot/
	@cp grub.cfg isodir/boot/grub
	grub-mkrescue -o lunarOS.iso isodir

qemu: $(ISO)
	qemu-system-$(shell cat .cache-arch) -cpu qemu64 -cdrom lunarOS.iso -m 1024M

qemu-debug: $(ISO)
	gdb -x kernel/scripts/kernel-$(shell cat .cache-arch).gdb
none:
	@echo "Please do 'make ARCH=PLATFORM' where PLATFORM is one of these:"
	@echo "   $(PLATS)"

# list targets that do not create files
.PHONY: all $(PLATS) clean none

# (end of Makefile)
