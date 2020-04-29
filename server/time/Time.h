/*
 * Copyright (C) 2009 Coen Bijlsma
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

#ifndef __TIME_TIME_H
#define __TIME_TIME_H

#include <Macros.h>
#include <Types.h>
#include <Device.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup time
 * @{
 */

/** @brief The base I/O port of the CMOS. */
#define RTC_PORT(x) (0x70 + (x))

/** @brief Offset in the CMOS for the current number of seconds. */
#define RTC_SECONDS             0

/** @brief Offset in the CMOS for the current number of minutes. */
#define RTC_MINUTES             2

/** @brief Offset in the CMOS for the current number of hours. */
#define RTC_HOURS               4

/** @brief Offset in the CMOS for the current day of the week. */
#define RTC_DAY_OF_WEEK         6

/** @brief Offset in the CMOS for the current day of the month. */
#define RTC_DAY_OF_MONTH        7

/** @brief Offset in the CMOS for the current month. */
#define RTC_MONTH               8

/**
 * @brief Offset in the CMOS for the current year.
 *
 * A one digit value means before 2000, and a two-digit value,
 * i.e. >= 100, is after the year 2000.
 */
#define RTC_YEAR                9

/** @brief Assume that a two-digit year is after 2000 */
#define CMOS_YEARS_OFFS         2000

/** @brief Offset in CMOS for the status A register. */
#define RTC_STATUS_A        10

/** @brief Offset in CMOS for the status B register. */
#define RTC_STATUS_B            11

/** @brief Update in progress flag. */
#define RTC_UIP                 0x80

/** @brief Daylight savings flag.  */
#define RTC_DLS                 0x01

/** @brief 24 hour mode flag. */
#define RTC_24H                 0x02

/** @brief Time/date in binary/BCD flag. */
#define RTC_BCD                 0x04

/**
 * @brief System Time server.
 *
 * This server is responsible for maintaining the global system
 * time via the CMOS. A lot of this code comes from the linux kernel
 * source, particularly from the file arch/x86/kernel/rtc.c.
 *
 * @see http://git.kernel.org/?p=linux/kernel/git/stable/linux-2.6.30.y.git;a=blob_plain;f=arch/x86/kernel/rtc.c;hb=HEAD
 */

class Time : public Device
{
    public:

    /**
     * @brief Constructor function.
     */
    Time();

    /**
     * @brief Initializes the time class.
     *
     * @return Error status code.
     */
    virtual Error initialize();

    /**
     * @brief Read the system time.
     *
     * @param buffer Buffer to save the read bytes.
     * @param size Number of bytes to read.
     * @param offset Offset in the file to read.
     *
     * @return Number of bytes on success and ZERO on failure.
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

  private:

    /**
     * @brief Returns the value stored at the given address
     *        from the CMOS.
     *
     * @param addr The address to read from the CMOS
     * @return The value at the given address.
     *
     * @note I almost copied this code completely from the linux source
     *       from the file arch/x86/kernel/rtc.h so you should also
     *       take a look over there how they do it.
     */
    unsigned char readCMOS(unsigned char addr);

    /**
     * @brief Convert from binary coded decimal to binary form.
     *
     * @param val The value to convert.
     *
     * @return A binary integer.
     */
    unsigned bcd2bin(unsigned char val);
};

/**
 * @}
 * @}
 */

#endif /* __TIME_TIME_H */
