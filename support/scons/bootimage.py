#
# Copyright (C) 2010 Niek Linnenbank
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
import build

#
# Modifies the dependency list by parsing boot image config.
#
def boot_image_emitter(target, source, env):

    # Open boot configuration.
    f = open(str(source[0]))

    # Loop all lines.
    for line in f.readlines():

	# Skip comments, empty lines and subtrees.
	if len(line.strip()) <= 0 or \
	       line.find('=') != -1 or \
	       line[0] == '{' or \
	       line[0] == '}' or \
	       line[0] == '#':
	    continue

        symboltype, symbolname = line.strip().split(' ')

	# Make us depend on the program/file.
	source.append('#' + env['BUILDROOT'] + '/' + symbolname)

    # We also depend on the mkimage utility.
    source.append('#' + build.host['BUILDROOT'] + '/bin/img/img')

    # All done.
    f.close()
    return target, source

#
# Generate a boot image.
#
def boot_image_func(target, source, env):

    # Invoke the mkimage utility to generate a boot image.
    d = os.getcwd()

    os.chdir(env['BUILDPATH'])

    img_cmd = "'" + build.host['BUILDROOT'] + "/bin/img/img' '" \
                  + "--prefix=" + env['BUILDROOT'] + "' '" \
                  + str(source[0]) + "' '" \
                  + str(target[0]) + "'"

    r = os.system(img_cmd)
    if r != 0:
        raise Exception("failed to generate boot image `" + str(target[0]) + "': command `" + img_cmd + "' failed: exit code " + str(r>>8))

    os.chdir(d)

def boot_image_str(target, source, env):

    return "  IMG  " + str(target[0])

#
# Add ourselves to the given environment.
#
def generate(env):

    builder =  env.Builder(action  = env.Action(boot_image_func, boot_image_str),
			   emitter = boot_image_emitter)
    env.Append(BUILDERS = { 'BootImage' : builder })

#
# We always exist.
#
def exists(env):
    return True
