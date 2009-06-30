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

#include "RoomFactory.h"
#include "RoomCocoa.h"

using namespace CalaosHome;

Room* RoomFactory::factory(string roomClass, string ioClass)
{
        if(roomClass == "Room")
                return new Room(ioClass);
        else if(roomClass == "RoomCocoa")
                return new RoomCocoa(ioClass);
        else
        {
                cout << "RoomFactory::factory(), Room class "
                << ioClass << " unknown."
                << endl;
                return new Room(ioClass);
        }
}
