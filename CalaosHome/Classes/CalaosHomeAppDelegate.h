//
//  CalaosHomeAppDelegate.h
//  CalaosHome
//
//  Created by Raoul on 16/06/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LoginViewController.h"
#import "NotificationObject.h"
#import "MBProgressHUD.h"

@interface CalaosHomeAppDelegate : NSObject <UIApplicationDelegate, UITabBarControllerDelegate> 
{
        UIWindow *window;
        UITabBarController *tabBarController;
        LoginViewController *loginViewController;
        MBProgressHUD *loadingHUD;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UITabBarController *tabBarController;
@property (nonatomic, retain) IBOutlet LoginViewController *loginViewController;

@end
