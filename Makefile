H_FILES := $(wildcard include/**/**/*.hpp)
CPP_FILES := $(wildcard src/**/*.cpp)

# C Compiler
GCC = i686-elf-g++
CFLAGS = -ffreestanding -g  -O2 -Wall -Wextra -fno-exceptions -fno-rtti -c -I include -T src/linker.ld

# Assembler
AA = i686-elf-as
AFLAGS =

# Linker
LD = i686-elf-gcc
LDFLAGS = -ffreestanding -rdynamic -O2 -nostdlib -lgcc


.PHONY: clean all Boot TextMode Main Libgcc

all: Boot Main TextMode Libc executable 

clean:
	@rm -rf obj
	@mkdir obj
	@rm -rf isodir
	@rm -f OSZin.iso

Boot: src/OSZin/asm/boot.s
	@$(AA) $< -o obj/$@.o

Main: src/OSZin/kernel/Main.cpp
	@$(GCC) ${CFLAGS} $< -o obj/$@.o

TextMode: src/OSZin/modules/TextMode.cpp
	@$(GCC) ${CFLAGS} $< -o obj/$@.o

Libc: uitoa itoa memmove strlen

strlen: src/libc/string/strlen.cpp
	@$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

itoa: src/libc/string/itoa.cpp
	@$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

uitoa: src/libc/string/uitoa.cpp
	@$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

memmove: src/libc/string/memmove.cpp
	@$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o



OBJ_FILES := $(wildcard obj/*.o)

executable:
	@$(GCC) -T src/linker.ld -o obj/OSZin.bin -ffreestanding -O2 -nostdlib ${OBJ_FILES} -lgcc
	@mkdir -p isodir
	@mkdir -p isodir/boot
	@cp obj/OSZin.bin isodir/boot/OSZin.bin
	@mkdir -p isodir/boot/grub
	@cp src/grub.cfg isodir/boot/grub/grub.cfg
	@grub-mkrescue -o OSZin.iso isodir

