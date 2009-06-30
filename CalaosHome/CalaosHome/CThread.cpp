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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "CThread.h"
#include "ObjCWrapper.h"
//---------------------------------------------------------------------------
CThread::CThread() : started(false), stopped(false)
{
        th = 0;
}
//---------------------------------------------------------------------------
CThread::~CThread()
{
        stopped = true;
        End();
}
//---------------------------------------------------------------------------
static void *CThreadProc(void *p)
{
        CThread *pThis = (CThread *)p;
        pThis->started = true;

        //if (pThis->stopped) return NULL;
        pThis->ThreadProc_objc();

        pthread_exit(0);
}
//---------------------------------------------------------------------------
void CThread::ThreadProc()
{
        return;
}
//---------------------------------------------------------------------------
void CThread::ThreadProc_objc()
{
        call_thread_with_objcpool(this);
}
//---------------------------------------------------------------------------
void CThread::Start()
{
        if (th)
                End(); // pour etre sur.

        // Demarre le thread
        if (pthread_create(&th, NULL, CThreadProc, (void *)this) != 0)
                cout << "CThread::Start(), pthread_create() error..." << endl;
}
//---------------------------------------------------------------------------
void CThread::KillThread()
{
        if(th)
            pthread_kill(th, SIGKILL);
        th = 0;
}
//---------------------------------------------------------------------------
void CThread::End()
{
        stopped = true;
        if (th)
        {
                cout << "CThread::End(), pthread_join() waiting for thread to finish his job..." << endl;
                pthread_join(th, NULL);
        }

        started = false;
        th = 0;
}
//---------------------------------------------------------------------------
