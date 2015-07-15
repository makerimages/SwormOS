#include <multiboot.h>
#include <textmode.h>
#include <pmm.h>
#include <stdint.h>
#include <stddef.h>
#include <idt.h>
#include <pic.h>
#include <timer.h>
#include <elf.h>
#include <acpi.h>
#include <ps2.h>
#include <keyboard.h>
#include <pmm_new.h>
#include <paging.h>

uint32_t totalMem;
uint32_t usableMem;
uintptr_t free_at;
void kernel_main(multiboot_info_t *mbt, unsigned int magic) {
    elf_init (&(mbt->u.elf_sec));
    textmode_init(80,25);

    /** Welcome Message **/
    kputs("OS Zin version 0.0.1 booting... \n");
    kputs("Copyright (c) Makerimages 2014-2015. MIT \n");
    kputs("Visit www.oszin.cf for more information. Sources available on GitHub.\n");

    /** Print system details **/
    kputcolor(lightBlue,black);
    kputs("System info START\n");
    kputcolor(lightGrey, black);

    /** MAGIC **/
    kprintf("\tMagic number is: 0x%x.", magic);
    if(magic == 0x2badb002) {
        kputcolor(green, black);
    	kputs(" OK\n");
        kputcolor(lightGrey, black);
    } else {
        kputcolor(green, black);
    	kputs(" FAIL\n");
        kputcolor(lightGrey, black);
    }


    /* Print the amount of memory. */
	kprintf("\tMultiboot info points to: 0x%x\n", &mbt);
	if (mbt->flags & (1 << 6))
    {
		kputcolor(green, black);
		kputs("\tUsing passed memory map, which follows.\n");
		kputcolor(lightGrey,black);

		multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) (mbt->mmap_addr);
		while (mmap < (multiboot_memory_map_t *) (mbt->mmap_addr+ mbt->mmap_length))
        {
            mmap = (multiboot_memory_map_t *) ((uintptr_t) (mmap) + mmap->size+ sizeof (unsigned int));
			totalMem += mmap->len;
			kprintf("\t\tRegion, address 0x%x size %d KB type %d.\n",(uint32_t)mmap->addr,(uint32_t)mmap->len/1024,mmap->type);

			if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
				usableMem += mmap->len;
                free_at = (uintptr_t) mmap->addr;

			}


		}

        /* Print the total memory. */
   		totalMem = totalMem / 1024;
   		usableMem = usableMem / 1024;
   		kprintf("\tTotal memory: %d KB.\n", totalMem);
   		kprintf("\tOf which %d KB is usable.\n", usableMem);
        pmm_new_init(free_at,usableMem*1024);
        pmm_new_map();
        init_paging();

    //   pmm_init(usableMem/1024,0x100000);
	//	pmm_map();

	}

    else if (mbt->flags & (1 << 0))
    {
        /* No memory map provided. */
		kputcolor(red,black);
		kputs("\tNo memory map passed, using mem_upper & mem_lower.\n");
		kprintf("\tTotal memory: %d KB.\n", mbt->mem_upper + mbt->mem_lower);
		kputcolor(lightGrey,black);
	}

    /** Print system details end message**/
    kputcolor(lightBlue,black);
    kputs("System info END\n");
    kputcolor(lightGrey, black);


    kputs("Enabling interrupts ");
    idt_initialize ();
    pic_initialize ();
    init_timer(1000);

    __asm__ __volatile__ ("sti");

    kputcolor(green, black);
    kputs("OK.\n");
    kputcolor(lightGrey,black);

    // Initialize the PIT timer.


    acpi_init();

    ps2_init();
    init_keyboard();


}
