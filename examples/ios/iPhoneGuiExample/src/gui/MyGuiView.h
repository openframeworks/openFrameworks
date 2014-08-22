//
//  MyGuiView.h
//  iPhone Empty Example
//
//  Created by theo on 26/01/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "ofApp.h"

@interface MyGuiView : UIViewController {
	IBOutlet UILabel *displayText;
	
	ofApp *myApp;		// points to our instance of ofApp
}

-(void)setStatusString:(NSString *)trackStr;

-(IBAction)adjustPoints:(id)sender;
-(IBAction)fillSwitch:(id)sender;

-(IBAction)more:(id)sender;
-(IBAction)less:(id)sender;

-(IBAction)hide:(id)sender;

@end
