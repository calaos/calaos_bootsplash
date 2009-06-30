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

#include "Rooms.h"

using namespace CalaosHome;

Rooms& Rooms::instance()
{
        static Rooms r;
        return r;
}

Rooms::Rooms():rooms(NULL),isLoaded(false),loadInProgress(false),mutex(false)
{
        sigRoomLoadDone.connect(sigc::mem_fun(*this, &Rooms::IPCRoomLoadDoneCB));
        IPC::Instance().AddHandler("CalaosHome::Rooms","roomLoadDone",sigRoomLoadDone, NULL);

        sigIOStateChange.connect(sigc::mem_fun(*this, &Rooms::IPCIOStateChange));
        IPC::Instance().AddHandler("CalaosNetwork::Notify,io,change","*",sigIOStateChange, NULL);
        sigIONew.connect(sigc::mem_fun(*this, &Rooms::IPCIONew));
        IPC::Instance().AddHandler("CalaosNetwork::Notify,io,new","*",sigIONew, NULL);
        sigIODel.connect(sigc::mem_fun(*this, &Rooms::IPCIODel));
        IPC::Instance().AddHandler("CalaosNetwork::Notify,io,del","*",sigIODel, NULL);

        sigRoomModify.connect(sigc::mem_fun(*this, &Rooms::IPCRoomModify));
        IPC::Instance().AddHandler("CalaosNetwork::Notify,room,modify","*",sigRoomModify, NULL);

        sigRoomDel.connect(sigc::mem_fun(*this, &Rooms::IPCRoomDel));
        IPC::Instance().AddHandler("CalaosNetwork::Notify,room,del","*",sigRoomDel, NULL);
        sigRoomNew.connect(sigc::mem_fun(*this, &Rooms::IPCRoomNew));
        IPC::Instance().AddHandler("CalaosNetwork::Notify,room,new","*",sigRoomNew, NULL);

        sigReloadAll.connect(sigc::mem_fun(*this, &Rooms::IPCReloadAll));
        IPC::Instance().AddHandler("CalaosHome::Notify", "reload,all", sigReloadAll, NULL);

        sigReconnect.connect(sigc::mem_fun(*this, &Rooms::IPCReloadAll));
        IPC::Instance().AddHandler("CalaosNetwork::Notify", "reconnect", sigReconnect, NULL);

        ioClass = "IO";
}

Rooms::~Rooms()
{
//         list<Room*>::iterator it;
//         for(it=rooms.begin();it!=rooms.end();it++)
//                 delete (*it);
        for_each(rooms.begin(), rooms.end(), Delete());
        rooms.clear();

        threadClose();
}

void Rooms::init(string _roomClass, string _ioClass)
{
        roomClass = _roomClass;
        ioClass = _ioClass;
        loadRoomList();
}

list<Room*>& Rooms::getRooms()
{
        return rooms;
}

void Rooms::setRooms(list<Room*>& r)
{
        rooms = r;
}

void Rooms::loadRoomList()
{
        loadInProgress = true;
        Start();
}

Room* Rooms::loadNewRoom(list<Room*> &localRooms,string name, string type)
{
        Room *room = RoomFactory::factory(roomClass,ioClass);
        room->setSendStateServer(true);
        room->load(name,type);
        localRooms.push_back(room);

        return room;
}

void Rooms::loadNewRoom(list<Room*> &localRooms,int id, string type)
{
        Room *room = RoomFactory::factory(roomClass,ioClass);
        room->setSendStateServer(true);
        room->load(id,type);
        localRooms.push_back(room);
}

void Rooms::load()
{
        string answer;
        loadInProgress = true;
        isLoaded = false;
        TCPSocket *socket = SocketManager::Instance().get_socket(0);

        cout << "Rooms::load()"<< endl;

        IPC::Instance().SendEvent("CalaosHome::Rooms","roomLoadStart",NULL);

        if(socket && SocketManager::Instance().Send(socket, "home ?"))
        {
                if (!SocketManager::Instance().Recv(socket, answer, NETWORK_TIMEOUT))
                {
                        loadInProgress = false;

                        IPC::Instance().SendEvent("CalaosHome::Notify", "reload,all", NULL);
                        return;
                }

                list<Room*> localRooms;

                vector<string> splitter;
                Utils::split(answer, splitter);
                for_each(splitter.begin(), splitter.end(), UrlDecode());

                //no room defined, exit thread
                if (splitter.size() <= 1)
                {
                        loadInProgress = false;
                        return;
                };

                for (uint i = 1;i < splitter.size();i++)
                {
                        vector<string> tmp;
                        Utils::split(splitter[i], tmp, ":");
                        int nb;
                        if (tmp.size() < 2) continue;
                        from_string(tmp[1], nb);
                        for(int j = 0;j < nb;j++)
                        {
                                loadNewRoom(localRooms,j,tmp[0]);
                        }

                        if (!nb) continue;
                }

                setRooms(localRooms);

                isLoaded = true;
                IPC::Instance().SendEvent("CalaosHome::Rooms","roomLoadDone",NULL);
        }
        loadInProgress = false;
}



string Rooms::toString()
{
        string s;
        list<Room*>::iterator it;
        for(it = rooms.begin(); it!=rooms.end(); ++it)
                s+= (*it)->toString();
        return s;
}

IO* Rooms::getIOFromIO(IO *io)
{
        list<Room*>::iterator itRoom;
        list<IO*>::iterator itIO;

        for(itRoom = rooms.begin();itRoom != rooms.end(); itRoom++)
        {
                for(itIO = (*itRoom)->getIOs().begin();
                                itIO != (*itRoom)->getIOs().end(); itIO++)
                {
                        if((*itIO)->getId()==io->getId())
                                return *itIO;
                }
        }
        return *itIO;
}

 Room* Rooms::getRoomFromNameAndType(string name, string type)
 {
         list<Room*>::iterator itRoom;
         for(itRoom = rooms.begin(); itRoom!=rooms.end(); itRoom++)
                 if((*itRoom)->getType() == type && (*itRoom)->getName() == name)
                         return *itRoom;

         return NULL;
 }

list<Room*> Rooms::getRoomListOfType(string type)
{
        list<Room*>::iterator itRoom;
        list<Room*> res;

        for(itRoom = rooms.begin(); itRoom!=rooms.end(); itRoom++)
                if((*itRoom)->getType() == type)
                        res.push_back(*itRoom);

        return res;
}

IO* Rooms::getIOFromID(string id)
{
        list<Room*>::iterator itRoom;
        list<IO*>::iterator itIO;

        for(itRoom = rooms.begin(); itRoom!=rooms.end(); itRoom++)
                for(itIO = (*itRoom)->getIOs().begin();
                                itIO != (*itRoom)->getIOs().end();
                                itIO++)
                {
                        if( (*itIO)->getId() == id)
                                return *itIO;
                }

        return NULL;
}

void Rooms::getRoomsAssynchronous()
{
        if(isLoaded)
                IPC::Instance().SendEvent("CalaosHome::Rooms","roomListGet",NULL);
        else if(!isLoaded && !loadInProgress)
                loadRoomList();
}


vector<IO*> Rooms::getScenariosPref()
{
        vector<IO*> res;
        list<Room*>::iterator itRoom;
        list<IO*>::iterator itIO;

        for(int i=0;i<6;i++)
                res.push_back(NULL);

        for(itRoom = rooms.begin(); itRoom!=rooms.end(); itRoom++)
        {
                for(itIO = (*itRoom)->getIOs().begin();
                itIO != (*itRoom)->getIOs().end();
                itIO++)
                {
                        if( (*itIO)->getType() == "scenario"
                            && (*itIO)->getScenarioPref() >= 1
                            && (*itIO)->getScenarioPref() <= 6
                            && (*itIO)->getIOType()==INPUT)
                        {
                                res[(*itIO)->getScenarioPref()-1] = *itIO;
                        }
                }
        }

        //complete the list with others scenarios
        int id_scenario = 0;
        for(int i=0;i<6;i++)
        {
                if(res[i] == NULL)
                {
                        int j = 0;
                        //search a scenario
                        for(itRoom = rooms.begin(); itRoom!=rooms.end() && !res[i]; itRoom++)
                        {
                                for(itIO = (*itRoom)->getIOs().begin();
                                itIO != (*itRoom)->getIOs().end() && !res[i];
                                itIO++)
                                {
                                        if( (*itIO)->getType() == "scenario"
                                            && ( (*itIO)->getScenarioPref() < 1
                                                 || (*itIO)->getScenarioPref() > 6 )
                                                && (*itIO)->getIOType()==INPUT)
                                        {
                                                if(j>=id_scenario)
                                                        res[i] = *itIO;
                                                else
                                                        j++;
                                        }
                                }
                        }

                        id_scenario++;
                }
        }

        return res;
}

/**
 * IPC CALLBACK
 */
void Rooms::IPCRoomLoadDoneCB(string source, string emission, void* listener_data, void* sender_data)
{
        cout
                << "Rooms::IPCRoomLoadDoneCB(), Rooms loaded "<< endl;
        IPC::Instance().SendEvent("CalaosHome::Rooms","roomListGet", NULL);
}

void Rooms::IPCReloadAll(string source, string emission, void* listener_data, void* sender_data)
{
        if (emission == "reconnect" || emission == "reload,all")
        {
                for_each(rooms.begin(), rooms.end(), Delete());
                rooms.clear();

                loadRoomList();
        }
}

void Rooms::IPCIOStateChange(string source, string emission, void*, void*)
{
        cout
                << "Rooms::IPCIOStateChange(), "<< emission << endl;

        vector<string> msgSplit, msgSplit2;

        split(emission,msgSplit);
        for_each(msgSplit.begin(), msgSplit.end(), UrlDecode());

        string type = msgSplit[0];
        string id = msgSplit[1];

        split(msgSplit[2], msgSplit2, ":");

        IO* ioInput = NULL;
        IO* ioOutput = NULL;
        list<Room*>::iterator itRoom;
        for(itRoom=rooms.begin();itRoom!=rooms.end();itRoom++)
        {
                list<IO*>::iterator itIO;
                for(itIO = (*itRoom)->getIOs().begin();
                                itIO != (*itRoom)->getIOs().end(); itIO++)
                {
                        if( (*itIO)->getIOType() == OUTPUT && (*itIO)->getId() == id)
                        {
                                ioOutput = (*itIO);
                                break;
                        }
                }
                for(itIO = (*itRoom)->getIOs().begin();
                                itIO != (*itRoom)->getIOs().end(); itIO++)
                {
                        if( (*itIO)->getIOType() == INPUT && (*itIO)->getId() == id)
                        {
                                ioInput = (*itIO);
                                break ;
                        }
                }
        }

        if(msgSplit2[0] == "state")
        {
                string state = msgSplit2[1];
                if(ioInput)
                {
                        ioInput->setLocalState(state);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOStateChange",ioInput);
                }
                if(ioOutput)
                {
                        ioOutput->setLocalState(state);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOStateChange",ioOutput);
                }
        }
        else if(msgSplit2[0] == "visible")
        {
                bool visible = (msgSplit2[1] == "true");

                if(ioInput)
                {
                        ioInput->setVisible(visible);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOVisibleChange",ioInput);
                }
                if(ioOutput)
                {
                        ioOutput->setVisible(visible);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOVisibleChange",ioOutput);
                }
        }
        else if(msgSplit2[0] == "name")
        {
                if(ioInput)
                {
                        ioInput->setName(msgSplit2[1]);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IONameChange",ioInput);
                }
                if(ioOutput)
                {
                        ioOutput->setName(msgSplit2[1]);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IONameChange",ioOutput);
                }
        }
        else if(msgSplit2[0] == "type")
        {
                if(ioInput)
                {
                        ioInput->setType(msgSplit2[1]);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOTypeChange",ioInput);
                }
                if(ioOutput)
                {
                        ioOutput->setType(msgSplit2[1]);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOTypeChange",ioOutput);
                }
        }
        else if(msgSplit2[0] == "gtype")
        {
                if(ioInput)
                {
                        ioInput->setGType(msgSplit2[1]);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOGTypeChange",ioInput);
                }
                if(ioOutput)
                {
                        ioOutput->setGType(msgSplit2[1]);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOGTypeChange",ioOutput);
                }
        }
        else if(msgSplit2[0] == "rw")
        {
                bool editable = (msgSplit2[1] == "true");
                if(ioInput)
                {
                        ioInput->setEditable(editable);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOEditableChange",
                                        ioInput);
                }
                if(ioOutput)
                {
                        ioOutput->setEditable(editable);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOEditableChange",
                                        ioOutput);
                }
        }
        else if(msgSplit2[0] == "chauffage_id")
        {
                int entier;
                from_string(msgSplit2[1],entier);
                if(ioInput)
                {
                        ioInput->setChauffId(entier);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOChauffageIDChange",
                                        ioInput);
                }
                if(ioOutput)
                {
                        ioOutput->setChauffId(entier);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOChauffageIDChange",
                                        ioOutput);
                }
        }
        else if(msgSplit2[0] == "id")
        {
                if(ioInput)
                {
                        ioInput->setId(msgSplit2[1]);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOIDChange",
                                        ioInput);
                }
                if(ioOutput)
                {
                        ioOutput->setId(msgSplit2[1]);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOIDChange",
                                        ioOutput);
                }
        }
        else if(msgSplit2[0] == "hits")
        {
                int hits;
                from_string(msgSplit2[1],hits);
                if(ioInput)
                {
                        ioInput->setHits(hits);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOHitsChange",
                                        ioInput);
                        ioInput->getRoom()->getIOs().sort(IOHitsCompare);
                }
                if(ioOutput)
                {
                        ioOutput->setHits(hits);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOHitsChange",
                                        ioOutput);

                        ioOutput->getRoom()->getIOs().sort(IOHitsCompare);
                }
        }
        else if(msgSplit2[0] == "scenarioPref")
        {
                int pref;
                from_string(msgSplit2[1],pref);
                if(ioInput)
                {
                        ioInput->setScenarioPref(pref);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOScenarioPrefChange",
                                        ioInput);
                }
                if(ioOutput)
                {
                        ioOutput->setScenarioPref(pref);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IOScenarioPrefChange",
                                        ioOutput);
                }
        }
}

void Rooms::IPCIONew(string source, string emission, void*, void*)
{
        cout
                << "Rooms::IPCIONew(), "<<emission<< endl;

        vector<string> msgSplit, msgSplit2, msgSplit3, msgSplit4;

        split(emission,msgSplit);
        for_each(msgSplit.begin(), msgSplit.end(), UrlDecode());

        string type = msgSplit[0];

        split(msgSplit[1], msgSplit2, ":");
        string id = msgSplit2[1];

        split(msgSplit[2], msgSplit3, ":");
        string roomName = msgSplit3[1];

        split(msgSplit[3], msgSplit4, ":");
        string roomType = msgSplit4[1];

        //search the Room
        list<Room*>::iterator itRoom;

        for(itRoom=rooms.begin();itRoom!=rooms.end();itRoom++)
        {
                if( (*itRoom)->getName() == roomName
                                && (*itRoom)->getType() == roomType)
                {
                        IO *io;
                        if(type == "new_output")
                                io = (*itRoom)->loadNewIO(id,OUTPUT);
                        else
                                io = (*itRoom)->loadNewIO(id,INPUT);
                        IPC::Instance().SendEvent("CalaosHome::Rooms","IONew",io);
                        return ;
                }
        }
}

void Rooms::IPCIODel(string source, string emission, void*, void*)
{
        cout
                << "Rooms::IPCIODel(), "<<emission<< endl;

        vector<string> msgSplit, msgSplit2, msgSplit3, msgSplit4;

        split(emission,msgSplit);
        for_each(msgSplit.begin(), msgSplit.end(), UrlDecode());

        string type = msgSplit[0];

        string id = msgSplit[1];

        split(msgSplit[2], msgSplit3, ":");
        string roomName = msgSplit3[1];

        split(msgSplit[3], msgSplit4, ":");
        string roomType = msgSplit4[1];

        //search the Room
        list<Room*>::iterator itRoom;

        for(itRoom=rooms.begin();itRoom!=rooms.end();itRoom++)
        {
                if( (*itRoom)->getName() == roomName
                                && (*itRoom)->getType() == roomType)
                {
                        IO *io = (*itRoom)->getIOFromId(id);
                        if(!io)
                        {
                                cout
                                        << "Rooms::IPCIONew(), Can not found this IO"<< endl;
                                return ;
                        }

                        (*itRoom)->removeIOWithoutfree(io);


                        IPC::Instance().SendEvent("CalaosHome::Rooms","IODel",IPCData(io, new DeletorT<IO *>()),true);
                        return ;
                }
        }
}


void Rooms::IPCRoomModify(string source, string emission, void*, void*)
{
        cout
                << "Rooms::IPCRoomModify(), "<<emission<< endl;

        vector<string> msgSplit, msgSplit2, msgSplit3, msgSplit4, msgSplit5;

        split(emission,msgSplit);
        for_each(msgSplit.begin(), msgSplit.end(), UrlDecode());

        string action = msgSplit[0];

        string name, type;

        split(msgSplit[1], msgSplit2, ":");
        string _old = msgSplit2[1];

        if(msgSplit2[0] == "old_room_name")
                name = msgSplit2[1];
        else if(msgSplit2[0] == "old_room_type")
                type = msgSplit2[1];

        split(msgSplit[2], msgSplit3, ":");
        string _new = msgSplit3[1];

        split(msgSplit[3], msgSplit4, ":");
        if(msgSplit2[0] == "old_room_name")
                type = msgSplit4[1];
        else if(msgSplit2[0] == "old_room_type")
                name = msgSplit4[1];
        else if(msgSplit2[0] == "old_room_hits")
        {
                name = msgSplit4[1];
                split(msgSplit[4], msgSplit5, ":");
                type = msgSplit5[1];
        }

        //search the Room
        list<Room*>::iterator itRoom;

        for(itRoom=rooms.begin();itRoom!=rooms.end();itRoom++)
        {
                if( (*itRoom)->getName() == name
                                && (*itRoom)->getType() == type)
                {
                        if(msgSplit2[0] == "old_room_name")
                        {
                                (*itRoom)->setName(_new);
                                IPC::Instance().SendEvent("CalaosHome::Rooms","roomNameChange",(*itRoom));
                        }
                        else if(msgSplit2[0] == "old_room_type")
                        {
                                (*itRoom)->setType(_new);
                                IPC::Instance().SendEvent("CalaosHome::Rooms","roomTypeChange",(*itRoom));
                        }
                        else if(msgSplit2[0] == "old_room_hits")
                        {
                                int hits;
                                from_string(_new,hits);
                                (*itRoom)->setHits(hits);
                                rooms.sort(RoomHitsCompare);
                                IPC::Instance().SendEvent("CalaosHome::Rooms","roomHitsChange",(*itRoom));
                        }
                        return ;
                }
        }
}

void Rooms::IPCRoomDel(string source, string emission, void*, void*)
{
        cout
                << "Rooms::IPCRoomDel(), "<<emission<< endl;

        vector<string> msgSplit, msgSplit2, msgSplit3;

        split(emission,msgSplit);
        for_each(msgSplit.begin(), msgSplit.end(), UrlDecode());

        string action = msgSplit[0];

        split(msgSplit[1], msgSplit2, ":");
        string name = msgSplit2[1];

        split(msgSplit[2], msgSplit3, ":");
        string type = msgSplit3[1];

        //search the Room
        list<Room*>::iterator itRoom;

        for(itRoom=rooms.begin();itRoom!=rooms.end();itRoom++)
        {
                if( (*itRoom)->getName() == name
                                && (*itRoom)->getType() == type)
                {
                        IPC::Instance().SendEvent("CalaosHome::Rooms","roomDel",IPCData(*itRoom, new DeletorT<Room *>()),true);
                        rooms.erase(itRoom);
                        return ;
                }
        }
}

void Rooms::IPCRoomNew(string source, string emission, void*, void*)
{
        cout
                << "Rooms::IPCRoomNew(), "<<emission<< endl;

        vector<string> msgSplit, msgSplit2, msgSplit3;

        split(emission,msgSplit);
        for_each(msgSplit.begin(), msgSplit.end(), UrlDecode());

        string action = msgSplit[0];

        split(msgSplit[1], msgSplit2, ":");
        string name = msgSplit2[1];

        split(msgSplit[2], msgSplit3, ":");
        string type = msgSplit3[1];

        Room *r = loadNewRoom(rooms,name, type);

        rooms.sort(RoomHitsCompare);
        IPC::Instance().SendEvent("CalaosHome::Rooms","roomNew",r);
}

/**
 * CThread functions
 */
void Rooms::ThreadProc()
{
	//if mutex is already locked, return imediatly because the
	//thread is being deleted
	if (!mutex.try_lock())
		return;
	
	load();
	mutex.unlock();		
}

void Rooms::threadClose()
{
/*	
        //Wait 2s for the thread to stop
        //after 2s close all network connections
        double t = ecore_time_get();
        while (ecore_time_get() - t < 2.0)
        {
                if (mutex.try_lock())
                {
                        //thread has quit
                        break;
                }

                struct timespec t;
                t.tv_sec = 0;
                t.tv_nsec = 150000000; //150ms
                nanosleep(&t, NULL);
        }

        //reset connections
        if (ecore_time_get() - t > 1.0)
        {
 */
                SocketManager::Instance().Connect(0);
                SocketManager::Instance().Connect(1);
                SocketManager::Instance().Connect(2);
/*
		}
*/
        End(); //wait for the thread
}
