#!/bin/bash
#
# Copyright (C) 2019 Niek Linnenbank
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Trace commands and stop on error
set -e
set -x

# Set wait time and retry time
WAITTIME=15
RETRIES=10

#
# Generic function which retries a particular command until it succeeds.
#
# Arguments:
#  $1 : command to run
#
function run_command_retry
{
    for i in $(seq 1 $RETRIES); do
        if $1; then
            return
        else
            echo "Command '$1' failed (retry #$i)"
            sleep $WAITTIME
        fi
    done

    echo "Command '$1' failed (tried $RETRIES times): terminating"
    exit 1
}
