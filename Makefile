TARGET := i686-elf
BUILDDIR := build-$(TARGET)

H_FILES := $(wildcard src/**/*.h)

CPP_FILES := $(wildcard src/**/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

# C Compiler
CC = i686-elf-g++
CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# Assembler
AA = i686-elf-as
AFLAGS =

# Linker
LD = i686-elf-gcc
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc


.PHONY: all Kernel

all: clean Kernel

clean:
	rm -rf obj
	mkdir obj

Kernel: src/kernel/Kernel.cpp
	CC -c $< -o obj/$@.o
