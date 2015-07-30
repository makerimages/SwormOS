#include <paging.h>
#include <textmode.h>
#include <pic.h>
#include <idt.h>

static void pagefault_callback(interrupt_context * regs) {
	kpanic("Page fault!");
}

void init_paging() {
    kputs("Initializing paging... ");
	uint32_t* address = alloc_phys();
	uint32_t junk = alloc_phys(); //Junk allocate, so the memset doesn't mess up our PMM
	junk = alloc_phys(); //Junk allocate again , so the memset doesn't mess up our PMM

	page_directory_t* kernel_directory = (page_directory_t *) address;
	memset(kernel_directory, 0, sizeof(page_directory_t));
	current_directory = *kernel_directory;
}


void map_to_phys(page_t *page, int is_kernel, int is_writeable) {
	if(page->frame != 0) {
		return; // Aready mapped
	} else {
		uint32_t* at = alloc_phys();
		page -> present = 1;
		page -> rw = (is_writeable)?1:0;
		page -> user = (is_kernel)?0:1;
		page -> frame = at;
	}
}


void unmap_from_phys(page_t *page) {
	uint32_t frame;
	if(!(frame= page-> frame)) {
		return;
	} else {
		kfree(&frame);
		page -> frame = 0x0;
	}
}
