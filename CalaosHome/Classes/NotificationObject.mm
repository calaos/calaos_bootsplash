//
//  NotificationObject.m
//  CalaosHome
//
//  Created by Raoul on 18/06/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "NotificationObject.h"


@implementation NotificationObject

- (id)initWithSource:(NSString *)_source andEmission:(NSString *)_emission
{
        if (![super init])
                return nil;
        
        source = _source;
        signal = _emission;
        
        return self;
}

+ (id) notificationObjectWithSource:(NSString *)_source andEmission:(NSString *)_emission
{
        id obj = [[[self class] alloc] initWithSource:_source
                                          andEmission:_emission];
        
        return [obj autorelease];
}

- (void)dealloc
{
        [super dealloc];
}

- (NSString *)getSource
{
        return source;
}

- (NSString *)getSignal
{
        return signal;
}

@end
