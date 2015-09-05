# Makefile for the Sworm OS kernel.

# Note: if `make' fails with "Error 1" after
# 	[ GRUB ]  SwormOS.iso
# Then you can view grub-mkrescue.log which contains the redirected output of
# `grub-mkrescue'. By default, that file is deleted if `grub-mkrescue' ran
# without errors.

# This Makefile relies on implicit targets, so don't be surprised if you
# don't see something like:
# 	%.o: %.c
# 		$(CC) $(CFLAGS) $< -o $@
# If you want to know more, read the Make Manual at http://www.gnu.org/:
# 	https://www.gnu.org/software/make/manual/make.pdf.


# Cross-compiler prefix.
CROSS_PREFIX ?= i686-elf-

# The system root.
SYSROOT := ../sysroot

# C/C++ compiler.
CC = @echo " [ CC  ] " $@ ; $(CROSS_PREFIX)gcc
CXX = @echo " [ CC  ] " $@ ; $(CROSS_PREFIX)g++
CFLAGS := -c -ffreestanding -Wall -Wextra -std=c99
CPPFLAGS := -Iinclude/ --sysroot=$(SYSROOT) -isystem=/usr/include -D__is_kernel
# Assembler
AS = @echo " [ AS  ] " $@ ; $(CROSS_PREFIX)gcc
ASFLAGS := $(CFLAGS)

# Linker.
LD = @echo " [ LD  ] " $@ ; $(CROSS_PREFIX)g++
LDFLAGS := -ffreestanding -Wall -Wextra -Werror -T linker.ld\
	--sysroot=$(SYSROOT)
LDLIBS := -lk -lgcc -nostdlib

# Source files to compile.
SRC := $(wildcard ./*.[sc] ./*/*.[sc])
SRC += $(wildcard ./*.cc ./*/*.cc)

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


all: $(OUTPUT) $(ISOFILE)

$(OUTPUT): $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(ISOFILE): $(OUTPUT) $(ISODIR)/boot/grub | install
	@echo " [GRUB ] " $@
	@cp grub.cfg $(ISODIR)/boot/grub/
	@grub-mkrescue -o $@ $(ISODIR) 1>grub-mkrescue.log 2>&1 \
		&& rm grub-mkrescue.log

$(ISODIR)/boot/grub:
	@mkdir -p $@

install: $(INSTALLDIR)/$(OUTPUT)

$(INSTALLDIR)/$(OUTPUT): $(OUTPUT)
	@echo " [ CP  ] " $< ; cp $< $@

clean:
	rm -rf $(wildcard ./*.o ./*/*.o) $(OUTPUT) $(ISOFILE)
