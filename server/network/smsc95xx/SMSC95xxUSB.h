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

#ifndef __LIBNET_SMSC95XXUSB_H
#define __LIBNET_SMSC95XXUSB_H

#include <USBDevice.h>
#include <Ethernet.h>
#include <NetworkQueue.h>

class NetworkServer;
class SMSC95xx;

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup smsc95xx
 * @{
 */

/**
 * SMSC95xx USB-based Ethernet controller.
 */
class SMSC95xxUSB : public USBDevice
{
  private:

    /** vendor ID in the USB device descriptor for this device */
    static const u16 vendorId = 0x0424;

    /** product ID in the USB device descriptor for this device */
    static const u16 productId = 0xec00;

    /**
     * Hardware registers.
     */
    enum Register
    {
        DeviceID        = 0x00,
        InterruptStatus = 0x08,
        ReceiveConfig   = 0x0c,
        TransmitConfig  = 0x10,
        HardwareConfig  = 0x14,
        ReceiveFIFOInfo = 0x18,
        PowerControl    = 0x20,
        LEDConfig       = 0x24,
        GPIOConfig      = 0x28,
        FlowControl     = 0x2c,
        EEPromCommand   = 0x30,
        EEPromData      = 0x34,
        BurstCap        = 0x38,
        GPIOWake        = 0x64,
        MACControl      = 0x100,
        MACAddrHigh     = 0x104,
        MACAddrLow      = 0x108,
        MIIControl      = 0x114
    };

    /**
     * InterruptStatus register flags.
     */
    enum InterruptStatusFlags
    {
        TransferStop    = 0x00020000,
        ReceiveStop     = 0x00010000,
        PhyInterrupt    = 0x00008000
    };

    /**
     * TransmitConfig register flags.
     */
    enum TransmitConfigFlags
    {
        TransmitOn      = 0x00000004,
        TransmitOff     = 0x00000002,
        TransmitFlush   = 0x00000001
    };

    /**
     * HardwareConfig register flags.
     */
    enum HardwareConfigFlags
    {
        BulkIn          = 0x00001000,
        MultipleEther   = 0x00000020,
        BCE             = 0x00000002
    };

    /**
     * MACControl register flags.
     */
    enum MACControlFlags
    {
        MACTransmit     = 0x00000008,
        MACReceive      = 0x00000004
    };

    /**
     * Various vendor-specific USB control message requests.
     */
    enum USBRequest
    {
        WriteRegister = 0xa0,
        ReadRegister  = 0xa1,
        GetStatus     = 0xa2
    };

    /**
     * Transmit Command Words.
     */
    enum TransmitCommand
    {
        TxCommandADataOffset = 0x001F0000,
        TxCommandAFirstSeg   = 0x00002000,
        TxCommandALastSeg    = 0x00001000,
        TxCommandABufSize    = 0x000007FF,
        TxCommandBChecksum   = 0x00004000,
        TxCommandBCRC        = 0x00002000,
        TxCommandBPadding    = 0x00001000,
        TxCommandBLength     = 0x000007FF
    };

    /**
     * Receive Command Words.
     */
    enum ReceiveCommand
    {
        RxCommandFilterFail      = 0x40000000,
        RxCommandFrameLength     = 0x3fff0000,
        RxCommandErrorSummary    = 0x00008000,
        RxCommandBroadcastFrame  = 0x00002000,
        RxCommandLengthError     = 0x00001000,
        RxCommandRuntFrame       = 0x00000800,
        RxCommandMulticastFrame  = 0x00000400,
        RxCommandFrameTooLong    = 0x00000080,
        RxCommandFrameCollision  = 0x00000040,
        RxCommandFrameType       = 0x00000020,
        RxCommandReceiveWatchdog = 0x00000010,
        RxCommandMIIError        = 0x00000008,
        RxCommandDribbling       = 0x00000004,
        RxCommandCRCError        = 0x00000002
    };

  public:

    /** Transmit Command Word size. */
    static const Size TransmitCommandSize = 8;

    /** Receive Command Word size. */
    static const Size ReceiveCommandSize = 4;

    /**
     * Constructor
     */
    SMSC95xxUSB(u8 deviceId,
                const char *usbPath,
                NetworkServer *server,
                SMSC95xx *smsc);

    /**
     * Destructor
     */
    virtual ~SMSC95xxUSB();

    /**
     * Initialize the controller.
     *
     * @return Result code
     */
    virtual Error initialize();

    /**
     * Transmit one network packet
     *
     * @param pkt Network packet buffer
     */
    virtual Error transmit(NetworkQueue::Packet *pkt);

  private:

    void readStart();
    void readFinished(FileSystemMessage *message);

    void writeStart();
    void writeFinished(FileSystemMessage *message);

    /**
     * Get MAC address.
     */
    Error getMACAddress(Ethernet::Address *address);

    /**
     * Set MAC address.
     */
    Error setMACAddress(Ethernet::Address address);

    /**
     * Read a SMSC95xx register.
     */
    u32 read(Register reg);

    /**
     * Write a SMSC95xx register.
     */
    void write(Register reg, u32 value);

  private:

    /** Word aligned temporary value */
    u32 *m_value;

    /** Callback object for readFinished() */
    Callback<SMSC95xxUSB, FileSystemMessage> *m_readFinished;

    /** Callback object for writeFinished() */
    Callback<SMSC95xxUSB, FileSystemMessage> *m_writeFinished;

    Size m_packetSize;
    NetworkServer *m_server;
    SMSC95xx *m_smsc;
    NetworkQueue::Packet *m_rxPacket;
    NetworkQueue::Packet *m_txPacket;
};

/**
 * @}
 * @}
 */

#endif /* __LIBNET_SMSC95XXUSB_H */
