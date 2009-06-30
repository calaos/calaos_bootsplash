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

#include "RoomEdje.h"

using namespace CalaosHome;

RoomEdje::RoomEdje():Room()
{
        ;
}

RoomEdje::RoomEdje(string ioClass):Room(ioClass)
{
        ;
}

void RoomEdje::setName(string s)
{
        Room::setName(s);

        list<EdjeObject*>::iterator it;
        for(it=edjeObjects.begin();it!=edjeObjects.end();it++)
        {
                edje_object_part_text_set((*it)->get_edje(), "object.text",getName().c_str());
        }
}

EdjeObject* RoomEdje::newEdjeObject(string theme, Evas* evas, string group)
{
        EdjeObject *e = new EdjeObject(theme, evas);
        e->LoadEdje(group);
        edje_object_part_text_set(e->get_edje(), "object.text",getName().c_str());

        e->Show();
        edje_object_signal_emit(e->get_edje(), "show", "calaos");

        addEdjeObject(e);

        return e;
}

void RoomEdje::addEdjeObject(EdjeObject *e)
{
        edjeObjects.push_back(e);
}

void RoomEdje::removeEdjeObject(EdjeObject *e)
{
        list<EdjeObject*>::iterator it;
        for(it=edjeObjects.begin();it!=edjeObjects.end();it++)
        {
                if((*it)==e)
                {
                        edjeObjects.erase(it);
                        break;
                }
        }
}
