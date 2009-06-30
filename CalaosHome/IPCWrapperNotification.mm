//
//  IPCWrapperNotification.mm
//  CalaosHome
//
//  Created by Raoul on 17/06/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "IPCWrapperNotification.h"
#import "CThread.h"

@implementation IPCWrapperNotification

- (void)registerForIPC
{
    NSNotificationCenter *nc;
    nc = [NSNotificationCenter defaultCenter];
    
    [nc addObserver:self 
        selector:@selector(handleIPC_Notification:)
        name:@"Calaos-IPC" 
        object:nil];
}

+ (void)sendIPC_Notification
{
	NSString *notificationName = @"Calaos-IPC";
	/*
	NSNotification *notif = [NSNotification notificationWithName:notificationName object:nil];
	[notif autorelease];
	
	[[NSNotificationQueue defaultQueue] enqueueNotification:notif
				postingStyle:NSPostASAP
                                coalesceMask:NSNotificationNoCoalescing
                                forModes:nil];
        */
        [[NSNotificationCenter defaultCenter] postNotificationName:notificationName object:nil];
}

- (void)handleIPC_Notification: (NSNotification *)notif
{
	calaos_handle_ipc();
}

@end

void calaos_broadcast_ipc(void)
{
	[IPCWrapperNotification sendIPC_Notification];
}

void call_thread_with_objcpool(CThread *thread)
{
        NSAutoreleasePool *autoreleasepool= [[NSAutoreleasePool alloc] init];
        
        thread->ThreadProc();
        
        [autoreleasepool release];
}

void cocoa_send_notification(std::string source, std::string emission)
{
        NSString *notificationName = @"Calaos-IPC-ObjC";
        
        NotificationObject *obj = [NotificationObject notificationObjectWithSource:[NSString stringWithCString:source.c_str()] andEmission:[NSString stringWithCString:emission.c_str()]];
        
        [[NSNotificationCenter defaultCenter] postNotificationName:notificationName object:obj];
}

