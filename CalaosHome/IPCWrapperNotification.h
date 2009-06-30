//
//  IPCWrapperNotification.h
//  CalaosHome
//
//  Created by Raoul on 17/06/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Wrapper.h"
#import "NotificationObject.h"

@interface IPCWrapperNotification : NSObject {

}

+ (void)sendIPC_Notification;
- (void)registerForIPC;

@end
