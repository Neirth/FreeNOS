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
#include <stdio.h>
#include "AnalogPort.h"

AnalogPort::AnalogPort(I2C *i2c, uint port)
    : Device(CharacterDeviceFile)
{
    m_i2c  = i2c;
    m_port = port;
    m_identifier << "analog" << port;
}

Error AnalogPort::initialize()
{
    return ESUCCESS;
}

Error AnalogPort::read(IOBuffer & buffer, Size size, Size offset)
{
    u8 command[4];
    char tmp[16];
    int n;

    if (offset >= 1)
        return 0;

    DEBUG("port =" << m_port);

    // Send a AnalogRead command to the GrovePi
    // including the port number and Analog value (HIGH/LOW)
    command[0] = 3;
    command[1] = m_port;
    command[2] = 0;
    command[3] = 0;
    m_i2c->setAddress(0x4);
    m_i2c->write(command, sizeof(command));
    m_i2c->read(command, 4);

    uint value = command[1] * 256 + command[2];
    n = snprintf(tmp, sizeof(tmp), "%10d", value);
    buffer.write(tmp, n);
    return n;
}
