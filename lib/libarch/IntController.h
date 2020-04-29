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

#ifndef __LIBARCH_INTCONTROLLER_H
#define __LIBARCH_INTCONTROLLER_H

#include <Types.h>
#include <Macros.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 */

/**
 * Interrupt controller interface.
 */
class IntController
{
  public:

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        InvalidIRQ,
        InvalidFrequency,
        IOError,
        NotFound
    };

    /**
     * Constructor.
     */
    IntController();

    /**
     * Get interrupt number base offset.
     *
     * Some interrupt controllers remap the interrupt numbers
     * to a certain base offset.
     *
     * @return Interrupt number base offset.
     */
    uint getBase() const;

    /**
     * Enable hardware interrupt (IRQ).
     *
     * @param irq Interrupt Request number.
     *
     * @return Result code.
     */
    virtual Result enable(uint irq) = 0;

    /**
     * Disable hardware interrupt (IRQ).
     *
     * @param irq Interrupt Request number.
     *
     * @return Result code.
     */
    virtual Result disable(uint irq) = 0;

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
    virtual Result clear(uint irq) = 0;

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

  protected:

    /** Interrupt number base offset */
    uint m_base;

};

/**
 * @}
 * @}
 */

#endif /* __LIBARCH_INTCONTROLLER_H */
