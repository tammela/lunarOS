# Makefile for compiling the LunarOS Kernel

# == CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT =======================

# Your platform. See PLATS for possible values.
PLAT= none

export CC= gcc
export CFLAGS= -O2 -Wall -Wextra -Werror
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

test:
	@echo "TODO"

none:
	@echo "Please do 'make PLATFORM' where PLATFORM is one of these:"
	@echo "   $(PLATS)"

# list targets that do not create files
.PHONY: all $(PLATS) clean test none

# (end of Makefile)
