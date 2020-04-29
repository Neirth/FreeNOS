About {#mainpage}
=====

FreeNOS (Free Niek's Operating System) is an experimental microkernel based
operating system for learning purposes written in C++. You may use the code
as you wish under the terms of the GPLv3.

Visit the project website at http://www.FreeNOS.org for more information.

Features
========

* Intel x86 (PC) and ARMv6/ARMv7 architectures (Raspberry Pi 1,2,3)
* Virtual memory
* Simple task scheduling
* Inter Process Communication (IPC)
* Symmetric Multi Processing with MPI support (Intel x86 only)
* Devices:
    * VGA/Keyboard consoles (also supported by Ed's libteken (http://80386.nl/projects/libteken/)
    * i8250 serial UART
    * PCI host controller
    * CMOS RTC clock
    * ATA host controller
    * USB controller and (root)hub (Raspberry Pi only)
    * Loopback network and SMSC95xx ethernet (Raspberry Pi only)
* Filesystems:
    * Virtual file system (VFS)
    * Temporary file system (TmpFS)
    * Linnenbank file system (LinnFS)
* Networking (IP, UDP, ICMP, for Raspberry pi 1 only)
* POSIX, ANSI C libraries
* Dynamic and Shared memory
* Fully automatic autotester
* All sources documented with Doxygen (http://www.doxygen.org)
* User and kernel code written from scratch in C++
* Very small microkernel (~2K lines of C++ code including a tiny part in assembly)
* Builds with recent GCC (http://gcc.gnu.org), LLVM (http://www.llvm.org) and SCons (http://www.scons.org) versions on POSIX systems

Host Setup
==========

First install all required build dependencies. FreeNOS needs SCons, an C++ compiler and for Intel targets a tool to generate ISO images.
Follow the instructions below to install the build dependencies on your host OS.

*Ubuntu*

Update your system repository cache and install the required development tools using:

    $ sudo apt-get update
    $ sudo apt-get install build-essential scons genisoimage xorriso qemu-system binutils-multiarch

If your Ubuntu host is 64-bit, you need to install the GCC multilib package
to cross compile for the 32-bit architecture:

    $ sudo apt-get install gcc-multilib g++-multilib

Alternatively, you can install the LLVM/Clang compiler as well:

    $ sudo apt-get install clang

*FreeBSD*

Update your system repository cache and install the required development tools using:

    % su -
    # pkg update
    # pkg install qemu scons cdrkit-genisoimage xorriso gcc

On FreeBSD, make sure that the latest version of the GNU linker (from pkg) is used:

    # mv /usr/bin/ld.bfd /usr/bin/ld.bfd.orig

Building FreeNOS
================

To download and extract the released source code on your host OS, run the
following commands. Replace 'x.x.x' with the version of FreeNOS:

    $ wget http://www.FreeNOS.org/pub/FreeNOS/source/FreeNOS-x.x.x.tar.gz
    $ tar zxf FreeNOS-x.x.x.tar.gz

To get the latest development source, you can clone the GIT archive:

    $ git clone https://github.com/nieklinnenbank/FreeNOS

To build FreeNOS using default settings (Intel, using GCC with debugging enabled), run:

    $ scons

To build FreeNOS with all full build commands printed on the console, set the
VERBOSE build variable to True:

    $ scons VERBOSE=True

To build FreeNOS with compiler optimizations enabled, set DEBUG to False.
Note that will make debugging using GDB more difficult:

    $ scons DEBUG=False

Instead of providing build variables on the command line, you can
also change the 'build.conf' configuration file for the target. The build configuration
file contains build variables, such as compiler flags and parameters for the target.
Similary, the 'build.host.conf' file contains build variables for the host OS programs.
See the example build.conf and build.host.conf files for more details.

Additionally, any environment variables set in the shell will be automatically converted
to identical build variables when running scons.

To build for the Raspberry Pi 2, copy the example build configuration file and run SCons:

    $ cp config/arm/raspberry2/build.conf .
    $ scons

To cleanup your build directory, use:

    $ scons -c

Or use the following command to also remove temporary SCons files:

    $ rm -rf build .sconf_temp .sconsign.dblite

Running FreeNOS
===============

To run the autotester of FreeNOS on the host OS, run:

    $ scons test

To run the same autotester on FreeNOS under Qemu, run:

    $ scons qemu_test

To start FreeNOS in a Qemu virtual machine with a serial console,
use the following command:

    $ scons qemu

To debug FreeNOS using GDB, you need to have build using the DEBUG=False build variable.
Ensure that your host OS has GDB available for debugging the target architecture (Intel or ARM).
For Ubuntu:

    $ sudo apt-get install gdb-multiarch

Open two terminals. In the first terminal, launch FreeNOS in Qemu with the internal gdbserver enabled:

    $ scons qemu_debug

In the second terminal, start GDB and connect to the Qemu internal gdbserver using:

    $ gdb-multiarch ./build/intel/pc/kernel/intel/pc/kernel
    (gdb) target remote localhost:1234

You can now use standard GDB commands to interactively debug the FreeNOS kernel for intel.
Similarly, you can also debug a user program (./build/intel/pc/bin/XXX) or the FreeNOS
kernel for ARM (./build/arm/raspberry2/kernel/arm/raspberry2/kernel).

intel/pc
--------

For Intel targets, you can also launch Qemu using the graphical VGA console (using SDL):

    $ scons qemu_sdl

To test FreeNOS on real hardware, write the boot ISO to a bootable device, such as an USB disk.
Insert the USB disk to your host OS and find the appropriate device name:

    $ fdisk -l

Alternatively, use the dmesg command to find the device name. Write the boot ISO
using the following command, where sdX is the name of the USB disk device.
Note: make sure to select the correct device to prevent data loss:

    $ sudo dd if=build/intel/pc/boot.iso of=/dev/sdX
    $ sudo sync

Insert the USB disk device to the target system and make sure to choose it as
boot device when the computer starts (e.g. via BIOS). You should see the FreeNOS VGA console.

arm/raspberry
-------------

To build FreeNOS for running on the Raspberry Pi 1, use the provided build.conf file
and rebuild the system:

    $ cp config/arm/raspberry/build.conf .
    $ scons

To test FreeNOS for the raspberry pi 1 under Qemu, rebuild the system
with the QEMU_BUILD build variable set to True:

    $ scons QEMU_BUILD=True
    $ scons qemu

To test on a real Raspberry Pi using SD card, download the latest Raspbian 'Lite' image
from https://www.raspberrypi.org/downloads/raspbian/ and unzip it. Insert the SD card to
your reader/writer slot on your host OS and find the appropriate device name using:

    $ fdisk -l

Alternatively, use the dmesg command to find the device name. Write the Raspbian image to the
SD card using the following command, where sdX is the name of the SD card device.
Note: make sure to select the correct device to prevent data loss:

    $ sudo dd if=/path/to/raspbian/image.img of=/dev/sdX
    $ sudo sync

Re-insert the SD-card to auto mount it on your host OS. Copy the FreeNOS raspberry pi kernel executable
to the '/boot' partition on the Raspbian image using (on Ubuntu):

    $ sudo cp build/arm/raspberry/kernel.img /media/boot/kernel.img
    $ sudo sync
    $ sudo umount /media/boot

You will need a USB-to-TTL-serial cable to connect to the Raspberry Pi UART.
Any USB-to-TTL-serial cable will work, for example the Adafruit USB-to-TTL-serial cable:

    https://www.adafruit.com/product/954

Connect your USB-to-serial cable to the Raspberry Pi using the GPIO pins:

    [ ] [ ]
    [ ] [X]  Pin 4  (V5)     <---  Red (only needed if powered via USB)
    [ ] [X]  Pin 6  (Ground) <---  Black
    [ ] [X]  Pin 8  (RX)     <---  White
    [ ] [X]  Pin 10 (TX)     <---  Green
    [ ] [ ]
    [ ] [ ]
    [ ] [ ]
    [ ] [ ]
    [ ] [ ]

Also see the following tutorial by Adafruit on connecting the serial cable to the Raspberry Pi:

    https://learn.adafruit.com/adafruits-raspberry-pi-lesson-5-using-a-console-cable/connect-the-lead

Connect your Raspberry Pi to the host to power it via USB or use an external power adapter.
On the host OS, launch a serial console client program such as minicom and connect it to the
USB serial device (baudrate 115200):

    $ minicom -D /dev/ttyUSB0

You should now see the FreeNOS console output.

arm/raspberry2
--------------

Follow the same instructions as for the Raspberry Pi 1 above, but use the Raspberry Pi 2
configuration file instead to build FreeNOS:

    $ cp config/arm/raspberry2/build.conf .
    $ scons

When copying the kernel executable to the SD card '/boot' partition, use the 'kernel7.img' filename
to make sure the Raspberry Pi bootloader correctly starts the kernel (ARMv7):

    $ cp build/arm/raspberry/kernel.img /media/boot/kernel7.img

Additionally, for raspberry pi 2 and newer, add the following to the /boot/config.txt
file to ensure the first UART is available on GPIO pins 8 and 10:

    dtoverlay=pi3-miniuart-bt
    enable_uart=1

Using FreeNOS
=============

When FreeNOS starts the system will print bootup output and present the system
login prompt. Currently the login will accept any username value and does not
ask for a password. The FreeNOS interactive console prompt looks like the following:

    (localhost) / #

FreeNOS has a UNIX-like interface and you may enter any of the commands available
in the /bin directory:

    (localhost) / # ls bin

For example, the 'ps' command prints a list of all processes running in the system:

    (localhost) / # ps

You can also run the fully automatic autotester inside Qemu or on real hardware
using the following command:

    (localhost) / # /test/run

To view some information about the FreeNOS version and hardware settings
you can use the 'sysinfo' command:

    (localhost) / # sysinfo

The '/' in the prompt indicates the current active directory. Change it with the 'cd'
built-in shell command:

    (localhost) / # cd /tmp
    (localhost) /tmp #

The FreeNOS shell has several built-in commands, use the 'help' command
to view all the built-in shell commands:

    (localhost) / # help

Example application program for calculating prime numbers is the /bin/prime command.
To let it compute all prime numbers up to 1024 and output the prime number results use:

    (localhost) / # prime --stdout 1024

For Intel, the prime command also has a MPI variant called 'mpiprime' which can
compute the prime numbers in parallel using multiple cores. To run it and let the
shell measure the time taken use:

    (localhost) / # time mpiprime 2000000

You can compare the time result versus the time take of the single core program
where it computes the same number of primes:

    (localhost) / # time prime 2000000

Jenkins Continuous Integration
==============================

Master Setup
------------

$ sudo apt-get install vagrant vagrant-libvirt libvirt-bin qemu-kvm

Install Jenkins on your host OS using your favorite package manager or from the official website (https://jenkins.io/).
Follow the installation wizard instructions and after installation go to the Jenkins web interface at: http://localhost:8080

After installation, navigate to: Manage Jenkins > Manage Plugins
Make sure the following plugins are installed. Choose the plugins from the 'Available' tab to find the plugins
which are not yet installed:

- Matrix Project Plugin (https://wiki.jenkins-ci.org/display/JENKINS/Matrix+Project+Plugin)
- Matrix Combinations Plugin (https://wiki.jenkins-ci.org/display/JENKINS/matrix+combinations+plugin)
- Node and Label Parameter Plugin (https://wiki.jenkins-ci.org/display/JENKINS/NodeLabel+Parameter+Plugin)
- Libvirt Slaves Plugin (https://wiki.jenkins.io/display/JENKINS/Libvirt+Slaves+Plugin)
- SSH Plugin (https://wiki.jenkins-ci.org/display/JENKINS/SSH+plugin)
- Workspace Cleanup Plugin (https://wiki.jenkins-ci.org/display/JENKINS/Workspace+Cleanup+Plugin)

After finishing Jenkins configuration, install KVM and Libvirt on your host OS. For Ubuntu use:

    $ sudo apt-get install qemu-kvm libvirt-bin virtinst

Connect Jenkins to libvirt via SSH, navigate to: Manage Jenkins > Configure System
Choose 'Add a new cloud' and select 'Hypervisor Type': QEMU,
'Hypervisor Host': localhost and choose your desired SSH username.

Press 'Advanced' to choose authentication parameters and press 'Test Connection' to
verify Jenkins can access libvirt via SSH. For Ubuntu, you need to ensure the SSH user
is in the 'kvm' and 'libvirtd' groups:

    $ sudo usermod -a -G kvm,libvirtd jenkins

Jenkins need to find the KVM guests by their hostname. You can either manually configure
the DNS hostname to IP translation in the /etc/hosts file or automatically using the
internal libvirt DNSMasq server.

To setup automatic DNS translation in KVM, first edit the 'default' libvirt network interface
to add the '.kvm' local-only domain:

    $ virsh net-edit default

Add the following line inside the <network>....</network> tag:

    <domain name='kvm' localOnly='yes'/>

To use the libvirt internal DNSMasq server on your host, you may change the /etc/resolv.conf
file to add the following:

    nameserver 192.168.122.1

If your host OS uses NetworkManager for networking, add the following files:

    $ sudo -s
    # cat > /etc/NetworkManager/conf.d/localdns.conf
    [main]
    dns=dnsmasq

    # cat > /etc/NetworkManager/dnsmasq.d/libvirt_dnsmasq.conf
    server=/kvm/192.168.122.1

Ensure that all KVM guests have their hostname set in the .kvm domain (e.g. someguest.kvm).
Restart libvirt and optionally NetworkManager to apply the changes:

    $ sudo /etc/init.d/libvirt-bin restart
    $ sudo /etc/init.d/NetworkManager restart

You should now be able to resolve the hostnames of your guests in the .kvm domain after they
are started, for example:

    $ virsh start ubuntu-1804
    $ host ubuntu-1804.kvm

Also visit the following page for more details on this automatic DNS setup for KVM:
https://liquidat.wordpress.com/2017/03/03/howto-automated-dns-resolution-for-kvmlibvirt-guests-with-a-local-domain/

FreeBSD 12.0 Slave
------------------

Run the example installation script in ./support/jenkins/freebsd-12.sh from the FreeNOS sources
to setup the KVM guest with FreeBSD 12.0. Also see the comments in the installation script for more info:

    jenkins@host$ cd support/jenkins
    jenkins@host$ ./freebsd-12.sh

To configure the FreeBSD 12.0 slave in Jenkins for building FreeNOS, you first need to
make sure the Jenkins user can login to the slave using SSH. Optionally, you can
configure this with public key authentication:

    jenkins@host$ virsh start freebsd-12
    jenkins@host$ ssh-keygen
    jenkins@host$ ssh-copy-id jenkins@freebsd-12.kvm

Test if the Jenkins user can login to the FreeBSD KVM guest with SSH (via password or key):

    jenkins@host$ ssh jenkins@freebsd-12.kvm

Copy the example node configuration XML file to the Jenkins installation directory:

    jenkins@host$ mkdir /var/lib/jenkins/nodes/freebsd-12-test
    jenkins@host$ cp freebsd-12.node.xml /var/lib/jenkins/nodes/freebsd-12-test/config.xml

Also copy the job configuration XML file to the Jenkins installation directory:

    jenkins@host$ mkdir /var/lib/jenkins/jobs/FreeNOS-freebsd12-test
    jenkins@host$ cp freebsd-12.job.xml /var/lib/jenkins/jobs/FreeNOS-freebsd12-test/config.xml

Restart the Jenkins server to use the new configuration files:

    $ sudo /etc/init.d/jenkins restart

Jenkins still needs authentication credentials to connect to the slave. Navigate to:

    Manage Jenkins > Manage Nodes > freebsd-12 > Configure

In the sub-menu 'Secondary launch method', under 'Launch agents via SSH',
select existing SSH credentials to let jenkins use it to login to the slave node
or add a new credentials with the 'Add' button. If you generated an SSH key for
the jenkins user on the host OS, insert the SSH private key or provide username/password.

Press the 'Save' button to finish and use 'Launch Agent' to test the connection.

You can now press the 'Schedule a Build' button on the 'FreeNOS-freebsd12' build job to test.

Ubuntu 18.04 Slave
------------------

Follow the same instructions as the FreeBSD 12.0 Slave above and just
replace the slave name with 'ubuntu-1804'.

Authors
=======

The FreeNOS code has been written from scratch by Niek Linnenbank.
People who contributed to FreeNOS are:

* Dan Rulos (AmayaOS)
* Coen Bijlsma (libparse, srv/time)
* Ed Schouten (libteken)
* Alexander Schrijver (OpenBSD patches)
