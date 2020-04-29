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

#ifndef __LIBIPC_CHANNELSERVER_H
#define __LIBIPC_CHANNELSERVER_H

#include <FreeNOS/System.h>
#include <FreeNOS/ProcessEvent.h>
#include <FreeNOS/ProcessShares.h>
#include <HashIterator.h>
#include <Timer.h>
#include "MemoryChannel.h"
#include "ChannelClient.h"
#include "ChannelRegistry.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libipc
 * @{
 */

/**
 * Message handler function (dummy) container.
 */
template <class Func> struct MessageHandler
{
    /**
     * Constructor function.
     *
     * @param f Function to execute.
     * @param r Send a reply?
     */
    MessageHandler(Func f, bool r) : exec(f), sendReply(r)
    {
    }

    /** Handler function. */
    Func exec;

    /** Whether to send a reply or not. */
    bool sendReply;
};

/**
 * Template class which serves incoming messages from Channels using MessageHandlers.
 *
 * @param MsgType Type of Message to serve.
 */
template <class Base, class MsgType> class ChannelServer
{
  protected:

    /** Member function pointer inside Base, to handle IPC messages. */
    typedef void (Base::*IPCHandlerFunction)(MsgType *);

    /** Member function pointer inside Base, to handle interrupts. */
    typedef void (Base::*IRQHandlerFunction)(Size);

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
    };

    /**
     * Constructor function.
     *
     * @param num Number of message handlers to support.
     */
    ChannelServer(Base *inst, Size num = 32)
        : m_sendReply(true), m_instance(inst)
    {
        m_self = ProcessCtl(SELF, GetPID, 0);

        m_client   = ChannelClient::instance;
        m_registry = m_client->getRegistry();

        m_ipcHandlers = new Vector<MessageHandler<IPCHandlerFunction> *>(num);
        m_ipcHandlers->fill(ZERO);
        m_irqHandlers = new Vector<MessageHandler<IRQHandlerFunction> *>(num);
        m_irqHandlers->fill(ZERO);

        // Setup kernel event channel
        SystemInformation info;
        ProcessShares::MemoryShare share;
        share.pid    = KERNEL_PID;
        share.coreId = info.coreId;
        share.tagId  = 0;

        if (VMShare(SELF, API::Read, &share) != API::Success)
        {
            ERROR("failed to get kernel event channel");
        }
        else
        {
            m_kernelEvent.setMode(Channel::Consumer);
            m_kernelEvent.setMessageSize(sizeof(ProcessEvent));
            m_kernelEvent.setVirtual(share.range.virt,
                                     share.range.virt + PAGESIZE);
        }
    }

    /**
     * Destructor function.
     */
    virtual ~ChannelServer()
    {
        delete m_client;
        delete m_registry;
        delete m_ipcHandlers;
        delete m_irqHandlers;
    }

    /**
     * Enters an infinite loop, serving incoming requests
     *
     * @return Never.
     */
    int run()
    {
        // Enter loop
        while (true)
        {
            // Reset
            m_sendReply = true;

            // Process kernel events
            readKernelEvents();

            // Process user messages
            readChannels();

            // Retry requests until all served (EAGAIN or return value)
            retryAllRequests();

            // Sleep with timeout or return in case the process is
            // woken up by an external (wakeup) interrupt.
            DEBUG("EnterSleep");
            Address expiry = 0;

            if (m_expiry.frequency)
                expiry = (Address) &m_expiry;

            Error r = ProcessCtl(SELF, EnterSleep, expiry, (Address) &m_time);
            DEBUG("EnterSleep returned: " << (int)r);

            // Check for sleep timeout
            if (m_expiry.frequency)
            {
                if (ProcessCtl(SELF, InfoTimer, (Address) &m_time) != API::Success)
                {
                    ERROR("failed to retrieve system timer");
                }
                else if (m_expiry.ticks < m_time.ticks)
                {
                    m_expiry.frequency = 0;
                    timeout();
                }
            }
        }

        // Satify compiler
        return 0;
    }

    /**
     * Register a new IPC message action handler.
     *
     * @param slot Action value to trigger h.
     * @param h Handler to execute.
     * @param r Does the handler need to send a reply (per default) ?
     */
    void addIPCHandler(Size slot, IPCHandlerFunction h, bool sendReply = true)
    {
        m_ipcHandlers->insert(slot, new MessageHandler<IPCHandlerFunction>(h, sendReply));
    }

    /**
     * Register a new IRQ message vector handler
     *
     * @param slot Vector value to trigger h.
     * @param h Handler to execute.
     */
    void addIRQHandler(Size slot, IRQHandlerFunction h)
    {
        m_irqHandlers->insert(slot, new MessageHandler<IRQHandlerFunction>(h, false));
    }

    /**
     * Called when sleep timeout is reached
     */
    virtual void timeout()
    {
        DEBUG("");
    }

    /**
     * Retry any pending requests
     *
     * @return True if retry is needed again, false if all requests processed
     */
    virtual bool retryRequests()
    {
        return false;
    }

    /**
     * Set a sleep timeout
     *
     * @param msec Milliseconds to sleep (approximately)
     */
    void setTimeout(uint msec)
    {
        DEBUG("msec = " << msec);

        if (ProcessCtl(SELF, InfoTimer, (Address) &m_time) != API::Success)
        {
            ERROR("failed to retrieve system timer info");
            return;
        }

        Size msecPerTick = 1000 / m_time.frequency;
        m_expiry.frequency = m_time.frequency;
        m_expiry.ticks     = m_time.ticks + ((msec / msecPerTick) + 1);
    }

  private:

    /**
     * Accept new channel connection.
     *
     * @param pid ProcessID
     * @param range Memory range of shared mapping
     *
     * @return Result code
     */
    Result accept(ProcessID pid, Memory::Range range)
    {
        // Create consumer
        if (!m_registry->getConsumer(pid))
        {
            MemoryChannel *consumer = new MemoryChannel;
            consumer->setMode(Channel::Consumer);
            consumer->setMessageSize(sizeof(MsgType));
            consumer->setVirtual(range.virt, range.virt + PAGESIZE);
            m_registry->registerConsumer(pid, consumer);
        }
        // Create producer
        if (!m_registry->getProducer(pid))
        {
            MemoryChannel *producer = new MemoryChannel;
            producer->setMode(Channel::Producer);
            producer->setMessageSize(sizeof(MsgType));
            producer->setVirtual(range.virt + (PAGESIZE*2),
                                 range.virt + (PAGESIZE*3));
            m_registry->registerProducer(pid, producer);
        }
        // Done
        return Success;
    }

    /**
     * Read and process kernel events.
     *
     * @return Result code.
     */
    Result readKernelEvents()
    {
        ProcessEvent event;

        // Try to read a message on the kernel event channel
        while (m_kernelEvent.read(&event) == Channel::Success)
        {
            DEBUG(m_self << ": got kernel event: " << (int) event.type);

            switch (event.type)
            {
                case ShareCreated:
                {
                    DEBUG(m_self << ": share created for PID: " << event.share.pid);
                    accept(event.share.pid, event.share.range);
                    break;
                }
                case InterruptEvent:
                {
                    DEBUG(m_self << ": interrupt: " << event.number);

                    if (m_irqHandlers->at(event.number))
                    {
                        (m_instance->*(m_irqHandlers->at(event.number))->exec) (event.number);
                    }
                    break;
                }
                case ProcessTerminated:
                {
                    DEBUG(m_self << ": process terminated: PID " << event.number);
                    m_registry->unregisterConsumer(event.number);
                    m_registry->unregisterProducer(event.number);

                    // cleanup the VMShare area now for that process
                    VMShare(event.number, API::Delete, ZERO);
                    break;
                }
                default:
                    DEBUG(m_self << ": ???\n");
                    break;
            }
        }
        return Success;
    }

    /**
     * Read each Channel for messages.
     *
     * @return Result code
     */
    Result readChannels()
    {
        MsgType msg;

        // Try to receive message on each consumer channel
        for (HashIterator<ProcessID, Channel *> i(m_registry->getConsumers()); i.hasCurrent(); i++)
        {
            Channel *ch = i.current();
            DEBUG(m_self << ": trying to receive from PID " << i.key());

            // Read all messages in the consumer channel
            while (ch->read(&msg) == Channel::Success)
            {
                DEBUG(m_self << ": received message");
                msg.from = i.key();

                // Is the message a response from earlier client request?
                if (msg.type == ChannelMessage::Response)
                {
                    if (m_client->processResponse(msg.from, &msg) != ChannelClient::Success)
                    {
                        ERROR(m_self << ": failed to process client response from PID " <<
                               msg.from << " with identifier " << msg.identifier);
                    }
                }
                // Message is a request to us
                else if (m_ipcHandlers->at(msg.action))
                {
                    m_sendReply = m_ipcHandlers->at(msg.action)->sendReply;
                    (m_instance->*(m_ipcHandlers->at(msg.action))->exec) (&msg);

                    // Send reply
                    if (m_sendReply)
                    {
                        Channel *ch = m_registry->getProducer(i.key());
                        if (!ch)
                        {
                            ERROR(m_self << ": no producer channel found for PID: " << i.key());
                        }
                        else if (ch->write(&msg) != Channel::Success)
                        {
                            ERROR(m_self << ": failed to send reply message to PID: " << i.key());
                        }
                        else
                            ProcessCtl(i.key(), Resume, 0);
                    }
                }
            }
        }
        return Success;
    }

    /**
     * Keep retrying requests until all served
     */
    void retryAllRequests()
    {
        while (m_instance->retryRequests())
            ;
    }

  protected:

    /** Contains registered channels */
    ChannelRegistry *m_registry;

    /** Client for sending replies */
    ChannelClient *m_client;

    /** Kernel event channel */
    MemoryChannel m_kernelEvent;

    /** Should we send a reply message? */
    bool m_sendReply;

    /** IPC handler functions. */
    Vector<MessageHandler<IPCHandlerFunction> *> *m_ipcHandlers;

    /** IRQ handler functions. */
    Vector<MessageHandler<IRQHandlerFunction> *> *m_irqHandlers;

    /** Server object instance. */
    Base *m_instance;

    /** ProcessID of ourselves */
    ProcessID m_self;

    /** System timer value */
    Timer::Info m_time;

  private:

    /** System timer expiration value */
    Timer::Info m_expiry;
};

/**
 * @}
 * @}
 */

#endif /* __LIBIPC_CHANNELSERVER_H */
