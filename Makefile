# Makefile for compiling the LunarOS Kernel

# == CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT =======================

# Your platform. See PLATS for possible values.
PLAT= none

export CC= gcc
export CFLAGS= -fno-builtin -O2 -nostdinc -nostdlib -ffreestanding -g -Wall -Wextra \
	-Werror -MMD -mno-red-zone \
	-mcmodel=kernel -fno-pie
export ASFLAGS= $(CFLAGS) -Wa,--divide

# Other utilities.
RM= rm -f

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
	cd kernel/ && $(MAKE)

clean:
	$(RM) $(shell find $(SRC) -name "*.o")
	$(RM) $(shell find $(SRC) -name "*.d")

test:
	@echo "TODO"

none:
	@echo "Please do 'make PLATFORM' where PLATFORM is one of these:"
	@echo "   $(PLATS)"

# list targets that do not create files
.PHONY: all $(PLATS) clean test none

# (end of Makefile)
