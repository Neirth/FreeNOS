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
#include <MemoryBlock.h>
#include <NetworkClient.h>
#include <NetworkSocket.h>
#include <IPV4.h>
#include <ICMP.h>
#include "NetPing.h"

//
// Send manual formatted network packets:
//
//   $ NetPing smsc --arp --dest=192.168.1.123
//
// Receive and dump network packets:
//
//   $ netrecv smsc
//
// Change device parameters:
//
//   $ devctl smsc ip_address=192.168.1.2 ether_address=00:11:22:33:44:55
//   $ devctl serial0 baudrate=9600
//
// Show device status and statistics:
//
//   $ devstat smsc
//   $ devstat serial0
//

NetPing::NetPing(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("send network pings");
    parser().registerPositional("DEVICE", "device name of network adapter");
    parser().registerPositional("HOST", "host address to ping");
    parser().registerFlag('a', "arp", "send ARP pings");
    parser().registerFlag('i', "icmp", "send ICMP pings");
}

NetPing::~NetPing()
{
}

NetPing::Result NetPing::exec()
{
    DEBUG("");

    const char *dev  = arguments().get("DEVICE");
    const char *host = arguments().get("HOST");
    const char *icmp = arguments().get("icmp");

    DEBUG("sending on device: " << dev);

    if (icmp)
    {
        DEBUG("sending ICMP packets");
        icmpPing(dev, host);
    }
    else
    {
        DEBUG("sending ARP packets");
        arpPing(dev, host);
    }
    return Success;
}

NetPing::Result NetPing::arpPing(const char *dev,
                                 const char *host)
{
    DEBUG("");

    NetworkClient client(dev);
    Ethernet::Address ethAddr;
    IPV4::Address ipAddr;
    int sock;

    // Initialize networking client
    if (client.initialize() != NetworkClient::Success)
    {
        ERROR("failed to initialize network client for device: " << dev);
        return IOError;
    }

    // Create an ARP socket
    if (client.createSocket(NetworkClient::ARP, &sock) != NetworkClient::Success)
    {
        ERROR("failed to create ARP socket");
        return IOError;
    }

    // Convert to IPV4 address
    if (!(ipAddr = IPV4::toAddress(host)))
    {
        ERROR("failed to convert to IPV4 address: " << host);
        return IOError;
    }
    printf("Sending ARP request to %s\n", host);

    // Send ARP request
    if (::write(sock, &ipAddr, sizeof(ipAddr)) < 0)
    {
        ERROR("failed to send ARP request: " << strerror(errno));
        return IOError;
    }
    // Receive ARP reply, if any
    if (::read(sock, &ethAddr, sizeof(ethAddr)) < 0)
    {
        ERROR("failed to receive ARP response: " << strerror(errno));
        return IOError;
    }
    // Print the MAC address received
    printf("Received reply for: ");

    for (Size i = 0; i < sizeof(Ethernet::Address); i++)
        printf("%x:", ethAddr.addr[i]);
    printf("\n");

    // Finished
    client.close(sock);
    return Success;
}

NetPing::Result NetPing::icmpPing(const char *dev, const char *host)
{
    DEBUG("");

    NetworkClient client(dev);
    int sock;

    // Initialize networking client
    if (client.initialize() != NetworkClient::Success)
    {
        ERROR("failed to initialize network client for device: " << dev);
        return IOError;
    }

    // Create an ICMP socket
    if (client.createSocket(NetworkClient::ICMP, &sock) != NetworkClient::Success)
    {
        ERROR("failed to create ARP socket");
        return IOError;
    }
    // Connect socket to the given host
    if (client.connectSocket(sock, IPV4::toAddress(host)) != NetworkClient::Success)
    {
        ERROR("failed to connect ICMP socket");
        return IOError;
    }
    // Send an echo request
    ICMP::Header msg;
    msg.type     = ICMP::EchoRequest;
    msg.code     = 0;
    msg.checksum = 0;
    msg.id       = 1;
    msg.sequence = 1;

    // Generate checksum
    msg.checksum = ICMP::checksum(&msg);

    // Send the packet
    if (::write(sock, &msg, sizeof(msg)) <= 0)
    {
        ERROR("failed to send ICMP request");
        return IOError;
    }
    // Receive echo reply
    if (::read(sock, &msg, sizeof(msg)) <= 0)
    {
        ERROR("failed to receive ICMP response");
        return IOError;
    }
    // Check message type
    if (msg.type != ICMP::EchoReply)
    {
        ERROR("invalid ICMP code in response: " << (int) msg.type);
        return IOError;
    }
    // Print the ICMP address received
    printf("Received ICMP response with id=%d sequence=%d\n",
            msg.id, msg.sequence);

    // Finished
    ::close(sock);
    return Success;
}
