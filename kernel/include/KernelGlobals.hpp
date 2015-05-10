#ifndef KernelGlobals_H
#define KernelGlobals_H

#include <modules/TextMode.hpp>
#include <arch/ia32/modules/PIT.hpp>
#include <arch/ia32/modules/PMM.hpp>

extern TextMode tm;
extern uint32_t totalMem;
extern uint32_t usableMem;
extern Pit pTimer;
extern PMM pmm;
#endif
