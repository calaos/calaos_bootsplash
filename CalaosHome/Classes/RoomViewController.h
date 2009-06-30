//
//  RoomViewController.h
//  CalaosHome
//
//  Created by Raoul on 24/06/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface RoomViewController : UIViewController 
{
        IBOutlet UIImageView *roomImage;
        IBOutlet UILabel *roomNameLabel;
        IBOutlet UILabel *roomTempLabel;
        IBOutlet UILabel *roomConsigneLabel;
        
        NSString *roomName, *roomType, *roomTemp, *roomConsigne;
        BOOL hasHeat;
        id delegate;
}

@property(nonatomic,retain) IBOutlet UIImageView *roomImage;
@property(nonatomic,retain) IBOutlet UILabel *roomNameLabel;
@property(nonatomic,retain) IBOutlet UILabel *roomTempLabel;
@property(nonatomic,retain) IBOutlet UILabel *roomConsigneLabel;

@property (assign) id delegate;

- (IBAction) btMore:(id) sender;

- (void) setRoomName:(NSString *) name;
- (void) setRoomType:(NSString *) _roomtype;
- (void) setRoomHeat:(NSString *) temp andConsigne:(NSString *) consigne;
- (void) showHeat:(BOOL) visible;

@end

@protocol RoomViewControllerDelegate 

- (void) moreButtonClicked;

@end