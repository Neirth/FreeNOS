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

#ifndef __LIBPOSIX_TIME_H
#define __LIBPOSIX_TIME_H

#include <Macros.h>
#include "sys/types.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * The <time.h> header shall declare the timespec structure.
 */
struct timespec
{
    /** Seconds. */
    time_t tv_sec;

    /** Nanoseconds. */
    long tv_nsec;
};

/**
 * Convert given time values to UNIX timestamp (seconds since epoch)
 *
 * @param year Year value
 * @param month Month value
 * @param day Day value
 * @param hour Hour value (0-23)
 * @param min Minute value (0-59)
 * @param sec Second value (0-59)
 *
 * @return Converted timestamp in seconds since epoch (UNIX time)
 *
 * @note This code is based on the linux source in kernel/time.c
 */
extern C unsigned long mktime(const unsigned int year, const unsigned int month,
                              const unsigned int day, const unsigned int hour,
                              const unsigned int min, const unsigned int sec);
/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_TIME_H */
