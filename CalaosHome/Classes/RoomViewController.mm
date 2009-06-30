//
//  RoomViewController.m
//  CalaosHome
//
//  Created by Raoul on 24/06/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "RoomViewController.h"
#import <iostream>

@implementation RoomViewController

@synthesize roomImage;
@synthesize roomNameLabel;
@synthesize roomTempLabel;
@synthesize roomConsigneLabel;
@synthesize delegate;

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
        if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) 
        {
                // Custom initialization
                std::cout << self << "  -  " << roomNameLabel << std::endl;
        }
        return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
        [super viewDidLoad];
        
        roomNameLabel.text = roomName;
        roomTempLabel.text = roomTemp;
        roomConsigneLabel.text = roomConsigne;
        roomTempLabel.hidden = hasHeat;
        roomConsigneLabel.hidden = hasHeat;
        
        [ self setRoomType: roomType ];
}

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

- (IBAction) btMore:(id) sender
{
        NSLog(@"Show more");
        
        // If delegate was set make the callback
	if (delegate != nil && [ delegate respondsToSelector: @selector(moreButtonClicked) ]) 
        {
		[ delegate moreButtonClicked ];
	}
}

- (void) setRoomName:(NSString *) name
{
        roomName = name;
        roomNameLabel.text = roomName;
}

- (void) setRoomType:(NSString *) _roomtype
{
        roomType = _roomtype;
        
        if ([roomType isEqualToString: @"salon"]) roomType = @"lounge";
        if ([roomType isEqualToString: @"chambre"]) roomType = @"bedroom";
        if ([roomType isEqualToString: @"cuisine"]) roomType = @"kitchen";
        if ([roomType isEqualToString: @"bureau"]) roomType = @"office";
        if ([roomType isEqualToString: @"sam"]) roomType = @"diningroom";
        if ([roomType isEqualToString: @"cave"]) roomType = @"cellar";
        if ([roomType isEqualToString: @"divers"]) roomType = @"various";
        if ([roomType isEqualToString: @"exterieur"]) roomType = @"outside";
        if ([roomType isEqualToString: @"sdb"]) roomType = @"bathroom";
        if ([roomType isEqualToString: @"hall"]) roomType = @"corridor";
        if ([roomType isEqualToString: @"couloir"]) roomType = @"corridor";
        
        NSString *file = [ NSString stringWithFormat: @"room_%@.png", roomType ];
        UIImage *image = [ UIImage imageNamed: file ];
        
        if (image == nil)
                image = [ UIImage imageNamed: @"room.png" ];
        
        roomImage.image = image;
}

- (void) setRoomHeat:(NSString *) temp andConsigne:(NSString *) consigne
{
        roomTemp = temp;
        roomConsigne = consigne;
        roomTempLabel.text = roomTemp;
        roomConsigneLabel.text = roomConsigne;
}

- (void) showHeat:(BOOL) visible
{
        hasHeat = visible;
        roomTempLabel.hidden = hasHeat;
        roomConsigneLabel.hidden = hasHeat;
}

- (void)dealloc 
{
        [super dealloc];
}


@end
