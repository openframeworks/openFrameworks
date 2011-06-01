//
//  MyGuiView.m
//  iPhone Empty Example
//
//  Created by theo on 26/01/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MyGuiView.h"
#include "ofxiPhoneExtras.h"


@implementation MyGuiView

// called automatically after the view is loaded, can be treated like the constructor or setup() of this class
-(void)viewDidLoad {
	myApp = (testApp*)ofGetAppPtr();
}

//----------------------------------------------------------------
-(void)setStatusString:(NSString *)trackStr{
	displayText.text = trackStr;
}

//----------------------------------------------------------------
-(IBAction)more:(id)sender{
	myApp->lengthRatio += 0.1;
	
	string statusStr = " Status: ratio is " + ofToString(myApp->lengthRatio, 2);
	[self setStatusString:ofxStringToNSString(statusStr)];		
}

//----------------------------------------------------------------
-(IBAction)less:(id)sender{
	myApp->lengthRatio -= 0.1;
	if( myApp->lengthRatio < 0.1 ){
		myApp->lengthRatio = 0.1;
	}

	string statusStr = " Status: ratio is " + ofToString(myApp->lengthRatio, 2);
	[self setStatusString:ofxStringToNSString(statusStr)];		
}

//----------------------------------------------------------------
-(IBAction)hide:(id)sender{
	self.view.hidden = YES;
}

//----------------------------------------------------------------
-(IBAction)adjustPoints:(id)sender{
	
	UISlider * slider = sender;
	printf("slider value is - %f\n", [slider value]);
	
	myApp->numPoints = 3 + [slider value] * 28;
	
	string statusStr = " Status: numPoints is " + ofToString(myApp->numPoints);
	[self setStatusString:ofxStringToNSString(statusStr)];
	
}

//----------------------------------------------------------------
-(IBAction)fillSwitch:(id)sender{
	
	UISwitch * toggle = sender;
	printf("switch value is - %i\n", [toggle isOn]);
	
	myApp->bFill = [toggle isOn];
	
	string statusStr = " Status: fill is " + ofToString(myApp->bFill);
	[self setStatusString:ofxStringToNSString(statusStr)];	
}

@end
