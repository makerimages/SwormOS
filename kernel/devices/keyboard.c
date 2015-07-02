#include <keyboard.h>
#include <pic.h>
#include <textmode.h>
#include <io.h>
char keys[128] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,9,'~',0,0,0,0,0,0,'q','1',0,0,0,'z','s','a','w',
    '2',0,0,'c','x','d','e','4','3',0,0,' ','v','f','t','r','5',0,0,'n','b','h',
    'g','y','6',0,0,0,'m','j','u','7','8',0,0,',','k','i','o','0','9',0,0,'.',
    '/','l',';','p','-',0,0,0,'\'',0,'[','=',0,0,0,0,'\n',']',0,'\\',0,0,0,0,0,0,
    0,0,'\b',0,0,0,0
};


static void key_callback(interrupt_context * regs) {
    if(done) {
        ps2_wait();
        previous = scancode;
        scancode = inb(0x60);
        if(scancode & 0x80) {

        } else {

            if(previous == 0xf0) {
                kputc(keys[scancode]);
            }

        }


    }

}



void init_keyboard() {
    done = false;
    set_handler(1,1,&key_callback);
    outb (PIC_MASTER + PIC_DATA, 0xFC);
    done = true;
}
