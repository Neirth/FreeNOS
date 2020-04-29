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
#include <MemoryBlock.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <Runtime.h>
#include <FileDescriptor.h>
#include "USBDevice.h"
#include "USBDescriptor.h"

USBDevice::USBDevice(u8 deviceId, const char *busPath)
    : Device(CharacterDeviceFile)
{
    m_id = deviceId;
    m_busPath = busPath;
    m_transferFile = -1;
    m_device = new USBDescriptor::Device;
    m_config = new USBDescriptor::Configuration;

    for (Size i = 0; i < m_endpointsPacketId.size(); i++)
        m_endpointsPacketId.insert(i, 0);

    MemoryBlock::set(m_device, 0, sizeof(*m_device));
    MemoryBlock::set(m_config, 0, sizeof(*m_config));
}

USBDevice::~USBDevice()
{
    delete m_device;
    delete m_config;
}

Error USBDevice::initialize()
{
    DEBUG("");

    String usbTransfer;
    Address actualId = m_id;

    usbTransfer << *m_busPath << "/transfer";

    // Try to open the USB transfer file on the USB host controller
    if ((m_transferFile = ::open(*usbTransfer, O_RDWR)) < 0)
    {
        ERROR("failed to open " << *usbTransfer << ": " << strerror(errno) << "\n");
        return errno;
    }
    DEBUG("opened: " << *usbTransfer);

    // Every unconfigured USB device starts with address zero
    m_id = 0;

    // First retrieve the device descriptor in 8-bytes packet only
    // This is to figure out the maxPacketSize which is needed for transfers
    DEBUG("getting max packet size");
    if (getDeviceDescriptor(m_device, 8) != ESUCCESS)
    {
        ERROR("failed to get max packet size");
        return EIO;
    }

    DEBUG("maxPacketSize = " << m_device->maxPacketSize);

    // Retrieve device descriptor
    DEBUG("getting device descriptor");
    if (getDeviceDescriptor(m_device) != ESUCCESS)
    {
        ERROR("failed to get device descriptor");
        return EIO;
    }

    DEBUG("got descriptor: vendorId=" << m_device->vendorId <<
                        " productId=" << m_device->productId <<
                        " class=" << m_device->deviceClass <<
                        " subClass=" << m_device->deviceSubClass <<
                        " maxpacketsize=" << m_device->maxPacketSize <<
                        " protocol=" << m_device->deviceProtocol);

    // Retrieve configuration descriptor
    if (getConfigDescriptor(m_config) != ESUCCESS)
    {
        ERROR("failed to get configuration descriptor");
        return EIO;
    }
    // Check configuration length
    if (m_config->totalLength < sizeof(USBDescriptor::Configuration))
    {
        ERROR("invalid size for configuration descriptor: " << m_config->totalLength);
        return EIO;
    }

    // Set address of the USB device.
    if (setAddress(actualId) != ESUCCESS)
    {
        ERROR("failed to set address to: " << actualId);
        return EIO;
    }

    // Activate the first configuration on the USB device.
    if (setConfiguration(m_config->configurationValue) != ESUCCESS)
    {
        ERROR("failed to activate configurationValue: " << m_config->configurationValue);
        return EIO;
    }

    // Retrieve the full configuration descriptor, which
    // includes other descriptors (interface and endpoints)
    // just after the configuration descriptor.
    Address desc = (Address) new u8[m_config->totalLength];
    if (!desc)
    {
        ERROR("failed to allocate descriptors buffer");
        return EIO;
    }
    if (getConfigDescriptor((USBDescriptor::Configuration *) desc, m_config->totalLength) != ESUCCESS)
    {
        ERROR("failed to get full configuration descriptors");
        return EIO;
    }

    // Parse all descriptors received
    for (Size offset = 0; offset < m_config->totalLength;)
    {
        USBDescriptor::Configuration *conf = (USBDescriptor::Configuration *) (desc + offset);
        switch (conf->descriptorType)
        {
            case USBDescriptor::DeviceType:
                DEBUG("::Device at " << offset);
                offset += conf->length;
                break;

            case USBDescriptor::ConfigurationType:
                DEBUG("::Configuration at " << offset);
                offset += conf->length;
                break;

            case USBDescriptor::InterfaceType: {
                DEBUG("::Interface at " << offset);
                USBDescriptor::Interface *iface = (USBDescriptor::Interface *) (desc + offset);
                m_interfaces.insert(*iface);
                offset += iface->length;
                break;
            }
            case USBDescriptor::EndpointType: {
                USBDescriptor::Endpoint *ep = (USBDescriptor::Endpoint *) (desc + offset);
                DEBUG("::Endpoint at " << offset << " addr = " << (ep->endpointAddress & 0xf) <<
                      " dir = " << (ep->endpointAddress >> 7) << " attr = " << ((ep->attributes) & 0x3));
                m_endpoints.insert(*ep);
                offset += ep->length;
                break;
            }
            case USBDescriptor::StringType: {
                DEBUG("::String at " << offset);
                USBDescriptor::String *str = (USBDescriptor::String *) (desc + offset);
                m_strings.insert(*str);
                offset += str->length;
                break;
            }
            case USBDescriptor::HubType: {
                DEBUG("::Hub at " << offset);
                offset += conf->length;
                break;
            }
            default: {
                DEBUG("unknown descriptor at " << offset);
                offset += conf->length;
                break;
            }
        }
    }
    return ESUCCESS;
}

Error USBDevice::getDeviceDescriptor(USBDescriptor::Device *desc, Size size)
{
    DEBUG("");

    return controlMessage(USBTransfer::GetDescriptor,
                          USBTransfer::In,
                          USBTransfer::Standard,
                          USBTransfer::Device,
                          USBDescriptor::DeviceType << 8, 0,
                          desc, size);
}

Error USBDevice::getConfigDescriptor(USBDescriptor::Configuration *desc, Size size)
{
    DEBUG("");

    return controlMessage(USBTransfer::GetDescriptor,
                          USBTransfer::In,
                          USBTransfer::Standard,
                          USBTransfer::Device,
                          USBDescriptor::ConfigurationType << 8, 0,
                          desc, size);
}

Error USBDevice::getInterfaceDescriptor(USBDescriptor::Interface *desc)
{
    DEBUG("");

    return controlMessage(USBTransfer::GetDescriptor,
                          USBTransfer::In,
                          USBTransfer::Standard,
                          USBTransfer::Device,
                          USBDescriptor::InterfaceType << 8, 0,
                          desc, sizeof(*desc));
}

Error USBDevice::getEndpointDescriptor(u8 endpointId, USBDescriptor::Endpoint *desc)
{
    DEBUG("endpointId = " << endpointId);

    return controlMessage(USBTransfer::GetDescriptor,
                          USBTransfer::In,
                          USBTransfer::Standard,
                          USBTransfer::Device,
                          USBDescriptor::EndpointType << 8, endpointId,
                          desc, sizeof(*desc));
}

Error USBDevice::setAddress(u8 address)
{
    DEBUG("address =" << address);

    // Send the request
    Error r = controlMessage(USBTransfer::SetAddress,
                             USBTransfer::Out,
                             USBTransfer::Standard,
                             USBTransfer::Device,
                             address, 0, 0, 0);

    // Set member, which is word aligned.
    if (r == ESUCCESS)
        m_id = address;

    return r;
}

Error USBDevice::setConfiguration(u8 configId)
{
    DEBUG("configId =" << configId);
    return controlMessage(USBTransfer::SetConfiguration,
                          USBTransfer::Out,
                          USBTransfer::Standard,
                          USBTransfer::Device,
                          configId, 0, 0, 0);
}

Error USBDevice::controlMessage(u8 request,
                                const USBTransfer::Direction direction,
                                const USBTransfer::RequestType type,
                                const USBTransfer::Recipient recipient,
                                u16 value,
                                u16 index,
                                void *buffer,
                                Size size)
{
    DEBUG("");

    USBMessage msg;
    msg.direction     = direction;
    msg.speed         = m_speed;
    msg.type          = USBTransfer::Control;
    msg.state         = USBMessage::Setup;
    msg.maxPacketSize = !m_device->maxPacketSize ? 8 : m_device->maxPacketSize;
    msg.hubAddress    = 1;
    msg.portAddress   = 1;

    msg.setup.requestType = (direction << 7) | (type << 5) | recipient;
    msg.setup.request     = request;
    msg.setup.value       = value;
    msg.setup.index       = index;
    msg.setup.length      = size;

    msg.deviceId      = m_id;
    msg.endpointId    = 0;
    msg.size          = size;

    // Use the physical address of the buffer
    if (buffer)
    {
        Memory::Range range;
        range.virt = (Address) buffer;
        VMCtl(SELF, LookupVirtual, &range);
        msg.buffer = range.phys;
    }
    else
        msg.buffer = 0;

    // Perform control message transfer
    return submit(msg);
}

Error USBDevice::transfer(const USBTransfer::Type type,
                          const USBTransfer::Direction direction,
                          Address endpointId,
                          void *buffer,
                          Size size,
                          Size maxPacketSize)
{
    DEBUG("");

    USBMessage msg;
    msg.direction     = direction;
    msg.speed         = m_speed;
    msg.type          = type;
    msg.state         = USBMessage::Status;

    if (maxPacketSize)
        msg.maxPacketSize = maxPacketSize;
    else
        msg.maxPacketSize = !m_device->maxPacketSize ? 8 : m_device->maxPacketSize;

    msg.deviceId      = m_id;
    msg.endpointId    = endpointId & 0xf;
    msg.size          = size;

    // Use the physical address of the buffer
    Memory::Range range;
    range.virt = (Address) buffer;
    VMCtl(SELF, LookupVirtual, &range);
    msg.buffer = range.phys;

    // Perform control message transfer
    return submit(msg);
}

Error USBDevice::beginTransfer(
    const USBTransfer::Type type,
    const USBTransfer::Direction direction,
    Address endpointId,
    void *buffer,
    Size size,
    Size maxPacketSize,
    CallbackFunction *callback)
{
    DEBUG("");

    USBMessage *msg = new USBMessage;
    msg->direction     = direction;
    msg->speed         = m_speed;
    msg->type          = type;
    msg->state         = USBMessage::Status;

    if (maxPacketSize)
        msg->maxPacketSize = maxPacketSize;
    else
        msg->maxPacketSize = !m_device->maxPacketSize ? 8 : m_device->maxPacketSize;

    msg->deviceId      = m_id;
    msg->endpointId    = endpointId & 0xf;
    msg->size          = size;
    msg->packetId      = m_endpointsPacketId.at(msg->endpointId);

    // Use the physical address of the buffer
    Memory::Range range;
    range.virt = (Address) buffer;
    VMCtl(SELF, LookupVirtual, &range);
    msg->buffer = range.phys;

    FileSystemMessage fs;
    FileDescriptor *files = getFiles();

    // Write the file
    if (m_transferFile > 0 && m_transferFile < FILE_DESCRIPTOR_MAX && files[m_transferFile].open)
    {
        // Prepare the message. We use the offset field to remember virtual input buffer address
        FileDescriptor *fd = &files[m_transferFile];
        fs.type   = ChannelMessage::Request;
        fs.action = WriteFile;
        fs.path   = fd->path;
        fs.buffer = (char *) msg;
        fs.size   = sizeof(*msg);
        fs.offset = (Size) buffer;
        fs.from   = SELF;
        fs.deviceID.minor = fd->identifier;
        if (ChannelClient::instance->sendRequest(fd->mount, &fs, callback) == ChannelClient::Success)
            return ESUCCESS;
    }
    return EIO;
}

Error USBDevice::finishTransfer(FileSystemMessage *msg)
{
    DEBUG("");

    // Update the packet id for non-control transfers
    USBMessage *usb = (USBMessage *) msg->buffer;
    if (usb->type != USBTransfer::Control)
        m_endpointsPacketId.insert(usb->endpointId, usb->packetId);

    // Release buffer
    delete msg->buffer;
    return ESUCCESS;
}

Error USBDevice::submit(USBMessage & msg)
{
    DEBUG("");

    // Make an USB transfer by writing the USB transfer file
    if (::write(m_transferFile, &msg, sizeof(msg)) != sizeof(msg))
    {
        ERROR("failed to write USB transfer file: " << strerror(errno));
        return EIO;
    }
    // Note that the USB controller will also implicitely write the
    // result of the transfer to the USBMessage buffer.
    DEBUG("transfer completed. USBMessage.state =" << (int)msg.state);
    return ESUCCESS;
}
