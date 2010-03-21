//
//  MyGuiView.m
//  iPhone Empty Example
//
//  Created by theo on 26/01/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MyGuiView.h"
#include "ofxiPhoneExtras.h"

//these are defined in testApp.mm
extern int numPoints;
extern float lengthRatio;
extern bool bFill;

@implementation MyGuiView

//----------------------------------------------------------------
-(void)setStatusString:(NSString *)trackStr{
	displayText.text = trackStr;
}

//----------------------------------------------------------------
-(IBAction)more:(id)sender{
	lengthRatio += 0.1;
	
	string statusStr = " Status: ratio is " + ofToString(lengthRatio, 2);
	[self setStatusString:ofxStringToNSString(statusStr)];		
}

//----------------------------------------------------------------
-(IBAction)less:(id)sender{
	lengthRatio -= 0.1;
	if( lengthRatio < 0.1 ){
		lengthRatio = 0.1;
	}

	string statusStr = " Status: ratio is " + ofToString(lengthRatio, 2);
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
	
	numPoints = 3 + [slider value] * 28;
	
	string statusStr = " Status: numPoints is " + ofToString(numPoints);
	[self setStatusString:ofxStringToNSString(statusStr)];
	
}

//----------------------------------------------------------------
-(IBAction)fillSwitch:(id)sender{
	
	UISwitch * toggle = sender;
	printf("switch value is - %i\n", [toggle isOn]);
	
	bFill = [toggle isOn];
	
	string statusStr = " Status: fill is " + ofToString(bFill);
	[self setStatusString:ofxStringToNSString(statusStr)];	
}

@end
