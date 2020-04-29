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

#include <NetworkClient.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>

extern C int sendto(int sockfd, const void *buf, size_t len, int flags,
                    const struct sockaddr *addr, socklen_t addrlen)
{
    char packet[2048];
    NetworkClient::SocketInfo info;

    if (len > sizeof(packet) - addrlen)
        return ERANGE;

    info.address = addr->addr;
    info.port = addr->port;

    memcpy(packet, &info, sizeof(info));
    memcpy(packet + sizeof(info), buf, len);

    return ::write(sockfd, packet, len + sizeof(info));
}
