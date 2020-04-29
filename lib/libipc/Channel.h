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

#ifndef __LIBIPC_CHANNEL_H
#define __LIBIPC_CHANNEL_H

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libipc
 * @{
 */

/**
 * Unidirectional point-to-point messaging channel.
 */
class Channel
{
  public:

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        InvalidArgument,
        InvalidMode,
        InvalidSize,
        IOError,
        ChannelFull,
        NotFound
    };

    /**
     * Channel modes.
     */
    enum Mode
    {
        Producer,
        Consumer
    };

  public:

    /**
     * Constructor.
     */
    Channel();

    /**
     * Destructor.
     */
    virtual ~Channel();

    /**
     * Get message size
     *
     * @return Message size
     */
    const Size getMessageSize() const;

    /**
     * Get maximum message count.
     *
     * @return Maximum message count.
     */
    const Size getMaximumMessages() const;

    /**
     * Set mode.
     *
     * @param mode Channel mode.
     *
     * @return Result code.
     */
    Result setMode(Mode mode);

    /**
     * Set message size.
     *
     * @param size New message size.
     *
     * @return Result code.
     */
    virtual Result setMessageSize(Size size);

    /**
     * Read a message.
     *
     * @param buffer Output buffer for the message.
     *
     * @return Result code.
     */
    virtual Result read(void *buffer) = 0;

    /**
     * Write a message.
     *
     * @param buffer Input buffer for the message.
     *
     * @return Result code.
     */
    virtual Result write(void *buffer) = 0;

    /**
     * Flush message buffers.
     *
     * Ensures that all messages are written through caches.
     *
     * @return Result code.
     */
    virtual Result flush() = 0;

  protected:

    /** Channel mode. */
    Mode m_mode;

    /** Message size. */
    Size m_messageSize;

    /** Maximum number of message that the Channel can hold. */
    Size m_maximumMessages;
};

/**
 * @}
 * @}
 */

#endif /* __LIBIPC_CHANNEL_H */
