#import "ShootStuffUpViewController.h"

@implementation ShootStuffUpViewController

- (id) init {
    self = [ super initWithStyle: UITableViewStylePlain ];
	
	if (self != nil) {
		self.title = @"Game Settings";
	}
	return self;
}

- (void) loadView {
	[ super loadView ];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {

    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)didReceiveMemoryWarning {

    [ super didReceiveMemoryWarning ];
}

- (void)dealloc {
	[ musicVolumeControl release ];
	[ gameVolumeControl release ];
	[ difficultyControl release ];
	[ shipStabilityControl release ];
	[ badGuyControl release ];
	[ debugControl release ];
	[ versionControl release ];
    [ super dealloc ];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 3;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    switch (section) {
        case(0):
			return 3;
			break;
		case(1):
			return 3;
			break;
		case(2):
			return 1;
			break;
	}
	
	return 0;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section { 
	switch (section) {
		case(0):
			return @"Game Settings";
			break;
		case(1):
			return @"Advanced Settings";
			break;
		case(2):
			return @"About";
			break;
	}
	return nil;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	NSString *CellIdentifier = [ NSString stringWithFormat: @"%d:%d", [ indexPath indexAtPosition: 0 ],
								[ indexPath indexAtPosition:1 ] ];
	
    UITableViewCell *cell = [ tableView dequeueReusableCellWithIdentifier: CellIdentifier ];
    if (cell == nil) {
        cell = [ [ [ UITableViewCell alloc ] initWithFrame: CGRectZero reuseIdentifier: CellIdentifier ] autorelease ];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
		
		switch ([ indexPath indexAtPosition: 0]) {
			case(0):
				switch([ indexPath indexAtPosition: 1]) {
					case(0):
						musicVolumeControl = [ [ UISlider alloc ] initWithFrame: CGRectMake(170.0, 0.0, 125.0, 50.0) ];
						musicVolumeControl.minimumValue = 0.0;
						musicVolumeControl.maximumValue = 10.0;
						musicVolumeControl.value = 3.5;
						[ cell addSubview: musicVolumeControl ];
						cell.text = @"Music Volume";
						break;
					case(1):
						gameVolumeControl = [ [ UISlider alloc ] initWithFrame: CGRectMake(170.0, 0.0, 125.0, 50.0) ];
						gameVolumeControl.minimumValue = 0.0;
						gameVolumeControl.maximumValue = 10.0;
						gameVolumeControl.value = 3.5;
						[ cell addSubview: gameVolumeControl ];
						cell.text = @"Game Volume";
						break;
					case(2):
						difficultyControl = [ [ UISegmentedControl alloc ] initWithFrame: CGRectMake(170.0, 5.0, 125.0, 35.0) ];
						[ difficultyControl insertSegmentWithTitle: @"Easy" atIndex: 0 animated: YES ];
						[ difficultyControl insertSegmentWithTitle: @"Hard" atIndex: 1 animated: YES ];
					    difficultyControl.selectedSegmentIndex = 0;
						[ cell addSubview: difficultyControl ];
						cell.text = @"Difficulty";
						break;
				}
				break;
			case(1):
				switch ([ indexPath indexAtPosition: 1 ]) {
					case(0):
						shipStabilityControl = [ [ UISlider alloc ] initWithFrame: CGRectMake(170.0, 0.0, 125.0, 50.0) ];
						shipStabilityControl.minimumValue = 0.0;
						shipStabilityControl.maximumValue = 10.0;
						shipStabilityControl.value = 3.5;
						[ cell addSubview: shipStabilityControl ];
						cell.text = @"Ship Stability";
						break;
					case(1):
						badGuyControl = [ [ UISwitch alloc ] initWithFrame: CGRectMake(200.0, 10.0, 0.0, 0.0) ];
						badGuyControl.on = YES;
						[ cell addSubview: badGuyControl ];
						cell.text = @"Bad Guys";
						break;
					case(2):
						debugControl = [ [ UISwitch alloc ] initWithFrame: CGRectMake(200.0, 10.0, 0.0, 0.0) ];
						debugControl.on = NO;
						[ cell addSubview: debugControl ];
						cell.text = @"Debug";
						break;	
				}
				break;
			case(2):
				versionControl = [ [ UITextField alloc ] initWithFrame: CGRectMake(170.0, 10.0, 125.0, 38.0) ];
				versionControl.text = @"1.0.0 Rev. B";
				[ cell addSubview: versionControl ];
				[ versionControl setEnabled: NO ];
				cell.text = @"Version";
				break;
		}
	}
	
    return cell;
}

@end
