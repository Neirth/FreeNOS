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

#ifndef __LIBIPC_CHANNELMESSAGE_H
#define __LIBIPC_CHANNELMESSAGE_H

#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libipc
 * @{
 */

/**
 * Basic message format for sending on a Channel
 */
class ChannelMessage
{
  public:

    /**
     * Message types
     */
    enum Type
    {
        Request  = 0,
        Response = 1
    };

    /** Message type is either a request or response */
    Type type:1;

    /** Optional request identifier */
    Size identifier:31;
};

/**
 * @}
 * @}
 */

#endif /* __LIBIPC_CHANNELMESSAGE_H */
