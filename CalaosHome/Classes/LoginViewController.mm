//
//  LoginViewController.m
//  CalaosHome
//
//  Created by Raoul on 23/06/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "LoginViewController.h"
#import <QuartzCore/CAAnimation.h>

@implementation LoginViewController

@synthesize editLogin;
@synthesize editPassword;

// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
        if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) 
        {
                shown = NO;
        }
        return self;
}

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
        [super viewDidLoad];
        shown = NO;
        
        editLogin.text = @"showroom";
        editPassword.text = @"pass";
}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
//    return (interfaceOrientation == UIInterfaceOrientationPortrait);
        return YES;
}
*/

- (void)didReceiveMemoryWarning 
{
        [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
        // Release anything that's not essential, such as cached data
}


- (void)dealloc {
    [super dealloc];
}

- (IBAction) btLogin:(id) sender
{
        NSLog(@"Login button click");
        //Here we set up the user/pass for CalaosHome
        
        calaos_set_username([[editLogin text] UTF8String]);
        calaos_set_password([[editPassword text] UTF8String]);

        IPC::Instance().SendEvent("CalaosHome::Notify", "reload,all");
        
        [self hideView];
}

- (BOOL)textFieldShouldReturn:(UITextField *)theTextField 
{
	if (theTextField == editLogin)
                [editPassword becomeFirstResponder];
        else
                [theTextField resignFirstResponder];
	
        return YES;
}

- (void) showView
{
        if (shown) return;
        shown = YES;
        
        CGRect rect = CGRectMake(0.0f, -480.0f, 320.0f, 480.0f);
        
        self.view.frame = rect;
        [self.view setAlpha:1.0];
        
        [ UIView beginAnimations: nil context: nil ];
        [ UIView setAnimationCurve: UIViewAnimationCurveEaseOut ];
        [ UIView setAnimationDuration: 0.6f ];
        
        [self.view setAlpha:1.0];
        self.view.frame = CGRectMake(0.0f, 0.0f, 320.0f, 480.0f);
        
        [ UIView commitAnimations ];
}

- (void) hideView
{
//        if (!shown) return;
        shown = NO;
        
        CGRect rect = CGRectMake(0.0f, 0.0f, 320.0f, 480.0f);
        
        self.view.frame = rect;
        [self.view setAlpha:1.0];
        
        [ UIView beginAnimations: nil context: nil ];
        [ UIView setAnimationCurve: UIViewAnimationCurveEaseIn ];
        [ UIView setAnimationDuration: 0.6f ];
        
        [self.view setAlpha:0.5];
        self.view.frame = CGRectMake(0.0f, -480.0f, 320.0f, 480.0f);
        
        [ UIView commitAnimations ];
}

@end
