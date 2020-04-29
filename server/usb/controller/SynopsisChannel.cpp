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
#include <errno.h>
#include "SynopsisChannel.h"
#include "USBDescriptor.h"

SynopsisChannel::SynopsisChannel(Size id, Arch::IO *io)
{
    m_id    = id;
    m_io    = io;
    m_base  = HostChannel + (ChannelSize * id);
    m_state = Idle;
    m_usb   = 0;
    m_msg   = 0;
}

Size SynopsisChannel::getId() const
{
    return m_id;
}

SynopsisChannel::State SynopsisChannel::getState() const
{
    return m_state;
}

const FileSystemMessage * SynopsisChannel::getMessage() const
{
    return m_msg;
}

SynopsisChannel::Result SynopsisChannel::initialize()
{
    // Clear and mask all interrupts
    m_io->write(m_base + ChannelMask, 0);
    m_io->write(m_base + ChannelInterrupt, 0);
    return Success;
}

SynopsisChannel::Result SynopsisChannel::interrupt()
{
    u32 val  = m_io->read(m_base + ChannelInterrupt);

    DEBUG("chanid =" << m_id << " intr = " << val);

    // Print interrupt flags
    if (val & StallResponse)    { ERROR("stalled");        m_usb->state = USBMessage::Failure; }
    if (val & NAKResponse)      { ERROR("no acknowledge"); m_usb->state = USBMessage::Failure; }
    if (val & NYETResponse)     { ERROR("NYET");           m_usb->state = USBMessage::Failure; }
    if (val & TransferError)    { ERROR("txerror");        m_usb->state = USBMessage::Failure; }
    if (val & BusError)         { ERROR("bus error");      m_usb->state = USBMessage::Failure; }
    if (val & FrameOverrun)     { ERROR("frameoverrun");   m_usb->state = USBMessage::Failure; }
    if (val & DataToggleError)  { ERROR("datatoggle");     m_usb->state = USBMessage::Failure; }
    if (val & BufferNotReady)   { ERROR("bufnotready");    m_usb->state = USBMessage::Failure; }
    if (val & TransferExcess)   { ERROR("txexcess");       m_usb->state = USBMessage::Failure; }
    if (val & FrameListRolover) { ERROR("framelist");      m_usb->state = USBMessage::Failure; }

    // Clear and mask all interrupts
    m_io->write(m_base + ChannelMask, 0);
    m_io->write(m_base + ChannelInterrupt, 0);

    // Post-process interrupt
    switch (m_usb->state)
    {
        case USBMessage::Setup:
            m_state = Active;
            if (m_usb->size)
                m_usb->state = USBMessage::Data;
            else
                m_usb->state = USBMessage::Status;
            break;

        case USBMessage::Data:
            m_state = Active;
            m_usb->state = USBMessage::Status;
            break;

        case USBMessage::Status:
            m_usb->state = USBMessage::Success;

            // Only for IN transfers update the size field
            // which represents the actual number of bytes transferred.
            if (m_usb->direction == USBTransfer::In)
            {
                m_usb->size -= (m_io->read(m_base + ChannelTransfer) & 0x7ffff);
                DEBUG("got " << m_usb->size << " bytes from IN transfer");
            }
            // Save the next packet id;
            m_usb->packetId = (u8) ((m_io->read(m_base + ChannelTransfer) >> 29) & 0x3);

            // Reset state
            m_state = Idle;
            m_usb   = ZERO;
            m_msg   = ZERO;
            break;

        case USBMessage::Failure:
            m_state = Idle;
            break;

        default:
            ERROR("unexpected interrupt");
            break;
    }

    // Done
    return Success;
}

SynopsisChannel::Result SynopsisChannel::transfer(const FileSystemMessage *msg,
                                                  USBMessage *usb)
{
    Size size, packetCount, frameNumber;
    PacketId packetId;
    USBTransfer::Direction dir;
    Address data;
    Memory::Range range;

    // Dont let channel take a different message if its claimed already
    if (m_state != Idle && msg != m_msg)
    {
        ERROR("refusing transfer: channel is already processing");
        return Busy;
    }
    // Only submit if we are not already transfering
    if (m_state == Transfer)
        return TransferStarted;

    // Check for buffer alignment
    if (usb->buffer % 4)
    {
        ERROR("data address unaligned: " << (void *)usb->buffer);
        return IOError;
    }

    // Update members
    m_state = Transfer;
    m_usb   = usb;
    m_msg   = msg;

    if (usb->type == USBTransfer::Control)
    {
        switch (usb->state)
        {
            case USBMessage::Setup:
                //DEBUG("id = " << m_id << " setup request = " << (int) m_usb->setup.request);
                size = sizeof(m_usb->setup);
                packetId = SetupId;
                dir = USBTransfer::Out;

                // Flush the entire L1 cache to ensure setup buffer
                // is flushed to system memory.
                VMCtl(SELF, CacheClean, 0);

                // Lookup physical address of setup buffer
                range.virt = (Address) &m_usb->setup;
                VMCtl(SELF, LookupVirtual, &range);
                data = range.phys;
                break;

            case USBMessage::Data:
                //DEBUG("id = " << m_id << " data buffer =" << (void *) m_usb->buffer << " size = " << usb->size);
                size = usb->size;
                packetId = Data1;
                dir = m_usb->direction;
                data = m_usb->buffer;
                break;

            case USBMessage::Status:
                //DEBUG("id = " << m_id << " status");
                size = 0;
                packetId = Data1;
                data = ZERO;
                dir = m_usb->direction == USBTransfer::In ? USBTransfer::Out : USBTransfer::In;
                break;

            default:
                ERROR("unexpected state =" << (int) m_usb->state);
                return IOError;
        }
    }
    // Non-Control transfer
    else
    {
        size = m_usb->size;
        data = m_usb->buffer;
        dir  = m_usb->direction;

        // The packet id is is associated with the endpoint. For non-control
        // transfers the next packet id is saved from the previous transfer.
        packetId = (PacketId) m_usb->packetId;
    }

    // Calculate packet count
    packetCount = CEIL(size, m_usb->maxPacketSize);
    if (!packetCount)
        packetCount = 1;

    DEBUG("chanid = " << m_id << " packets = " << packetCount << " maxpacketsize = "
           << usb->maxPacketSize << " dir = " << (int)dir << " pid = " << (int)packetId);

    // Program channel registers.
    m_io->write(m_base + Characteristics, (usb->endpointId << 11) |
                                          (dir << 15) |
                                          (usb->type << 18) |
                                          (usb->maxPacketSize) |
                                          (1 << 20) /* packetsPerFrame */ |
                                          (usb->deviceId << 22) );

    // SplitControl is needed for Full & Low speed devices only
    m_io->write(m_base + SplitControl, 0);

    // Setup DMA
    m_io->write(m_base + ChannelDMA, data);
    m_io->write(m_base + ChannelTransfer,
                size | (packetId << 29) | (packetCount << 19));

    // Clear and program interrupts
    m_io->write(m_base + ChannelMask, 0);
    m_io->write(m_base + ChannelInterrupt, 0xffffffff);
    m_io->write(m_base + ChannelMask, ChannelHalted);
    m_io->set(HostChannelMask, 1 << m_id);

    // Increase frame number. Set odd flag. Begin transfer.
    frameNumber = (m_io->read(HostFrameNumber) & 0xffff) + 1;
    m_io->set(m_base + Characteristics, ((frameNumber & 1) << 29) | ChannelEnable);
    return TransferStarted;
}

bool SynopsisChannel::operator==(const SynopsisChannel & ch) const
{
    return ch.m_id == m_id;
}

bool SynopsisChannel::operator!=(const SynopsisChannel & ch) const
{
    return ch.m_id != m_id;
}
