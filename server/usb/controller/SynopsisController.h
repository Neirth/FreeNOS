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

#ifndef __LIBUSB_SYNOPSISUSB_H
#define __LIBUSB_SYNOPSISUSB_H

#include <FreeNOS/System.h>
#include <Types.h>
#include <Index.h>
#include <arm/broadcom/BroadcomPower.h>
#include "USBController.h"
#include "SynopsisChannel.h"

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup usb
 * @{
 *
 * @addtogroup usb_controller
 * @{
 */

/**
 * Synopsis DesignWare USB Host Controller implementation.
 *
 * @see https://github.com/xinu-os/xinu/system/platforms/arm-rpi/usb_dwc_regs.h
 */
class SynopsisController : public USBController
{
  private:

    /** Register base (offset from I/O base) for the USB controller. */
    static const Address Base = 0x980000;

    /** Vendor Identity number for this device */
    static const uint DefaultVendorId = 0x4f54280a;

    /** Interrupt number for the USB controller in BCM2835. */
    static const uint InterruptNumber = 9;

    /** Number of transfer channels supported by hardware. */
    static const Size ChannelCount = 8;

    /**
     * Hardware registers.
     */
    enum Registers
    {
        OTGControl        = 0x000,
        OTGInterrupt      = 0x004,
        AHBConfig         = 0x008,
        CoreConfig        = 0x00c,
        CoreReset         = 0x010,
        CoreInterrupt     = 0x014,
        CoreIntMask       = 0x018,
        ReceiveStatus     = 0x01c,
        ReceiveStatusPop  = 0x020,
        RxFIFOSize        = 0x024,
        TxFIFOSize        = 0x028,
        TxFIFOStatus      = 0x02c,
        I2CControl        = 0x030,
        PhyVendorControl  = 0x034,
        GPIO              = 0x038,
        UserId            = 0x03c,
        VendorId          = 0x040,
        HardwareConfig1   = 0x044,
        HardwareConfig2   = 0x048,
        HardwareConfig3   = 0x04c,
        HardwareConfig4   = 0x050,
        CoreLPMConfig     = 0x054,
        GlobalPowerDn     = 0x058,
        GlobalFIFOConfig  = 0x05c,
        ADPControl        = 0x060,
        PeriodTxFIFOSize  = 0x100,
        HostConfig        = 0x400,
        HostFrameInterval = 0x404,
        HostFrameNumber   = 0x408,
        HostFIFOStatus    = 0x410,
        HostChannelInt    = 0x414,
        HostChannelMask   = 0x418,
        HostFrameList     = 0x41c,
        HostPortControl   = 0x440,
        HostChannel       = 0x500,
        Power             = 0xe00
    };

    /**
     * AHBConfig register flags.
     */
    enum AHBConfigFlags
    {
        InterruptEnable = (1 << 0),
        AXIWait         = (1 << 4),
        DMAEnable       = (1 << 5)
    };

    /**
     * CoreInterrupt register flags.
     */
    enum CoreIntFlags
    {
        CoreIntPort    = (1 << 24), /**< Host port status changed. */
        CoreIntChannel = (1 << 25)  /**< Channel interrupt occurred. */
    };

    /**
     * HostPortControl register flags.
     */
    enum HostPortControlFlags
    {
        HostPortConnect        = (1 << 0),
        HostPortConnectChanged = (1 << 1),
        HostPortEnable         = (1 << 2),
        HostPortEnableChanged  = (1 << 3),
        HostPortCurrent        = (1 << 4),
        HostPortCurrentChanged = (1 << 5),
        HostPortReset          = (1 << 8),
        HostPortPower          = (1 << 12),
        /** 00: high speed 01: full speed 10: low speed */
        HostPortSpeed          = (1 << 17) | (1 << 18),
    };

    /**
     * Packet ID constants.
     */
    enum PacketId
    {
        Data0   = 0,
        Data1   = 2,
        Data2   = 1,
        SetupId = 3
    };

  public:

    /**
     * Constructor
     */
    SynopsisController(const char *path);

    /**
     * Initialize the Controller.
     *
     * @return Result code
     */
    virtual Error initialize();

    /**
     * Submit USB transfer.
     *
     * @return Result code
     */
    virtual Error transfer(const FileSystemMessage *msg,
                           USBMessage *usb);

  private:

    /**
     * Get a Channel.
     *
     * @return Channel object pointer or ZERO.
     */
    SynopsisChannel * getChannel(const FileSystemMessage *msg,
                                 USBMessage *usb);

    /**
     * Interrupt request handler.
     *
     * @param vector Interrupt vector
     */
    virtual void interruptHandler(Size vector);

    /**
     * Called when the Host Port status changes.
     */
    void hostPortChanged();

    /**
     * Reset the Controller.
     */
    void softReset();

  private:

    /** Power Management. */
    BroadcomPower m_power;

    /** Channels. */
    Index<SynopsisChannel> m_channels;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBUSB_SYNOPSISUSB_H */
