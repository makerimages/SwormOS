# OS Zin
OS Zin is a hobbyist OS, built mostly from scratch and in development since 30.12.2014. Restarted 20.th February 2015.

This repository contains the source code and a Makefile for a ```i686-elf``` targeting assembler and cross-compiler. Find instructions on how to make yours [here](http://wiki.osdev.org/GCC_Cross-Compiler). It also contains a ```.bocshrc``` file to run bochs with.

## Contributing

Fork the repository.
Add bugfixes or new features.
Pull request.

### Requierments for dev env.

A UNIX system. Grub2 and xorriso. [And the compiler.](http://wiki.osdev.org/GCC_Cross-Compiler).

## TO-DO
- [X] Interrupts
- [X] Exception handling
- [ ] PIT
- [ ] PMM
- [ ] Keyboard driver
- [ ] File system (VFS too)
- [ ] Commands



## Thank you:
* The wiki, forums and IRC of osdev.org for being there. If it weren't for them, I'd still be stuck at writing a bootloader.
* Jonas 'Sortie' Termansen - GDT and IDT implementation sources.