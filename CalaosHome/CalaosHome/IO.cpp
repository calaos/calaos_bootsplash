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


#include "IO.h"
#include "Rooms.h"

using namespace CalaosHome;

bool CalaosHome::IOHitsCompare(const IO* lhs, const IO* rhs)
{
        return lhs->getHits() > rhs->getHits();
}

IO::IO():room(NULL),editable(true),chauffId(-1),scenarioBoolStateID(""),scenarioPref(-1),booleanScenarioID(""),sendStateServer(false)
{
        ;
}

IO::~IO()
{
        ;
}

IO* IO::clone()
{
        return new IO(*this);
}

void IO::setRoom(Room *r)
{
        room = r;
}

Room* IO::getRoom()
{
        return room;
}
void IO::setSendStateServer(bool b)
{
        sendStateServer = b;
}
int IO::getSendStateServer()
{
        return sendStateServer;
}
void IO::setState(string s)
{
        if(sendStateServer)
        {
                string cmd = "output " + id + " set " + s;

                if(type == "WODali" || type == "WODaliRVB")
                        cmd = "output " + id + " set set%20" + s;

                if(type == "scenario")
                        cmd = "output " + id + " set true";

                TCPSocket *socket = SocketManager::Instance().get_socket(0);
                SocketManager::Instance().SendAsync(socket, cmd);
        }
        else
                setLocalState(s);
}

void IO::setLocalState(string s)
{
        IO *io;
        if(type == "InternalBool" && booleanScenarioID != ""
                        && (io=Rooms::instance().getIOFromID(booleanScenarioID))
                        && io->getState() != state)
        {
                io->setLocalState(getState());
        }


        if(type=="WODaliRVB" && s == "0")
        {
                WODaliRVBPreviousState = state;
        }

        state = s;
}

string IO::getState()
{
        return state;
}

void IO::setGType(string s)
{
        gType = s;
}

string IO::getGType()
{
        return gType;
}

string IO::getId()
{
        return id;
}
void IO::setId(string s)
{
        id = s;
}
string IO::getName()
{
        return name;
}
void IO::setName(string s)
{
        name = s;
}
int IO::getIOType()
{
        return ioType;
}
void IO::setIOType(int t)
{
        ioType = t;
}
int IO::getHits() const
{
        return hits;
}
void IO::setHits(int t)
{
        hits = t;
}
int IO::getChauffId() const
{
        return chauffId;
}
void IO::setChauffId(int t)
{
        chauffId = t;
}
string IO::getType()
{
        return type;
}
void IO::setType(string s)
{
        type = s;
}
string IO::getVarType()
{
        return varType;
}
void IO::setVarType(string s)
{
        varType = s;
}
bool IO::isVisible()
{
        return visible;
}
void IO::setVisible(bool b)
{
        visible = b;
}
bool IO::isEditable()
{
        return editable;
}
void IO::setEditable(bool b)
{
        editable = b;
}
string IO::toString()
{
        string s;
        if(ioType==INPUT)
                s="\t\tINPUT: ";
        else
                s="\t\tOUTPUT: ";
        s+="["+id+"] ["+name+"] ["+type+"] [varType: "+varType+ "] [hits: "+Utils::to_string(hits)+"] [state: "+state+"]";

        if(gType!="")
                s+=" [gType: "+gType+"]";

        if(visible)
                s+=" [visible]\n";
        else
                s+=" [non visible]\n";
        return s;
}

int IO::getPercentWoVoletSmart()
{
        if(type!="WOVoletSmart")
        {
                cout <<
                        "IO::getIntWoVoletSmart(): the IO is not a WoVoletSmart but a "<<type <<endl;
                return 0;
        }

        vector<string> tokens;
        int percent;

        Utils::split(state, tokens);
        from_string(tokens[1], percent);
        return percent;
}


void IO::setPercentWoVoletSmart(int percent)
{
        if(type!="WOVoletSmart")
        {
                cout <<
                        "IO::setIntWoVoletSmart(): the IO is not a WoVoletSmart but a "<<type <<endl;
                return ;
        }

        vector<string> tokens;

        Utils::split(state, tokens);
        string s = tokens[0] + " " + Utils::to_string(percent);

        if(sendStateServer)
        {
                string cmd = "output " + id + " set set%20" + Utils::to_string(percent);

                TCPSocket *socket = SocketManager::Instance().get_socket(0);
                SocketManager::Instance().SendAsync(socket, cmd);
        }
        else
                setLocalState(s);
}

string IO::getStatusWoVoletSmart()
{
        if(type!="WOVoletSmart")
        {
                cout <<
                        "IO::getStringWoVoletSmart(): the IO is not a WoVoletSmart but a "<<type <<endl;
                return 0;
        }

        vector<string> tokens;
        Utils::split(state, tokens);
        return tokens[0];
}


void IO::setStatusWoVoletSmart(string s)
{
        if(type!="WOVoletSmart")
        {
                cout <<
                        "IO::setStringWoVoletSmart(): the IO is not a WoVoletSmart but a "<<type <<endl;
                return ;
        }

        vector<string> tokens;
        Utils::split(state, tokens);
        s = s + " " + tokens[1];
        setState(s);
}


bool IO::isInternal()
{
        if(type=="scenario"
                        || type=="InternalBool"
                        || type=="InternalInt"
                        || type=="InternalString"
                        || type=="AudioOutput")
                return true;

        return false;
}

void IO::reload()
{
        load();
}

void IO::load()
{
        string answer;
        TCPSocket *socket = SocketManager::Instance().get_socket(0);

        vector<string> tmp;
        vector<string> splitter;

        string input = "input";
        if(ioType==OUTPUT)
                input = "output";

        if(socket && SocketManager::Instance().Send(socket, input+" " + Utils::to_string(id) + " get"))
        {
                SocketManager::Instance().Recv(socket, answer, NETWORK_TIMEOUT);

                //cout<<"## "<<answer<<"\n";
                Utils::split(answer, splitter);
                for_each(splitter.begin(), splitter.end(), UrlDecode());
                for (uint b = 1;b < splitter.size();b++)
                {
                        tmp.clear();
                        Utils::split(splitter[b], tmp, ":");

                        if (tmp[0] == "name")
                                setName(tmp[1]);
                        else if(tmp[0] == "type")
                                setType(tmp[1]);
                        else if(tmp[0] == "var_type")
                                setVarType(tmp[1]);
                        else if(tmp[0] == "hits")
                                from_string(tmp[1],hits);
                        else if(tmp[0] == "gtype")
                                setGType(tmp[1]);
                        else if(tmp[0] == "visible" && tmp[1] == "true")
                                setVisible(true);
                        else if(tmp[0] == "visible" && tmp[1] == "false")
                                setVisible(false);
                        else if(tmp[0] == "state")
                                if(tmp.size()>1)
                                        setLocalState(tmp[1]);
                                else
                                        setLocalState("");
                        else if(tmp[0] == "chauffage_id")
                                from_string(tmp[1],chauffId);
                        else if(tmp[0] == "rw")
                        {
                                if(tmp[1] == "true")
                                        editable = true;
                                else
                                        editable = false;
                        }
                        else if(tmp[0] == "ioBoolState")
                                scenarioBoolStateID = tmp[1];
                        else if(tmp[0] == "scenarioID")
                                booleanScenarioID = tmp[1];
                        else if(tmp[0] == "scenarioPref")
                                from_string( tmp[1],scenarioPref);
                }
        }
}

