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

#include <FreeNOS/ProcessManager.h>
#include <FreeNOS/System.h>
#include <Macros.h>
#include <List.h>
#include <ListIterator.h>
#include <SplitAllocator.h>
#include <Vector.h>
#include <MemoryBlock.h>
#include <String.h>
#include <BootImage.h>
#include <intel/IntelMap.h>
#include <intel/IntelBoot.h>
#include "IntelKernel.h"

extern C void executeInterrupt(CPUState state)
{
    Kernel::instance->executeIntVector(state.vector, &state);
}

IntelKernel::IntelKernel(CoreInfo *info)
    : Kernel(info)
{

    IntelMap map;
    IntelCore core;
    IntelPaging memContext(&map, core.readCR3(), m_alloc);

    // Refresh MemoryContext::current()
    memContext.activate();

    // Install interruptRun() callback
    interruptRun = ::executeInterrupt;

    // Setup exception handlers
    for (int i = 0; i < 17; i++)
    {
        hookIntVector(i, exception, 0);
    }
    // Setup IRQ handlers
    for (int i = 17; i < 256; i++)
    {
        // Trap gate
        if (i == 0x90)
            hookIntVector(0x90, trap, 0);

        // Hardware Interrupt
        else
            hookIntVector(i, interrupt, 0);
    }

    // Only core0 uses PIC and PIT.
    if (info->coreId == 0)
    {
        // Set PIT interrupt frequency to 250 hertz
        m_pit.setFrequency(100);

        // Configure the master and slave PICs
        m_pic.initialize();
        m_intControl = &m_pic;
    }
    else
        m_intControl = &m_apic;

    // Try to configure the APIC.
    if (m_apic.initialize() == Timer::Success)
    {
        NOTICE("Using APIC timer");

        // Enable APIC timer interrupt
        hookIntVector(m_apic.getInterrupt(), clocktick, 0);

        m_timer = &m_apic;

        if (m_coreInfo->timerCounter == 0)
        {
            m_apic.start(&m_pit);
            m_coreInfo->timerCounter = m_apic.getCounter();
        }
        else
            m_apic.start(m_coreInfo->timerCounter, m_pit.getFrequency());
    }
    // Use PIT as system timer.
    else
    {
        NOTICE("Using PIT timer");
        m_timer = &m_pit;

        // Install PIT interrupt vector handler
        hookIntVector(m_intControl->getBase() +
                      m_pit.getInterrupt(), clocktick, 0);

        // Enable PIT interrupt
        enableIRQ(m_pit.getInterrupt(), true);
    }

    // Initialize TSS Segment
    Address tssAddr = (Address) &kernelTss;
    gdt[KERNEL_TSS].limitLow    = sizeof(TSS) + (0xfff / 8);
    gdt[KERNEL_TSS].baseLow     = (tssAddr) & 0xffff;
    gdt[KERNEL_TSS].baseMid     = (tssAddr >> 16) & 0xff;
    gdt[KERNEL_TSS].type        = 9;
    gdt[KERNEL_TSS].privilege   = 0;
    gdt[KERNEL_TSS].present     = 1;
    gdt[KERNEL_TSS].limitHigh   = 0;
    gdt[KERNEL_TSS].granularity = 8;
    gdt[KERNEL_TSS].baseHigh    = (tssAddr >> 24) & 0xff;

    // Fill the Task State Segment (TSS).
    MemoryBlock::set(&kernelTss, 0, sizeof(TSS));
    kernelTss.ss0    = KERNEL_DS_SEL;
    kernelTss.esp0   = 0;
    kernelTss.bitmap = sizeof(TSS);
    ltr(KERNEL_TSS_SEL);
}

void IntelKernel::enableIRQ(u32 irq, bool enabled)
{
    if (irq == m_apic.getInterrupt())
    {
        if (enabled)
            m_apic.start();
        else
            m_apic.stop();

        return;
    }

    Kernel::enableIRQ(irq, enabled);
}

void IntelKernel::exception(CPUState *state, ulong param, ulong vector)
{
    IntelCore core;
    ProcessManager *procs = Kernel::instance->getProcessManager();

    ERROR("Exception in Process: " << procs->current()->getID());
    core.logException(state);

    assert(procs->current() != ZERO);
    procs->remove(procs->current());
    procs->schedule();
}

void IntelKernel::interrupt(CPUState *state, ulong param, ulong vector)
{
    IntelKernel *kern = (IntelKernel *) Kernel::instance;

    if (kern->m_intControl)
    {
        kern->m_intControl->clear(
            state->vector - kern->m_intControl->getBase()
        );
    }
}

void IntelKernel::trap(CPUState *state, ulong param, ulong vector)
{
    state->regs.eax = Kernel::instance->getAPI()->invoke(
        (API::Number) state->regs.eax,
                      state->regs.ecx,
                      state->regs.ebx,
                      state->regs.edx,
                      state->regs.esi,
                      state->regs.edi
    );
}

void IntelKernel::clocktick(CPUState *state, ulong param, ulong vector)
{
    IntelKernel *kern = (IntelKernel *) Kernel::instance;
    Size irq = kern->m_timer->getInterrupt();

    kern->enableIRQ(irq, true);

    // Ensure the APIC timer gets end-of-interrupt
    if (irq == kern->m_apic.getInterrupt())
        kern->m_apic.clear(irq);

    kern->m_timer->tick();
    kern->getProcessManager()->schedule();
}
