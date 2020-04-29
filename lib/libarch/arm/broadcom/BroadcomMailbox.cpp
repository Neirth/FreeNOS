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

#include <FreeNOS/System.h>
#include "BroadcomMailbox.h"

BroadcomMailbox::BroadcomMailbox()
{
}

BroadcomMailbox::Result BroadcomMailbox::initialize()
{
    // Map Mailbox registers
    if (m_io.map(IO_BASE + Base, PAGESIZE,
                 Memory::User|Memory::Readable|Memory::Writable|Memory::Device) != IO::Success)
        return IOError;

    // Initialize registers
    m_io.write(Config, 0);
    return Success;
}

BroadcomMailbox::Result BroadcomMailbox::read(
    Channel channel,
    u32 *message) const
{
    // Busy wait until mailbox has data
    while (m_io.read(Status) & Empty);

    // Read message
    for (;;)
    {
        if (((*message = m_io.read(Read)) & ChannelMask) == channel)
            break;
    }
    *message &= ~(ChannelMask);
    return Success;
}

BroadcomMailbox::Result BroadcomMailbox::write(
    Channel channel,
    u32 message)
{
    // Busy wait until mailbox becomes free
    while (m_io.read(Status) & Full);

    // Write message
    m_io.write(Write, (message << 4) | channel);
    return Success;
}
