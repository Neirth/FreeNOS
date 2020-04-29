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

#include "IPCTestServer.h"

IPCTestServer::IPCTestServer()
    : ChannelServer<IPCTestServer, IPCTestMessage>(this)
{
    NOTICE("");

    // Register message handlers
    addIPCHandler(TestActionA, &IPCTestServer::testActionAHandler);
    addIPCHandler(TestActionB, &IPCTestServer::testActionBHandler);
}

IPCTestServer::~IPCTestServer()
{
}

void IPCTestServer::testActionAHandler(IPCTestMessage *msg)
{
    NOTICE("data: " << (void *) msg->data);
    msg->data = 0xaaaaaaaa;
}

void IPCTestServer::testActionBHandler(IPCTestMessage *msg)
{
    NOTICE("data: " << (void *) msg->data);
    msg->data = 0xbbbbbbbb;
}
