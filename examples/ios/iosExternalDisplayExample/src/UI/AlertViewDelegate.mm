//
//  AlertViewDelegate.m
//  emptyExample
//
//  Created by lukasz karluk on 22/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "AlertViewDelegate.h"
#import "ofxiOSExternalDisplay.h"
#import "ofApp.h"

@implementation AlertViewDelegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex{

    if(buttonIndex == 0){
        // do nothing.
    } else if(buttonIndex == 1){
        ofxiOSExternalDisplay::displayOnExternalScreenWithPreferredDisplayMode();
    } else {
        vector<ofxiOSExternalDisplayMode> displayModes;
        displayModes = ofxiOSExternalDisplay::getExternalDisplayModes();
        
        if(displayModes.size()==0){
            return; // no display modes found.
        }
        
        int i = buttonIndex - 2;
        ofxiOSExternalDisplay::displayOnExternalScreen(displayModes[i]);
    }
    
    ((ofApp *)ofGetAppPtr())->popupDismissed();
}

@end
