/*
 * Copyright (C) 2019 Niek Linnenbank
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

#ifndef __SERVER_IPCTEST_IPCTESTSERVER_H
#define __SERVER_IPCTEST_IPCTESTSERVER_H

#include <FreeNOS/System.h>
#include <ChannelServer.h>
#include "IPCTestMessage.h"

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup server_ipctest
 * @{
 */

/**
 * Test server for inter process communication (IPC).
 */
class IPCTestServer : public ChannelServer<IPCTestServer, IPCTestMessage>
{
  public:

    /**
     * Constructor function.
     */
    IPCTestServer();

    /**
     * Destructor function.
     */
    virtual ~IPCTestServer();

  private:

    /**
     * Handler for ActionA messages.
     *
     * @param msg Input message
     */
    void testActionAHandler(IPCTestMessage *msg);

    /**
     * Handler for ActionB messages.
     *
     * @param msg Input message
     */
    void testActionBHandler(IPCTestMessage *msg);

};

/**
 * @}
 * @}
 */

#endif /* __SERVER_IPCTEST_IPCTESTSERVER_H */
