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

#
# This script installs a slave node with FreeBSD 12.0.
# To use it in Jenkins, simple schedule the associated FreeBSD 12.0 build job.
#
# See the README file for more details on Jenkins setup.
#

NAME="freebsd12"
JENKINS_PACKAGES="git openjdk11 bash"
COMPILER_PACKAGES="gcc gcc48 gcc6 gcc7 gcc8 gcc9 \
                   llvm60 llvm70 llvm80 llvm90 \
                   arm-none-eabi-gcc"
MISC_PACKAGES="scons cdrkit-genisoimage xorriso"
PACKAGES="$JENKINS_PACKAGES $COMPILER_PACKAGES $MISC_PACKAGES"
RELEASE="`uname -r|cut -f 1,2 -d -`"
CHROOT="freebsd32"
CHROOTDIR="/usr/chroot/$CHROOT"
CHROOTURL="http://ftp.nl.freebsd.org/pub/FreeBSD/releases/i386/i386/$RELEASE/base.txz"

# Include common functions
source common.sh

# Set system hostname
sed "s/bazinga.localdomain/$NAME/" /etc/defaults/rc.conf > /tmp/rc.conf
mv /tmp/rc.conf /etc/defaults/rc.conf
sed "s/bazinga.localdomain/$NAME.localdomain/" /etc/hosts > /etc/hosts.bak
sed "s/bazinga/$NAME/" /etc/hosts.bak > /etc/hosts
rm /etc/hosts.bak
hostname -s $NAME

# Update system to latest patches
run_command_retry "freebsd-update -F --not-running-from-cron fetch"
run_command_retry "freebsd-update -F --not-running-from-cron install"
/usr/sbin/pkg || true
run_command_retry "pkg update -f"
run_command_retry "pkg upgrade -y"

# Create chroot
if [ ! -d $CHROOTDIR ] ; then
    mkdir -p $CHROOTDIR
    cd $CHROOTDIR
    run_command_retry "fetch $CHROOTURL"
    tar xpf base.txz
    rm base.txz
    cd ~vagrant
fi

# Inherit users and home directories from host
cp -Rp /usr/home $CHROOTDIR/usr
cp /etc/passwd $CHROOTDIR/etc
cp /etc/master.passwd $CHROOTDIR/etc
cp /etc/group $CHROOTDIR/etc
chroot $CHROOTDIR /usr/sbin/pwd_mkdb -p /etc/master.passwd

# Extract generated source archive
rm -rf $CHROOTDIR/usr/home/vagrant/FreeNOS
tar xf ~vagrant/src.tar -C $CHROOTDIR/usr/home/vagrant
rm -f ~vagrant/src.tar
rm -f $CHROOTDIR/home
ln -s /usr/home $CHROOTDIR/home

# Move Qemu source to chroot (if found)
if [ -e ~vagrant/qemu-src.tar.gz ] ; then
    mv ~vagrant/qemu-src.tar.gz $CHROOTDIR/usr/home/vagrant/
fi

# Ensure DNS configuration is used
cp /etc/resolv.conf $CHROOTDIR/etc/

# Install static bash (needed for the chroot)
run_command_retry "pkg install -y bash-static"
cp /bin/bash $CHROOTDIR/bin/bash

# Prepare script to setup the chroot
cp common.sh $CHROOTDIR/usr/home/vagrant
cat > $CHROOTDIR/usr/home/vagrant/setup.sh << EOF
#!/bin/bash

# Include generic functions
source /usr/home/vagrant/common.sh

# Needed for some pkg commands
export ASSUME_ALWAYS_YES=yes

# Update chroot to latest patches
run_command_retry "freebsd-update -F --not-running-from-cron fetch"
run_command_retry "freebsd-update -F --not-running-from-cron install"
ASSUME_ALWAYS_YES=yes /usr/sbin/pkg || true
run_command_retry "pkg update -f"
run_command_retry "pkg upgrade -y"

# Install required packages for development
run_command_retry "pkg install -y $PACKAGES"

# Use Qemu from PKG if not provided
if [ ! -e ~vagrant/qemu-src.tar.gz ] ; then
    run_command_retry "pkg install -y qemu"
elif [ ! -e /usr/local/bin/qemu-system-arm ] ; then
    # Compile Qemu from source
    run_command_retry "pkg install -y python pkgconf gmake bison flex gettext glib pixman"

    tar zxf ~vagrant/qemu-src.tar.gz -C ~vagrant
    rm ~vagrant/qemu-src.tar.gz
    cd ~vagrant/qemu-* && ./configure --prefix=/usr/local --target-list=arm-softmmu,i386-softmmu

    if [ ! -z "$SLAVE_CPUS" ] ; then
        cd ~vagrant/qemu-* && gmake -j$SLAVE_CPUS
    else
        cd ~vagrant/qemu-* && gmake -j5
    fi
    cd ~vagrant/qemu-* && gmake install
    rm -rf ~vagrant/qemu-*
fi

# Disable the FreeBSD linker, use the GNU linker
if [ -e /usr/bin/ld.bfd ] ; then
    mv /usr/bin/ld.bfd /usr/bin/ld.bfd.orig
fi

# Configure SSH daemon
if [ "`grep sshd_enable=\"YES\" /etc/defaults/rc.conf|wc -l`" -eq "0" ] ; then
   echo 'sshd_enable="YES"' >> /etc/defaults/rc.conf
fi
if [ "`grep 'Port 2222' /etc/ssh/sshd_config|wc -l`" -eq "0" ] ; then
   echo 'Port 2222' >> /etc/ssh/sshd_config
fi
EOF

# Run the chroot setup script
chmod +x $CHROOTDIR/usr/home/vagrant/common.sh
chmod +x $CHROOTDIR/usr/home/vagrant/setup.sh
chroot $CHROOTDIR sh -c /usr/home/vagrant/setup.sh

# Install SSH init script
cat > /etc/rc.d/sshd_chroot << EOF
#!/bin/sh
mount -t devfs devfs $CHROOTDIR/dev || true
chroot $CHROOTDIR /etc/rc.d/sshd \$1
EOF
chmod +x /etc/rc.d/sshd_chroot

# Start SSH daemon
/etc/rc.d/sshd_chroot start || true
