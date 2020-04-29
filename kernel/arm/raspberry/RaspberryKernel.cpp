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
#include <arm/broadcom/BroadcomInterrupt.h>
#include "RaspberryKernel.h"

RaspberryKernel::RaspberryKernel(CoreInfo *info)
    : ARMKernel(info)
#ifdef BCM2836
    , m_bcm(info->coreId)
#endif /* BMC2836 */
{
    ARMControl ctrl;

    NOTICE("");

    // Setup interrupt callbacks
    m_intControl = &m_bcmIntr;
    m_exception.install(ARMException::IRQ, interrupt);
    m_exception.install(ARMException::FIQ, interrupt);

    // Configure clocks and irqs. For BCM2836, only use the generic ARM timer
    // when running under Qemu. Unfortunately, Qemu dropped support for the
    // broadcom timer in recent versions. On hardware, use the broadcom timer.
#ifdef BCM2836
    u32 system_frequency = ctrl.read(ARMControl::SystemFrequency);
    NOTICE("sysfreq = " << system_frequency);
    if (system_frequency == 62500000)
    {
        // Use ARM generic timer
        m_timer = &m_armTimer;
        m_timerIrq = ARMTIMER_IRQ;
        m_armTimer.setFrequency(100);

        // Setup IRQ routing
        m_bcm.setCoreTimerIrq(Broadcom2836::PhysicalTimer1, true);
    }
#endif /* BCM2836 */

    /* Default to broadcom timer and interrupt handling */
    if (m_timer == NULL)
    {
        m_timer = &m_bcmTimer;
        m_timerIrq = BCM_IRQ_SYSTIMERM1;
        m_bcmTimer.setFrequency( 250 ); /* trigger timer interrupts at 250Hz (clock runs at 1Mhz) */
        m_intControl->enable(BCM_IRQ_SYSTIMERM1);
    }
}

void RaspberryKernel::interrupt(volatile CPUState state)
{
    RaspberryKernel *kernel = (RaspberryKernel *) Kernel::instance;
    ARMProcess *proc = (ARMProcess *) Kernel::instance->getProcessManager()->current(), *next;
    bool tick;

    DEBUG("procId = " << proc->getID());

#ifdef BCM2836
    if (kernel->m_timer == &kernel->m_armTimer)
    {
        tick = kernel->m_bcm.getCoreTimerIrqStatus(Broadcom2836::PhysicalTimer1);
    }
    else
#endif /* BCM2836 */
    {
        tick = kernel->m_intControl->isTriggered(BCM_IRQ_SYSTIMERM1);
    }

    if (tick)
    {
        kernel->m_timer->tick();
        kernel->getProcessManager()->schedule();
    }

    for (uint i = kernel->m_timerIrq + 1; i < 64; i++)
    {
        if (kernel->m_intControl->isTriggered(i))
        {
            kernel->executeIntVector(i, (CPUState *)&state);
        }
    }

    next = (ARMProcess *) kernel->getProcessManager()->current();
    if (next != proc)
    {
        proc->setCpuState((const CPUState *)&state);
        MemoryBlock::copy((void *)&state, next->cpuState(), sizeof(state));
    }
}
