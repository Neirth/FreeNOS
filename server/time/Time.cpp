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

#include <FreeNOS/System.h>
#include <Macros.h>
#include <Types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Time.h"

Time::Time()
    : Device(CharacterDeviceFile)
{
    m_identifier << "time0";
}

Error Time::initialize()
{
    return ESUCCESS;
}

Error Time::read(IOBuffer & buffer, Size size, Size offset)
{
    unsigned int year, month, day, hour, min, sec = 0, time;
    char tmp[16];
    int n;

    // PHONY read
    if(offset >= 10)
    {
        return 0;
    }

    // If UIP is clear, then we have >= 244 microseconds before
    // RTC registers will be updated.  Spec sheet says that this
    // is the reliable way to read RTC - registers. If UIP is set
    // then the register access might be invalid
    while((readCMOS(RTC_STATUS_A) & RTC_UIP));

    // Read the date/time values from the CMOS
    sec   = readCMOS(RTC_SECONDS);
    min   = readCMOS(RTC_MINUTES);
    hour  = readCMOS(RTC_HOURS);
    day   = readCMOS(RTC_DAY_OF_MONTH);
    month = readCMOS(RTC_MONTH);
    year  = readCMOS(RTC_YEAR);

    // Check if the time values are stored in binary or BCD format
    if( (readCMOS(RTC_STATUS_B) & RTC_BCD) )
    {
        // Convert from binary coded decimal (bcd) to machine numbers
        sec = bcd2bin(sec);
        min = bcd2bin(min);
        hour = bcd2bin(hour);
        day = bcd2bin(day);
        month = bcd2bin(month);
        year = bcd2bin(year);
    }

    // Assume that a two-digit year is after 2000
    if(year < 100)
    {
        year += CMOS_YEARS_OFFS;
    }

    // Format as an ASCII string
    time = mktime(year, month, day, hour, min, sec);
    n = snprintf(tmp, size < sizeof(tmp) ? size : sizeof(tmp), "%u", time);
    buffer.write(tmp, n);

    // Done
    return (Error) size;
}

unsigned char Time::readCMOS(unsigned char addr)
{
    WriteByte(RTC_PORT(0), addr);
    return ReadByte(RTC_PORT(1));
}

unsigned Time::bcd2bin(unsigned char val)
{
    return (val & 0x0f) + (val >> 4) * 10;
}
