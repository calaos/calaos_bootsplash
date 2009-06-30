#import <UIKit/UIKit.h>

@interface ShootStuffUpViewController : UITableViewController {

	UISlider *musicVolumeControl;
	UISlider *gameVolumeControl;
	UISegmentedControl *difficultyControl;
	
	UISlider *shipStabilityControl;
	UISwitch *badGuyControl;
	UISwitch *debugControl;
	
	UITextField *versionControl;
}

- (id) init;
- (void) dealloc;
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView;
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section;
- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section;
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath;

@end

