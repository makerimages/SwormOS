#include <modules/Keyboard.hpp>
#include <arch/ia32/modules/PIC.hpp>
#include <arch/ia32/modules/IDT.hpp>
#include <KernelGlobals.hpp>

Keyboard::Keyboard() {

}


static void keyHandler(interrupt_context * regs) {
  tm.kputsf("Keypress! \n");
}

void Keyboard::init() {
  setHandler(1,1,&keyHandler);

}
