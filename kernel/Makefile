# Makefile for the jk1 kernel.

# Note: if `make' fails with "Error 1" after
# 	[ GRUB ]  jk1.iso
# Then you can view grub-mkrescue.log which contains the redirected output of
# `grub-mkrescue'. By default, that file is deleted if `grub-mkrescue' ran
# without errors.

# This Makefile relies on implicit targets, so don't be surprised if you
# don't see something like:
# 	%.o: %.c
# 		$(CC) $(CFLAGS) $< -o $@
# If you want to know more, read the Make Manual at http://www.gnu.org/:
# 	https://www.gnu.org/software/make/manual/make.pdf.

# Target architecture.
ARCH ?= ia32
ARCHDIR := arch/$(ARCH)

# Cross-compiler prefix.
CROSS_PREFIX ?= i686-elf-

# The system root.
SYSROOT := ../sysroot

# C/C++ compiler.
CC = @echo " [ CC  ] " $@ ; $(CROSS_PREFIX)gcc
CXX = @echo " [ CC  ] " $@ ; $(CROSS_PREFIX)g++
CFLAGS := -c -ffreestanding -Wall -Wextra -Werror
CPPFLAGS := -Iinclude/ --sysroot=$(SYSROOT) -isystem=/usr/include -D__is_kernel

# Assembler
AS = @echo " [ AS  ] " $@ ; $(CROSS_PREFIX)gcc
ASFLAGS := $(CFLAGS)

# Linker.
LD = @echo " [ LD  ] " $@ ; $(CROSS_PREFIX)g++
LDFLAGS := -ffreestanding -Wall -Wextra -Werror -T $(ARCHDIR)/linker.ld\
	--sysroot=$(SYSROOT)
LDLIBS := -lk -lgcc -nostdlib

# Source files to compile.
SRC := $(wildcard src/*.[sc] src/*/*.[sc])
SRC += $(wildcard src/*.cc src/*/*.cc)

# Object files.
OBJ := $(subst .cc,.o,$(SRC))
OBJ := $(subst .c,.o,$(OBJ))
OBJ := $(subst .s,.o,$(OBJ))

# Compact-disk image.
ISOFILE := ../OSZin.iso
# Directory with files for the CD.
ISODIR := $(SYSROOT)

# Compiled kernel.
OUTPUT := kernel

# Where to install the kernel.
INSTALLDIR := $(SYSROOT)/boot

# Include Makefile for the target arch.
include $(ARCHDIR)/Makefile.inc

# Phony targets.
.PHONY: all install clean

all: $(OUTPUT) $(ISOFILE)

$(OUTPUT): $(ARCH_OBJ) $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(ISOFILE): $(OUTPUT) $(ISODIR)/boot/grub | install
	@echo " [GRUB ] " $@
	@cp grub.cfg $(ISODIR)/boot/grub/
	@grub2-mkrescue -o $@ $(ISODIR) 1>grub-mkrescue.log 2>&1 \
		&& rm grub-mkrescue.log

$(ISODIR)/boot/grub:
	@mkdir -p $@

install: $(INSTALLDIR)/$(OUTPUT)

$(INSTALLDIR)/$(OUTPUT): $(OUTPUT)
	@echo " [ CP  ] " $< ; cp $< $@

clean:
	rm -rf $(wildcard src/*.o src/*/*.o $(ARCHDIR)/*.o $(ARCHDIR)/*/*.o) $(OUTPUT) $(ISOFILE)
