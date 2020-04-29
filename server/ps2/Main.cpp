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

#include <FileType.h>
#include <DeviceServer.h>
#include "Keyboard.h"
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    DeviceServer server("/dev/ps2");
    server.initialize();

    // Create a new keyboard object
    Keyboard *kb = new Keyboard;

    // Register it with the DeviceServer
    server.registerDevice(kb, "keyboard0");
    server.registerInterrupt(kb, PS2_IRQ);

    // Start processing requests
    return server.run();
}
