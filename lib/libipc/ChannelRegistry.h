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

#ifndef __LIBIPC_CHANNELREGISTRY_H
#define __LIBIPC_CHANNELREGISTRY_H

#include <HashTable.h>
#include <Types.h>

class Channel;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libipc
 * @{
 */

/**
 * Registration for Channels.
 */
class ChannelRegistry
{
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
        NotFound
    };

  public:

    /**
     * Constructor.
     */
    ChannelRegistry();

    /**
     * Destructor
     */
    virtual ~ChannelRegistry();

    /**
     * Get one consumer.
     *
     * @param pid ProcessID of the channel to get
     *
     * @return Channel pointer if found or ZERO
     */
    Channel * getConsumer(ProcessID pid);

    /**
     * Get one producer.
     *
     * @param pid ProcessID of the channel to get
     *
     * @return Channel pointer if found or ZERO
     */
    Channel * getProducer(ProcessID pid);

    /**
     * Get all consumers
     *
     * @return HashTable with consumer channels
     */
    HashTable<ProcessID, Channel *> & getConsumers();

    /**
     * Get all producers
     *
     * @return HashTable with all producer channels
     */
    HashTable<ProcessID, Channel *> & getProducers();

    /**
     * Register consumer channel.
     *
     * @param pid ProcessID of the attached process
     * @param channel Channel object
     *
     * @return Result code
     */
    Result registerConsumer(ProcessID pid, Channel *channel);

    /**
     * Register producer channel.
     *
     * @param pid ProcessID of the attached process
     * @param channel Channel object
     *
     * @return Result code
     */
    Result registerProducer(ProcessID pid, Channel *channel);

    /**
     * Unregister consumer channel.
     *
     * @param pid ProcessID of the attached process
     *
     * @return Result code
     */
    Result unregisterConsumer(ProcessID pid);

    /**
     * Unregister producer channel.
     *
     * @param pid ProcessID of the attached process
     *
     * @return Result code
     */
    Result unregisterProducer(ProcessID pid);

  private:

    /** Contains registered consumer channels */
    HashTable<ProcessID, Channel *> m_consumer;

    /** Contains registered producer channels */
    HashTable<ProcessID, Channel *> m_producer;
};

/**
 * @}
 * @}
 */

#endif /* __LIBIPC_CHANNELREGISTRY_H */
