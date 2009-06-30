//
//  RoomDetailsController.m
//  CalaosHome
//
//  Created by Raoul on 29/06/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "RoomDetailsController.h"


@implementation RoomDetailsController

-(id)initWithFrame:(CGRect)frame 
{
	self = [ super init ];
	if (self != nil) 
        {
                tableView = [[UITableView alloc] initWithFrame:frame style: UITableViewStylePlain];
                [tableView setDelegate:self];
                [tableView setDataSource:self];
                //	[tableView setBackgroundColor:[UIColor colorWithRed:1.0f green:1.0f blue:1.0f alpha:0.35f]];
                [self.view addSubview:tableView];
                [tableView release];
                
                self.view.frame = frame;
                self.view.backgroundColor = [ UIColor redColor ];
                tableView.backgroundColor = [ UIColor blackColor ];
	}
	return self;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView 
{
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section 
{
	return [[UIFont	familyNames] count];
}

- (UITableViewCell *)tableView:(UITableView *)tView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	// Use re-usable cells to minimize the memory load
	UITableViewCell *cell = [tView dequeueReusableCellWithIdentifier:@"any-cell"];
	if (!cell) cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:@"any-cell"] autorelease];
	
	// Set up the cell's text
	cell.text = [[UIFont familyNames] objectAtIndex:[indexPath row]];
	return cell;
}

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

// Implement loadView to create a view hierarchy programmatically, without using a nib.
/*- (void)loadView 
{
	     
}*/


/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
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
