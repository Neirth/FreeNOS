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

#ifndef __LIBPOSIX_TIME_H
#define __LIBPOSIX_TIME_H

#include <Macros.h>
#include "types.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * Time value information.
 */
struct timeval
{
    /** Seconds */
    time_t tv_sec;

    /** Microseconds */
    uint   tv_usec;
};

/**
 * Time zone information.
 */
struct timezone
{
    /** minutes west of Greenwich */
    int tz_minuteswest;

    /** type of DST correction */
    int tz_dsttime;
};

/**
 * Get current time of day.
 *
 * @param tv Timeval struct object pointer.
 * @param tz Timezone struct object pointer.
 *
 * @return Zero on success and -1 on error.
 */
extern C int gettimeofday(struct timeval *tv, struct timezone *tz);

/**
 * Print time difference.
 *
 * @param t1 First timeval struct.
 * @param t2 Second timeval struct.
 *
 * @return Zero on success or -1 on failure.
 */
extern C int printtimediff(struct timeval *t1, struct timeval *t2);

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_TIME_H */
