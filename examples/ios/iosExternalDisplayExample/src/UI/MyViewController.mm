//
//  MyViewController.m
//  emptyExample
//
//  Created by lukasz karluk on 22/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MyViewController.h"
#import "ofxiOSExternalDisplay.h"

@implementation MyViewController

-(IBAction)displayOnDeviceScreenButtonPressed:(id)sender {
    ofxiOSExternalDisplay::displayOnDeviceScreen();
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsPortrait(interfaceOrientation);
}

@end
