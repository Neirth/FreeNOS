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

#ifndef __DOXYGEN_H
#define __DOXYGEN_H

/**
 * @defgroup bin bin
 *
 * User program binaries that can be executed from the command line as a command or via forkexec().
 */

/**
 * @defgroup sh sh
 *
 * The Shell is a command interpreter program that can run external programs or builtin commands.
 */

/**
 * @defgroup kernel kernel
 *
 * Kernel runs privileged code that schedules processes and performs virtual memory management.
 */

/**
 * @defgroup kernelapi API (system call interface)
 *
 * Application Programming Interface (API) provided by the Kernel for user programs.
 */

/**
 * @defgroup kernelapi_handler API (kernel handlers)
 *
 * API handlers contain the kernel side implementation of the Application Programming Interface (API).
 */

/**
 * @defgroup kernel_intel Intel
 *
 * Intel architecture specific kernel code such as context switching and bootup code.
 */

/**
 * @defgroup kernel_arm ARM
 *
 * ARM architecture specific kernel code such as context switching and bootup code.
 */

/**
 * @defgroup lib lib
 *
 * Libraries contain shared code that can be used anywhere, such as in user programs, servers and the kernel.
 */

/**
 * @defgroup liballoc liballoc
 *
 * Memory allocator library for managing the dynamic heap and virtual memory mappings.
 */

/**
 * @defgroup libarch libarch
 *
 * The architectures library provides code for specific processor architectures.
 */

/**
 * @defgroup libarch_arm ARM
 *
 * ARM architecture specific code such as caching, virtual memory and exception/interrupt handling.
 */

/**
 * @defgroup libarch_bcm Broadcom
 *
 * Broadcom System-on-Chip (SoC) device specific code including timers, interrupt controllers and more.
 */

/**
 * @defgroup libarch_intel Intel
 *
 * Intel architecture specific code such as multiprocessor, virtual memory and exception/interrupt handling.
 */

/**
 * @defgroup libexec libexec
 *
 * Executable formats library can parse and extract program code and data.
 */

/**
 * @defgroup libfs libfs
 *
 * The filesystems library provides generic support code for filesystems.
 */

/**
 * @defgroup libi2c libi2c
 *
 * This library provides support code for the I2C protocol used on some I/O devices.
 */

/**
 * @defgroup libipc libipc
 *
 * The Inter Process Communication (IPC) library allows processes to communicate, for example by message passing.
 */

/**
 * @defgroup libmpi libmpi
 *
 * MPI (Message Passing Interface) is a standard communication interface for parallel programming.
 */

/**
 * @defgroup libnet libnet
 *
 * The networking library implements standard networking protocols and networking support code.
 */

/**
 * @defgroup libposix libposix
 *
 * Provides support for the POSIX.1-2008 portable operating system interface for user programs.
 */

/**
 * @defgroup libspi libspi
 *
 * This library provides support code for the SPI protocol used on some I/O devices.
 */

/**
 * @defgroup libstd libstd
 *
 * Contains standard algorithms and datastructures for generic programming.
 */

/**
 * @defgroup libtest libtest
 *
 * Generic test framework with support for assertions, automatic test discovery and test reporting.
 */

/**
 * @defgroup libusb libusb
 *
 * This library provides support code for the USB protocol used on some I/O devices.
 */

/**
 * @defgroup server server
 *
 * Servers are programs that provide a part of the operating system in userspace, for example device drivers or filesystems.
 */

/**
 * @defgroup ATA ATA
 *
 * The ATA (Advanced Technology Attachment) driver provides a block I/O storage device.
 */

/**
 * @defgroup core core
 *
 * The CoreServer manages a single processor core on the target system.
 */

/**
 * @defgroup linnfs LinnFS
 *
 * The Linnenbank Filesystem (LinnFS) provides an inode based filesystem similar to Ext2.
 */

/**
 * @defgroup linncreate linncreate
 *
 * The LinnCreate program can create a new LinnFS filesystem.
 */

/**
 * @defgroup tmpfs TmpFS
 *
 * The temporary filesystem (TmpFS) provides an in-memory non-persistent filesystem.
 */

/**
 * @defgroup sysfs SysFS
 *
 * The system information filesystem (SysFS) provides current hardware and software information.
 */

/**
 * @defgroup grovepi grovepi
 *
 * The GrovePI is an I/O extension platform for the Raspberry Pi 1/2/3.
 */

/**
 * @defgroup server_ipctest ipctest
 *
 * Server for testing inter-process communication (IPC) using libipc.
 */

/**
 * @defgroup loopback loopback
 *
 * Local-only network interface driver that loops back all packets transmitted.
 */

/**
 * @defgroup smsc95xx smsc95xx
 *
 * Network interface driver for the SMSC95xx family of ethernet over USB devices.
 */

/**
 * @defgroup pn532 pn532
 *
 * The PN532 is a NFC reader device connected via an SPI bus.
 */

/**
 * @defgroup ps2 ps2
 *
 * The IBM Personal System/2 drivers supports keyboard input.
 */

/**
 * @defgroup serial serial
 *
 * The serial driver supports UART devices such as the i8250 and PL011.
 */

/**
 * @defgroup terminal terminal
 *
 * The terminal server provides the system console via input/output devices.
 */

/**
 * @defgroup time time
 *
 * The time driver contains an interface to the system Real Time Clock (RTC).
 */

/**
 * @defgroup usb usb
 *
 * These servers contain USB device driver implementations
 */

/**
 * @defgroup usb_controller controller
 *
 * These servers contain USB controller device driver implementations
 */

/**
 * @defgroup video video
 *
 * This server provides support for video devices, including VGA.
 */


#endif /* __DOXYGEN_H */
