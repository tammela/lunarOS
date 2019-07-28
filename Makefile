# Makefile for compiling the LunarOS Kernel

# == CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT =======================

# Your platform. See PLATS for possible values.
PLAT= none

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
all:	$(PLAT)

$(PLATS):
	$(shell echo "$@" > .cache-arch)
	@cd kernel/ && $(MAKE) ARCH="$@"

clean:
	@cd kernel/ && $(MAKE) ARCH=$(shell cat .cache-arch) $@
	@rm .cache-arch

iso:
	@mkdir -p isodir/boot/grub
	@cp kernel/lunar.kernel isodir/boot/
	@cp grub.cfg isodir/boot/grub
	grub-mkrescue -o lunaros.iso isodir

qemu:
	qemu-system-$(shell cat .cache-arch) -enable-kvm -cpu host -cdrom lunaros.iso -m 1024M

qemu-debug:
	qemu-system-$(shell cat .cache-arch) -enable-kvm -cpu host -cdrom lunaros.iso -m 1024M -s -S
none:
	@echo "Please do 'make PLATFORM' where PLATFORM is one of these:"
	@echo "   $(PLATS)"

# list targets that do not create files
.PHONY: all $(PLATS) clean qemu none

# (end of Makefile)
