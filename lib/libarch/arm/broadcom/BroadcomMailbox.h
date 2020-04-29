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

#ifndef __LIBARCH_ARM_BROADCOM_MAILBOX_H
#define __LIBARCH_ARM_BROADCOM_MAILBOX_H

#include <Types.h>
#include <Macros.h>
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
 * Broadcom System-on-Chip Mailbox interface.
 *
 * The Mailbox on the BCM2835 is a message-based interface between
 * the ARM and the GPU. It provides channels for sending and receiving
 * messages.
 *
 * @see https://github.com/raspberrypi/firmware/wiki/Mailboxes
 * @see https://github.com/raspberrypi/firmware/wiki/Accessing-mailboxes
 * @see https://github.com/torvalds/linux/blob/master/drivers/mailbox/bcm2835-mailbox.c
 */
class BroadcomMailbox
{
  private:

    /** Register base offset for the Mailbox. */
    static const Address Base = 0xb000;

    /** Channel Mask when reading or writing (lowest 4-bits). */
    static const uint ChannelMask = 0xf;

    /**
     * Hardware registers.
     */
    enum Register
    {
        Read   = 0x0880,
        Status = 0x0898,
        Config = 0x089c,
        Write  = 0x08a0
    };

    /**
     * Status register flags.
     */
    enum StatusFlags
    {
        Full   = 0x80000000,
        Empty  = 0x40000000
    };

  public:

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        IOError
    };

    /**
     * Mailbox channels.
     */
    enum Channel
    {
        PowerManagement = 0,
        FrameBuffer     = 1,
        VirtualUART     = 2,
        VCHIQ           = 3,
        LED             = 4,
        Button          = 5,
        TouchScreen     = 6,
        VCProperty      = 8,
        ARMProperty     = 9
    };

  public:

    /**
     * Constructor
     */
    BroadcomMailbox();

    /**
     * Initialize the Mailbox.
     *
     * @return Result code.
     */
    Result initialize();

    /**
     * Read 28-bit message.
     *
     * @param channel
     * @param message 28-bit message on output
     *
     * @return Result code.
     */
    Result read(Channel channel, u32 *message) const;

    /**
     * Write 28-bit message.
     *
     * @param channel Channel to write to.
     * @param message 28-bit message to write.
     *
     * @return Result code.
     */
    Result write(Channel channel, u32 message);

  private:

    /** I/O object */
    ARMIO m_io;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_ARM_BROADCOM_MAILBOX_H */
