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
#include "IPC.h"
#import "ObjCWrapper.h"
//-----------------------------------------------------------------------------
/*
static int _calaos_ipc_event(void *data, Ecore_Fd_Handler *fdh)
{
        //We got something from the pipe
        IPC::Instance().BroadcastEvent();

        return 1;
}
*/
//-----------------------------------------------------------------------------
IPC::IPC(): mutex(false)
{
/*
        int fds[2];

        if (pipe(fds) == 0)
        {
                fd_read = fds[0];
                fd_write = fds[1];
                //set the read pipe to non blocking
                fcntl(fd_read, F_SETFL, O_NONBLOCK);

                //add to ecore
                fd_handler = ecore_main_fd_handler_add(fd_read, ECORE_FD_READ,
                                _calaos_ipc_event, NULL,
                                NULL, NULL);
                ecore_main_fd_handler_active_set(fd_handler, ECORE_FD_READ);
        }
        else
        {
                cout << "IPC::IPC(): Error creating pipe !" << endl;
        }
*/
}
//-----------------------------------------------------------------------------
IPC::~IPC()
{
//        close(fd_read);
//        close(fd_write);
}
//-----------------------------------------------------------------------------
void IPC::AddHandler(string source, string emission, sigc::signal<void, std::string, std::string, void*, void*> &signal, void *data)
{
        IPCSignal s;
        s.source = source;
        s.emission = emission;
        s.data = data;
        s.signal = &signal;

        signals.push_back(s);
}
//-----------------------------------------------------------------------------
void IPC::DeleteHandler(sigc::signal<void, std::string, std::string, void*, void*> &signal)
{
        list<IPCSignal>::iterator it;

        for(it=signals.begin();it!=signals.end();it++)
        {
                if ((*it).signal == &signal)
                {
                        signals.erase(it);
                        break;
                }
        }
}
//-----------------------------------------------------------------------------
void IPC::SendEvent(string source, string emission, void* data)
{
        IPCMsg msg;
        msg.source = source;
        msg.emission = emission;
        msg.data = data;
        msg.auto_delete = false;

        mutex.lock();
        events.push_back(msg);
        mutex.unlock();

        calaos_broadcast_ipc();
	
        /* The mutex2 slows down everything, it's faster to write everytime to the pipe */
//         if (mutex2.try_lock())
//         {
//                 mutex2.unlock();

                //wake up the listener
                //string s = "wake_up";
                //if (write(fd_write, s.c_str(), s.length()) < 0)
                //{
                //        cout << "IPC::IPC(): Error writing to pipe !" << endl;
                //}
//         }
}
//-----------------------------------------------------------------------------
void IPC::SendEvent(string source, string emission, IPCData data, bool auto_delete_data)
{
        IPCMsg msg;
        msg.source = source;
        msg.emission = emission;
        msg.data = data.data;
        msg.auto_delete = auto_delete_data;
        msg.del_data = data;

        mutex.lock();
        events.push_back(msg);
        mutex.unlock();
	
        calaos_broadcast_ipc();

        /* The mutex2 slows down everything, it's faster to write everytime to the pipe */
//         if (mutex2.try_lock())
//         {
//                 mutex2.unlock();

                //wake up the listener
                //string s = "wake_up";
                //if (write(fd_write, s.c_str(), s.length()) < 0)
                //{
                //        cout << "IPC::IPC(): Error writing to pipe !" << endl;
                //}
//         }
}
//-----------------------------------------------------------------------------
void IPC::BroadcastEvent()
{
        char evname[MAX_EVENT_NAME];
        memset(evname, '\0', MAX_EVENT_NAME);

        //if (read(fd_read, evname, MAX_EVENT_NAME) > 0)
        {
                /* The mutex2 slows down everything, it's faster to write everytime to the pipe */
//                mutex2.lock();
                while(events.size() > 0)
                {
                        /*
                           Never remove an element from events outside of this block !
                           Otherwise the events.begin iterator can become invalid
                           (if there is no element in the list)
                        */
                        mutex.lock();
                        list<IPCMsg>::iterator itd = events.begin();
                        if(itd == events.end())
                                break;
                        IPCMsg msg = (*itd);
                        events.erase(itd);
                        mutex.unlock();

                        cout << "IPC::BroadcastEvent(\"" <<
                                msg.source<<"\"  ,  \""<<msg.emission<< "\" , " <<to_string(msg.data)+")" << endl;
                        list<IPCSignal>::iterator it;
                        //we work on a copy to be sure we doesn't have a conflict
                        //sometimes the method call by signal->emit
                        //   call deleteHandler on the current handler
                        mutex.lock();
                        list<IPCSignal> signalsCopy = signals;
                        mutex.unlock();
                        for(it=signalsCopy.begin();it!=signalsCopy.end();it++)
                        {
                                if ( (msg.source == (*it).source || (*it).source == "*")
                                && (msg.emission == (*it).emission || (*it).emission == "*"))
                                {
                                        (*it).signal->emit(msg.source, msg.emission,
                                                        (*it).data, msg.data);
                                }
                        }

                        //Delete data if flag is set
                        if (msg.auto_delete)
                        {
                                if (msg.data && msg.del_data.destroy)
                                {
                                        (*msg.del_data.destroy)(msg.data);
                                        delete msg.del_data.destroy;
                                }
                                else
                                {
                                        cout << "IPC::BroadcastEvent(\"" <<
                                                msg.source<<"\"  ,  \""<<msg.emission<< "\") " <<
                                                "DeletorBase or data is NULL, can't delete. (Memory leak?)" << endl;
                                }
                        }
                        
                        //Here we also send the IPC message to the Cocoa framework, so UI can react on them
                        cocoa_send_notification(msg.source, msg.emission);
                }
//                 mutex2.unlock();
        }
}
//-----------------------------------------------------------------------------
