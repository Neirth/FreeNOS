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
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <MemoryBlock.h>
#include <NetworkClient.h>
#include <NetworkSocket.h>
#include <IPV4.h>
#include <ICMP.h>
#include "NetCat.h"

NetCat::NetCat(int argc, char **argv)
    : POSIXApplication(argc, argv)
    , m_client(0)
    , m_socket(0)
    , m_lineLen(0)
    , m_host(0)
    , m_port(0)
{
    parser().setDescription("network send/receive");
    parser().registerPositional("DEVICE", "device name of network adapter");
    parser().registerPositional("HOST", "host address");
    parser().registerPositional("PORT", "host port");
    parser().registerFlag('u', "udp", "use UDP for transport");
    parser().registerFlag('t', "tcp", "use TCP for transport");
    parser().registerFlag('l', "listen", "listen mode");
}

NetCat::~NetCat()
{
}

NetCat::Result NetCat::initialize()
{
    DEBUG("");

    // Create a network client
    m_client = new NetworkClient(arguments().get("DEVICE"));

    // Initialize networking client
    if (m_client->initialize() != NetworkClient::Success)
    {
        ERROR("failed to initialize network client for device: "
               << arguments().get("DEVICE"));
        return IOError;
    }
    // Create an UDP socket
    if (m_client->createSocket(NetworkClient::UDP, &m_socket) != NetworkClient::Success)
    {
        ERROR("failed to create UDP socket");
        return IOError;
    }
    // Convert to IPV4 address
    if (!(m_host = IPV4::toAddress(arguments().get("HOST"))))
    {
        ERROR("failed to convert to IPV4 address: " << arguments().get("HOST"));
        return IOError;
    }
    // Convert to port
    m_port = atoi(arguments().get("PORT"));

    // Bind to a local port.
    if (m_client->bindSocket(m_socket, 0, arguments().get("listen") ? m_port : 0))
    {
        ERROR("failed to bind socket");
        return IOError;
    }
    // Success
    return Success;
}

NetCat::Result NetCat::exec()
{
    DEBUG("");

    DEBUG("sending on device: " << arguments().get("DEVICE"));
    DEBUG("sending to host: " << arguments().get("HOST") <<
          " on port " << arguments().get("PORT"));

    if (arguments().get("listen"))
    {
        // Keep receiving from UDP
        while (1)
        {
            udpReceive();
            printLine();
        }
    }
    else
    {
        // Keep reading and sending UDP
        while (1)
        {
            readLine();
            udpSend();
        }
    }
    return Success;
}

NetCat::Result NetCat::printLine()
{
    printf("%s\n", m_lineBuf);
    m_lineBuf[0] = 0;
    m_lineLen = 0;
    return Success;
}

NetCat::Result NetCat::readLine()
{
    DEBUG("");

    // Reset
    m_lineLen = 0;
    bool reading = true;

    // Read a line
    while (m_lineLen < sizeof(m_lineBuf) - 3 && reading)
    {
        // Read a character
        read(0, &m_lineBuf[m_lineLen], 1);
        
        // Process character
        switch (m_lineBuf[m_lineLen])
        {
            case '\r':
            case '\n':
                printf("\r\n");
                reading = false;
                break;

            case '\b':
                if (m_lineLen > 0)
                {
                    m_lineLen--;
                    printf("\b \b");
                }
                break;
        
            default:
                printf("%c", m_lineBuf[m_lineLen]);
                m_lineLen++;
                break;
        }
    }

    // Done
    m_lineBuf[m_lineLen++] = '\r';
    m_lineBuf[m_lineLen++] = '\n';
    m_lineBuf[m_lineLen] = ZERO;

    // Single line dot means quit
    if (strcmp(m_lineBuf, ".\r\n") == 0)
        ::exit(0);

    return Success;
}

NetCat::Result NetCat::udpSend()
{
    DEBUG("line = " << m_lineBuf);

    // Send UDP datagram
    struct sockaddr addr;
    addr.addr = m_host;
    addr.port = m_port;

    Error r = ::sendto(m_socket, m_lineBuf, m_lineLen, 0,
                       &addr, sizeof(addr));
    if (r <= 0)
    {
        ERROR("failed to send UDP datagram: " << strerror(errno));
        return IOError;
    }
    return Success;
}

NetCat::Result NetCat::udpReceive()
{
    struct sockaddr addr;

    // Receive UDP datagram
    int r = recvfrom(m_socket, m_lineBuf, sizeof(m_lineBuf), 0,
                     &addr, sizeof(addr));
    if (r < 0)
    {
        ERROR("failed to receive UDP datagram: " << strerror(errno));
        return IOError;
    }
    DEBUG("got " << r << " bytes from: " << addr.addr << " at port: " << addr.port);
    m_lineBuf[r] = ZERO;
    return Success;
}
