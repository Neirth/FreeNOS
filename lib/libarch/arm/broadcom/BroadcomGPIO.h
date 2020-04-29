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

#ifndef __LIBARCH_ARM_BROADCOMGPIO_H
#define __LIBARCH_ARM_BROADCOMGPIO_H

#include <FreeNOS/System.h>
#include <Types.h>

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
 * Broadcom (2835) GPIO controller implementation.
 */
class BroadcomGPIO
{
  private:

    static const Address GPIOBase = 0x200000;

    /**
     * Hardware registers.
     */
    enum Registers
    {
        FunctionSelect0 = 0
    };

  public:

    /**
     * Pin modes.
     */
    enum Mode
    {
        Input     = 0,
        Output    = 1,
        Function0 = 4,
        Function1 = 5,
        Function2 = 6,
        Function3 = 7,
        Function4 = 3,
        Function5 = 2
    };

    /**
     * Result code.
     */
    enum Result
    {
        Success,
        NotFound,
        IOError
    };

  public:

    /**
     * Constructor
     */
    BroadcomGPIO();

    /**
     * Initialize the Controller.
     *
     * @return Result code
     */
    virtual Result initialize();

    /**
     * Set alternate function.
     *
     * @param pin Pin number to set.
     * @param function Alternate function number to set.
     *
     * @return Result code.
     */
    virtual Result setAltFunction(Size pin, uint function);

  private:

    /** I/O instance */
    Arch::IO m_io;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_ARM_BROADCOMGPIO_H */
