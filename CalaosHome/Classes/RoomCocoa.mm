/******************************************************************************
 **  Copyright (c) 2007-2009, Calaos. All Rights Reserved.
 **
 **  This file is part of Calaos Home.
 **
 ******************************************************************************/

#include "RoomCocoa.h"

using namespace CalaosHome;

RoomCocoa::RoomCocoa(): Room()
{
}

RoomCocoa::RoomCocoa(string ioClass): Room(ioClass)
{
}

void RoomCocoa::setName(string s)
{
        Room::setName(s);

        cocoaObject.roomNameLabel.text = [ NSString stringWithUTF8String: getName().c_str() ];
        
        cout << "setName: (" << this << ") " << getName() << endl;
}

void RoomCocoa::setCocoaObject(RoomViewController *e)
{
        cocoaObject = e;
        
        [ cocoaObject setRoomName: [ NSString stringWithUTF8String: getName().c_str() ] ];
        [ cocoaObject setRoomType: [ NSString stringWithUTF8String: getType().c_str() ] ];
        [ cocoaObject showHeat: NO ];
}

