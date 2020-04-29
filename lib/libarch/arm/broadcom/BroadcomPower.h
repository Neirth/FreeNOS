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

#ifndef __LIBARCH_ARM_BROADCOMPOWER_H
#define __LIBARCH_ARM_BROADCOMPOWER_H

#include <Types.h>
#include <Macros.h>
#include <arm/broadcom/BroadcomMailbox.h>

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
 * Broadcom Power Management.
 */
class BroadcomPower
{
  public:

    /**
     * Powered devices.
     */
    enum Device
    {
        SD    = (1 << 0),
        UART0 = (1 << 1),
        UART1 = (1 << 2),
        USB   = (1 << 3)
    };

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        IOError
    };

  public:

    /**
     * Constructor.
     */
    BroadcomPower();

    /**
     * Initialize the power manager.
     *
     * @return Result code.
     */
    Result initialize();

    /**
     * Set power on.
     *
     * @param device Device to power on.
     *
     * @return Result code.
     */
    Result enable(Device device);

  private:

    /** Mailbox for communicating with the GPU. */
    BroadcomMailbox m_mailbox;

    /** Current bitmask of enabled devices. */
    u32 m_mask;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_ARM_BROADCOMPOWER_H */
