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

#ifndef __LIBNET_NETWORKSERVER_H
#define __LIBNET_NETWORKSERVER_H

#include <Types.h>
#include <DeviceServer.h>
#include "NetworkDevice.h"
#include "Ethernet.h"
#include "IPV4.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * Networking server.
 */
class NetworkServer : public DeviceServer
{
  public:

    /**
     * Constructor
     */
    NetworkServer(const char *path);

    /**
     * Destructor
     */
    virtual ~NetworkServer();

    /**
     * Initialize the NetworkServer.
     */
    virtual Error initialize();
};

/**
 * @}
 * @}
 */

#endif /* __LIBNET_NETWORKSERVER_H */
