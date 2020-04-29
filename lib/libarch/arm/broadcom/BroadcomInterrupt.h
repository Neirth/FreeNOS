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

#ifndef __LIBARCH_ARM_BROADCOMINTERRUPT_H
#define __LIBARCH_ARM_BROADCOMINTERRUPT_H

#include <IntController.h>
#include <arm/ARMIO.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_bcm
 * @{
 */

/**
 * Triggered when the system timer matches the C1 register
 *
 * @see table page 113 of the BCM2835 Arm Peripherals datasheet for vector numbers
 */
#define BCM_IRQ_SYSTIMERM1 1

/**
 * Raspberry Pi Interrupt Controller implementation
 */
class BroadcomInterrupt : public IntController
{
  public:

    /**
     * Constructor
     */
    BroadcomInterrupt();

    /**
     * Enable an IRQ vector
     *
     * @return Result code
     */
    virtual Result enable(uint vector);

    /**
     * Disable an IRQ vector
     *
     * @return Result code
     */
    virtual Result disable(uint vector);

    /**
     * Clear an IRQ vector
     *
     * @return Result code
     */
    virtual Result clear(uint vector);

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
     * @return Result code
     */
    virtual bool isTriggered(uint vector);

  private:

    /** I/O instance */
    ARMIO m_io;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* LIBARCH_ARM_BROADCOMINTERRUPT_H */
