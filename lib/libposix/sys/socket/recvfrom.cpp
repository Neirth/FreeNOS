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

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <NetworkClient.h>
#include <sys/socket.h>
#include <errno.h>

extern C int recvfrom(int sockfd, void *buf, size_t len, int flags,
                      struct sockaddr *addr, socklen_t addrlen)
{
    char packet[2048];
    NetworkClient::SocketInfo info;

    if (len > sizeof(packet) - addrlen)
        return ERANGE;

    Error r = ::read(sockfd, packet, sizeof(packet));
    if (r < 0)
        return r;

    memcpy(&info, packet, sizeof(info));
    addr->addr = info.address;
    addr->port = info.port;

    memcpy(buf, packet + sizeof(info), r - sizeof(info));
    return r - sizeof(info);
}
