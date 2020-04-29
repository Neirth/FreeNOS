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

#ifndef __ATA_ATACONTROLLER_H
#define __ATA_ATACONTROLLER_H

#include <List.h>
#include <Device.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup ATA
 * @{
 */

/**
 * @name ATA I/O Bases.
 * @{
 */

/** @brief First ATA Bus Command I/O Base. */
#define ATA_BASE_CMD0   0x1f0

/** @brief Second ATA Bus Command I/O Base. */
#define ATA_BASE_CMD1   0x170

/** @brief First ATA Bus Control I/O Base. */
#define ATA_BASE_CTL0   0x3f6

/** @brief Second ATA Bus Control I/O Base. */
#define ATA_BASE_CTL1   0x376

/**
 * @}
 */

/**
 * @name ATA Command Registers.
 * @see http://wiki.osdev.org/ATA_PIO_Mode#Registers
 * @{
 */

/**
 * @brief Data port.
 * Read and write PIO data bytes on this port.
 */
#define ATA_REG_DATA    0

/**
 * @brief Features and Error info.
 * Mostly used with ATAPI.
 */
#define ATA_REG_ERROR   1

/**
 * @brief Sector Count.
 * Number of sectors to read or write (0 = special value).
 */
#define ATA_REG_COUNT   2

/**
 * @brief Partial Disk Sector address.
 * This register is CHS, LBA28 and LBA48 specific.
 */
#define ATA_REG_ADDR0   3

/**
 * @brief Partial Disk Sector address.
 */
#define ATA_REG_ADDR1   4

/**
 * @brief Partial Disk Sector address.
 */
#define ATA_REG_ADDR2   5

/**
 * @brief Drive Select bit, Flag bits, Extra address bits.
 */
#define ATA_REG_SELECT  6

/**
 * @brief Command port and Regular Status port.
 * Used to write commands and read status.
 */
#define ATA_REG_CMD 7

/**
 * @brief Regular Status port.
 * This is the same register as the command register.
 * @see ATA_REG_CMD
 */
#define ATA_REG_STATUS  7

/**
 * @}
 */

/**
 * @name ATA Status Registers.
 * @{
 */

/**
 * @brief Error flag (when set).
 * Send a new command to clear it (or nuke it with a Software Reset).
 */
#define ATA_STATUS_ERROR 0x01

/**
 * @brief Drive data ready for transfer.
 * Set when the drive has PIO data to transfer, or is ready to accept PIO data.
 */
#define ATA_STATUS_DATA  0x08

/**
 * @brief Drive is preparing to accept or send data.
 * Wait until this bit clears. If it never clears, do a Software Reset.
 * Technically, when BSY is set, the other bits in the Status bytes are meaningless.
 */
#define ATA_STATUS_BUSY  0x80

/**
 * @}
 */

/**
 * @name ATA Control Registers.
 * @see http://wiki.osdev.org/ATA_PIO_Mode#Device_Control_Register_.2F_Alternate_Status
 * @{
 */

/**
 * @brief Software Reset.
 * Set this to reset all ATA drives on a bus, if one is misbehaving.
 */
#define ATA_REG_RESET 0x4

/**
 * @brief Interrupt Disable.
 * Set this to stop the current device from sending interrupts.
 */
#define ATA_REG_INTR  0x2

/**
 * @}
 */

/**
 * @name ATA Device Selector Flags.
 * @{
 */

/** @brief Master Drive in Legacy mode. */
#define ATA_SEL_MASTER      0xa0

/** @brief Master Drive in 28-bit LBA mode. */
#define ATA_SEL_MASTER_28   0xe0

/** @brief Master Drive in 48-bit LBA mode. */
#define ATA_SEL_MASTER_48   0x40

/**
 * @}
 */

/**
 * @name ATA Commands.
 * @{
 */

/** @brief Identifies an ATA device, if any. */
#define ATA_CMD_IDENTIFY 0xec

/** @brief Reads sectors from an ATA device. */
#define ATA_CMD_READ     0x20

/**
 * @}
 */

/**
 * @brief Swap ASCII bytes from IDENTIFY.
 *
 * This macro is needed to swap ASCII bytes in
 * the IDENTIFY result buffer, because IDENTIFY returns
 * 256 little-endian words.
 */
#define IDENTIFY_TEXT_SWAP(field,size) \
    \
    ({ \
    u8 tmp; \
        \
        for (int i = 0; i < (size); i+=2) \
        { \
        tmp = (field)[i]; \
        (field)[i]   = (field)[i+1]; \
        (field)[i+1] = tmp; \
    } \
    })

/**
 * @brief IDENTIFY data presentation.
 */
typedef struct IdentifyData
{
    u16 type;
    u16 reserved1[9];
    u8  serial[20];
    u16 reserved2[3];
    u8  firmware[8];
    u8  model[40];
    u16 maxTransfer;
    u16 trustedFeatures;
    u16 capabilities[2];
    u16 reserved3[8];
    u32 sectors28;
    u16 reserved4[18];
    u16 majorRevision;
    u16 minorRevision;
    u16 supported[6];
    u16 reserved5[12];
    u64 sectors48;
    u16 reserved6[2];
    u16 sectorSize;
}
IdentifyData;

/**
 * @brief Represents a Drive on the ATA bus.
 */
typedef struct ATADrive
{
    /** Bytes returned from IDENTIFY. */
    IdentifyData identity;

    /** Number of sectors. */
    Size sectors;
}
ATADrive;

/**
 * @brief AT Attachment (ATA) Host Controller Device.
 */
class ATAController : public Device
{
  public:

    /**
     * @brief Constructor function.
     */
    ATAController();

    /**
     * @brief Configures the ATA controller.
     *
     * @return Error result code.
     */
    virtual Error initialize();

    /**
     * Read bytes from a drive attached to the ATA controller.
     *
     * @param buffer Buffer to store bytes to read.
     * @param size Number of bytes to read.
     * @param offset Offset in the device.
     *
     * @return Number of bytes on success and an error code on failure.
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

    /**
     * @brief Process ATA interrupts.
     *
     * @param vector Interrupt number.
     *
     * @return Error result code.
     */
    virtual Error interrupt(Size vector);

  private:

    /**
     * @brief Polls the Regular Status register.
     *
     * @param noData Don't wait for the ATA_STATUS_DATA flag to set.
     */
    void pollReady(bool noData = false);

  private:

    /** @brief Drives detected on the ATA bus. */
    List<ATADrive *> drives;
};

/**
 * @}
 * @}
 */

#endif /* __ATA_ATACONTROLLER_H */
