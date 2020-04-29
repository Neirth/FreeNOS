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

#ifndef __LIBARCH_COREMANAGER_H
#define __LIBARCH_COREMANAGER_H

#include <Types.h>
#include <List.h>
#include "CoreInfo.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 */

/**
 * Generic Core Manager.
 */
class CoreManager
{
  private:

  public:

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        IOError,
        NotFound
    };

    /**
     * Constructor
     */
    CoreManager();

    /**
     * Get list of core identities.
     *
     * @return List of core identities.
     */
    List<uint> & getCores();

    /**
     * Discover processors.
     *
     * @return Result code.
     */
    virtual Result discover() = 0;

    /**
     * Boot a processor.
     *
     * @param info CoreInfo object pointer.
     *
     * @return Result code.
     */
    virtual Result boot(CoreInfo *info) = 0;

  protected:

    /** List of core ids found. */
    List<uint> m_cores;
};

/**
 * @}
 * @}
 */

#endif /* __LIBARCH_COREMANAGER_H */
