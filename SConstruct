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

import sys

sys.path.insert(1, './support/scons')

from build import *

#
# Target build
#
build_env = target
Export('build_env')

# Build programs, libraries and servers.
VariantDir(target['BUILDROOT'] + '/lib', '#lib', duplicate = 0)
VariantDir(target['BUILDROOT'] + '/bin', '#bin', duplicate = 0)
VariantDir(target['BUILDROOT'] + '/server', '#server', duplicate = 0)
VariantDir(target['BUILDROOT'] + '/test', '#test', duplicate = 0)
VariantDir(target['BUILDROOT'] + '/kernel', '#kernel', duplicate = 0)

# Install files to the target RootFS
target.TargetInstall('VERSION', target['etc'])
target.TargetInstall('build.conf', target['etc'])
target.TargetInstall('build.host.conf', target['etc'])
target.TargetInstall(target['BUILDROOT'] + '/include/Config.h', target['etc'])
target.TargetInstall('config/' + target['ARCH'] + '/' + target['SYSTEM'] + '/init.sh', target['etc'])

SConscript(target['BUILDROOT'] + '/lib/SConscript')
SConscript(target['BUILDROOT'] + '/bin/SConscript')
SConscript(target['BUILDROOT'] + '/server/SConscript')
SConscript(target['BUILDROOT'] + '/test/SConscript')
SConscript(target['BUILDROOT'] + '/kernel/' + target['ARCH'] + '/' + target['SYSTEM'] + '/SConscript')

#
# Host build
#
build_env = host
Export('build_env')

# Build programs and libraries.
VariantDir(host['BUILDROOT'] + '/lib', '#lib', duplicate = 0)
VariantDir(host['BUILDROOT'] + '/bin', '#bin', duplicate = 0)
VariantDir(host['BUILDROOT'] + '/server', '#server', duplicate = 0)
VariantDir(host['BUILDROOT'] + '/test', '#test', duplicate = 0)
SConscript(host['BUILDROOT'] + '/lib/SConscript')
SConscript(host['BUILDROOT'] + '/bin/SConscript')
SConscript(host['BUILDROOT'] + '/server/SConscript')
SConscript(host['BUILDROOT'] + '/test/SConscript')

#
# Boot Image
#
target.BootImage('#${BUILDROOT}/boot.img', '#config/' + target['ARCH'] + '/' + target['SYSTEM'] + '/boot.imgdesc')
target.Gzip('#${BUILDROOT}/boot.img.gz', '#${BUILDROOT}/boot.img')

#
# RootFS
#
Import('rootfs_files')
target.LinnImage('#${BUILDROOT}/rootfs.linn', rootfs_files)
target.Depends('#${BUILDROOT}/rootfs.linn', '#build/host')

#
# Source Release
#
target.Targets(release = 'git archive --format tar --prefix "FreeNOS-${VERSION}/" HEAD | gzip > "FreeNOS-${VERSION}.tar.gz"; \
                          git log --decorate=short > ChangeLog-${VERSION}; \
                          md5sum "FreeNOS-${VERSION}.tar.gz" > "FreeNOS-${VERSION}.tar.gz.md5"; \
                          sha1sum "FreeNOS-${VERSION}.tar.gz" > "FreeNOS-${VERSION}.tar.gz.sha1"')
