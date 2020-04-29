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
#include "ARMCore.h"

void ARMCore::logException(CPUState *state) const
{
    logState(state);
}

void ARMCore::logState(CPUState *state) const
{
    ARMControl ctrl;

    logRegister("cpsr", state->cpsr);
    logRegister("sp", state->sp);
    logRegister("lr", state->lr);
    logRegister("pc", state->pc);
    logRegister("ifar", ctrl.read(ARMControl::InstructionFaultAddress));
    logRegister("dfar", ctrl.read(ARMControl::DataFaultAddress));

    u32 ifsr = ctrl.read(ARMControl::InstructionFaultStatus) & 0xf;
    const char *ifsrText = "unknown";
    switch (ifsr)
    {
        case 0: ifsrText = "no function"; break;
        case 1: ifsrText = "alignment fault"; break;
        case 2: ifsrText = "instruction debug event fault"; break;
        case 3: ifsrText = "access bit fault on section"; break;
        case 4: ifsrText = "no function"; break;
        case 5: ifsrText = "translation section fault"; break;
        case 6: ifsrText = "access bit fault on page"; break;
        case 7: ifsrText = "translation page fault"; break;
        case 8: ifsrText = "precise external abort"; break;
        case 9: ifsrText = "domain section fault"; break;
        case 10: ifsrText = "no function"; break;
        case 11: ifsrText = "domain page fault"; break;
        case 12: ifsrText = "external abort on translation first level"; break;
        case 13: ifsrText = "permission section fault"; break;
        case 14: ifsrText = "external abort on translation second level"; break;
        case 15: ifsrText = "permission page fault"; break;
    }
    logRegister("ifsr", ifsr, ifsrText);

    u32 dfsr = ctrl.read(ARMControl::DataFaultStatus) & 0xf;
    const char *dfsrText = "unknown";
    if (ctrl.read(ARMControl::DataFaultStatus) & (1 << 10))
    {
        dfsrText = (dfsr == 6 ? "imprecise external abort" : "no function");
    }
    else
    {
        switch (dfsr)
        {
            case 0: dfsrText = "no function"; break;
            case 1: dfsrText = "alignment fault"; break;
            case 2: dfsrText = "instruction debug event fault"; break;
            case 3: dfsrText = "access bit fault on section"; break;
            case 4: dfsrText = "instruction cache maintenance operation fault"; break;
            case 5: dfsrText = "translation section fault"; break;
            case 6: dfsrText = "access bit fault on page"; break;
            case 7: dfsrText = "translation page fault"; break;
            case 8: dfsrText = "precise external abort"; break;
            case 9: dfsrText = "domain section fault"; break;
            case 10: dfsrText = "no function"; break;
            case 11: dfsrText = "domain page fault"; break;
            case 12: dfsrText = "external abort on translation first level"; break;
            case 13: dfsrText = "permission section fault"; break;
            case 14: dfsrText = "external abort on translation second level"; break;
            case 15: dfsrText = "permission page fault"; break;
        }
    }
    logRegister("dfsr", dfsr, dfsrText);

    logRegister("r0", state->r0);
    logRegister("r1", state->r1);
    logRegister("r2", state->r2);
    logRegister("r3", state->r3);
    logRegister("r4", state->r4);
    logRegister("r5", state->r5);
    logRegister("r6", state->r6);
    logRegister("r7", state->r7);
    logRegister("r8", state->r8);
    logRegister("r9", state->r9);
    logRegister("r10", state->r10);
    logRegister("r11", state->r11);
    logRegister("r12", state->r12);
}

void ARMCore::logRegister(const char *name, u32 reg, const char *text) const
{
    String s;
    s << Number::Hex << name << " = " << reg << Number::Dec << " (" << reg << ") " << text;
    ERROR(*s);
}
