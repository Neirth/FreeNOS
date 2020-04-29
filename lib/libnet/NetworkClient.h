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

#ifndef __LIBNET_NETWORKCLIENT_H
#define __LIBNET_NETWORKCLIENT_H

#include <Types.h>
#include "IPV4.h"
#include "Ethernet.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * Networking Client implementation.
 *
 * Maintains also the network state of each TCP/UDP socket, creates the
 * packets and posts them to ethernet. Also receives packets and extracts socket info.
 *
 * Because of the client and multiplexer separation the networking stack
 * is very parallel. It can scale very well for multicore/manycore systems
 * as each client may run on a different (possibly dedicated using pinning) core
 * thus running multiple network stack instances on each core in parallel.
 */
class NetworkClient
{
  public:

    /**
     * Socket actions
     */
    enum SocketAction
    {
        Connect,
        Listen
    };

    /**
     * Socket information
     *
     * This struct is written to a socket
     * to put it in either connect or listen state.
     */
    typedef struct SocketInfo
    {
        IPV4::Address address;
        u16 port;
        SocketAction action;
    }
    SocketInfo;

    /**
     * Socket types
     */
    enum SocketType
    {
        ARP,
        ICMP,
        TCP,
        UDP
    };

    /**
     * Result codes
     */
    enum Result
    {
        Success,
        IOError,
        NotFound
    };

  public:

    /**
     * Constructor
     *
     * @param networkDevice Name of the network device to use
     */
    NetworkClient(const char *networkDevice);

    /**
     * Destructor
     */
    virtual ~NetworkClient();

    /**
     * Perform initialization.
     *
     * @return Result code
     */
    Result initialize();

    /**
     * Create new socket.
     *
     * @param type Type of the socket to create
     * @param socket Outputs a pointer to NetworkSocket
     *
     * @return Result code
     */
    Result createSocket(SocketType type, int *socket);

    /**
     * Connect socket to address/port.
     *
     * @param sock Socket index
     * @param addr Address of the host to connect to
     * @param port Port of the host to connect to (or ZERO to ignore)
     *
     * @return Result code
     */
    Result connectSocket(int sock, IPV4::Address addr, u16 port = 0);

    /**
     * Bind socket to address/port.
     *
     * @param sock Socket index
     * @param addr Address of the address to bind to
     * @param port Port to bind to
     *
     * @return Result code
     */
    Result bindSocket(int sock, IPV4::Address addr = 0, u16 port = 0);

    /**
     * Close the socket.
     *
     * @param socket Network socket to close
     *
     * @return Result code
     */
    Result close(int sock);

  private:

    /**
     * Set socket to new state.
     */
    Result writeSocketInfo(int sock, IPV4::Address addr,
                           u16 port, SocketAction action);

  private:

    /** Network device name */
    String m_deviceName;
};

/**
 * @}
 * @}
 */

#endif /* __LIBNET_NETWORKCLIENT_H */
