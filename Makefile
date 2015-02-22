H_FILES := $(wildcard include/**/**/*.hpp)
CPP_FILES := $(wildcard src/**/*.cpp)

# C Compiler
GCC = i686-elf-g++
CFLAGS = -ffreestanding -g  -O2 -Wall -Wextra -fno-omit-frame-pointer -fno-exceptions -fno-rtti -c -I include -T src/linker.ld

# Assembler
AA = i686-elf-as
AFLAGS =

# Linker
LD = i686-elf-gcc
LDFLAGS = -ffreestanding -rdynamic -O2 -nostdlib -lgcc


.PHONY: clean all Boot TextMode  Main Libgcc

all: IEntry Boot Main Elf Interrupt GDT PIC IDT TextMode  Libc executable 

clean:
	@rm -rf obj
	@mkdir obj
	@rm -rf isodir
	@rm -f OSZin.iso

Boot: src/OSZin/asm/boot.s
	@$(AA) $< -o obj/$@.o

IEntry: src/OSZin/asm/interrupt-entry.s
	@$(AA) $< -o obj/$@.o

Main: src/OSZin/kernel/Main.cpp
	@$(GCC) ${CFLAGS} $< -o obj/$@.o

Elf: src/OSZin/modules/Elf.cpp
	@$(GCC) ${CFLAGS} $< -o obj/$@.o

Interrupt: src/OSZin/modules/Interrupt.cpp
	@$(GCC) ${CFLAGS} $< -o obj/$@.o

IDT: src/OSZin/modules/IDT.cpp
	@$(GCC) ${CFLAGS} $< -o obj/$@.o

GDT: src/OSZin/modules/GDT.cpp
	@$(GCC) ${CFLAGS} $< -o obj/$@.o

PIC: src/OSZin/modules/PIC.cpp
	@$(GCC) ${CFLAGS} $< -o obj/$@.o

TextMode: src/OSZin/modules/TextMode.cpp
	@$(GCC) ${CFLAGS} $< -o obj/$@.o

Libc: uitoa itoa memmove memset strlen strcmp

strlen: src/libc/string/strlen.cpp
	@$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

itoa: src/libc/string/itoa.cpp
	@$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

uitoa: src/libc/string/uitoa.cpp
	@$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

memmove: src/libc/string/memmove.cpp
	@$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

memset: src/libc/string/memset.cpp
	@$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o


strcmp: src/libc/string/strcmp.cpp
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

