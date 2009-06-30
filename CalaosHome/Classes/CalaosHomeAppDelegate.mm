//
//  CalaosHomeAppDelegate.m
//  CalaosHome
//
//  Created by Raoul on 16/06/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "CalaosHomeAppDelegate.h"
#import "HomeViewController.h"
#include "Rooms.h"

@implementation CalaosHomeAppDelegate

@synthesize window;
@synthesize tabBarController;
@synthesize loginViewController;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
        // Add the tab bar controller's current view as a subview of the window
        [window addSubview: tabBarController.view];
        
        loginViewController = [[LoginViewController alloc]
                                        initWithNibName:@"LoginView" bundle:[NSBundle mainBundle]];
        
        [window addSubview: loginViewController.view];

        //Add the loading hud
        loadingHUD = [ [ MBProgressHUD alloc ] initWithWindow: [ UIApplication sharedApplication ].keyWindow ];
        [window addSubview: loadingHUD];
        
        NSNotificationCenter *nc;
        nc = [NSNotificationCenter defaultCenter];
        [nc addObserver:self 
               selector:@selector(handleIPC_Notification:)
               name:@"Calaos-IPC-ObjC" 
               object:nil];
        
        [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleBlackOpaque;
        
        [window makeKeyAndVisible];
}

- (void)handleIPC_Notification: (NSNotification *)notif
{
        NotificationObject *obj = notif.object;

	if ([ [ obj getSource ] isEqualToString: @"wrongPassword" ] && [ [ obj getSignal ] isEqualToString: @"wrongPassword" ])
        {
                [ loadingHUD hideHUD ];
                [ loginViewController showView ];
        }
        else if ([ [ obj getSource ] isEqualToString: @"CalaosHome::Rooms" ] && [ [ obj getSignal ] isEqualToString: @"roomLoadDone" ])
        {
                HomeViewController *view = [tabBarController.viewControllers objectAtIndex:0];
                [ view loadRooms ];
                
                [ loadingHUD hideHUD ];                
        }
        else if ([ [ obj getSource ] isEqualToString: @"CalaosHome::Rooms" ] && [ [ obj getSignal ] isEqualToString: @"roomLoadStart" ])
        {
                loadingHUD.labelText = @"Chargement...";
                loadingHUD.detailsLabelText = @"Connexion en cours";
                
                [ loadingHUD showHUD ];
        }
        
}

/*
// Optional UITabBarControllerDelegate method
- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController {
}
*/

/*
// Optional UITabBarControllerDelegate method
- (void)tabBarController:(UITabBarController *)tabBarController didEndCustomizingViewControllers:(NSArray *)viewControllers changed:(BOOL)changed {
}
*/


- (void)dealloc 
{
        [ loadingHUD release ];
        [ loginViewController release ];
        [ tabBarController release ];
        [ window release ];
        [ super dealloc ];
}

@end

