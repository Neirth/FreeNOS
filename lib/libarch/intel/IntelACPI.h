/*
 * Copyright (C) 2015 Niek Linnenbank
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

#ifndef __LIBARCH_INTEL_ACPI_H
#define __LIBARCH_INTEL_ACPI_H

#include <Types.h>
#include <CoreManager.h>
#include "IntelIO.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_intel
 * @{
 */

/**
 * Intel Advanced Configuration and Power Interface (ACPI).
 */
class IntelACPI : public CoreManager
{
  private:

    /** Memory base address for searching the RootSystemDescriptor. */
    static const uint RSDBase = 0x1000;

    /** Size of the memory region for searching the RootSystemDescriptor. */
    static const uint RSDSize = MegaByte(1) - 0x1000;

    /** Signature to detect a valid RootSystemDescriptor (part 1). */
    static const uint RootSystemSignature1 = 0x20445352;

    /** Signature to detect a valid RootSystemDescriptor (part 2). */
    static const uint RootSystemSignature2 = 0x20525450;

    /** Signature for the Root System Descriptor Table (RSDT). */
    static const u32 RootSystemTableSignature = 0x54445352;

    /** Signature for the Extended System Descriptor Table (XSDT). */
    static const u32 ExtendedSystemTableSignature = 0x54445358;

    /** Signature for the Multiple APIC Descriptor Table (MADT). */
    static const u32 MultipleAPICTableSignature = 0x43495041;

    /**
     * Root System Description Pointer (ACPI v1.0).
     */
    typedef struct RootSystemDescriptor1
    {
        u32  signature[2];
        u8   checksum;
        char vendorId[6];
        u8   revision;
        u32  rsdtAddress;
    } __attribute__((packed))
    RootSystemDescriptor1;

    /**
     * Root System Description Pointer (ACPI v2.0)
     */
    typedef struct RootSystemDescriptor2
    {
        RootSystemDescriptor1 v1;
        u32 length;
        u64 xsdtAddress;
        u8  extendedChecksum;
        u8  reserved[3];
    } __attribute__((packed))
    RootSystemDescriptor2;

    /**
     * System Descriptor Header (ACPI v3.0)
     */
    typedef struct SystemDescriptorHeader
    {
        u32 signature;
        u32 length;
        u8  revision;
        u8  checksum;
        char vendorId[6];
        char vendorTableId[8];
        u32  vendorRevision;
        u32  creatorId;
        u32  creatorRevision;
    } __attribute__((packed))
    SystemDescriptorHeader;

    /**
     * Root System Descriptor Table (RSDT)
     */
    typedef struct RootSystemTable
    {
        SystemDescriptorHeader header;
        u32 entry[];
    }
    RootSystemTable;

    /**
     * Extended System Descriptor Table (XSDT)
     */
    typedef struct ExtendedSystemTable
    {
        SystemDescriptorHeader header;
        u64 entry[];
    }
    ExtendedSystemTable;

    /**
     * Multiple APIC Description Table (MADT) entry.
     */
    typedef struct MultipleAPICTableEntry
    {
        u8 type;
        u8 length;
    }
    MultipleAPICTableEntry;

    /**
     * Multiple APIC Description Table (MADT) processor entry.
     */
    typedef struct MultipleAPICTableProc
    {
        MultipleAPICTableEntry header;
        u8 coreId;
        u8 apicId;
        u32 flags;
    }
    MultipleAPICTableProc;

    /**
     * Multiple APIC Description Table (MADT).
     */
    typedef struct MultipleAPICTable
    {
        SystemDescriptorHeader header;
        u32 ctrlAddress;
        u32 flags;
        MultipleAPICTableEntry entry[];
    }
    MultipleAPICTable;

    /**
     * Hardware registers.
     */
    enum Registers
    {
    };

  public:

    /**
     * Constructor
     */
    IntelACPI();

    /**
     * Initialize the ACPI.
     *
     * @return Result code.
     */
    Result initialize();

    /**
     * Discover processors.
     *
     * @return Result code.
     */
    virtual Result discover();

    /**
     * Boot a processor.
     *
     * @param info CoreInfo object pointer.
     *
     * @return Result code.
     */
    virtual Result boot(CoreInfo *info);

  private:

    /**
     * Scan for cores in the APIC tables.
     *
     * @return Result code.
     */
    Result scanAPIC(MultipleAPICTable *madt);

  private:

    /** I/O object for searching the RootSystemDescriptor. */
    IntelIO m_bios;

    /** Root/Extended SDT table I/O object. */
    IntelIO m_rootIO;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_INTEL_ACPI_H */
