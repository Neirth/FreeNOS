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

#ifndef __HOST__
#include <FreeNOS/System.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ARP.h>
#include <Ethernet.h>
#include <MemoryBlock.h>
#include "NetSend.h"

//
// Send manual formatted network packets:
//
//   $ netsend smsc --arp --dest=192.168.1.123
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

NetSend::NetSend(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("send network packets");
    parser().registerPositional("DEVICE", "device name of network adapter");
    parser().registerPositional("ARGS", "optional key=value arguments", 0);
    parser().registerFlag('a', "arp", "send ARP packet(s)");
}

NetSend::~NetSend()
{
}

NetSend::Result NetSend::exec()
{
    DEBUG("");

    IPV4::Address ipAddr = (192 << 24) | (168 << 16) | (1 << 8) | (123);
    Ethernet::Address etherAddr;

    return arpRequest(ipAddr, &etherAddr);
}

NetSend::Result NetSend::arpRequest(IPV4::Address ipAddr,
                                    Ethernet::Address *ethAddr)
{
#ifndef __HOST__
    u8 packet[ sizeof(Ethernet::Header) +
               sizeof(ARP::Header) ];

    Ethernet::Header *ether = (Ethernet::Header *) packet;
    ARP::Header *arp = (ARP::Header *) (ether+1);

    DEBUG("");

    // Ethernet packet
    ether->source.addr[0] = 00;
    ether->source.addr[1] = 11;
    ether->source.addr[2] = 22;
    ether->source.addr[3] = 33;
    ether->source.addr[4] = 44;
    ether->source.addr[5] = 55;
    MemoryBlock::set(&ether->destination, 0xff, sizeof(Ethernet::Address));
    ether->type = cpu_to_be16(Ethernet::ARP);

    // ARP packet
    arp->hardwareType   = cpu_to_be16(ARP::Ethernet);
    arp->protocolType   = cpu_to_be16(ARP::IPV4);
    arp->hardwareLength = sizeof(Ethernet::Address);
    arp->protocolLength = sizeof(IPV4::Address);
    arp->operation      = cpu_to_be16(ARP::Request);

    MemoryBlock::copy(&arp->etherSender, &ether->source, sizeof(Ethernet::Address));
    arp->ipSender = 0xffffffff;
    MemoryBlock::copy(&arp->etherTarget, &ether->destination, sizeof(Ethernet::Address));
    arp->ipTarget = cpu_to_be32(ipAddr);;

    // Transmit on physical device
    const char *device = *arguments().getPositionals()[0]->getValue();
    int fd = open(device, O_RDWR);
    if (fd < 0)
    {
        printf("failed to open device '%s': %s\n", device, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("sending on device: %s\n", device);
    Error r = write(fd, packet, sizeof(packet));
    if (r < 0)
    {
        printf("failed to send packet on device '%s': %s\n",
                device, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("send %d bytes\n", r);
#endif
    // Done
    return Success;
}
