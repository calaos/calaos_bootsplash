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


#ifndef  ROOMS_INC
#define  ROOMS_INC

#include "RoomFactory.h"
#include "Utils.h"

/**
 * CalaosHome contains a list of class which represent a home:
 *      Rooms and IO (light, music player ...).
 * These information are keep synchronise with the server by the singleton Rooms which
 * retrieves all information sent by the server to inform a change in the room (light on/off, new room ...)
 *
 * Structure description:
 *  - Rooms: a singleton with a list of Room
 *  - Room: contains some information as the room's name and a list of IO
 *  - IO: information about an IO as the name, the state (on, off ...)
 *  - IOEdje and RoomEdje represent an IO and a Room which can be represent by an edje object (graphical object). They can be found in the apps in the Home panel for example (items in a list) and displays the state of the IO and allows to modify the IO (turn on a light ...).
 */
namespace CalaosHome
{


        using namespace std;
        using namespace CalaosNetwork;

        /**
         * - Rooms is a singleton which contains the room list and for each room the list of inputs and outputs
         * - These information are synchronise with the server
         *
         * - Methods called load* retrieve information from the server,
         *   consequently they use a network connection. To prevent some lag on the main thread,
         *   run theses methods in a thread.
         * - The method getRooms() returns an empty list while no rooms is loaded
         *
         *
         * - List of signals sent by the singleton :
         *   - "source ; signal": comments
         *   - "CalaosHome::Rooms ; roomLoadDone":  sent when the room list is loaded
         *                                              (see loadRoomList())
         *   - "CalaosHome::Rooms ; roomLoadStart": sent when we start (re)loading rooms and ios.
         *   - "CalaosHome::Rooms ; roomLoadFailed": sent when the load failed.
         *   - "CalasHome::Rooms ; roomListGet":   sent when we can retrieve the room list
         *                                              (see getRoomsAssynchronous())
         *   - "CalaosHome::Rooms ; IOStateChange": sent when the state of the io change
         *   - "CalaosHome::Rooms ; IOVisibleChange": sent when the visible value of the io change
         *   - "CalaosHome::Rooms ; IONameChange": sent when the name value of the io change
         *   - "CalaosHome::Rooms ; IOTypeChange": sent when the type value of the io change
         *   - "CalaosHome::Rooms ; IOGtypeChange": sent when the gtype value of the io change
         *   - "CalaosHome::Rooms ; IOEditableChange": sent when the editable value of the io change
         *   - "CalaosHome::Rooms ; IOIDChange": sent when the id value of the io change
         *   - "CalaosHome::Rooms ; IOHitsChange": sent when the hits value of the io change
         *   - "CalaosHome::Rooms ; IONew": sent when a new io is add
         *   - "CalaosHome::Rooms ; IOChauffageIDChange": sent when the chauffage id change
         *   - "CalaosHome::Rooms ; IODel": sent when the io is delete, The IO is deleted after all callbacks are called.
         *   - "CalaosHome::Rooms ; roomNameChange": sent when the name value of the room change
         *   - "CalaosHome::Rooms ; roomTypeChange": sent when the type value of the room change
         *   - "CalaosHome::Rooms ; roomHitsChange": sent when the hits value of the room change
         *   - "CalaosHome::Rooms ; roomDel": sent when the room is delete, The Room is deleted after all callbacks are called.
         *   - "CalaosHome::Rooms ; roomNew": sent when a new room is add
         *
         *
         *
         * - List of signals received by the singleton
         *   - "source ; signal": method called by the signal
         *   - "CalaosHome::Rooms ; roomLoadDone":  roomLoadDoneCB()
         *   - "CalaosNetwork::Notify,io,change ; *": IPCIOStateChange
         *   - "CalaosNetwork::Notify,io,new ; *": IPCIONew
         *   - "CalaosNetwork::Notify,io,del ; *": IPCIODel
         *   - "CalaosNetwork::Notify,room,modify ; *": IPCRoomModify
         *   - "CalaosNetwork::Notify,room,del ; *": IPCRoomDel
         *   - "CalaosNetwork::Notify,room,new ; *": IPCRoomNew
         *   - "CalaosHome::Notify", "reload,all" ; : load failed, restart loading
         */


        class Rooms: public CThread
        {
                private:
                        Rooms();

                        /**
                         * The type of IO created by the IOFactory (IO, IOEdje ...)
                         */
                        string ioClass;

                        /**
                         * The type of Room created by the RoomFactory (Room, RoomEdje ...)
                         */
                        string roomClass;


                        /**
                         * the room list
                         */
                        list<Room*> rooms;

                        /**
                         * True if the list room is loaded
                         */
                        bool isLoaded;

                        /**
                         * True if the list of rooms and io are currently loaded
                         */
                        bool loadInProgress;

                        /**
                         * signal called when all rooms are loaded
                         */
                        sigc::signal<void, string, string, void*, void*> sigRoomLoadDone;

                        /**
                         * signal called whe the state of an IO change
                         */
                        sigc::signal<void, string, string, void*, void*> sigIOStateChange;
                        sigc::signal<void, string, string, void*, void*> sigIONew;
                        sigc::signal<void, string, string, void*, void*> sigIODel;
                        sigc::signal<void, string, string, void*, void*> sigRoomModify;
                        sigc::signal<void, string, string, void*, void*> sigRoomDel;
                        sigc::signal<void, string, string, void*, void*> sigRoomNew;
                        sigc::signal<void, string, string, void*, void*> sigReloadAll;
                        sigc::signal<void, string, string, void*, void*> sigReconnect;

                        /**
                         * Mutex used by the thread to prevent running 2 threads
                         */
                        Mutex mutex;

                public:
                        /** Returns the instance of the singleton */
                        static Rooms& instance();

                        /** Destructor */

                        ~Rooms();
                        /** Returns the list of rooms */
                        list<Room*>& getRooms();

                        /** Set the list of rooms */
                        void setRooms(list<Room*>& r);

                        /** init the singleton
                         * @param _roomClass defines the type of room (Room or RoomEdje)
                         * @param _ioClass defines type type of IO (IO or IOEdje)
                         */
                        void init(string _roomClass, string _ioClass);

                        /**
                         * Send the signal "roomListGet" :
                         *   - immediately if the room list is loaded
                         *   - after the room list is loaded
                         */
                        void getRoomsAssynchronous();

                        /**
                         * A returns a list which contains
                         * all rooms of type "type"
                         */
                        list<Room*> getRoomListOfType(string type);

                        /**
                         * Load the list of room from the server
                         */
                        void loadRoomList();

                        /**
                         * Load the list of rooms
                         * Called this method in a thread
                         * loadRoomlist do this
                         * This method load all rooms in a local variables before affect theses rooms to
                         * the attribute of the class
                         */
                        void load();

                        /**
                         * Load a room in a local list
                         */
                        void loadNewRoom(list<Room*> &localRooms,int id, string type);
                        Room* loadNewRoom(list<Room*> &localRooms,string name, string type);

                        /**
                         * Return a reference in a IO which has the same
                         * id than the parameter IO
                         */
                        IO* getIOFromIO(IO* io);

                        /**
                         * Return the IO* object with the id "id"
                         */
                        IO* getIOFromID(string id);

                        /**
                         * Return the Room* object with the name "name" and the type "type"
                         */
                        Room* getRoomFromNameAndType(string name, string type);

                        /**
                         * Returns a string which describes the home (Room and IO)
                         */
                        string toString();


                        /**
                          * Returns the list of 6 prefered scenario
                          */
                        vector<IO*> getScenariosPref();

                        /*
                         * IPCs callbacks
                         */

                        void IPCRoomLoadDoneCB(string source, string emission, void*, void*);

                        void IPCIOStateChange(string source, string emission, void*, void*);
                        void IPCIONew(string source, string emission, void*, void*);
                        void IPCIODel(string source, string emission, void*, void*);

                        void IPCRoomModify(string source, string emission, void*, void*);
                        void IPCRoomDel(string source, string emission, void*, void*);
                        void IPCRoomNew(string source, string emission, void*, void*);

                        void IPCReloadAll(string source, string emission, void*, void*);

                        /*
                         * CThread functions
                         */
                        virtual void ThreadProc();
                        void threadClose();
        };

}

#endif   /* ----- #ifndef ROOMS_INC  ----- */

