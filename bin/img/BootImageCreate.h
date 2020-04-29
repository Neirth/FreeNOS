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

#ifndef __BIN_IMG_BOOTIMAGECREATE_H
#define __BIN_IMG_BOOTIMAGECREATE_H

#include <Application.h>
#include <ExecutableFormat.h>
#include <BootImage.h>

/**
 * @addtogroup bin
 * @{
 */

/** Maximum memory regions. */
#define BOOTENTRY_MAX_REGIONS 16

/**
 * Executable entry for use inside a boot image.
 */
typedef struct BootEntry
{
    /** BootSymbol definition */
    BootSymbol symbol;

    /** Memory regions for this symbol */
    ExecutableFormat::Region regions[BOOTENTRY_MAX_REGIONS];

    /** Number of memory regions. */
    Size numRegions;
}
BootEntry;

/**
 * Create system boot image
 */
class BootImageCreate : public Application
{
  public:

    /**
     * Constructor
     *
     * @param argc Argument count
     * @param argv Argument values
     */
    BootImageCreate(int argc, char **argv);

    /**
     * Destructor
     */
    virtual ~BootImageCreate();

    /**
     * Execute the application.
     *
     * @return Result code
     */
    virtual Result exec();

  protected:

    /**
     * Print text to output.
     *
     * @param string Text to print to program output.
     * @return Result code.
     */
    virtual Result output(const char *string) const;

  private:

    /**
     * Read boot symbols using a configuration file.
     *
     * @param file Name of the boot configuration file.
     * @param prefix Prefix each entry from the config file with the given path or NULL
     * @param entries Pointer to Vector with BootEntry's
     *
     * @return Number of bytes read
     */
    Size readBootSymbols(const char *file,
                         const char *prefix,
                         Vector<BootEntry *> *entries);
};

/**
 * @}
 */

#endif /* __BIN_IMG_BOOTIMAGECREATE_H */
