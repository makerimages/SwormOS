#include <paging.h>
#include <textmode.h>

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

void init_paging() {
    kputs("Init paging");
    page_directory = 0x100000;
    for(int i=0; i< 1024; i++) {
        page_directory[i] = 0x00000003;
    }
    kputs("Dir ");
    first_page_table = 0x101000;
    for(int j = 0; j< 1024; j++) {
        first_page_table[j] = (j*0x1000) | 3;
    }
    page_directory[0] = ((unsigned int) first_page_table) | 3;
    kputs("Table ");
    loadPageDirectory(page_directory);
    enablePaging();
    kputs("K\n");


}
