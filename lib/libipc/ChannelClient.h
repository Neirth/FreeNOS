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

#ifndef __LIBIPC_CHANNELCLIENT_H
#define __LIBIPC_CHANNELCLIENT_H

#include <Singleton.h>
#include <Callback.h>
#include <Index.h>
#include "ChannelRegistry.h"
#include "Channel.h"
#include "ChannelMessage.h"
#include <FileSystemMessage.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libipc
 * @{
 */

/**
 * Client for using Channels.
 */
class ChannelClient : public Singleton<ChannelClient>
{
  private:

    /**
     * Holds an outgoing request
     */
    typedef struct Request
    {
        bool active;
        ProcessID pid;
        ChannelMessage *message;
        CallbackFunction *callback;

        const bool operator == (const struct Request & req) const
        {
            return req.message == message && req.callback == callback;
        }

        const bool operator != (const struct Request & req) const
        {
            return req.message != message || req.callback != callback;
        }
    }
    Request;

  public:

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        InvalidArgument,
        InvalidSize,
        IOError,
        OutOfMemory,
        NotFound
    };

  public:

    /**
     * Constructor.
     */
    ChannelClient();

    /**
     * Destructor
     */
    virtual ~ChannelClient();

    /**
     * Get channel registry.
     *
     * @return ChannelRegistry object pointer or ZERO if not set
     */
    ChannelRegistry * getRegistry();

    /**
     * Assign channel registry.
     *
     * @param registry ChannelRegistry object pointer
     *
     * @return Result code
     */
    Result setRegistry(ChannelRegistry *registry);

    /**
     * Initialize the ChannelClient.
     *
     * @return Result code
     */
    virtual Result initialize();

    /**
     * Connect to a process.
     *
     * This function creates a producer and consumer Channel
     * to the given process and registers it with the ChannelRegistry.
     *
     * @param pid ProcessID for the process to connect to.
     * @param msgSize Default message size to use.
     *
     * @return Result code
     */
    virtual Result connect(ProcessID pid, Size msgSize = sizeof(FileSystemMessage));

    /**
     * Try to receive message from any channel.
     *
     * @param buffer Message buffer for output
     * @param pid ProcessID for output
     *
     * @return Result code
     */
    virtual Result receiveAny(void *buffer, ProcessID *pid);

    /**
     * Send asynchronous request message
     *
     * The client assigns an internal request identifier
     * for the message and ensures that the callback will be
     * called when a response messages is received.
     *
     * @param pid ProcessID to send the message to
     * @param buffer Points to message to send
     * @param callback Called when response message is received
     *
     * @return Result code
     */
    virtual Result sendRequest(ProcessID pid,
                               void *buffer,
                               CallbackFunction *callback);

    /**
     * Process a response message
     *
     * @param pid ProcessID from which we receive the message
     * @param msg Message which is received
     *
     * @return Result code
     */
    virtual Result processResponse(ProcessID pid,
                                   ChannelMessage *msg);

    /**
     * Synchronous receive from one process.
     *
     * @param buffer Message buffer for output
     * @param pid ProcessID for the channel
     *
     * @return Result code
     */
    virtual Result syncReceiveFrom(void *buffer, ProcessID pid);

    /**
     * Synchronous send to one process.
     *
     * @param buffer Message buffer to send
     * @param pid ProcessID for the channel
     *
     * @return Result code
     */
    virtual Result syncSendTo(void *buffer, ProcessID pid);

    /**
     * Synchronous send and receive to/from one process.
     *
     * @param buffer Message buffer to send/receive
     * @param pid ProcessID for the channel
     *
     * @return Result code
     */
    virtual Result syncSendReceive(void *buffer, ProcessID pid);

  private:

    /**
     * Get consumer for a process.
     *
     * @param pid ProcessID of the process
     *
     * @return Channel object if found or ZERO otherwise.
     */
    Channel * findConsumer(ProcessID pid);

    /**
     * Get producer for a process.
     *
     * @param pid ProcessID of the process
     *
     * @return Channel object if found or ZERO otherwise.
     */
    Channel * findProducer(ProcessID pid);

  private:

    /** Contains registered channels */
    ChannelRegistry *m_registry;

    /** Contains ongoing requests */
    Index<Request> m_requests;
};

/**
 * @}
 * @}
 */

#endif /* __LIBIPC_CHANNELCLIENT_H */
