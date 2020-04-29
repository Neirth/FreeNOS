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
#include <Log.h>
#include <SplitAllocator.h>
#include "ARMProcess.h"

#define MEMALIGN8 8

static bool firstProcess = true;
extern u8 svcStack[PAGESIZE];

ARMProcess::ARMProcess(ProcessID id, Address entry, bool privileged, const MemoryMap &map)
    : Process(id, entry, privileged, map)
{
}

Process::Result ARMProcess::initialize()
{
    Memory::Range range;
    Allocator::Arguments alloc_args;

    // Create MMU context
    m_memoryContext = new ARMPaging(&m_map, Kernel::instance->getAllocator());
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

    // Fill usermode program registers
    MemoryBlock::set(&m_cpuState, 0, sizeof(m_cpuState));
    m_cpuState.sp = range.virt + range.size - MEMALIGN8;  // user stack pointer
    m_cpuState.pc = m_entry;      // user program counter
    m_cpuState.cpsr = (m_privileged ? SYS_MODE : USR_MODE); // current program status (CPSR)

    // Finalize with generic initialization
    return Process::initialize();
}

ARMProcess::~ARMProcess()
{
}

const CPUState * ARMProcess::cpuState() const
{
    return &m_cpuState;
}

void ARMProcess::setCpuState(const CPUState *cpuState)
{
    MemoryBlock::copy(&m_cpuState, cpuState, sizeof(*cpuState));
}

void ARMProcess::setWaitResult(uint result)
{
    if (m_state != Waiting)
    {
        FATAL("set wait result while process is not waiting: status: " << (uint) m_state);
    }
    m_cpuState.r0 = result;
}

void ARMProcess::execute(Process *previous)
{
    // Activate the memory context of this process
    m_memoryContext->activate();

    // First process starts from loadCoreState0
    if (firstProcess)
    {
        firstProcess = false;

        CPUState *ptr = ((CPUState *) (svcStack)) - 1;
        MemoryBlock::copy(ptr, &m_cpuState, sizeof(*ptr));

        // Switch to the actual SVC stack and switch to usermode
        asm volatile ("ldr sp, =svcStack\n"
                      "sub sp, sp, %0\n"
                      "ldr r0, =loadCoreState0\n"
                      "bx r0\n" : : "i" (sizeof(m_cpuState) - sizeof(m_cpuState.padding)) );
    }
}
