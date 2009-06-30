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

#ifndef  SOCKETLISTENER_INC
#define  SOCKETLISTENER_INC

#include "SocketManager.h"

namespace CalaosNetwork
{

        class SocketListener: public CThread
        {
                private:
                        string address;
                        int port;
                        TCPSocket * socket;

                        bool stopThread;
                public:
                        SocketListener(string address, int port);
                        ~SocketListener();
                        virtual void ThreadProc();
                        void threadClose();

                        void processMessage(string msg);
        };

}

#endif   /* ----- #ifndef SOCKETLISTENER_INC  ----- */

