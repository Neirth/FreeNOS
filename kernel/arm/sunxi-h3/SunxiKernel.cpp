/*
 * Copyright (C) 2019 Niek Linnenbank
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

#include <FreeNOS/System.h>
#include <Log.h>
#include <SplitAllocator.h>
#include <CoreInfo.h>
#include <arm/ARMException.h>
#include <arm/ARMConstant.h>
#include "SunxiKernel.h"

SunxiKernel::SunxiKernel(CoreInfo *info)
    : ARMKernel(info)
    , m_gic(GIC_DIST_BASE, GIC_CPU_BASE)
{
    ARMControl ctrl;

    NOTICE("");

    // Setup interrupt callbacks
    m_intControl = &m_gic;
    m_exception.install(ARMException::IRQ, interrupt);
    m_exception.install(ARMException::FIQ, interrupt);

    // Configure clocks and irqs
    m_timer = &m_armTimer;
    m_armTimer.setFrequency(100);
    m_intControl->enable(ARMTIMER_IRQ);
}

void SunxiKernel::interrupt(volatile CPUState state)
{
    SunxiKernel *kernel = (SunxiKernel *) Kernel::instance;
    ARMProcess *proc = (ARMProcess *) kernel->getProcessManager()->current(), *next;
    uint irq;
    bool tick = false;

    DEBUG("procId = " << proc->getID());

    IntController::Result result = kernel->m_intControl->nextPending(irq);
    if (result == IntController::Success)
    {
        if (irq == ARMTIMER_IRQ)
            tick = true;
        else
            kernel->executeIntVector(irq, (CPUState *)&state);

        kernel->m_intControl->clear(irq);
    }

    if (tick)
    {
        kernel->m_timer->tick();
        kernel->getProcessManager()->schedule();
    }

    // If we scheduled a new process, switch the registers now
    next = (ARMProcess *) kernel->getProcessManager()->current();
    if (next != proc)
    {
        proc->setCpuState((const CPUState *)&state);
        MemoryBlock::copy((void *)&state, next->cpuState(), sizeof(state));
    }
}
