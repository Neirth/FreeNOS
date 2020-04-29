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

#ifndef __ARM_ARMPROCESS_H
#define __ARM_ARMPROCESS_H

#include <FreeNOS/Process.h>

/**
 * @addtogroup kernel
 * @{
 *
 * @addtogroup kernel_arm
 * @{
 */

/**
 * ARM specific process implementation.
 */
class ARMProcess : public Process
{
  public:

    /**
     * Constructor function.
     *
     * @param id Process Identifier
     * @param entry Initial program counter value.
     * @param privileged If true, the process has unlimited access to hardware.
     */
    ARMProcess(ProcessID id, Address entry, bool privileged, const MemoryMap &map);

    /**
     * Destructor function.
     */
    virtual ~ARMProcess();

    /**
     * Overwrite the saved CPU registers for this task
     *
     * @param cpuState Pointer to newly saved CPU registers
     */
    void setCpuState(const CPUState *cpuState);

    /**
     * Retrieve saved CPU state.
     *
     * @return Saved CPUState for this task.
     */
    const CPUState * cpuState() const;

    /**
     * Set wait result.
     *
     * @param result Exit status of the other process
     */
    virtual void setWaitResult(uint result);

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
     * Allow the Process to run on the CPU.
     */
    virtual void execute(Process *previous);

  private:

    /** Contains all the CPU registers for this task */
    CPUState m_cpuState;
};


namespace Arch
{
    typedef ARMProcess Process;
};

/**
 * @}
 * @}
 */

#endif /* __ARM_ARMPROCESS_H */
