//
//  main.m
//  CalaosHome
//
//  Created by Raoul on 16/06/09.
//  Copyright Calaos 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "IPCWrapperNotification.h"
#import "Wrapper.h"

int main(int argc, char *argv[]) 
{
        NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
        IPCWrapperNotification *ipc = [[IPCWrapperNotification alloc] init];
        [ipc registerForIPC];
	
        //createMyHome();
        calaos_connect_home();
	
        int retVal = UIApplicationMain(argc, argv, nil, nil);
	
	[ipc release];
        [pool release];
        return retVal;
}

