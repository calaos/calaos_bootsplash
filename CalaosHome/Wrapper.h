/*
 *  Wrapper.h
 *  CalaosHome
 *
 *  Created by Raoul on 17/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _WRAPPER_H
#define _WRAPPER_H

void createMyHome(void);

void calaos_connect_home(void);
void calaos_handle_ipc(void);

void calaos_set_username(const char *str);
void calaos_set_password(const char *str);

void calaos_send_ipc(char *source, char *signal);

#endif
