//
//  MyGuiView.m
//  assimpExample
//
//  Created by azuremous on 29/01/2024.

#import "MyGuiView.h"
#include "ofxiOSExtras.h"


@implementation MyGuiView

// called automatically after the view is loaded, can be treated like the constructor or setup() of this class
-(void)viewDidLoad {
	myApp = (ofApp*)ofGetAppPtr();
    // change view size for enable touch event on ofApp
    CGRect newFrame = self.view.frame;
    newFrame.size.width = self.view.frame.size.width;
    newFrame.size.height = 300;
    self.view.frame = newFrame;
    
    animationSlideView.hidden = true;
}

//----------------------------------------------------------------
-(void)setStatusString:(NSString *)trackStr{
	displayText.text = trackStr;
}

//----------------------------------------------------------------
-(IBAction)more:(id)sender{
    myApp->loadModel();
}

//----------------------------------------------------------------
-(IBAction)less:(id)sender{
    myApp->loadModel(true);
}

//----------------------------------------------------------------
-(IBAction)hide:(id)sender{
	self.view.hidden = YES;
}

//----------------------------------------------------------------
-(IBAction)animationPosition:(id)sender{
	UISlider * slider = sender;
    float val = [slider value];
    if(!myApp->bAnimate){
        myApp->updateAnimation(val);
    }
	
}

//----------------------------------------------------------------
-(IBAction)showNextAnimation:(id)sender{
    UIButton * button = sender;
    bool isSelected = [button isSelected];
    myApp->showNextAnimation();
}

//----------------------------------------------------------------
-(IBAction)toggleAnimation:(id)sender{
	UISwitch * toggle = sender;
    bool isOn = [toggle isOn];
    myApp->toggleAnimation(isOn);
    animationSlideView.hidden = isOn;
}

-(IBAction)toggleCamera:(id)sender{
    UISwitch * toggle = sender;
    bool isOn = [toggle isOn];
    myApp->toggleCamera(isOn);
}

@end
