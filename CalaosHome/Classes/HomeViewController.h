//
//  FirstViewController.h
//  CalaosHome
//
//  Created by Raoul on 16/06/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PageScrollView.h"
#include "RoomCocoa.h"
#include "Rooms.h"
#import "RoomDetailsController.h"
#import "RoomViewController.h"

@interface HomeViewController : UIViewController <RoomViewControllerDelegate>
{
        IBOutlet UIView *mainView;
        IBOutlet UINavigationItem *navbar;
        
        NSMutableArray *pages;
	PageScrollView *scrollView;
        
        RoomDetailsController *roomDetailsView;
}

@property(nonatomic,retain) IBOutlet UIView *mainView;
@property(nonatomic,retain) IBOutlet UINavigationItem *navbar;

-(void) pageScrollViewDidChangeCurrentPage:(PageScrollView *)pageScrollView currentPage:(int)currentPage;

/* called by the IPC when all rooms are loaded */
- (void) loadRooms;

- (void) showRoomDetails;
- (void) showRoomList:(id) sender;

@end
