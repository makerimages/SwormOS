TARGET := i686-elf
BUILDDIR := build-$(TARGET)

H_FILES := $(wildcard src/**/**/*.h)

CPP_FILES := $(wildcard src/**/*.cpp)

# C Compiler
GCC = i686-elf-g++
CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -c

# Assembler
AA = i686-elf-as
AFLAGS =

ISO = mkisofs

# Linker
LD = i686-elf-gcc
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc


.PHONY: all Kernel Terminal

all: clean System Interrupt GDT IDT PIC Terminal Kernel boot ientry executable

clean:
	rm -rf obj
	mkdir obj
	rm -rf isodir
	rm -f OSZin.iso

Kernel: src/kernel/Kernel.cpp ${H_FILES}
	$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

System: src/drivers/System.cpp ${H_FILES}
	$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

Terminal: src/drivers/Terminal.cpp ${H_FILES}
	$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

PIC: src/drivers/PIC.cpp ${H_FILES}
	$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

GDT: src/drivers/gdt.cpp ${H_FILES}
	$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

Interrupt: src/drivers/Interrupt.cpp ${H_FILES}
	$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

IDT: src/drivers/InterruptDescriptorTable.cpp ${H_FILES}
	$(GCC) -T src/linker.ld ${CFLAGS} $< -o obj/$@.o

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

