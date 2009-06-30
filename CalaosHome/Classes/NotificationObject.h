//
//  NotificationObject.h
//  CalaosHome
//
//  Created by Raoul on 18/06/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface NotificationObject : NSObject 
{
        NSString *source;
        NSString *signal;
}

+ (id) notificationObjectWithSource:(NSString *)_source andEmission:(NSString *)_emission;

- (NSString *)getSource;
- (NSString *)getSignal;

@end
