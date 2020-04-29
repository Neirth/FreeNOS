/*
 * Copyright (C) 2015 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Log.h>
#include <String.h>
#include "IntelCore.h"

#pragma clang optimize off
#pragma GCC push_options
#pragma GCC optimize ("O0")

void IntelCore::logException(CPUState *state) const
{
    String s;

    switch (state->vector)
    {
        case INTEL_DIVZERO:
            ERROR("Divide Error Exception");
            break;

        case INTEL_DEBUGEX:
            ERROR("Debug Exception");
            break;

        case INTEL_NMI:
            ERROR("Non-Maskable Interrupt Exception");
            break;

        case INTEL_BREAKP:
            ERROR("Breakpoint Exception");
            break;

        case INTEL_OVERFLOW:
            ERROR("Overflow Exception");
            break;

        case INTEL_BOUNDS:
            ERROR("BOUND Range Exceeded Exception");
            break;

        case INTEL_OPCODE:
            ERROR("Invalid Opcode Exception");
            break;

        case INTEL_DEVERR:
            ERROR("Device Not Available Exception");
            break;

        case INTEL_DOUBLEF:
            ERROR("Double Fault Exception");
            break;

        case INTEL_COSEG:
            ERROR("Coprocessor Segment Overrun Exception");
            break;

        case INTEL_TSSERR:
            ERROR("Invalid Task State Segment Exception");
            break;

        case INTEL_SEGERR:
            ERROR("Segment Not Present Exception");
            break;

        case INTEL_STACKERR:
            ERROR("Stack Fault Exception");
            break;

        case INTEL_GENERR:
            ERROR("General Protection Fault Exception");
            break;

        case INTEL_PAGEFAULT:
            ERROR("Page Fault Exception");
            s << "Error " << state->error << " at " << Number::Hex << readCR2();
            ERROR(*s);
            break;

        case INTEL_FLOATERR:
            ERROR("Floating Point Error Exception");
            break;

        case INTEL_ALIGNERR:
            ERROR("Alignment Check Exception");
            break;

        case INTEL_MACHCHK:
            ERROR("Machine Check Exception");
            break;

        case INTEL_SIMD:
            ERROR("SIMD Floating Point Exception");
            break;

        case INTEL_VIRTERR:
            ERROR("Virtualization Exception");
            break;

        default:
            ERROR("Unknown Exception: " << state->vector);
            break;
    }
    logState(state);
}

void IntelCore::logState(CPUState *state) const
{
    logRegister("eip", state->irq.eip);
    logRegister("eax", state->regs.eax);
    logRegister("ebx", state->regs.ebx);
    logRegister("ecx", state->regs.ecx);
    logRegister("edx", state->regs.edx);
    logRegister("esi", state->regs.esi);
    logRegister("edi", state->regs.edi);
    logRegister("ebp", state->regs.ebp);
    logRegister("esp", state->regs.esp0);
}

void IntelCore::logRegister(const char *name, u32 reg) const
{
    String s;
    s << Number::Hex << name << " = " << reg << Number::Dec << " (" << reg << ")";

    ERROR(*s);
}

volatile u32 IntelCore::readCR2() const
{
    volatile u32 cr2;
    asm volatile("mov %%cr2, %%eax\n"
                 "mov %%eax, %0\n" : "=r" (cr2));
    return cr2;
}

volatile u32 IntelCore::readCR3() const
{
    volatile u32 cr3;
    asm volatile("mov %%cr3, %%eax\n"
                 "mov %%eax, %0\n" : "=r" (cr3));
    return cr3;
}

void IntelCore::writeCR3(u32 cr3) const
{
    asm volatile("mov %0, %%eax\n"
                 "mov %%eax, %%cr3" :: "r" (cr3));
}
