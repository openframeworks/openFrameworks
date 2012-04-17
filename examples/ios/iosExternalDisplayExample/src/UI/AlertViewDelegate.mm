//
//  AlertViewDelegate.m
//  emptyExample
//
//  Created by lukasz karluk on 22/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "AlertViewDelegate.h"
#import "ofxiPhoneExternalDisplay.h"
#import "testApp.h"

@implementation AlertViewDelegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex{

    if(buttonIndex == 0){
        // do nothing.
    } else if(buttonIndex == 1){
        ofxiPhoneExternalDisplay::displayOnExternalScreenWithPreferredDisplayMode();
    } else {
        vector<ofxiPhoneExternalDisplayMode> displayModes;
        displayModes = ofxiPhoneExternalDisplay::getExternalDisplayModes();
        
        if(displayModes.size()==0){
            return; // no display modes found.
        }
        
        int i = buttonIndex - 2;
        ofxiPhoneExternalDisplay::displayOnExternalScreen(displayModes[i]);
    }
    
    ((testApp *)ofGetAppPtr())->popupDismissed();
}

@end
