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
#include <KernelLog.h>
#include <DeviceServer.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef ARM
#include "PL011.h"
#include "NS16550.h"
#else
#include "i8250.h"
#endif

struct SerialAddress
{
    u16 port;
    u16 irq;
}
uarts[] =
{
#ifdef ARM
    { 0x0, UART0_IRQ }
#else
    { 0x3f8, 4 },
    { 0x2f8, 3 },
    { 0x3e8, 4 },
    { 0x2e8, 3 },
#endif
};

int main(int argc, char **argv)
{
    DeviceServer server("/dev/serial");

    // Open the logging facilities
    Log *log = new KernelLog();
    log->setMinimumLogLevel(Log::Notice);

#ifdef ARM
#ifdef BCM2835
    PL011 *dev = ZERO;
    dev = new PL011(uarts[0].irq);
#else
    NS16550 *dev = ZERO;
    dev = new NS16550(uarts[0].irq);
#endif /* BCM2835 */
#else
    // Assume first UART is available
    i8250 *dev = ZERO;
    dev = new i8250(uarts[0].port, uarts[0].irq);
#endif /* ARM */

    server.initialize();

    server.insertFileCache(new Directory, "/serial0");
    server.getRoot()->insert(DirectoryFile, "serial0");

    server.registerDevice(dev, "/serial0/io");
    server.registerInterrupt(dev, uarts[0].irq);

    // Perform log
    INFO("detected at PORT=" << uarts[0].port << " IRQ=" << uarts[0].irq);

    // Start serving requests
    return server.run();
}
