# Note, that this Makefile may have some stupid mistakes, but I don't see them. Anyway, it works as
# expected. Difference from the original Makefile:
#  1. puts objects near their source files instead of obj/ directory
#  2. creates a bin/ directory and puts there OSZin.bin and OSZin.iso
#  3. recompiles files if necessary (note that if you change header files, source files that #include
#     them won't be recompiled)
#  4. uses -Wl,-Bdynamic instead of -rdynamic ('cause the last one doesn't work with my cross-compiler)
#  5. uses -Werror which makes compiler to fail if any warning/error was produced

# OSZin.bin and .iso are there.
BINDIR := bin

# GRUB .iso image.
CDIMG := $(BINDIR)/OSZin.iso

# Cross-Compiler Toolchain Prefix.
CROSS_PREFIX := i686-elf-

# C++ Compiler.
CC := $(CROSS_PREFIX)g++
CFLAGS := -c -ffreestanding -g -O2 -Wall -Wextra -Werror -fno-omit-frame-pointer -fno-exceptions -fno-rtti -Iinclude/

# Assembler.
AS := $(CROSS_PREFIX)gcc
ASFLAGS := -c -ffreestanding -O0 -Wall -Wextra -Werror

# Linker.
LD := $(CROSS_PREFIX)g++
LDFLAGS := -T src/linker.ld -Wl,-Bdynamic -nostdlib -lgcc

SRC := $(wildcard src/*/*/*.cpp)
SRC := $(SRC) $(wildcard src/*/*/*.s)
OBJ := $(SRC:.s=.o)
OBJ := $(OBJ:.cpp=.o)

all: $(OBJ) | $(BINDIR)
	$(LD) $(LDFLAGS) $(OBJ) -o $(BINDIR)/OSZin.bin
	mkdir -p isodir/boot/grub
	cp $(BINDIR)/OSZin.bin isodir/boot/
	cp src/grub.cfg isodir/boot/grub/
	grub-mkrescue -o $(CDIMG) isodir/

$(BINDIR):
	mkdir $(BINDIR)

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

run:
	@qemu-system-i386 -cdrom $(CDIMG) -boot d -m 64

clean:
	rm -rf $(OBJ) $(BINDIR)/OSZin.bin OSZin.iso
