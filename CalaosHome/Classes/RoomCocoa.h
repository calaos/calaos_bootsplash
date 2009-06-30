/******************************************************************************
 **  Copyright (c) 2007-2009, Calaos. All Rights Reserved.
 **
 **  This file is part of Calaos Home.
 **
 **
 ******************************************************************************/

#ifndef  ROOMCOCOA_INC
#define  ROOMCOCOA_INC

#include "Room.h"
#import "../Classes/RoomViewController.h"

using namespace std;
using namespace CalaosNetwork;

namespace CalaosHome
{

        /**
         * RoomCocoa represents a special room, a room wich can be represented by a Cocoa object
         * The room contains a Cocoa object and updates this object when the name of the 
         * room change (MVC pattern)
         */
        class RoomCocoa: public Room
        {
                private:
                        RoomViewController *cocoaObject;

                public:
                        /** Constructor */
                        RoomCocoa();
                        /** Constructor
                         * @param ioClass defines the type of IO*/
                        RoomCocoa(string ioClass);

                        /**
                         * See Room::setName() <br>
                         * + update all Cocoa objects
                         */
                        virtual void setName(string s);

                        /**
                         * Returns a Cocoa view which represents the Room
                         * and add this object in the cocoa objects list <br>
                         * This object will be updated when a change occurs in the room <br>
                         * Don't forget to call removeCocoaObject before deleting this object
                         */
                        RoomViewController *getCocoaObject() { return cocoaObject; }

                        /**
                         * Sets the Cocoa object
                         * This object will be updated when a change occurs in the room
                         */
                        void setCocoaObject(RoomViewController *e);
        };
}

#endif   /* ----- #ifndef ROOMCOCOA_INC  ----- */

