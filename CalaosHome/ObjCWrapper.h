/*
 *  ObjCWrapper.h
 *  CalaosHome
 *
 *  Created by Raoul on 17/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OBJC_WRAPPER_H
#define _OBJC_WRAPPER_H

#include "CThread.h"

extern void calaos_broadcast_ipc(void);
extern void call_thread_with_objcpool(CThread *thread);
extern void cocoa_send_notification(std::string source, std::string emission);

#endif
