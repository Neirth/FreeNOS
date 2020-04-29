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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <Runtime.h>
#include "MakeNode.h"

MakeNode::MakeNode(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Create new device file node");
    parser().registerPositional("FILE", "Name of the device file to create");
    parser().registerPositional("TYPE", "Type of file to create (c=char,b=block)");
    parser().registerPositional("MAJOR", "Major number for the device node");
    parser().registerPositional("MINOR", "Minor number for the device node");
}

MakeNode::~MakeNode()
{
}

MakeNode::Result MakeNode::exec()
{
    dev_t dev;

    // Refresh current filesystem mount points
    refreshMounts(0);

    // Fill in major/minor numbers
    dev.major = atoi(arguments().get("MAJOR"));
    dev.minor = atoi(arguments().get("MINOR"));

    // Attempt to create the file
    if (mknod(arguments().get("FILE"), S_IFCHR, dev) < 0)
    {
        ERROR("failed to create '" << arguments().get("FILE") << "': " << strerror(errno));
        return IOError;
    }
    return Success;
}
