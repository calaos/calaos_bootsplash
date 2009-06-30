/******************************************************************************
 **  Copyright (c) 2007-2008, Calaos. All Rights Reserved.
 **
 **  This file is part of Calaos Home.
 **
 **  Calaos Home is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Calaos Home is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Foobar; if not, write to the Free Software
 **  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 **
 ******************************************************************************/

#ifndef  ROOMFACTORY_INC
#define  ROOMFACTORY_INC

#include "Room.h"

namespace CalaosHome
{
        /**
         * Create a room from a string which describes the type of room (Room or RoomEdje)
         * See the pattern factory
         */
        class RoomFactory
        {
                public:
                        /**
                         * Create the Room
                         * @param roomClass the type of room (Room or RoomCocoa)
                         * @param ioClass the type of io which will be in the room
                         */
                        static Room* factory(string roomClass, string ioClass);
        };
}

#endif   /* ----- #ifndef ROOMFACTORY_INC  ----- */

