# Makefile to make compiling and running the Sworm OS kernel easier.

.PHONY: all libc kernel run bochs clean clean-kernel clean-libc clean-sysroot

# What to build and install.
BUILD := libc kernel

# Path to the CD image.
ISOFILE := SwormOS.iso

all: $(BUILD)

libc:
	@$(MAKE) -C $@ all
	@$(MAKE) -C $@ install

kernel:
	@$(MAKE) -C $@ all install

run:
	@qemu-system-i386 -cdrom $(ISOFILE) -boot d -m 1024 -vga std -serial stdio

bochs:
	@bochs -q

clean: clean-kernel clean-libc clean-sysroot

clean-kernel:
	@$(MAKE) -C $(subst clean-,,$@) clean

clean-libc:
	@$(MAKE) -C $(subst clean-,,$@) clean

clean-sysroot:
	rm -rf $(subst clean-,,$@)
