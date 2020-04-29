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
#include <ARP.h>
#include <Ethernet.h>
#include <MemoryBlock.h>
#include "NetReceive.h"

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

NetReceive::NetReceive(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("receive network packets");
    parser().registerPositional("DEVICE", "device name of network adapter");
    parser().registerPositional("ARGS", "optional key=value arguments", 0);
    parser().registerFlag('a', "arp", "receive ARP packet(s)");
}

NetReceive::~NetReceive()
{
}

NetReceive::Result NetReceive::exec()
{
    DEBUG("");
    return receiveArp();
}

NetReceive::Result NetReceive::receiveArp()
{
    u8 packet[1500];// sizeof(Ethernet::Header) +
                    // sizeof(ARP::Header) ];

    while (true)
    {
        if (receivePacket(packet, sizeof(packet)) != Success)
            continue;

        Ethernet::Header *ether = (Ethernet::Header *) packet;
        ARP::Header *arp = (ARP::Header *) (ether+1);

        if (be16_to_cpu(ether->type) == Ethernet::ARP)
        {
            printf("ARP: hw=%u proto=%u hwlen=%u protolen=%u op=%u\n",
                    be16_to_cpu(arp->hardwareType),
                    be16_to_cpu(arp->protocolType),
                    arp->hardwareLength,
                    arp->protocolLength,
                    be16_to_cpu(arp->operation));
            printf("ARP: mac src=%x", arp->etherSender.addr[0]);

            for (int i = 1; i < 6; i++)
                printf(":%x", arp->etherSender.addr[i]);

            printf(" mac dest=%x", arp->etherTarget.addr[0]);
            for (int i = 1; i < 6; i++)
                printf(":%x", arp->etherTarget.addr[i]);
            
            printf("\n");

            printf("ARP: ip src=%x ip dst=%x\n", be32_to_cpu(arp->ipSender), be32_to_cpu(arp->ipTarget));
            return Success;
        }
        else
            printf("skipped (not ARP, type %x)\n", be16_to_cpu(ether->type));
    }
    return Success;
}

NetReceive::Result NetReceive::receivePacket(u8 *packet, Size size)
{

    DEBUG("");

    // Receive on physical device
    const char *device = *arguments().getPositionals()[0]->getValue();
    int fd = open(device, O_RDWR);
    int result;

    if (fd < 0)
    {
        printf("failed to open device '%s': %s\n", device, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("receiving on device: %s\n", device);

    if ((result = read(fd, packet, size)) <= 0)
    {
        printf("failed to receive packet on device '%s': %s\n",
                device, strerror(errno));
        return IOError;
    }
    printf("received %d bytes\n", result);

    for (int i = 0; i < result; i++)
        printf("%x ", packet[i]);
    printf("\n");

    // Done
    close(fd);
    return Success;
}
