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
//-----------------------------------------------------------------------------



#ifndef  ROOM_INC
#define  ROOM_INC

#include <list>
#include <string>
#include "SocketManager.h"
#include "IO.h"
#include "IOFactory.h"

using namespace std;
using namespace CalaosNetwork;

namespace CalaosHome
{

        /**
         * Room represents a room and contains the input and output list of the room and some more information as the name<br>
         * Methods called load* retrieve information from the server,
         * consequently they use a network connection. To prevent some lag on the main thread,
         * run theses methods in a thread.
         */

        class Room
        {
                private:
                        /** The name of the io class used (simple IO, IOEdje ...) */
                        string ioClass;
                        /** the name of the room */
                        string name;
                        /** the id of the room in the type of room (see the type) */
                        int id;
                        /** the type of the room (salon, sdb ...) */
                        string type;
                        /** the priority of the room */
                        int hits;

                        /** the IO list */
                        list<IO*> ios;

                        /** if true, the io created by the room will
                         * send his state to the server
                         */
                        bool sendStateServer;
                public:

                        /** Constructor */
                        Room();
                        /** Constructor
                         * @parem ioClass  the type of io (IO or IOEdje)
                         */
                        Room(string ioClass);
                        /** Destructor */
                        ~Room();

                        /** Returns a clone of the Romm */
                        Room clone();

                        /** Clear the IO list */
                        void clearIOLists();

                        /** Set the name of the room */
                        virtual void setName(string s);
                        /** Returns the name of the room */
                        string getName();

                        /** Set the io list */
                        void setIOs(list<IO*> v);
                        /** Return the io list */
                        list<IO*> &getIOs();

                        /** Set if we want than new IO created by the room
                         * send their state to the server when a change occurs
                         */
                        void setSendStateServer(bool b);
                        /** Retrieve the sendStateServer, see setSendStateServer() */
                        int getSendStateServer();

                        /** Set the type of the room (sdb, salon ...) */
                        void setType(string s);
                        /** Returns the type of the room */
                        string getType();

                        /** Set the hits of the room <br>
                         * the list of room is ordered by their hits */
                        void setHits(int i);
                        /** Returns the hits of the room */
                        int getHits() const;

                        /** Set the id of the room */
                        void setId(int i);
                        /** Returns the id of the room */
                        int getId();

                        /** Returns a string which describes the room */
                        string toString();

                        /** Returns the higher IO's hits of the room */
                        int gethigherHits();

                        /**
                         * Returns the consigne of "io" <br>
                         * Return Null if no consigne is found
                         */
                        IO* getConsignOf(IO* io);

                        /**
                         * Returns the temp of the consigne "io" <br>
                         * Return Null if no temp is found
                         */
                        IO* getTempOf(IO* io);

                        /** Load a new input or output */
                        IO* loadNewIO(string id,int type);


                        /**
                         * Reload a room
                         * used this method when you want update a room
                         */
                        void reload();

                        /**
                         * The method retrieves all room's information (input, name ...) from the server
                         * This method is called in a thread by threadFunc::RoomsThread
                         */
                        void load(int id, string type);

                        /** Load the list of room from the id of the room and the type */
                        void load(string name, string type);

                        /**
                         * Add the copy of "io" in the io list
                         */
                        IO* addCopyIO(IO *io);

                        /**
                         * Remove an io and free the memory
                         * Return NULL if the io is delete, else the io if the io
                         * is not in the list
                         */
                        IO* removeIO(IO *io);

                        /**
                         * Remove the IO, doesn't free the IO
                         */
                        void removeIOWithoutfree(IO* io);

                        /**
                         * Search the IO with the id "id"
                         */
                        IO* getIOFromId(string id);;
        };

        /**
         * Uses this predicate to sort an Room list by hits
         */
        bool RoomHitsCompare(const Room* lhs, const Room* rhs);
}

#endif   /* ----- #ifndef ROOM_INC  ----- */

