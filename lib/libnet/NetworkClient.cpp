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

#include <fcntl.h>
#include <unistd.h>
#include <Log.h>
#include <String.h>
#include <Runtime.h>
#include "NetworkClient.h"
#include "ARP.h"
#include "ARPSocket.h"

NetworkClient::NetworkClient(const char *networkDevice)
{
    m_deviceName = networkDevice;
}

NetworkClient::~NetworkClient()
{
}

NetworkClient::Result NetworkClient::initialize()
{
    // Get a list of mounts
    refreshMounts(0);
    FileSystemMount *mounts = ::getMounts();
    FileSystemMount *match = 0;
    Size matchLen = 0;

    // Find closest matching device
    for (Size i = 0; i < FILESYSTEM_MAXMOUNTS; i++)
    {
        if (mounts[i].path[0] && strncmp(mounts[i].path, "/network/", 9) == 0)
        {
            Size len = 0;

            for (Size j = 0; mounts[i].path[j+9] && m_deviceName[j]; j++, len++)
            {
                if (mounts[i].path[j+9] != m_deviceName[j])
                {
                    len = 0;
                    break;
                }
            }
            if (len >= matchLen)
            {
                match = &mounts[i];
                matchLen = len;
            }
        }
    }
    if (!match)
    {
        ERROR("network device not found: " << *m_deviceName);
        return IOError;
    }
    m_deviceName = match->path;
    return Success;
}

NetworkClient::Result NetworkClient::createSocket(NetworkClient::SocketType type,
                                                  int *sock)
{
    String path = m_deviceName;

    switch (type)
    {
        case ARP:
            path << "/arp/socket";
            break;

        case ICMP:
            path << "/icmp/factory";
            break;

        case UDP:
            path << "/udp/factory";
            break;

        default:
            return NotFound;
    }
    if ((*sock = ::open(*path, O_RDWR)) != -1)
        return Success;
    else
        return IOError;
}

NetworkClient::Result NetworkClient::connectSocket(int sock, IPV4::Address addr, u16 port)
{
    DEBUG("");
    return writeSocketInfo(sock, addr, port, Connect);
}

NetworkClient::Result NetworkClient::bindSocket(int sock, IPV4::Address addr, u16 port)
{
    DEBUG("");
    return writeSocketInfo(sock, addr, port, Listen);
}

NetworkClient::Result NetworkClient::writeSocketInfo(
    int sock,
    IPV4::Address addr,
    u16 port,
    NetworkClient::SocketAction action)
{
    char buf[64];

    DEBUG("");

    // Read socket factory. The factory will create
    // a new socket for us. We need to read the new file path
    Error r = ::read(sock, buf, sizeof(buf));
    if (r < 0)
        return IOError;

    // Update the file descriptor path
    FileDescriptor *fd = &getFiles()[sock];
    MemoryBlock::copy(fd->path, buf, sizeof(buf));

    // Write address+port+action info to the socket
    SocketInfo info;
    info.address = addr;
    info.port    = port;
    info.action  = action;

    r = ::write(sock, &info, sizeof(info));
    if (r < 0)
        return IOError;

    // Done
    return Success;
}


NetworkClient::Result NetworkClient::close(int sock)
{
    ::close(sock);
    return Success;
}
