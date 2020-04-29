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

import os
import tempfile
import shutil

#
# Compress a file using GZIP
#
def gzip_func(target, source, env):
    gzip_cmd = 'gzip -c "' + str(source[0]) + '" > "' + str(target[0]) + '"'
    r = os.system(gzip_cmd)
    if r != 0:
        raise Exception("failed to compress `" + str(target[0]) + "': command `" + gzip_cmd + "' failed: exit code " + str(r >> 8))

#
# String command representation for GZIP builder.
#
def gzip_str(target, source, env):

    return "  GZIP  " + str(target[0])

#
# Add ourselves to the given environment.
#
def generate(env):

    builder = env.Builder(action  = env.Action(gzip_func, gzip_str))
    env.Append(BUILDERS = { 'Gzip' : builder })

#
# We always exist.
#
def exists(env):
    return Env.Detect('gzip')
