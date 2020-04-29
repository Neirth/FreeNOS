/*
 * Copyright (C) 2009 Niek Linnenbank
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
#include <Timer.h>
#include "unistd.h"
#include "errno.h"

unsigned int sleep(unsigned int seconds)
{
    Timer::Info info;

    // Get current kernel timer ticks
    if (ProcessCtl(SELF, InfoTimer, (Address) &info) != API::Success)
    {
        errno = EAGAIN;
        return seconds;
    }

    // Set time to wait
    info.ticks += (info.frequency * seconds);

    // Wait until the timer expires
    if (ProcessCtl(SELF, WaitTimer, (Address) &info) != API::Success)
    {
        errno = EIO;
        return seconds;
    }

    // Done
    return 0;
}
