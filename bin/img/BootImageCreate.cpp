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

#include <FreeNOS/System.h>
#include <FreeNOS/Config.h>
#include <Vector.h>
#include <ExecutableFormat.h>
#include <BootImage.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include "BootImageCreate.h"

BootImageCreate::BootImageCreate(int argc, char **argv)
    : Application(argc, argv)
{
    parser().setDescription("Create system boot image");
    parser().registerFlag('p', "prefix", "Prefix each entry from the config file with the given path");
    parser().registerPositional("CONFFILE", "Configuration file for the boot image");
    parser().registerPositional("OUTFILE", "Output file name of the boot image");
}

BootImageCreate::~BootImageCreate()
{
}

BootImageCreate::Result BootImageCreate::output(const char *string) const
{
    printf("%s", string);
    return Success;
}

Size BootImageCreate::readBootSymbols(const char *conf_file,
                                      const char *prefix,
                                      Vector<BootEntry *> *entries)
{
    char line[PATH_MAX];
    const char *prog = *(parser().name());
    const char *symbolname = 0;
    Size num = BOOTENTRY_MAX_REGIONS;
    Size totalBytes = 0, totalEntries = 0;
    BootEntry *entry;
    FILE *fp;
    ExecutableFormat *format;
    size_t prefix_len = prefix ? strlen(prefix) : 0;

    // Open configuration file
    if ((fp = fopen(conf_file, "r")) == NULL)
    {
        fprintf(stderr, "%s: failed to open `%s': %s\n",
                prog, conf_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Read out lines
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // Allocate new boot entry
        entry = new BootEntry;

        // Remove trailing newline
        if (strlen(line) < sizeof(line) - 1)
            line[strlen(line)-1] = 0;

        // Get boot symbol type
        if (strncmp(line, "BootProgram ", 12) == 0) {
            entry->symbol.type = BootProgram;
            symbolname = line + 12;
        } else if (strncmp(line, "BootPrivProgram ", 16) == 0) {
            entry->symbol.type = BootPrivProgram;
            symbolname = line + 16;
        } else if (strncmp(line, "BootData ", 9) == 0) {
            entry->symbol.type = BootData;
            symbolname = line + 9;
        } else {
            fprintf(stderr, "%s: symbol type unknown in line: %s\n",
                    prog, line);
            exit(EXIT_FAILURE);
        }

        // Fill boot symbol name first
        line[sizeof(line)-1] = 0;
        strncpy(entry->symbol.name, symbolname, BOOTIMAGE_NAMELEN);

        // Append path prefix, if set
        if (prefix_len && prefix_len < BOOTIMAGE_NAMELEN)
        {
            char tmp[PATH_MAX];
            snprintf(tmp, sizeof(tmp), "%s/%s", prefix, symbolname);
            strncpy(line, tmp, sizeof(line));
            symbolname = line;
        }

        // Find the file
        struct stat st;
        if (stat(symbolname, &st) == -1)
        {
            fprintf(stderr, "%s: failed to stat `%s': %s\n",
                    prog, symbolname, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // Allocate buffer
        u8 *buffer = new u8[st.st_size];

        // Read the file
        FILE *entry_fd = fopen(symbolname, "r");
        if (!entry_fd)
        {
            fprintf(stderr, "%s: failed to open `%s': %s\n",
                    prog, symbolname, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (fread(buffer, st.st_size, 1, entry_fd) != 1)
        {
            fprintf(stderr, "%s: failed to fread `%s': %s\n",
                    prog, symbolname, strerror(errno));
            exit(EXIT_FAILURE);
        }
        fclose(entry_fd);

        // Parse as BootProgram using libexec.
        if (entry->symbol.type == BootProgram || entry->symbol.type == BootPrivProgram)
        {
            if (ExecutableFormat::find(buffer, st.st_size, &format) != ExecutableFormat::Success)
            {
                fprintf(stderr, "%s: failed to parse executable image format in `%s': %s\n",
                            prog, symbolname, strerror(errno));
                exit(EXIT_FAILURE);
            }
            // Extract memory regions
            if (format->regions(entry->regions, &num) != ExecutableFormat::Success || num <= 0)
            {
                fprintf(stderr, "%s: failed to extract memory regions from `%s': %s\n",
                            prog, symbolname, strerror(errno));
                exit(EXIT_FAILURE);
            }
            entry->numRegions   = num;

            format->entry((Address *)&entry->symbol.entry);
        }
        // BootData
        else if (entry->symbol.type == BootData)
        {
            // Fill BootEntry
            entry->symbol.type = BootData;
            entry->numRegions  = 1;
            entry->regions[0].virt = 0;
            entry->regions[0].access = Memory::User | Memory::Readable | Memory::Writable;
            entry->regions[0].size = st.st_size;
            entry->regions[0].data = buffer;
        } else {
            fprintf(stderr, "%s: unknown boot symbol type: %d\n",
                    prog, (uint) entry->symbol.type);
            exit(EXIT_FAILURE);
        }
        // Insert into Array
        entries->insert(entry);
        totalEntries++;

        // Debug out memory sections
        for (Size i = 0; i < entry->numRegions; i++)
        {
            printf("%s[%u]: vaddr=%x size=%u\n",
                    symbolname, i, (uint) entry->regions[i].virt,
                    entry->regions[i].size);
            totalBytes += entry->regions[i].size;
        }
    }
    // Close config file
    fclose(fp);

    // All done
    printf("%d entries, %d bytes total\n", totalEntries, totalBytes);
    return totalEntries;
}

BootImageCreate::Result BootImageCreate::exec()
{
    Vector<BootEntry *> input;
    BootImage image;
    BootSymbol *symbols;
    BootSegment *segments;    
    FILE *fp;
    Size numSegments = 0, dataOffset = 0, lastDataOffset = 0;
    uint segCount = 0;
    const char *prog = *(parser().name());
    const char *conf_file = arguments().get("CONFFILE");
    const char *out_file = arguments().get("OUTFILE");
    const char *prefix = arguments().get("prefix");

    // Read boot symbols
    if (readBootSymbols(conf_file, prefix, &input) == 0)
    {
        fprintf(stderr, "%s: failed to read boot symbols\n", prog);
        return IOError;
    }

    // Allocate tables
    symbols = new BootSymbol[ input.count() ];
    memset(&image, 0, sizeof(image));
    memset(symbols, 0, sizeof(BootSymbol) * input.count());

    // Fill in the boot image header
    image.magic[0]       = BOOTIMAGE_MAGIC0;
    image.magic[1]       = BOOTIMAGE_MAGIC1;
    image.layoutRevision = BOOTIMAGE_REVISION;
    image.symbolTableOffset   = sizeof(BootImage);
    image.symbolTableCount    = input.count();
    image.segmentsTableOffset  = image.symbolTableOffset  +
                                (image.symbolTableCount   * sizeof(BootSymbol));

    // Fill in the boot symbols
    for (Size i = 0; i < input.count(); i++)
    {
        strncpy(symbols[i].name, input[i]->symbol.name, BOOTIMAGE_NAMELEN);
        symbols[i].type  = input[i]->symbol.type;
        symbols[i].entry = input[i]->symbol.entry;
        symbols[i].segmentsOffset = numSegments;
        symbols[i].segmentsCount  = input[i]->numRegions;
        symbols[i].segmentsTotalSize = 0;
        numSegments += input[i]->numRegions;
    }
    // Update BootImage
    image.segmentsTableCount = numSegments;
    
    // Now we allocate and clear the segments table
    segments = new BootSegment[numSegments];
    memset(segments, 0, sizeof(BootSegment) * numSegments);
    
    // Point segment data after the segments table
    dataOffset  = image.segmentsTableOffset +
                  image.segmentsTableCount  * sizeof(BootSegment);
    dataOffset += PAGESIZE - (dataOffset % PAGESIZE);
    
    // Fill the segments table by looping symbols
    for (Size i = 0; i < input.count(); i++)
    {
        // Loop the symbol segments
        for (Size j = 0; j < input[i]->numRegions; j++)
        {
            // Fill in the segment
            segments[segCount].virtualAddress = input[i]->regions[j].virt;
            segments[segCount].size           = input[i]->regions[j].size;
            segments[segCount].offset         = dataOffset;
            
            // Increment total segments size
            symbols[i].segmentsTotalSize += segments[segCount].size;

            // Increment data pointer. Align on memory page boundary
            dataOffset += segments[i].size;
            lastDataOffset = dataOffset;
            dataOffset += PAGESIZE - (dataOffset % PAGESIZE);
            segCount++;
        }
    }
    // Fill in the bootImageSize field
    image.bootImageSize = lastDataOffset;

    // Open boot image for writing
    if ((fp = fopen(out_file, "w")) == NULL)
    {
        fprintf(stderr, "%s: failed to open `%s': %s\n",
                prog, out_file, strerror(errno));
        return IOError;
    }

    // Write the final boot image headers
    if (fwrite(&image,    sizeof(image), 1, fp) <= 0 ||
        fwrite( symbols,  sizeof(BootSymbol)  * input.count(),  1, fp) <= 0 ||
        fwrite( segments, sizeof(BootSegment) * numSegments, 1, fp) <= 0)
    {
        fprintf(stderr, "%s: failed to write BootImage headers to `%s': %s\n",
                prog, out_file, strerror(errno));
        return IOError;
    }

    // Write the contents of the BootSegments
    for (Size i = 0; i < input.count(); i++)
    {
        // Loop regions/segments per boot symbol entry
        for (Size j = 0; j < input[i]->numRegions; j++)
        {
            // Adjust file pointer
            if (fseek(fp, segments[symbols[i].segmentsOffset].offset,
                      SEEK_SET) == -1)
            {
                fprintf(stderr, "%s: failed to seek to BootSegment contents in `%s': %s\n",
                        prog, out_file, strerror(errno));
                return IOError;
            }

            // Write segment contents
            if (fwrite(input[i]->regions[j].data,
                       input[i]->regions[j].size, 1, fp) <= 0)
            {
                fprintf(stderr, "%s: failed to write BootSegment contents to `%s': %s\n",
                        prog, out_file, strerror(errno));
                return IOError;
            }
        }
    }
    // Close file
    fclose(fp);
    
    // Done
    return Success;
}
