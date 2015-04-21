# Makefile for the C library used in the jk1 kernel.
# Most of the C library is taken from the musl libc <http://www.musl-libc.org/>.

# Warning: DO NOT execute the rules `all' and `install' at the same time (e.g.
# make all install), because there is a symbolic link that is created in one
# of the recipes ($(BITSDIR)), and needs to be a prerequisite of
# install-headers, but since it is created at runtime and `make' calculates
# prerequisites BEFORE executing any recipe, that symbolic link is not going
# to the prerequisites, so is not installed into the sysroot. To fix that,
# you can either run `make all install' and then `make install' again, or just
# run `make all' and then `make install', so that `make' puts the header files
# in that symlink directory ($(BITSDIR)) into the prerequisites of the
# `install-headers' target. The same thing is about libc.c and libk.a.

# This Makefile relies on implicit targets, so don't be surprised if you
# don't see something like:
# 	%.o: %.c
# 		$(CC) $(CFLAGS) $< -o $@
# If you want to know more, read the Make Manual at http://www.gnu.org:
# 	https://www.gnu.org/software/make/manual/make.pdf.

# Target architecture.
ARCH ?= ia32
ARCHDIR := arch/$(ARCH)

# Cross-compiler prefix.
CROSS_PREFIX ?= i686-elf-

# C compiler.
CC = @echo " [ CC  ] " $@ ; $(CROSS_PREFIX)gcc
CFLAGS := -std=c99 -c -Wall -Wextra -Werror -E
CPPFLAGS := -Iinclude/ -D__is_libc
KCFLAGS := -std=c99 -c -ffreestanding -Wall -Wextra -Werror
KCPPFLAGS := -Iinclude/ -D__is_kernel

# Assembler
AS = @echo " [ AS  ] " $@ ; $(CROSS_PREFIX)gcc
ASFLAGS := $(CFLAGS)

# Linker.
LD = @echo " [ LD  ] " $@ ; $(CROSS_PREFIX)gcc
LDFLAGS := -ffreestanding -Wall -Wextra -Werror -T $(ARCHDIR)/linker.ld
LDLIBS := -lgcc -nostdlib

# sed (see the $(ALLTYPES_H) target).
SED = @echo " [ SED ] " $@ ; sed
# ln (see the $(BITSDIR) target).
LN = @echo " [ LN  ] " $@ ; ln
# ar (see the $(OUTPUT) and $(KOUTPUT) targets).
AR = @echo " [ AR  ] " $@ ; ar

# Source files to compile.
SRC := $(wildcard src/*/*.c)

# Object files.
OBJ := $(subst .c,.o,$(SRC))
KOBJ := $(subst .c,.k.o,$(SRC))
# Note that there are no object files for specific architecture.

# C library for the userspace.
OUTPUT := libc.a
# Kernel version (without sys. calls) of the C library.
KOUTPUT := libk.a

# Directory with tools.
TOOLSDIR := tools

# The system root.
SYSROOT := ../sysroot

# Where to install the header files.
HEADERSDIR := $(SYSROOT)/usr/include
# Where to install the libraries (e.g. libc.a).
LIBSDIR := $(SYSROOT)/usr/lib

# This is a header file that is produced from two .in files using `sed'.
ALLTYPES_H := include/bits/alltypes.h
ALLTYPES_H_IN := $(ARCHDIR)/bits/alltypes.h.in include/alltypes.h.in

# Directory that is symlinked to $(BITSDIR).
ARCHBITSDIR := $(ARCHDIR)/bits
BITSDIR := include/bits

# Phony targets.
.PHONY: all install install-headers install-libs clean

# Files (targets) to delete on error.
.DELETE_ON_ERROR: $(ALLTYPES_H) $(BITSDIR)

all: $(BITSDIR) $(OUTPUT) $(KOUTPUT)

$(OUTPUT): $(ALLTYPES_H) $(OBJ)
	$(AR) rcs $@ $(OBJ)

$(KOUTPUT): $(ALLTYPES_H) $(KOBJ)
	$(AR) rcs $@ $(KOBJ)

%.k.o: %.c
	$(CC) $(KCFLAGS) $(KCPPFLAGS) $< -o $@

$(ALLTYPES_H): $(BITSDIR)
	$(SED) -f $(TOOLSDIR)/mkalltypes.sed $(ALLTYPES_H_IN) > $@

$(BITSDIR): $(ARCHBITSDIR)
	$(LN) -sf ../$< $@

install: all install-headers install-libs

install-headers: $(subst include/,$(HEADERSDIR)/,$(wildcard include/*.h)) \
				$(subst include/,$(HEADERSDIR)/,$(wildcard include/*/*.h))\
				| $(BITSDIR) $(HEADERSDIR)

$(HEADERSDIR)/%.h: include/%.h
	@mkdir -p $(dir $@)
	@echo " [ CP  ] " $< ; cp $< $@

$(HEADERSDIR):
	@mkdir -p $(HEADERSDIR)

install-libs: $(addprefix $(LIBSDIR)/,$(wildcard *.a))

$(LIBSDIR)/%.a: %.a
	@mkdir -p $(dir $@)
	@echo " [ CP  ] " $< ; cp $< $@

clean:
	rm -rf $(ALLTYPES_H) $(BITSDIR) $(OUTPUT) $(KOUTPUT) $(wildcard src/*/*.o) $(SYSROOT)
