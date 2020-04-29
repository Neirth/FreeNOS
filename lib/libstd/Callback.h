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

#ifndef __LIBSTD_CALLBACK_H
#define __LIBSTD_CALLBACK_H

#include "Types.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Represents a callback function
 */
class CallbackFunction
{
  public:

    /**
     * Execute the callback
     *
     * @param parameter Optional parameter to pass to the function
     */
    virtual void execute(void *parameter) = 0;
};

/**
 * Abstraction for providing a callback function to a object instance.
 */
template <class Base, class ParamType> class Callback : public CallbackFunction
{
  private:

    /** Callback function prototype */
    typedef void (Base::*Function)(ParamType *param);

  public:

    /**
     * Constructor
     *
     * @param object Object pointer
     * @param func Callback function
     */
    Callback(Base *object, Function func)
    {
        m_object   = object;
        m_function = func;
    }

    /**
     * Invoke the callback function on the object
     *
     * @param parameter Optional parameter to pass
     */
    virtual void execute(void *parameter)
    {
        executeOnObject((ParamType *) parameter);
    }

  private:

    /**
     * Execute the callback.
     *
     * @param parameter One parameter which is passed to the callback function.
     */
    virtual void executeOnObject(ParamType *parameter)
    {
        (m_object->*m_function)(parameter);
    }

  private:

    /** Object instance */
    Base *m_object;

    /** Function pointer */
    Function m_function;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_CALLBACK_H */
