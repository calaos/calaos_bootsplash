//
//  LoginViewController.h
//  CalaosHome
//
//  Created by Raoul on 23/06/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#include "IPC.h"
/*#ifdef nil 
#undef nil 
#endif*/
#import <UIKit/UIKit.h>
#import "Wrapper.h"

@interface LoginViewController : UIViewController 
{
        IBOutlet UITextField *editLogin;
        IBOutlet UITextField *editPassword;
        
        BOOL shown;
}

@property(nonatomic,retain) IBOutlet UITextField *editLogin;
@property(nonatomic,retain) IBOutlet UITextField *editPassword;

- (IBAction) btLogin:(id) sender;

- (void) showView;
- (void) hideView;

@end
