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

#ifndef __SERVER_IPCTEST_IPCTESTMESSAGE_H
#define __SERVER_IPCTEST_IPCTESTMESSAGE_H

#include <ChannelMessage.h>
#include <Types.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup server_ipctest
 * @{
 */

/**
 * Defines possible actions performed for IPCTestMessage.
 */
typedef enum IPCTestAction
{
    TestActionA = 0,
    TestActionB,
    TestActionC
}
IPCTestAction;

/**
 * Test IPC message.
 *
 * Mandatory fields for each message are 'from' and 'action'.
 * All other fields are custom.
 */
typedef struct IPCTestMessage : public ChannelMessage
{
    /**
     * Assignment operator. 
     *
     * @param m TestIPCMessage pointer to copy from.
     */
    void operator = (IPCTestMessage *m)
    {
        type        = m->type;
        identifier  = m->identifier;
        from        = m->from;
        action      = m->action;
        data        = m->data;
    }

    /**
     * @brief Comparison operator.
     *
     * @param m IPCTestMessage to compare with.
     *
     * @return True if equal, false otherwise.
     */
    bool operator == (IPCTestMessage *m)
    {
        return this->from   == m->from &&
               this->action == m->action;
    }

    /** Source of the message */
    ProcessID from;

    /** Action to perform. */
    IPCTestAction action;

    /** Data value. */
    u32 data;
}
IPCTestMessage;

/**
 * @}
 * @}
 */

#endif /* __SERVER_IPCTEST_IPCTESTMESSAGE_H */
