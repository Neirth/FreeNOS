#
# Copyright (C) 2015 Niek Linnenbank
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

import os.path
import config
from SCons.Script import *
from autoconf import *

""" A list of files which are installed in the final RootFS """
rootfs_files = []
Export('rootfs_files')

def UseLibraries(env, libs = [], arch = None):
    """
    Prepares a given environment, by adding library dependencies.
    """
    # By default exclude host. If arch filter is set, apply it.
    if (not arch and env['ARCH'] == 'host') or (arch and env['ARCH'] != arch):
        return

    # Loop all required libraries.
    for lib in libs:

        # Add them to the include and linker path.
        env.Append(CPPPATH = [ '#lib/' + lib ])
        env.Append(LIBPATH = [ '#' + env['BUILDROOT'] + '/lib/' + lib ])
        env.Append(LIBS    = [ lib ])

def UseServers(env, servers = []):
    """
    Prepares a given environment by adding server dependencies
    """
    if '#server' not in env['CPPPATH']:
        env.Append(CPPPATH = [ '#server' ])

    for serv in servers:
        env.Append(CPPPATH = [ '#server/' + serv ])

def HostProgram(env, target, source):
    if env['ARCH'] == 'host':
	env.Program(target, source)

def TargetProgram(env, target, source, install_dir = None):
    if env['ARCH'] != 'host':
	env.Program(target, source)
        if install_dir is not False:
	    env.TargetInstall(target, install_dir)

def TargetHostProgram(env, target, source, install_dir = None):
    HostProgram(env, target, source)
    TargetProgram(env, target, source, install_dir)

def TargetLibrary(env, lib, source):
    if env['ARCH'] != 'host':
	env.Library(lib, source)

def CopyStrFunc(target, source, env):
    return "  " + env.subst_target_source("COPY $SOURCE => $TARGET", 0, target, source)

def DirStrFunc(target):
    return "  MKDIR " + target

def TargetInstall(env, source, target = None):
    if env['ARCH'] != 'host':
	SCons.Tool.install.install_action.strfunction = CopyStrFunc

	if target is None:
	    target = '${ROOTFS}/' + Dir('.').srcnode().path

	env.Install(target, source)
	rootfs_files.append(str(target) + os.sep + os.path.basename(source))

def SubDirectories():
    dir_list = []
    dir_src  = Dir('.').srcnode().abspath

    if dir_src:
	for f in os.listdir(dir_src):
	    if os.path.isdir(dir_src + os.sep + f):
		dir_list.append(f)

	SConscript( dirs = dir_list )

#
# Reduce maximum number of open files resource limit.
# Internally, SCons and python's subprocess module contain a loop which invokes close(2)
# for 0..N files where N is the maximum number of open files.
# The loop is called every time a new process is created (e.g. from a builder).
# This loop becomes a serious performance issue when the limit
# is set to a high value, e.g. 1 million on an Ubuntu 18.04 Jenkins slave.
# Therefore, we override the limit here to prevent the potential performance penalty.
#
import resource
resource.setrlimit(resource.RLIMIT_NOFILE, (512, 512))

# Also override the cached value in the subprocess module of scons (in versions < 3.x.x)
try:
    SCons.compat.subprocess.MAXFD = 512
except:
    pass

# Override cached value in python's subprocess module
try:
    import subprocess
    subprocess.MAXFD = 512
except:
    pass

Export('SubDirectories')

# Create target, host and kernel environments.
host = Environment(tools    = ["default", "phony", "test"],
		   toolpath = ["support/scons"])
host.AddMethod(HostProgram, "HostProgram")
host.AddMethod(TargetProgram, "TargetProgram")
host.AddMethod(TargetHostProgram, "TargetHostProgram")
host.AddMethod(TargetLibrary, "TargetLibrary")
host.AddMethod(UseLibraries, "UseLibraries")
host.AddMethod(UseServers, "UseServers")
host.AddMethod(TargetInstall, "TargetInstall")
host.Append(ROOTFS = '#${BUILDROOT}/rootfs')
host.Append(ROOTFS_FILES = [])
host.Append(bin     = '${ROOTFS}/bin',
	    etc     = '${ROOTFS}/etc',
	    server  = '${ROOTFS}/server',
            boot    = '${ROOTFS}/boot')
host.Append(QEMU    = 'qemu-system')
host.Append(QEMUCMD = '${QEMU} ${QEMUFLAGS}')
host.Append(QEMUFLAGS = '')

target = host.Clone(tools    = ["default", "bootimage", "iso", "binary", "linn", "phony", "test", "compress"],
                    toolpath = ["support/scons"])

# Configuration build variables may come from, in order of priority:
#   Command-line arguments, OS environment or .conf file
#
# For the OS environment: ensure the 'ENV' environment variable does not exist,
# to avoid internal SCons exception while running the autoconf tests.
if 'ENV' in os.environ:
   del os.environ['ENV']

args = os.environ.copy()
args.update(ARGUMENTS)

# Apply configuration
config.initialize(target, host, args)
config.write_header(target)
config.write_header(host)

# Enables verbose compilation command output.
if not target['VERBOSE']:
    target['CXXCOMSTR']    = "  CXX  $TARGET"
    target['CCCOMSTR']     = "  CC   $TARGET"
    target['ASCOMSTR']     = "  AS   $TARGET"
    target['ASPPCOMSTR']   = "  AS   $TARGET"
    target['ARCOMSTR']     = "  AR   $TARGET"
    target['RANLIBCOMSTR'] = "  LIB  $TARGET"
    target['LINKCOMSTR']   = "  LD   $TARGET"
    target['COPYSTR']      = "  COPY $SOURCE => $TARGET"

if not host['VERBOSE']:
    host['CXXCOMSTR']    = "  HOSTCXX  $TARGET"
    host['CCCOMSTR']     = "  HOSTCC   $TARGET"
    host['ASCOMSTR']     = "  HOSTAS   $TARGET"
    host['ASPPCOMSTR']   = "  HOSTAS   $TARGET"
    host['ARCOMSTR']     = "  HOSTAR   $TARGET"
    host['RANLIBCOMSTR'] = "  HOSTLIB  $TARGET"
    host['LINKCOMSTR']   = "  HOSTLD   $TARGET"
    host['COPYSTR']      = "  COPY $SOURCE => $TARGET"

# Verify the configured CFLAGS.
if not GetOption('clean'):
    CheckCCFlags(target)
    CheckCXXFlags(target)
    CheckCCFlags(host)
    CheckCXXFlags(host)
