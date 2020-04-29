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

#ifndef __LIBARCH_ARM_BROADCOM2836_H
#define __LIBARCH_ARM_BROADCOM2836_H

#include <arm/ARMIO.h>
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
 * Broadcom 2836 device specific I/O configuration
 */
class Broadcom2836
{
  private:

    /** Base address for the BCM2836 I/O configuration */
    static const Address IOBase = 0x40000000;

    /**
     * Hardware registers.
     */
    enum Registers
    {
        CorePrescalerRegister   = 0x0008,
        CoreTimerRegister       = 0x0040,
        CoreIrqRegister         = 0x0060,
        CoreSetMailboxRegister  = 0x0080
    };

  public:

    /**
     * Hardware timers available in BCM 2836.
     */
    enum Timer
    {
        PhysicalTimer1
    };

    /**
     * Result code.
     */
    enum Result
    {
        Success,
        NotFound,
        IOError,
        FrequencyError
    };

  public:

    /**
     * Constructor
     */
    Broadcom2836(Size coreId);

    /**
     * Initialize the Controller.
     *
     * @return Result code
     */
    Result initialize();

    /**
     * Set Core Timer interrupt
     *
     * @param timer Timer to set
     * @param enable True to enable the timer interrupt
     *
     * @return Result code
     */
    Result setCoreTimerIrq(Timer timer, bool enable);

    /**
     * Get core timer interrupt status
     *
     * @param timer Timer to check for interrupt status
     *
     * @return True if timer IRQ is raised, false otherwise.
     */
    bool getCoreTimerIrqStatus(Timer timer) const;

  private:

    /** Core identifier */
    Size m_coreId;

    /** I/O instance */
    ARMIO m_io;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_ARM_BROADCOM2836_H */
