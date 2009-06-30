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


#ifndef  IO_INC
#define  IO_INC
#include <string>
#include "SocketManager.h"

using namespace CalaosNetwork;

namespace CalaosHome
{
        using namespace std;

        class Room;

        enum IOEnum
        {
                INPUT,
                OUTPUT
        };




        /**
         * IO represents an input or an output<br>
         * Methods called load* retrieve information from the server,
         * consequently they use a network connection. To prevent some lag on the main thread,
         * run theses methods in a thread.
         */

        class IO
        {
                protected:
                        //the room where the IO is
                        Room *room;

                        // The type of the inOuput (INPUT or OUTPUT)
                        int ioType;
                        // The id of the in/output
                        string id;
                        // The name
                        string name;
                        // The type (WIDigitalTriple)
                        string type;
                        // The type of the data (bool, float, string)
                        string varType;
                        /**
                         * If the in/output is visible, some internals input/output (variables ...)
                         * are hidden
                         */
                        bool visible;

                        // if the io is editable, a string io can be editable or not
                        bool editable;

                        // The priority of the in/output
                        int hits;

                        // The value of the in/output (depends of varType)
                        string state;
                        // Graphical type
                        string gType;

                        int chauffId;

                        // uses to retrieve the color before the io was off
                        string WODaliRVBPreviousState;

                        //If the io is a scenario
                        //this boolean defines the state of the scenario
                        //(run (true) or stop (false))
                        //The boolean is only valid in the case of a simple scenario
                        //(created from the touchscreen scenario editor)
                        string scenarioBoolStateID;

                        //If the io is a scenario
                        //defines the preference number (0..5)
                        //-1 if the scenario is not in the pref list
                        int scenarioPref;

                        //If the io is a boolean
                        //maybe the boolean is associated to a simple scenario
                        string booleanScenarioID;

                        //If true, when the state changes, it is sent to the server
                        bool sendStateServer;
                public:
                        /** Constructor */
                        IO();
                        /** Destructor */
                        ~IO();

                        /** Returns a clone of the IO */
                        virtual IO* clone();

                        /** Returns the room where the IO is */
                        Room* getRoom();
                        /** Set the room where the  IO is */
                        void setRoom(Room *r);

                        /** Returns the type of the IO (INPUT or OUTPUT) */
                        int getIOType();
                        /** Set the type of the IO (INPUT or OUTPUT) */
                        void setIOType(int t);

                        /** Returns the hits of the IO <br>
                         * In a room, the IO list is ordered by Hits
                         */
                        int getHits() const;
                        /** Set the hits of an IO */
                        void setHits(int t);

                        /** Returns the chaffage id of the IO <br>
                         * -1 if no chauffage id
                         */
                        int getChauffId() const;
                        void setChauffId(int t);
                        void setSendStateServer(bool b);
                        int getSendStateServer();

                        /**
                         * Set the state of an IO <br>
                         * If getSendStateServer() return true <br>
                         *      - the state 's' will be send to the server, <br>
                         *      - the state will not be change in the object <br>
                         *      - the server will send a notification when the state will be change <br>
                         *      - and finally the state in the object will be update <br>
                         *
                         *      - client --send change->  server
                         *      - server --send notification-> client
                         *      - client update the state
                         *
                         * else<br>
                         *      - the state will be update in the object <br>
                         *      - and not sent to the server (see setLocalState()) <br>
                         *      - For example, in the scenario editor, in the right list <br>
                         *              - when the state of an io is change, <br>
                         *              - the new state is not sent to the server <br>
                         *              - this state will be use to create a rules <br>
                         */
                        void setState(string s);

                        /** Change the state of the IO, <br>
                         * Do not send the change to the server.*/
                        virtual void setLocalState(string s);

                        /** Returns the state of the object <br>
                         * If the IO is a WoVoletSmart, see getPercentWoVoletSmart() and
                         * getStatusWoVoletSmart()
                         */
                        string getState();

                        /** Set the GType of the IO */
                        void setGType(string s);
                        /** Returns the GType of the IO */
                        string getGType();


                        /** Returns the type of the IO (WoVoletSmart, Light ...) */
                        string getType();
                        /** Set the type of the IO */
                        void setType(string s);

                        /** Returns the type of the state (Float, String ...) */
                        string getVarType();
                        /** Set the type of the state */
                        void setVarType(string s);

                        /** Returns the id of the IO <br>
                         * The id is unique in all the home
                         */
                        string getId();
                        /** Set the id of the IO */
                        void setId(string s);

                        /** Returns the name of the IO */
                        string getName();
                        /** Set the name of the IO */
                        virtual void setName(string s);

                        /** Returns true if the IO is visible */
                        bool isVisible();
                        /** Set if the IO is visible or not */
                        void setVisible(bool b);

                        /** Returns true if the IO is editable <br>
                         * Some IO as String IO can be editable or not */
                        bool isEditable();
                        /** Set if an IO is editable or not */
                        virtual void setEditable(bool b);

                        /** Returns a string which contains all information about the IO */
                        string toString();

                        /** return the pref number
                          * Available only for a scenario
                          */
                        int getScenarioPref() { return scenarioPref; }

                        /**
                          * Set the pref number (1..6)
                          */
                        void setScenarioPref(int pref) { scenarioPref = pref; }

                        /**
                         * The type WoVoletSmart has 2 information in the state
                         *  - the opening percent (56%, 75% ...)
                         *  - the current status (up, down, stop )
                         */
                        int getPercentWoVoletSmart();
                        /** See getPercentWoVoletSmart() */
                        void setPercentWoVoletSmart(int percent);
                        /** See getPercentWoVoletSmart() */
                        string getStatusWoVoletSmart();
                        /** See getPercentWoVoletSmart() */
                        void setStatusWoVoletSmart(string s);

                        /**
                         * Test if the io is an internal variable or a scenario. <br>
                         * Theses objects are input and output in the same time, consequently
                         * they are 2 times in the io list of a room
                         */
                        bool isInternal();

                        /**
                         * This method reload an input/output from the server <br>
                         * Used this method when you want update an input/ouput
                         */
                        void reload();

                        /**
                         * This method retrieves all information about an input or output (id, name, type ...)
                         * from the server. <br>
                         * This method is called in a thread by Room::load(). <br>
                         * Set the id before calling it
                         */
                        void load();
        };

        /**
         * Uses this predicate to sort an IO list by hits
         */
        bool IOHitsCompare(const IO* lhs, const IO* rhs);
}

#endif   /* ----- #ifndef IO_INC  ----- */

