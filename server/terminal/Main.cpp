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

#include <DeviceServer.h>
#include "Terminal.h"
#include <stdlib.h>
#include <unistd.h>
#include <Runtime.h>
#include <KernelLog.h>

int main(int argc, char **argv)
{
    KernelLog log;
    log.setMinimumLogLevel(Log::Notice);

    // Wait for the input/output devices to become available
    waitMount("/dev/ps2");
    waitMount("/dev/video");
    refreshMounts(0);

    // Register our device
    DeviceServer server("/console");
    server.initialize();

    // Start serving requests.
    server.registerDevice(new Terminal("/dev/ps2/keyboard0", "/dev/video/vga0"), "tty0");
    return server.run();
}
