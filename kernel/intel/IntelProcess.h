/*
 * Copyright (C) 2009 Niek Linnenbank
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

#ifndef __INTEL_PROCESS_H
#define __INTEL_PROCESS_H

#include <FreeNOS/Process.h>
#include <Types.h>

/**
 * @addtogroup kernel
 * @{
 *
 * @addtogroup kernel_intel
 * @{
 */

/**
 * Process which may execute on an Intel CPU.
 */
class IntelProcess : public Process
{
  private:

    /** Size of the kernel stack */
    static const Size KernelStackSize = PAGESIZE;

  public:

    /**
     * Constructor function.
     *
     * @param id Process Identifier.
     * @param entry Initial EIP register value.
     * @param privileged If true, the Process has unlimited access to hardware.
     * @param map Virtual memory layout.
     */
    IntelProcess(ProcessID id, Address entry, bool privileged, const MemoryMap &map);

    /**
     * Destructor function.
     */
    virtual ~IntelProcess();

    /**
     * Initialize the Process.
     *
     * Allocates various (architecture specific) resources,
     * creates MMU context and stacks.
     *
     * @return Result code
     */
    virtual Result initialize();

    /**
     * Execute the process.
     *
     * Saves and restores registers, then perform a context switch.
     */
    virtual void execute(Process *previous);

  private:

    /** Current kernel stack address (changes during execution). */
    Address m_kernelStack;

    /** Base kernel stack (fixed) */
    Address m_kernelStackBase;

};

namespace Arch
{
    typedef IntelProcess Process;
};

/**
 * @}
 * @}
 */

#endif /* __INTEL_PROCESS_H */
