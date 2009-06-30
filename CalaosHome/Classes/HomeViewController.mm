//
//  FirstViewController.m
//  CalaosHome
//
//  Created by Raoul on 16/06/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "HomeViewController.h"

using namespace CalaosHome;

@implementation HomeViewController

@synthesize mainView, navbar;

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
        NSLog(@"fuck de merde");
        if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) 
        {
        }
        return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView 
{
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
        [super viewDidLoad];
        
	pages = [ [ NSMutableArray alloc ] init ];
        
        CGRect frame = [ [ UIScreen mainScreen ] applicationFrame ];
        frame.size.height = 431;
	frame.origin.y = 21;
	scrollView = [ [ PageScrollView alloc ] initWithFrame: frame ];

	scrollView.delegate = self;
        scrollView.showsPageControl = YES;
        [ self.view addSubview: scrollView ];
        
        frame.size.height = 370;
        roomDetailsView = [ [ RoomDetailsController alloc ] initWithFrame: frame ];
}

- (void) moreButtonClicked
{
        [ self showRoomDetails ];
}

- (void) loadRooms
{
        [ pages removeAllObjects ];
        
        list<Room*> &lRooms = Rooms::instance().getRooms();
        list<Room*>::iterator it;
        
        for(it = lRooms.begin();it != lRooms.end();it++)
        {
                RoomCocoa *r = dynamic_cast<RoomCocoa*>(*it);
                
                if (!r) continue;
                if (r->getType() == "Internal") continue; //ignore special rooms
                
                RoomViewController *roomView = [[RoomViewController alloc]
                                                initWithNibName:@"RoomView" bundle:[NSBundle mainBundle]];
                roomView.delegate = self;
                
                r->setCocoaObject(roomView);
                
                cout << "roomView: " << roomView << "   r->getObject(): " << r->getCocoaObject() << endl;
                
                [ pages addObject: roomView ];
                
                [ roomView autorelease ];
        }
        
        scrollView.pages = pages;
}

-(void) pageScrollViewDidChangeCurrentPage:(PageScrollView *)pageScrollView currentPage:(int)currentPage 
{
	NSLog(@"I'm now on page %d\n", currentPage);
}

- (void) showRoomDetails
{
        [ UIView beginAnimations: nil context: NULL ];
	[ UIView setAnimationDuration: 0.75 ];
        
        [ UIView setAnimationTransition: UIViewAnimationTransitionFlipFromLeft
                                forView: self.view cache: YES ];
         
        [ scrollView removeFromSuperview ];
        [ self.view addSubview: roomDetailsView.view ];
	
	[UIView commitAnimations];
        
        RoomViewController *room = (RoomViewController *)[ pages objectAtIndex: scrollView.currentPage ];
        navbar.title = room.roomNameLabel.text;
        navbar.leftBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemReply target:self action:@selector(showRoomList:)];
}

- (void) showRoomList:(id) sender
{
        [ UIView beginAnimations: nil context: NULL ];
	[ UIView setAnimationDuration: 0.75 ];
        
        [ UIView setAnimationTransition: UIViewAnimationTransitionFlipFromRight
                                forView: self.view cache: YES ];
        
        [ roomDetailsView.view removeFromSuperview ];
        [ self.view addSubview: scrollView ];
	
	[UIView commitAnimations];
        
        navbar.title = @"Home";
        [ navbar.leftBarButtonItem release ];
        navbar.leftBarButtonItem = nil;
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    //return (interfaceOrientation == UIInterfaceOrientationPortrait);
	return YES;
}
*/

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}



- (void)dealloc {
    [super dealloc];
}

@end
