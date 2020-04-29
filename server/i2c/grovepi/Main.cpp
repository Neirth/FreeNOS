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

#include <DeviceServer.h>
#include <stdlib.h>
#include <unistd.h>
#include <BroadcomI2C.h>
#include <KernelLog.h>
#include "DigitalPort.h"
#include "AnalogPort.h"
#include "LCDBar.h"

int main(int argc, char **argv)
{
    char tmp[64];
    DeviceServer server("/dev/grove");
    server.initialize();

    // Setup logging
    Log *log = new KernelLog();
    log->setMinimumLogLevel(Log::Notice);

    // Initialize I2C controller
    BroadcomI2C *i2c = new BroadcomI2C();
    switch (i2c->initialize())
    {
        case BroadcomI2C::Success:
            break;

        case BroadcomI2C::NotFound:
            NOTICE("No I2C controller found");
            return EXIT_SUCCESS;

        default:
            ERROR("failed to initialize I2C controller");
            return EXIT_FAILURE;
    }

    // Set Digipin 3 to input mode
    u8 command[4];
    command[0] = 5;
    command[1] = 3;
    command[2] = 0;
    command[3] = 0;
    i2c->setAddress(0x4);
    i2c->write(command, sizeof(command));
    sleep(1);

    // Set Analog 0 to input mode
    command[0] = 5;
    command[1] = 0;
    command[2] = 0;
    command[3] = 0;
    i2c->setAddress(0x4);
    i2c->write(command, sizeof(command));

    // Add devices
    for (int i = 0; i < 8; i++)
        server.registerDevice(new DigitalPort(i2c, i+1), "digital%d", i);

    for (int j = 0; j < 3; j++)
        server.registerDevice(new AnalogPort(i2c, j), "analog%d", j);

    server.registerDevice(new LCDBar(i2c), "lcd");

    // Start serving requests
    return server.run();
}
