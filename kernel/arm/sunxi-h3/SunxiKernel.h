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

#ifndef __ARM_KERNEL_SUNXIKERNEL_H
#define __ARM_KERNEL_SUNXIKERNEL_H

#include <FreeNOS/arm/ARMKernel.h>
#include <arm/ARMGenericInterrupt.h>
#include <arm/ARMTimer.h>

/**
 * @addtogroup kernel
 * @{
 *
 * @addtogroup kernel_arm
 * @{
 */

/**
 * Represents the Sunxi kernel implementation
 */
class SunxiKernel : public ARMKernel
{
  public:

    /**
     * Constructor function.
     *
     * @param info Contains processor core specific information
     */
    SunxiKernel(CoreInfo *info);

  private:

    /**
     * Interrupt handler routine
     *
     * @param state Saved CPU register state
     */
    static void interrupt(CPUState state);

  private:

    /** ARM Generic Interrupt Controller */
    ARMGenericInterrupt m_gic;

    /** ARM generic timer */
    ARMTimer m_armTimer;
};

/**
 * @}
 * @}
 */

#endif /* __ARM_KERNEL_SUNXIKERNEL_H */
