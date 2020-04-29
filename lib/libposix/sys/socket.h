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

#ifndef __LIBPOSIX_SYS_SOCKET_H
#define __LIBPOSIX_SYS_SOCKET_H

#include <Macros.h>
#include "types.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * Communication domains
 */
enum
{
    AF_INET,
    AF_INET6,
    AF_UNIX
};

/**
 * Socket types
 */
enum
{
    SOCK_STREAM,
    SOCK_DGRAM
};

/**
 * Communication Protocols
 */
enum
{
    IPPROTO_TCP,
    IPPROTO_UDP,
    IPPROTO_ICMP
};

enum
{
    SHUT_RD,
    SHUT_WR,
    SHUT_RDWR
};

struct sockaddr
{
    u32 addr;
    u16 port;
};

typedef Size socklen_t;

/**
 * Create socket endpoint for communication
 *
 * @param domain Communication domain
 * @param
 */
extern C int socket(int domain, int type, int protocol);

extern C int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * Listen for connections on a socket
 *
 * @param sockfd pid Process ID of child to wait for.
 * @param backlog Number of pending connections which can be queued.
 * @return Zero on success and -1 on error.
 */
extern C int listen(int sockfd, int backlog);

extern C int accept(int sockfd, struct sockaddr *addr, socklen_t addrlen);

extern C int connect(int sockfd, struct sockaddr *addr, socklen_t addrlen);

extern C int recvfrom(int sockfd, void *buf, size_t len, int flags,
                      struct sockaddr *addr, socklen_t addrlen);

extern C int sendto(int sockfd, const void *buf, size_t len, int flags,
                    const struct sockaddr *addr, socklen_t addrlen);

extern C int shutdown(int sockfd, int how);

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_SYS_SOCKET_H */
