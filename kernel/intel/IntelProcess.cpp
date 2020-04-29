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

#include <FreeNOS/System.h>
#include <MemoryBlock.h>
#include <Memory.h>
#include <SplitAllocator.h>
#include <intel/IntelPaging.h>
#include "IntelProcess.h"

IntelProcess::IntelProcess(ProcessID id, Address entry, bool privileged, const MemoryMap &map)
    : Process(id, entry, privileged, map)
{
}

Process::Result IntelProcess::initialize()
{
    Address userStack;
    Memory::Range range;
    Allocator::Arguments alloc_args;
    CPUState *regs;
    u16 dataSel = m_privileged ? KERNEL_DS_SEL : USER_DS_SEL;
    u16 codeSel = m_privileged ? KERNEL_CS_SEL : USER_CS_SEL;

    // Create MMU context
    m_memoryContext = new IntelPaging(&m_map, Kernel::instance->getAllocator());
    if (!m_memoryContext)
    {
        ERROR("failed to create memory context");
        return OutOfMemory;
    }

    // Allocate User stack
    range = m_map.range(MemoryMap::UserStack);
    range.access = Memory::Readable | Memory::Writable | Memory::User;
    alloc_args.address = 0;
    alloc_args.size = range.size;
    alloc_args.alignment = PAGESIZE;

    if (Kernel::instance->getAllocator()->allocate(alloc_args) != Allocator::Success)
    {
        ERROR("failed to allocate user stack");
        return OutOfMemory;
    }
    range.phys = alloc_args.address;

    // Map User stack
    if (m_memoryContext->mapRange(&range) != MemoryContext::Success)
    {
        ERROR("failed to map user stack");
        return MemoryMapError;
    }
    userStack = range.virt + range.size - MEMALIGN;

    // Allocate Kernel stack
    alloc_args.address = 0;
    alloc_args.size = KernelStackSize;
    alloc_args.alignment = PAGESIZE;

    if (Kernel::instance->getAllocator()->allocateLow(alloc_args) != Allocator::Success)
    {
        ERROR("failed to allocate kernel stack");
        return OutOfMemory;
    }
    m_kernelStackBase = (Address) Kernel::instance->getAllocator()->toVirtual(alloc_args.address);
    m_kernelStackBase += KernelStackSize;
    m_kernelStack = m_kernelStackBase - sizeof(CPUState)
                                      - sizeof(IRQRegs0)
                                      - sizeof(CPURegs);

    // Fill kernel stack with initial (user)registers to restore
    // loadCoreState: struct CPUState
    regs = (CPUState *) m_kernelStackBase - 1;
    MemoryBlock::set(regs, 0, sizeof(CPUState));
    regs->seg.ss0    = KERNEL_DS_SEL;
    regs->seg.fs     = dataSel;
    regs->seg.gs     = dataSel;
    regs->seg.es     = dataSel;
    regs->seg.ds     = dataSel;
    regs->regs.ebp   = userStack;
    regs->regs.esp0  = m_kernelStack;
    regs->irq.eip    = m_entry;
    regs->irq.cs     = codeSel;
    regs->irq.eflags = INTEL_EFLAGS_DEFAULT |
                       INTEL_EFLAGS_IRQ;
    regs->irq.esp3   = userStack;
    regs->irq.ss3    = dataSel;

    // restoreState: iret
    IRQRegs0 *irq = (IRQRegs0 *) regs - 1;
    irq->eip = (Address) loadCoreState;
    irq->cs  = KERNEL_CS_SEL;
    irq->eflags = INTEL_EFLAGS_DEFAULT;

    // restoreState: popa
    CPURegs *pusha = (CPURegs *) irq - 1;
    MemoryBlock::set(pusha, 0, sizeof(CPURegs));
    pusha->ebp  = m_kernelStackBase - sizeof(CPURegs);
    pusha->esp0 = pusha->ebp;

    // Finalize with generic initialization
    return Process::initialize();
}

IntelProcess::~IntelProcess()
{
    // Release the kernel stack memory page
    SplitAllocator *alloc = Kernel::instance->getAllocator();
    alloc->release((Address)alloc->toPhysical(m_kernelStackBase) - KernelStackSize);
}

void IntelProcess::execute(Process *previous)
{
    IntelProcess *p = (IntelProcess *) previous;

    // Reload Task State Register (with kernel stack for interrupts)
    kernelTss.esp0 = m_kernelStackBase;

    // Activate the memory context of this process
    m_memoryContext->activate();

    // Switch kernel stack (includes saved userspace registers)
    switchCoreState( p ? &p->m_kernelStack : ZERO,
                     m_kernelStack );
}
