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

#include <stdlib.h>
#include <unistd.h>
#include <BroadcomSPI.h>
#include <KernelLog.h>
#include <DeviceServer.h>
#include "PN532.h"

int main(int argc, char **argv)
{
    DeviceServer server("/dev/nfc");
    server.initialize();

    // Setup logging
    Log *log = new KernelLog();
    log->setMinimumLogLevel(Log::Notice);

    // Initialize SPI controller
    BroadcomSPI *spi = new BroadcomSPI();
    switch (spi->initialize())
    {
        case BroadcomSPI::Success:
            break;
        case BroadcomSPI::NotFound:
            NOTICE("No SPI controller found");
            return EXIT_SUCCESS;

        default:
            ERROR("failed to initialize SPI controller");
            return EXIT_FAILURE;
    }
    // Setup PN538
    server.registerDevice(new PN532(spi), "pn538_spi");

    // Serve requests
    return server.run();
}
