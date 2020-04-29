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

JENKINS_URL="http://192.168.50.10:8080"
JENKINS_USER="admin"
JENKINS_PASS="admin"
WAITTIME=30
RETRIES=20

# Trace execution and stop on errors
set -e
set -x

# Start a retry loop
for i in $(seq 0 $RETRIES) ; do

    # Just fetch the jenkins URL with the given build token (configured per job)
    if curl -XPOST --silent --show-error $JENKINS_URL/job/$1/buildWithParameters?token=$1-JobToken ; then
        echo "Job $1 started via master at $JENKINS_URL"
        exit 0
    fi

    # Wait before retrying
    sleep $WAITTIME
done

echo "Failed to trigger build job $1 (tried $RETRIES times)"
exit 1
