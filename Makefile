TARGET := i686-elf
BUILDDIR := build-$(TARGET)

H_FILES := $(wildcard include/**/**/*.hpp)

CPP_FILES := $(wildcard src/**/*.cpp)

# C Compiler
GCC = i686-elf-g++
CFLAGS = -ffreestanding -g  -O2 -Wall -Wextra -fno-exceptions -fno-rtti -c -I include -T src/linker.ld

# Assembler
AA = i686-elf-as
AFLAGS =

ISO = mkisofs

# Linker
LD = i686-elf-gcc
LDFLAGS = -ffreestanding -rdynamic -O2 -nostdlib -lgcc


.PHONY: all Kernel Terminal

all: clean Libc Interrupt GDT IDT PIC Elf Terminal Keyboard Kernel boot ientry executable

clean:
	rm -rf obj
	mkdir obj
	rm -rf isodir
	rm -f OSZin.iso

Kernel: src/kernel/Kernel.cpp ${H_FILES}
	$(GCC) ${CFLAGS} $< -o obj/$@.o

Terminal: src/drivers/Terminal.cpp ${H_FILES}
	$(GCC) ${CFLAGS} $< -o obj/$@.o

Keyboard: src/drivers/Keyboard.cpp ${H_FILES}
	$(GCC) ${CFLAGS} $< -o obj/$@.o

PIC: src/modules/PIC.cpp ${H_FILES}
	$(GCC) ${CFLAGS} $< -o obj/$@.o

GDT: src/modules/GDT.cpp ${H_FILES}
	$(GCC) ${CFLAGS} $< -o obj/$@.o

Interrupt: src/modules/Interrupt.cpp ${H_FILES}
	$(GCC) ${CFLAGS} $< -o obj/$@.o

Elf: src/modules/Elf.cpp ${H_FILES}
	$(GCC) ${CFLAGS} $< -o obj/$@.o

IDT: src/modules/IDT.cpp ${H_FILES}
	$(GCC) ${CFLAGS} $< -o obj/$@.o

boot:  src/kernel/boot.s
	$(AA) $< -o obj/$@.o

ientry: src/kernel/interrupt-entry.s
	$(AA) $< -o obj/$@.o

OBJ_FILES := $(wildcard obj/*.o)

executable: obj/boot.o
	$(GCC) -T src/linker.ld -o obj/OSZin.bin -ffreestanding -O2 -nostdlib ${OBJ_FILES} -lgcc
	mkdir -p isodir
	mkdir -p isodir/boot
	cp obj/OSZin.bin isodir/boot/OSZin.bin
	mkdir -p isodir/boot/grub
	cp src/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o OSZin.iso isodir

Libc: strlen strcmp uitoa memmove

strlen: src/libc/string/strlen.cpp
	$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

strcmp: src/libc/string/strcmp.cpp
	$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

uitoa: src/libc/string/uitoa.cpp
	$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

memmove: src/libc/string/memmove.cpp
	$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o
