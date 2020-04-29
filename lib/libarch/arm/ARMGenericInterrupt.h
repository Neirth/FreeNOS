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

#ifndef __LIBARCH_ARM_ARMGENERICINTERRUPT_H
#define __LIBARCH_ARM_ARMGENERICINTERRUPT_H

#include <IntController.h>
#include "ARMIO.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_arm
 * @{
 */

/**
 * ARM Generic Interrupt Controller (GIC) version 2.
 */
class ARMGenericInterrupt : public IntController
{
  private:

    /**
     * Distributor register interface
     */
    enum DistRegisters
    {
        GICD_CTRL       = 0x0000,
        GICD_TYPER      = 0x0004,
        GICD_GROUPR     = 0x0080,
        GICD_ISENABLER  = 0x0100,
        GICD_ICENABLER  = 0x0180,
        GICD_ICPENDR    = 0x0280,
        GICD_ISACTIVER  = 0x0300,
        GICD_ICACTIVER  = 0x0380,
        GICD_IPRIORITYR = 0x0400,
        GICD_ITARGETSR  = 0x0800,
        GICD_ICFGR      = 0x0C00,
        GICD_ID2        = 0x0FE8
    };

    enum DistCtrlFlags
    {
        DistCtrlGroup0 = (1 << 0),
        DistCtrlGroup1 = (1 << 1)
    };

    enum DistTypeFlags
    {
        DistTypeIrqsMask = (0xf)
    };

    /**
     * CPU register interface
     */
    enum CpuRegisters
    {
        GICC_CTRL      = 0x0000,
        GICC_PMR       = 0x0004,
        GICC_IAR       = 0x000C,
        GICC_EOIR      = 0x0010,
        GICC_IDR       = 0x00FC,
        GICC_DIR       = 0x1000
    };

    enum CpuCtrlFlags
    {
        CpuCtrlGroup0 = (1 << 0),
        CpuCtrlGroup1 = (1 << 1)
    };

    enum CpuIrqAckFlags
    {
        CpuIrqAckMask = (0x3ff)
    };

  public:

    /**
     * Constructor
     *
     * @param distRegisterBase GIC distributor register base offset
     * @param cpuRegisterBase GIC CPU register base offset
     */
    ARMGenericInterrupt(Address distRegisterBase,
                        Address cpuRegisterBase);

    /**
     * Enable hardware interrupt (IRQ).
     *
     * @param irq Interrupt Request number.
     *
     * @return Result code.
     */
    virtual Result enable(uint irq);

    /**
     * Disable hardware interrupt (IRQ).
     *
     * @param irq Interrupt Request number.
     *
     * @return Result code.
     */
    virtual Result disable(uint irq);

    /**
     * Clear hardware interrupt (IRQ).
     *
     * Clearing marks the end of an interrupt service routine
     * and causes the controller to trigger the interrupt again
     * on the next trigger moment.
     *
     * @param irq Interrupt Request number to clear.
     *
     * @return Result code.
     */
    virtual Result clear(uint irq);

    /**
     * Retrieve the next pending interrupt (IRQ).
     *
     * @param irq Outputs the next pending interrupt on Success
     *
     * @return Result code.
     */
    virtual Result nextPending(uint & irq);

    /**
     * Check if an IRQ vector is set.
     *
     * @param irq Interrupt number
     *
     * @return True if triggered. False otherwise
     */
    virtual bool isTriggered(uint irq);

  private:

    /**
     * Calculate the number of 32-bit registers needed to represent given number of bits per IRQ.
     *
     * @param bits Bits needed per IRQ
     *
     * @return Number of registers needed
     */
    Size numRegisters(Size bits) const;

  private:

    /** ARM Generic Interrupt Controller Distributor Interface */
    ARMIO m_dist;

    /** ARM Generic Interrupt Controller CPU Interface */
    ARMIO m_cpu;

    /** Number of interrupts supported */
    Size m_numIrqs;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_ARM_ARMGENERICINTERRUPT_H */
