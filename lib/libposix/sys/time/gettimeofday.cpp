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
#include <sys/time.h>
#include <errno.h>

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    Timer::Info timer;

    // Get current system timer info
    ProcessCtl(SELF, InfoTimer, (Address) &timer);

    // Check for a valid frequency
    if (timer.frequency == 0)
    {
        errno = ERANGE;
        return -1;
    }

    // Fill the output variables
    tv->tv_sec  = timer.ticks / timer.frequency;
    tv->tv_usec = (1000000 / timer.frequency) * (timer.ticks % timer.frequency);
    return 0;
}
