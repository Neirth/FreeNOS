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

#include <Log.h>
#include "BroadcomPower.h"

BroadcomPower::BroadcomPower()
    : m_mask(UART0)
{
}

BroadcomPower::Result BroadcomPower::initialize()
{
    // Initialize mailbox
    if (m_mailbox.initialize() != BroadcomMailbox::Success)
    {
        ERROR("failed to initialize mailbox");
        return IOError;
    }
    // Initialize register stats
    m_mailbox.write(BroadcomMailbox::PowerManagement, m_mask);
    return Success;
}

BroadcomPower::Result BroadcomPower::enable(BroadcomPower::Device device)
{
    m_mask |= device;
    m_mailbox.write(BroadcomMailbox::PowerManagement, m_mask);
    m_mailbox.read(BroadcomMailbox::PowerManagement, &m_mask);
    return Success;
}
