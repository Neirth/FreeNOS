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

#ifndef __ARM_KERNEL_H
#define __ARM_KERNEL_H

#include <FreeNOS/Kernel.h>
#include <FreeNOS/Process.h>
#include <arm/ARMException.h>
#include <Types.h>

/** Forward declaration */
class IntController;

/**
 * @addtogroup kernel
 * @{
 *
 * @addtogroup kernel_arm
 * @{
 */

/**
 * @name ARM Kernel Traps
 *
 * ARM specific software interrupts.
 * These functions are called by the user program to
 * invoke the kernel APIs, also known as system calls.
 *
 * On ARM, there are no machine-specific input operand
 * constraints to specify registers for inline assembly.
 * Therefore, we need to use Local Register Variables syntax to
 * guarantee that specific registers are used.
 *
 * @see https://gcc.gnu.org/onlinedocs/gcc/Local-Register-Variables.html#Local-Register-Variables
 *
 * @{
 */

/**
 * Perform a kernel trap with 1 argument.
 *
 * @param api Unique number of the handler to execute.
 *
 * @return An integer.
 */
inline ulong trapKernel1(ulong api, ulong arg1)
{
    register unsigned reg0 asm ("r0") = api;
    register unsigned reg1 asm ("r1") = arg1;

    asm volatile ("swi #0\n"
                : "+r"(reg0)
                : "r"(reg0), "r"(reg1)
                : "memory");
    return reg0;
}

/**
 * Perform a kernel trap with 2 arguments.
 *
 * @param api Unique number of the handler to execute.
 * @param arg1 First argument
 * @param arg2 Second argument
 *
 * @return An integer
 */
inline ulong trapKernel2(ulong api, ulong arg1, ulong arg2)
{
    register unsigned reg0 asm ("r0") = api;
    register unsigned reg1 asm ("r1") = arg1;
    register unsigned reg2 asm ("r2") = arg2;

    asm volatile ("swi #0\n"
                : "+r"(reg0)
                : "r"(reg0), "r"(reg1), "r"(reg2)
                : "memory");
    return reg0;
}

/**
 * Perform a kernel trap with 3 arguments.
 *
 * @param api Unique number of the handler to execute.
 * @param arg1 First argument
 * @param arg2 Second argument
 * @param arg3 Third argument
 *
 * @return An integer.
 */
inline ulong trapKernel3(ulong api, ulong arg1, ulong arg2, ulong arg3)
{
    register unsigned reg0 asm ("r0") = api;
    register unsigned reg1 asm ("r1") = arg1;
    register unsigned reg2 asm ("r2") = arg2;
    register unsigned reg3 asm ("r3") = arg3;

    asm volatile ("swi #0\n"
                : "+r"(reg0)
                : "r"(reg0), "r"(reg1), "r"(reg2), "r"(reg3)
                : "memory");
    return reg0;
}

/**
 * Perform a kernel trap with 4 arguments.
 *
 * @param api Unique number of the handler to execute.
 * @param arg1 First argument
 * @param arg2 Second argument
 * @param arg3 Third argument
 * @param arg4 Fourth argument
 *
 * @return An integer.
 */
inline ulong trapKernel4(ulong api, ulong arg1, ulong arg2, ulong arg3,
                         ulong arg4)
{
    register unsigned reg0 asm ("r0") = api;
    register unsigned reg1 asm ("r1") = arg1;
    register unsigned reg2 asm ("r2") = arg2;
    register unsigned reg3 asm ("r3") = arg3;
    register unsigned reg4 asm ("r4") = arg4;

    asm volatile ("swi #0\n"
                : "+r"(reg0)
                : "r"(reg0), "r"(reg1), "r"(reg2), "r"(reg3), "r"(reg4)
                : "memory");
    return reg0;
}

/**
 * Perform a kernel trap with 5 arguments.
 *
 * @param api Unique number of the handler to execute.
 * @param arg1 First argument
 * @param arg2 Second argument
 * @param arg3 Third argument
 * @param arg4 Fourth argument
 * @param arg5 Fifth argument
 *
 * @return An integer.
 */
inline ulong trapKernel5(ulong api, ulong arg1, ulong arg2, ulong arg3,
                         ulong arg4, ulong arg5)
{
    register unsigned reg0 asm ("r0") = api;
    register unsigned reg1 asm ("r1") = arg1;
    register unsigned reg2 asm ("r2") = arg2;
    register unsigned reg3 asm ("r3") = arg3;
    register unsigned reg4 asm ("r4") = arg4;
    register unsigned reg5 asm ("r5") = arg5;

    asm volatile ("swi #0\n"
                : "+r"(reg0)
                : "r"(reg0), "r"(reg1), "r"(reg2), "r"(reg3), "r"(reg4), "r"(reg5)
                : "memory");
    return reg0;
}

/**
 * @}
 */

/**
 * Represents the ARM kernel implementation
 */
class ARMKernel : public Kernel
{
  public:

    /**
     * Constructor function.
     *
     * @param info Contains processor core specific information
     */
    ARMKernel(CoreInfo *info);

  private:

    /**
     * Interrupt handler routine
     *
     * @param state Saved CPU register state
     */
    static void interrupt(CPUState state);

    /**
     * Software trap routine
     *
     * @param state Saved CPU register state
     */
    static void trap(CPUState state);

    /**
     * Undefined instruction routine
     *
     * @param state Saved CPU register state
     */
    static void undefinedInstruction(CPUState state);

    /**
     * Prefetch abort routine
     *
     * @param state Saved CPU register state
     */
    static void prefetchAbort(CPUState state);

    /**
     * Data abort routine
     *
     * @param state Saved CPU register state
     */
    static void dataAbort(CPUState state);

    /**
     * Reserved routine
     *
     * @param state Saved CPU register state
     */
    static void reserved(CPUState state);

  protected:

    /** ARM exception handling subsystem. */
    ARMException m_exception;
};

/**
 * @}
 * @}
 */

#endif /* __ARM_KERNEL_H */
