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

#include <FreeNOS/System.h>
#include <Log.h>
#include <Runtime.h>
#include <Callback.h>
#include <NetworkServer.h>
#include "SMSC95xx.h"
#include "SMSC95xxUSB.h"

#define SMSC9512_HS_USB_PKT_SIZE 512
#define SMSC9512_DEFAULT_HS_BURST_CAP_SIZE (16 * 1024 + 5 * SMSC9512_HS_USB_PKT_SIZE)

SMSC95xxUSB::SMSC95xxUSB(u8 deviceId,
                         const char *usbPath,
                         NetworkServer *server,
                         SMSC95xx *smsc)
    : USBDevice(deviceId, usbPath)
{
    DEBUG("");

    m_rxPacket      = 0;
    m_txPacket      = 0;
    m_value         = new u32;
    m_packetSize    = 1500 + TransmitCommandSize;
    m_readFinished  = new Callback<SMSC95xxUSB, FileSystemMessage>(this, &SMSC95xxUSB::readFinished);
    m_writeFinished = new Callback<SMSC95xxUSB, FileSystemMessage>(this, &SMSC95xxUSB::writeFinished);
    m_server        = server;
    m_smsc          = smsc;

    // Set packet header size
    m_smsc->getTransmitQueue()->setHeaderSize(TransmitCommandSize);
}

SMSC95xxUSB::~SMSC95xxUSB()
{
    DEBUG("");

    delete m_value;
}

Error SMSC95xxUSB::initialize()
{
    DEBUG("");

    Error r = USBDevice::initialize();
    if (r != ESUCCESS)
    {
        ERROR("failed to initialize USBDevice");
        return r;
    }
    DEBUG("setting MAC");

    // Set MAC address
    Ethernet::Address a;
    a.addr[0] = 0x00;
    a.addr[1] = 0x11;
    a.addr[2] = 0x22;
    a.addr[3] = 0x33;
    a.addr[4] = 0x44;
    a.addr[5] = 0x55;
    setMACAddress(a);

    // Enable RX/TX bits on hardware
    write(HardwareConfig, read(HardwareConfig) | MultipleEther | BulkIn | BCE);
    write(BurstCap, SMSC9512_DEFAULT_HS_BURST_CAP_SIZE / SMSC9512_HS_USB_PKT_SIZE);
    write(MACControl, read(MACControl) | MACTransmit | MACReceive);
    write(TransmitConfig, TransmitOn);

    // Begin packet receive transfer
    readStart();

    // Done
    return ESUCCESS;
}

void SMSC95xxUSB::readStart()
{
    Size rxSize = m_packetSize;

    DEBUG("");

    // Get receive packet buffer
    m_rxPacket = m_smsc->getReceiveQueue()->get();
    if (!m_rxPacket)
    {
        ERROR("no free receive packet buffer available");
        return;
    }
    // Begin USB transfer
    Error err = beginTransfer(
        USBTransfer::Bulk,
        USBTransfer::In,
        m_endpoints[0].endpointAddress & 0xf,
        m_rxPacket->data,
        rxSize,
        m_endpoints[0].maxPacketSize,
        m_readFinished
    );
    if (err != ESUCCESS)
        ERROR("failed to submit packet receive request");
}

void SMSC95xxUSB::readFinished(FileSystemMessage *message)
{
    DEBUG("identifier = " << message->identifier << " result = " << (int)message->result);

    if (!m_rxPacket)
    {
        ERROR("unexpected readFinish with no receive packet buffer");
        return;
    }

    // Extract packet
    // Offset field contains virtual address of input data buffer
    u8 *data = (u8 *) message->offset;
    u32 receiveCmd = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
    Size frameLength = (receiveCmd & RxCommandFrameLength) >> 16;

    if (frameLength == 0 || frameLength > m_packetSize)
    {
        ERROR("invalid framelength: " << frameLength);
    }
    else
    {
        DEBUG("packet is " << frameLength << " bytes long");

        // Publish the packet to our parent
        m_rxPacket->size = frameLength;
        m_smsc->process(m_rxPacket, ReceiveCommandSize);

        // Release the packet buffer
        m_smsc->getReceiveQueue()->release(m_rxPacket);
        m_rxPacket = 0;
    }

    // Release USB transfer
    finishTransfer(message);

    // Restart read transfer
    readStart();
}

void SMSC95xxUSB::writeStart()
{
    DEBUG("");

    if (m_txPacket)
    {
        ERROR("transmit already in progress");
        return;
    }

    m_txPacket = m_smsc->getTransmitQueue()->pop();
    if (!m_txPacket)
    {
        DEBUG("no transmit data packet available");
        return;
    }

    // Flush L1 cache
    VMCtl(SELF, CacheClean, 0);

    // Start bulk transfer
    beginTransfer(
        USBTransfer::Bulk,
        USBTransfer::Out,
        m_endpoints[1].endpointAddress & 0xf,
        m_txPacket->data,
        m_txPacket->size,
        m_endpoints[1].maxPacketSize,
        m_writeFinished
    );
}

void SMSC95xxUSB::writeFinished(FileSystemMessage *message)
{
    DEBUG("identifier = " << message->identifier << " result = " << (int)message->result);

    if (!m_txPacket)
    {
        ERROR("no transmit packet in progress");
        return;
    }

    // Clean transmit packet
    m_smsc->getTransmitQueue()->release(m_txPacket);
    m_txPacket = 0;

    // Release USB transfer
    finishTransfer(message);

    // Continue with the next packet(s)
    writeStart();
}

Error SMSC95xxUSB::transmit(NetworkQueue::Packet *pkt)
{
    DEBUG("size = " << pkt->size);

    // Check packet size
    if (pkt->size > m_packetSize - TransmitCommandSize)
    {
        ERROR("packet size too large: " << pkt->size);
        return ERANGE;
    }
    // Prepend the physical header to the packet.
    // Note that the Ethernet/IP and/or other headers
    // are already in place inside the packet buffer.

    // Fill command word A
    u32 tx_cmd_a = (pkt->size - TransmitCommandSize) | TxCommandAFirstSeg | TxCommandALastSeg;
    pkt->data[0] = (tx_cmd_a >> 0)  & 0xff;
    pkt->data[1] = (tx_cmd_a >> 8)  & 0xff;
    pkt->data[2] = (tx_cmd_a >> 16) & 0xff;
    pkt->data[3] = (tx_cmd_a >> 24) & 0xff;

    // Fill command word B
    u32 tx_cmd_b = (pkt->size - TransmitCommandSize);
    pkt->data[4] = (tx_cmd_b >> 0)  & 0xff;
    pkt->data[5] = (tx_cmd_b >> 8)  & 0xff;
    pkt->data[6] = (tx_cmd_b >> 16) & 0xff;
    pkt->data[7] = (tx_cmd_b >> 24) & 0xff;

    // Submit packet for transmit
    m_smsc->getTransmitQueue()->push(pkt);

    // begin write, if not already doing
    if (!m_txPacket)
        writeStart();

    // Packet in transmission
    return pkt->size;
}

Error SMSC95xxUSB::getMACAddress(Ethernet::Address *address)
{
    DEBUG("");

    u32 low  = read(MACAddrLow);
    u32 high = read(MACAddrHigh);

    address->addr[0] = (low >> 0) & 0xff;
    address->addr[1] = (low >> 8) & 0xff;
    address->addr[2] = (low >> 16) & 0xff;
    address->addr[3] = (low >> 24) & 0xff;
    address->addr[4] = (high >> 0) & 0xff;
    address->addr[5] = (high >> 8) & 0xff;
    return ESUCCESS;
}

Error SMSC95xxUSB::setMACAddress(Ethernet::Address address)
{
    DEBUG("");

    write(MACAddrLow, address.addr[0] |
                      (address.addr[1] << 8) |
                      (address.addr[2] << 16) |
                      (address.addr[3] << 24));
    write(MACAddrHigh, address.addr[4] |
                      (address.addr[5] << 8));
    return ESUCCESS;
}

u32 SMSC95xxUSB::read(SMSC95xxUSB::Register reg)
{
    Error r = controlMessage(ReadRegister,
                             USBTransfer::In,
                             USBTransfer::Vendor,
                             USBTransfer::Device,
                             0, (u16)reg, m_value, sizeof(*m_value));

    if (r != ESUCCESS)
    {
        ERROR("failed to read SMSC95xxUSB register at offset " <<
               (uint) reg);
        return 0;
    }
    return (*m_value);
}

void SMSC95xxUSB::write(SMSC95xxUSB::Register reg, u32 value)
{
    *m_value = value;
    Error r = controlMessage(WriteRegister,
                             USBTransfer::Out,
                             USBTransfer::Vendor,
                             USBTransfer::Device,
                             0, (u16)reg, m_value, sizeof(*m_value));
    if (r != ESUCCESS)
    {
        ERROR("failed to write SMSC95xxUSB register at offset " <<
               (uint) reg << " with value " << *m_value);
    }
}
