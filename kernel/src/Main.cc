#include <Multiboot.h>
#include <KernelGlobals.hpp>
#include <modules/Elf.hpp>
#include <arch/ia32/modules/IDT.hpp>
#include <arch/ia32/modules/PIC.hpp>
#include <arch/ia32/modules/PIT.hpp>
#include <ioports.hpp>

/* Initialize the kernel globals. */
TextMode tm;
uint32_t totalMem;
uint32_t usableMem;
uintptr_t initial_esp;

extern "C"
void kernelMain(multiboot_info *mbt, unsigned int magic) {
	elfInit (&(mbt->u.elf_sec));

    /* Initialize the output. */
	tm.init ();

    /* Print the welcome message. */
	tm.kputs ("Booting OSZin version 0.0.1\n");
	tm.kputs ("Copyright (C) 2014, 2015 Makerimages. Licenced under the "
              "MIT licence.\n");
	tm.kputs ("Visit www.oszin.cf for more information. Sources are "
            " available on GitHub.\n");

    /* Start of system details. */
	tm.setColor (tm.lightBlue, tm.black);
	tm.kputs ("System details follow:\n");
	tm.resetColor ();

    /* Print the magic number. */
	tm.kputsf ("\tMagic number is: 0x%x.", magic);
	tm.setColor (tm.lightBlue, tm.black);
	tm.kputsf (" Not 0x2badb002? I don't know how you got this.\n");
	tm.resetColor ();

    /* Print the amount of memory. */
	tm.kputsf ("\tMultiboot info points to: 0x%x\n", &mbt);
	if (mbt->flags & (1 << 6))
    {
		tm.setColor (tm.green, tm.black);
		tm.kputs ("\tUsing passed memory map.\n");
		tm.resetColor ();

		multiboot_memory_map_t *mmap =
            (multiboot_memory_map_t *) (mbt->mmap_addr);
		while (mmap < (multiboot_memory_map_t *) (mbt->mmap_addr
                    + mbt->mmap_length))
        {
            mmap = (multiboot_memory_map_t *) ((uintptr_t) (mmap) + mmap->size
                    + sizeof (unsigned int));
			totalMem += mmap->len;
			if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
				usableMem += mmap->len;
		}

        /* Print the total memory. */
   		totalMem = totalMem / 1024;
   		usableMem = usableMem / 1024;
   		tm.kputsf ("\tTotal memory: %d KB.\n", totalMem);
   		tm.kputsf ("\tOf which %d KB is usable.\n", usableMem);
	}
    else if (mbt->flags & (1 << 0))
    {
        /* No memory map provided. */
		tm.setColor (tm.red,tm.black);
		tm.kputs ("\tNo memory map passed, using mem_upper & mem_lower.\n");
		tm.kputsf ("\tTotal memory: %d KB.\n", mbt->mem_upper + mbt->mem_lower);
		tm.resetColor ();
	}

    /* End of system details. */
	tm.setColor (tm.lightBlue, tm.black);
	tm.kputs ("System details ended.\n");
	tm.resetColor ();

    /* Enable interrupts. */
	tm.kputs ("Enabling interrupts.\n");
	idt_initialize ();
	pic_initialize ();

    /* Initialize the PIT timer. */
	Pit pp;
	pp.init (1000);
	tm.setColor (tm.green, tm.black);
	tm.kputs ("Enabled.\n");
	tm.resetColor ();

	asm volatile ("sti");
}
