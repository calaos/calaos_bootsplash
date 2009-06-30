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


#include "Room.h"

using namespace CalaosHome;

bool CalaosHome::RoomHitsCompare(const Room* lhs, const Room* rhs)
{
        return lhs->getHits() > rhs->getHits();
}

Room::Room():ioClass("IO")
{
        ;
}

Room::Room(string _ioClass):ioClass(_ioClass)
{
        ;
}

Room::~Room()
{
        clearIOLists();
}

void Room::clearIOLists()
{
        list<IO*>::iterator it;
        for(it=ios.begin();it!=ios.end();it++)
                delete (*it);
        ios.clear();
}

Room Room::clone()
{
        Room r;
        list<IO*>::iterator it;
        for(it=ios.begin();it!=ios.end();it++)
                r.addCopyIO(*it);

        return r;
}

void Room::setName(string s)
{
        name=s;
}
string Room::getName()
{
        return name;
}
void Room::setType(string s)
{
        type = s;
}
string Room::getType()
{
        return type;
}
void Room::setHits(int i)
{
        hits =i;
}
int Room::getHits() const
{
        return hits;
}

void Room::setSendStateServer(bool b)
{
        sendStateServer = b;
}
int Room::getSendStateServer()
{
        return sendStateServer;
}

void Room::setId(int i)
{
        id =i;
}
int Room::getId()
{
        return id;
}

void Room::setIOs(list<IO*> v)
{
        ios = v;
}
list<IO*> &Room::getIOs()
{
        return ios;
}
int Room::gethigherHits()
{
        if(ios.size()>0)
                return (*(ios.begin()))->getHits();
        return 0;
}

void Room::reload()
{
        list<IO*>::iterator it;
        for(it=ios.begin();it!=ios.end();it++)
                delete (*it);
        ios.clear();
        load(getId(),getType());
}

string Room::toString()
{
        list<IO*>::iterator it;
        string s;
        s="## "+name+"\n";
        s+="\t type: "+type+"\n";
        s+="\t id: "+Utils::to_string(id)+"\n";
        s+="\t hits: "+Utils::to_string(hits)+"\n";
        for(it = ios.begin(); it!=ios.end(); ++it)
                s+=(*it)->toString();

        return s;
}

IO* Room::getIOFromId(string id)
{
        list<IO*> ::iterator it;

        for(it=ios.begin();it!=ios.end();it++)
        {
                if((*it)->getId()==id)
                        return (*it);
        }
        return NULL;
}

IO* Room::loadNewIO(string id,int type)
{
        IO* io = IOFactory::factory(ioClass);
        io->setSendStateServer(sendStateServer);
        io->setIOType(type);
        io->setId(id);
        io->setRoom(this);
        io->load();

        ios.push_back(io);
        ios.sort(IOHitsCompare);

        return io;
}

IO* Room::getConsignOf(IO* io)
{
        list<IO*>::iterator itIO;

        for(itIO=ios.begin(); itIO!=ios.end(); itIO++)
        {
                if(io->getChauffId() == (*itIO)->getChauffId()
                                && (*itIO)->getType() == "InternalInt")
                        return *itIO;
        }

        return NULL;
}

IO* Room::getTempOf(IO* io)
{
        list<IO*>::iterator itIO;

        for(itIO=ios.begin(); itIO!=ios.end(); itIO++)
        {
                if(io->getChauffId() ==
                   (*itIO)->getChauffId()
                                && (*itIO)->getType() == "WITemp")
                        return *itIO;
        }

        return NULL;
}

void Room::load(int id, string type)
{
        setType(type);
        setId(id);

        string answer;
        TCPSocket *socket = SocketManager::Instance().get_socket(0);
        if(socket && SocketManager::Instance().Send(socket, "room "+type+" get "+Utils::to_string(id)))
        {
                SocketManager::Instance().Recv(socket, answer, NETWORK_TIMEOUT);

                vector<string> tmp;
                vector<string> splitter;

                Utils::split(answer, splitter);
                for_each(splitter.begin(), splitter.end(), UrlDecode());

                for (uint b = 2;b < splitter.size();b++)
                {
                        tmp.clear();
                        Utils::split(splitter[b], tmp, ":");
                        if (tmp[0] == "input")
                                loadNewIO(tmp[1],INPUT);
                        else if (tmp[0] == "output")
                                loadNewIO(tmp[1],OUTPUT);
                        else if(tmp[0] == "name")
                                name = tmp[1];
                        else if(tmp[0] == "hits")
                                Utils::from_string(tmp[1],hits);
                }
        }
	
	cout << toString() << endl;
}

void Room::load(string name, string type)
{
        setType(type);
        setName(name);

        string answer;
        TCPSocket *socket = SocketManager::Instance().get_socket(0);
        if(socket && SocketManager::Instance().Send(socket, "room "+type+" get "+name))
        {
                SocketManager::Instance().Recv(socket, answer, NETWORK_TIMEOUT);
                vector<string> tmp;
                vector<string> splitter;

                Utils::split(answer, splitter);
                for_each(splitter.begin(), splitter.end(), UrlDecode());

                for (uint b = 2;b < splitter.size();b++)
                {
                        tmp.clear();
                        Utils::split(splitter[b], tmp, ":");
                        if (tmp[0] == "input")
                                loadNewIO(tmp[1],INPUT);
                        else if (tmp[0] == "output")
                                loadNewIO(tmp[1],OUTPUT);
                        else if(tmp[0] == "name")
                                name = tmp[1];
                        else if(tmp[0] == "hits")
                                Utils::from_string(tmp[1],hits);
                }
        }
	
	cout << toString() << endl;
}

IO* Room::addCopyIO(IO* io)
{
        IO* pio= io->clone();
        pio->setRoom(this);
        pio->setSendStateServer(sendStateServer);

        ios.push_back(pio);
        ios.sort(IOHitsCompare);
        return pio;
}


IO* Room::removeIO(IO* io)
{
        list<IO*>::iterator it;

        for(it=ios.begin();it!=ios.end();it++)
        {
                if((*it)->getId()==io->getId())
                {
                        ios.erase(it);
                        delete io;
                        return NULL;
                }
        }
        return io;
}

void Room::removeIOWithoutfree(IO* io)
{
        list<IO*>::iterator it;

        for(it=ios.begin();it!=ios.end();it++)
                if((*it)->getId()==io->getId())
                {
                        ios.erase(it);
                        return ;
                }
}
