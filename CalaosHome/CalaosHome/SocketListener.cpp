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

#include "SocketListener.h"

using namespace CalaosNetwork;

SocketListener::SocketListener(string _address, int _port):address(_address),port(_port),stopThread(false)
{
        socket = SocketManager::Instance().Add(address, port, CALAOS);

        SocketManager::Instance().Send(socket, "listen", false);

        Start();
}

SocketListener::~SocketListener()
{
        threadClose();
}

void SocketListener::processMessage(string msg)
{
        vector<string> msgSplit;

        //cout<<"##"<<msg<<"\n";
        split(msg,msgSplit," ");

        if(msgSplit[0] == "output" || msgSplit[0] == "input")
                IPC::Instance().SendEvent("CalaosNetwork::Notify,io,change",msg,NULL);
        else if(msgSplit[0] == "new_output" || msgSplit[0] == "new_input")
                IPC::Instance().SendEvent("CalaosNetwork::Notify,io,new",msg,NULL);
        else if(msgSplit[0] == "delete_output" || msgSplit[0] == "delete_input")
                IPC::Instance().SendEvent("CalaosNetwork::Notify,io,del",msg,NULL);
        else if(msgSplit[0] == "modify_room")
                IPC::Instance().SendEvent("CalaosNetwork::Notify,room,modify",msg,NULL);
        else if(msgSplit[0] == "delete_room")
                IPC::Instance().SendEvent("CalaosNetwork::Notify,room,del",msg,NULL);
        else if(msgSplit[0] == "new_room")
                IPC::Instance().SendEvent("CalaosNetwork::Notify,room,new",msg,NULL);
}

void SocketListener::ThreadProc()
{
        while( 1 )
        {
                string msg;
                bool res = SocketManager::Instance().Recv(socket, msg, 0, false);

                if(stopThread)
                        break;
                if(!res)
                {
                        //wait some time before trying to reconnect
                        for (int i = 0;i < 10;i++)
                        {
                                struct timespec t;
                                t.tv_sec = 0;
                                t.tv_nsec = 100000000;
                                nanosleep(&t, NULL);

                                if (stopThread)
                                        return;
                        }

                        cout << "SocketListener: Trying to reconnect..." << endl;

                        if (SocketManager::Instance().Send(socket, "listen", false))
                        {
                                cout << "SocketListener: Reconnection successfull" << endl;

                                //Reload everything if a reconnection occured
                                IPC::Instance().SendEvent("CalaosNetwork::Notify", "reconnect", NULL);
                        }
                }
                else
                {
                        vector<string> msgSplit;
                        vector<string>::iterator it;

                        split(msg,msgSplit,"\n");
                        for(it = msgSplit.begin(); it != msgSplit.end(); it++)
                                processMessage(*it);
                }
        }
}

void SocketListener::threadClose()
{
        stopThread = true;
        SocketManager::Instance().Connect(socket);
        End(); //wait for the thread
}
