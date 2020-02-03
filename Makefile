# Makefile for compiling LunarOS

# == CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT =======================

ISO=lunarOS.iso
ISODIR=.buildiso

# Your architecture. See PLATS for possible values.
ARCH= none

export CC= gcc
export CFLAGS= -Og -Wall -Wextra -Werror -g -D__DEBUG__
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

ifeq ($(ARCH), none)
clean: $(ARCH)
else
clean:
	cd kernel/ && $(MAKE) $@
	@rm -rf $(ISODIR)
	@rm -rf $(ISO)
endif

$(ISODIR):
	@mkdir -p $@/boot/grub
	@cp $(SRC)/iso/grub.cfg $@/boot/grub

$(ISO): $(ARCH) $(ISODIR)
	@cp kernel/lunar.kernel $(ISODIR)/boot/
	@grub-mkrescue -o $(ISO) $(ISODIR)

iso: $(ISO)

qemu: iso
	qemu-system-$(ARCH) -cpu qemu64 -cdrom lunarOS.iso -m 1024M

qemu-debug: iso
	gdb -x kernel/scripts/kernel-$(ARCH).gdb
none:
	@echo "Please do 'make ARCH=PLATFORM' where PLATFORM is one of these:"
	@echo "   $(PLATS)"
	@exit 1

# list targets that do not create files
.PHONY: all $(PLATS) clean none

# (end of Makefile)
