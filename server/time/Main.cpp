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

#include <DeviceServer.h>
#include <stdlib.h>
#include <unistd.h>
#include "Time.h"

int main(int argc, char **argv)
{
    DeviceServer server("/dev/time");
    server.initialize();

    // Start serving requests
    Time* t = new Time();
    server.registerDevice(t, "rtc0");
    return server.run();
}
