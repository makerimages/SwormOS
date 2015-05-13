#ifndef KernelGlobals_H
#define KernelGlobals_H

#include <modules/TextMode.hpp>
#include <arch/ia32/modules/PIT.hpp>
#include <arch/ia32/modules/PMM.hpp>
#include <modules/Keyboard.hpp>
#include <modules/ACPI.hpp>

extern TextMode tm;
extern uint32_t totalMem;
extern uint32_t usableMem;
extern Pit pTimer;
extern PMM pmm;
extern Keyboard kbrd;
extern ACPI acpi;

#endif
