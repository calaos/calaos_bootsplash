/*
 *  Wrapper.cpp
 *  CalaosHome
 *
 *  Created by Raoul on 17/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Wrapper.h"
#import "SocketManager.h"
#import "SocketListener.h"
#import "Rooms.h"
#import "IPC.h"

using namespace CalaosHome;
using namespace CalaosNetwork;


void createMyHome(void)
{
	//Utils::set_config_option("calaos_user", "showroom");
	//Utils::set_config_option("calaos_password", "pass");

	std::string opt_host = "10.0.0.101";
	SocketManager::Instance().Add(opt_host, 4456, CALAOS);
	SocketManager::Instance().Add(opt_host, 4456, CALAOS); //dedicated to the audio player
	SocketManager::Instance().Add(opt_host, 4456, CALAOS); //dedicated to the playlist
	SocketListener socketListener(opt_host, 4456);
	
	Rooms::instance().init("RoomCocoa","IO");
}

void calaos_connect_home(void)
{
        //Utils::set_config_option("calaos_user", "showroom");
        //Utils::set_config_option("calaos_password", "pass");
        
        std::string opt_host = "10.0.0.101";
	SocketManager::Instance().Add(opt_host, 4456, CALAOS);
	SocketManager::Instance().Add(opt_host, 4456, CALAOS); //dedicated to the audio player
	SocketManager::Instance().Add(opt_host, 4456, CALAOS); //dedicated to the playlist
	SocketListener socketListener(opt_host, 4456);
	
	Rooms::instance().init("RoomCocoa","IO");
}

void calaos_handle_ipc(void)
{
	IPC::Instance().BroadcastEvent();
}

void calaos_set_username(const char *str)
{
        Utils::set_config_option("calaos_user", str);
}

void calaos_set_password(const char *str)
{
        Utils::set_config_option("calaos_password", str);
}

void calaos_send_ipc(char *source, char *signal)
{
        IPC::Instance().SendEvent(string(source), string(signal));
}
