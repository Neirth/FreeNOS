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

#include <Log.h>
#include <unistd.h>
#include <string.h>
#include "USBHub.h"

USBHub::USBHub(u8 deviceId, const char *usbPath)
    : USBDevice(deviceId, usbPath)
{
    DEBUG("");

    m_hub = new USBDescriptor::Hub;
}

USBHub::~USBHub()
{
    delete m_hub;
}

Error USBHub::initialize()
{
    Error r;

    if ((r = USBDevice::initialize()) != ESUCCESS)
    {
        ERROR("failed to initialize USB device for Hub");
        return r;
    }

    DEBUG("get hub descriptor");
    r = controlMessage(GetDescriptor,
                       USBTransfer::In,
                       USBTransfer::Class,
                       USBTransfer::Device,
                       USBDescriptor::HubType << 8, 0,
                       m_hub, sizeof(*m_hub));
    if (r != ESUCCESS)
    {
        ERROR("failed to get Hub descriptor");
        return r;
    }

    DEBUG("found " << m_hub->numPorts << " ports");
    DEBUG("characteristics: " << m_hub->hubCharacteristics);
    DEBUG("power-on ports");

    // Power-on all ports
    for (Size i = 1; i < m_hub->numPorts + 1u; i++)
    {
        if (setPortFeature(i, PortPower) != ESUCCESS)
        {
            ERROR("failed to power-on port: " << i);
        }
    }
    // Assume a device is attached on the first port
    if (portAttach(1) != ESUCCESS)
    {
        ERROR("failed to attach port 1");
    }
    return ESUCCESS;
}

Error USBHub::setPortFeature(u8 port, USBHub::PortFeature feature)
{
    DEBUG("port =" << port << " feature=" << (int)feature);

    return controlMessage(SetFeature,
                          USBTransfer::Out,
                          USBTransfer::Class,
                          USBTransfer::Other,
                          feature, port, 0, 0);
}

Error USBHub::portAttach(u8 port)
{
    DEBUG("");

    DEBUG("enable port");
    setPortFeature(port, PortReset);

    sleep(3);

    // Get descriptor of the next device
    u8 myId = m_id;
    USBDescriptor::Device *desc = new USBDescriptor::Device;
    m_id = 0;
    if (getDeviceDescriptor(desc) != ESUCCESS)
    {
        ERROR("failed to get device descriptor at port 1");
    }
    else
    {
        NOTICE("device on port " << port << " is: vendorId=" << desc->vendorId <<
                            " productId=" << desc->productId <<
                             " class=" << desc->deviceClass <<
                             " subclass=" << desc->deviceSubClass <<
                             " manufacturer=" << desc->manufacturer << 
                             " product=" << desc->product <<
                             " descType=" << desc->descriptorType);
    }
    m_id = myId;

    // Hardcoded assumption: SMSC95xx is on port 1 (or 3?) (raspberry pi)
    if (desc->vendorId == 1060 && desc->productId == 60416)
    {
        const char * argv[] = { "/server/network/smsc95xx/server", 0 };

        if (forkexec(argv[0], argv) == -1)
        {
            ERROR("failed to start USB device driver: " << strerror(errno));
            return EIO;
        }
        DEBUG("USB device driver started for port =" << port);
    }
    else
    {
        ERROR("no SMSC95xx found on port 1");
    }
    return ESUCCESS;
}
