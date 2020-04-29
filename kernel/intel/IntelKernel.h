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

#ifndef __INTEL_KERNEL_H
#define __INTEL_KERNEL_H

#include <Types.h>
#include <BootImage.h>

/**
 * @addtogroup kernel
 * @{
 *
 * @addtogroup kernel_intel
 * @{
 */

/**
 * @name Intel Kernel Traps
 *
 * Intel specific software interrupts.
 * These functions are called by the user program to
 * invoke the kernel APIs, also known as system calls.
 *
 * @{
 */

/**
 * Perform a kernel trap with 1 argument.
 *
 * @param num Unique number of the handler to execute.
 * @param arg1 First argument becomes ECX.
 *
 * @return An integer.
 */
inline ulong trapKernel1(ulong num, ulong arg1)
{
    ulong ret;
    asm volatile ("int $0x90" : "=a"(ret) : "a"(num), "c"(arg1) : "memory");
    return ret;
}

/**
 * Perform a kernel trap with 2 arguments.
 *
 * @param num Unique number of the handler to execute.
 * @param arg1 First argument becomes ECX.
 * @param arg2 Second argument becomes EBX.
 *
 * @return An integer.
 */
inline ulong trapKernel2(ulong num, ulong arg1, ulong arg2)
{
    ulong ret;
    asm volatile ("int $0x90" : "=a"(ret) : "a"(num), "c"(arg1), "b"(arg2) : "memory");
    return ret;
}

/**
 * Perform a kernel trap with 3 arguments.
 *
 * @param num Unique number of the handler to execute.
 * @param arg1 First argument becomes ECX.
 * @param arg2 Second argument becomes EBX.
 * @param arg3 Third argument becomes EDX.
 *
 * @return An integer.
 */
inline ulong trapKernel3(ulong num, ulong arg1, ulong arg2, ulong arg3)
{
    ulong ret;
    asm volatile ("int $0x90" : "=a"(ret) : "a"(num), "c"(arg1), "b"(arg2),
                        "d"(arg3) : "memory");
    return ret;
}

/**
 * Perform a kernel trap with 4 arguments.
 *
 * @param num Unique number of the handler to execute.
 * @param arg1 First argument becomes ECX.
 * @param arg2 Second argument becomes EBX.
 * @param arg3 Third argument becomes EDX.
 * @param arg4 Fourth argument becomes ESI.
 *
 * @return An integer.
 */
inline ulong trapKernel4(ulong num, ulong arg1, ulong arg2, ulong arg3,
             ulong arg4)
{
    ulong ret;
    asm volatile ("int $0x90" : "=a"(ret) : "a"(num), "c"(arg1), "b"(arg2),
                        "d"(arg3), "S"(arg4) : "memory");
    return ret;
}

/**
 * Perform a kernel trap with 5 arguments.
 *
 * @param num Unique number of the handler to execute.
 * @param arg1 First argument becomes ECX.
 * @param arg2 Second argument becomes EBX.
 * @param arg3 Third argument becomes EDX.
 * @param arg4 Fourth argument becomes ESI.
 * @param arg5 Fifth argument becomes EDI.
 *
 * @return An integer.
 */
inline ulong trapKernel5(ulong num, ulong arg1, ulong arg2, ulong arg3,
             ulong arg4, ulong arg5)
{
    ulong ret;
    asm volatile ("int $0x90" : "=a"(ret) : "a"(num), "c"(arg1), "b"(arg2),
                 "d"(arg3), "S"(arg4), "D"(arg5) : "memory");
    return ret;
}

/**
 * @}
 * @}
 * @}
 */

#include <FreeNOS/Kernel.h>
#include <intel/IntelPIT.h>
#include <intel/IntelPIC.h>
#include <intel/IntelAPIC.h>
#include <Timer.h>

/**
 * @addtogroup kernel
 * @{
 *
 * @addtogroup kernel_intel
 * @{
 */

/**
 * Remap IRQs to interrupt vectors 32-47.
 */
#define IRQ(vector) \
    (vector) + 32

/**
 * Implements an x86 compatible kernel.
 */
class IntelKernel : public Kernel
{
  public:

    /**
     * Constructor function.
     */
    IntelKernel(CoreInfo *info);

    /**
     * Enable or disable an hardware interrupt (IRQ).
     *
     * @param irq IRQ number.
     * @param enabled True to enable, and false to disable.
     */
    virtual void enableIRQ(u32 irq, bool enabled);

  private:

    /**
     * Called when the CPU detects a fault.
     *
     * @param state Contains CPU registers, interrupt vector and error code.
     * @param param Not used.
     * @param vector Not used.
     */
    static void exception(CPUState *state, ulong param, ulong vector);

    /**
     * Default interrupt handler.
     *
     * @param state Contains CPU registers, interrupt vector and error code.
     * @param param Not used.
     * @param vector Not used.
     */
    static void interrupt(CPUState *state, ulong param, ulong vector);

    /**
     * Kernel trap handler (system calls).
     *
     * @param state Contains the arguments for the APIHandler, in CPU registers.
     * @param param Not used.
     * @param vector Not used.
     */
    static void trap(CPUState *state, ulong param, ulong vector);

    /**
     * i8253 system clock interrupt handler.
     *
     * @param state CPU registers on time of interrupt.
     * @param param Not used.
     * @param vector Not used.
     */
    static void clocktick(CPUState *state, ulong param, ulong vector);

  private:

    /** PIT timer instance */
    IntelPIT m_pit;

    /** APIC instance (used if available) */
    IntelAPIC m_apic;

    /** PIC instance */
    IntelPIC m_pic;
};

/**
 * @}
 * @}
 */

#endif /* __INTEL_KERNEL_H */
