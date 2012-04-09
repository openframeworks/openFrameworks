//
//  ofxiPhoneExternalDisplay.cpp
//
//  Created by lukasz karluk on 22/03/12.
//  http://julapy.com
//

#include "ofxiPhoneExternalDisplay.h"
#include "ofxiPhoneAppDelegate.h"

//-----------------------------------------------------------------------------------------
static vector<ofxiPhoneExternalDisplay *> externalDisplayListeners;

void ofxiPhoneExternalDisplay::alertExternalDisplayConnected(){
    for(int i = 0; i < externalDisplayListeners.size(); i++){
        externalDisplayListeners[i]->externalDisplayConnected();
    }
}

void ofxiPhoneExternalDisplay::alertExternalDisplayDisconnected(){
    for(int i = 0; i < externalDisplayListeners.size(); i++){
        externalDisplayListeners[i]->externalDisplayDisconnected();
    }
}

void ofxiPhoneExternalDisplay::alertExternalDisplayChanged(){
    for(int i = 0; i < externalDisplayListeners.size(); i++){
        externalDisplayListeners[i]->externalDisplayChanged();
    }
}

//-----------------------------------------------------------------------------------------
vector<ofxiPhoneExternalDisplayMode> ofxiPhoneExternalDisplay::getExternalDisplayModes(){
    vector<ofxiPhoneExternalDisplayMode> externalDislayModes;
    if([[UIScreen screens] count] == 1){ // no external displays connected.
        return externalDislayModes;
    }
    UIScreen * externalScreen;
    externalScreen = [[UIScreen screens] objectAtIndex:1];
    
    for(int i = 0; i < [[externalScreen availableModes] count]; i++){
        UIScreenMode * screenMode = [[externalScreen availableModes] objectAtIndex:i];
        externalDislayModes.push_back(ofxiPhoneExternalDisplayMode());
        externalDislayModes.back().width = screenMode.size.width;
        externalDislayModes.back().height = screenMode.size.height;
        externalDislayModes.back().pixelAspectRatio = screenMode.pixelAspectRatio;
    }
    return externalDislayModes;
}

bool ofxiPhoneExternalDisplay::displayOnExternalScreen(ofxiPhoneExternalDisplayMode externalDisplayMode){
    if([[UIScreen screens] count] == 1){ // no external displays connected.
        return false;
    }
    UIScreen * externalScreen;
    externalScreen = [[UIScreen screens] objectAtIndex:1];
    
    for(int i = 0; i < [[externalScreen availableModes] count]; i++){
        UIScreenMode * screenMode = [[externalScreen availableModes] objectAtIndex:i];
        if (screenMode.size.width == externalDisplayMode.width &&
            screenMode.size.height == externalDisplayMode.height &&
            screenMode.pixelAspectRatio == externalDisplayMode.pixelAspectRatio){
           
            ofxiPhoneAppDelegate * appDelegate;
            appDelegate = [[UIApplication sharedApplication] delegate];
            
            bool bOk = [appDelegate displayOnScreenWithIndex:1 andScreenModeIndex:i];
            return bOk;
        }
    }
    
    return false;
}

bool ofxiPhoneExternalDisplay::displayOnExternalScreenWithPreferredDisplayMode(){
    if([[UIScreen screens] count] == 1){ // no external displays connected.
        return false;
    }
    UIScreen * externalScreen;
    externalScreen = [[UIScreen screens] objectAtIndex:1];

    int i;
    bool bFound = false;
    for(i = 0; i < [[externalScreen availableModes] count]; i++){
        if([[externalScreen availableModes] objectAtIndex:i] == externalScreen.preferredMode){
            bFound = true;
            break;
        }
    }
    
    if(!bFound){
        return false;
    }
    
    ofxiPhoneAppDelegate * appDelegate;
    appDelegate = [[UIApplication sharedApplication] delegate];
    
    bool bOk = [appDelegate displayOnScreenWithIndex:1 andScreenModeIndex:i];
    return bOk;
}

bool ofxiPhoneExternalDisplay::displayOnDeviceScreen(){
    ofxiPhoneAppDelegate * appDelegate;
    appDelegate = [[UIApplication sharedApplication] delegate];
    
    bool bOk = [appDelegate displayOnScreenWithIndex:0 andScreenModeIndex:0];
    return bOk;
}

bool ofxiPhoneExternalDisplay::mirrorOn(){
    ofxiPhoneAppDelegate * appDelegate;
    appDelegate = [[UIApplication sharedApplication] delegate];

    if([[UIScreen screens] count] == 1){
        return false;
    }
    if(appDelegate.currentScreenIndex == 1){
        return false;
    }

    [appDelegate destroyExternalWindow];
    return true;
    
    // the below code checks if the display is being mirrored.
    // the problem is that this does not take effect straight away and always returns as false.
    // the user will have to check themselves if mirroring is working by calling isMirroring()
    bool bMirroring = ([[[UIScreen screens] objectAtIndex:1] mirroredScreen] == [UIScreen mainScreen]);
    return bMirroring;
}

bool ofxiPhoneExternalDisplay::mirrorOff(){
    ofxiPhoneAppDelegate * appDelegate;
    appDelegate = [[UIApplication sharedApplication] delegate];

    if([[UIScreen screens] count] == 1){
        return false;
    }
    if(appDelegate.currentScreenIndex == 1){
        return false;
    }

    [appDelegate createExternalWindowWithPreferredMode];
    return true;
}

//-----------------------------------------------------------------------------------------
bool ofxiPhoneExternalDisplay::isDisplayingOnExternalScreen(){
    ofxiPhoneAppDelegate * appDelegate;
    appDelegate = [[UIApplication sharedApplication] delegate];
    return (appDelegate.currentScreenIndex == 1);
}

bool ofxiPhoneExternalDisplay::isDisplayingOnDeviceScreen(){
    ofxiPhoneAppDelegate * appDelegate;
    appDelegate = [[UIApplication sharedApplication] delegate];
    return (appDelegate.currentScreenIndex == 0);
}

bool ofxiPhoneExternalDisplay::isExternalScreenConnected(){
    return ([[UIScreen screens] count] > 1);
}

bool ofxiPhoneExternalDisplay::isMirroring(){
    if([[UIScreen screens] count] == 1){
        return false;
    }
    return ([[[UIScreen screens] objectAtIndex:1] mirroredScreen] == [UIScreen mainScreen]);
}

//-----------------------------------------------------------------------------------------
ofxiPhoneExternalDisplay::ofxiPhoneExternalDisplay(){
    externalDisplayListeners.push_back(this);
}

ofxiPhoneExternalDisplay::~ofxiPhoneExternalDisplay(){
    for(int i = 0; i < externalDisplayListeners.size(); i++){
        if(externalDisplayListeners[i]==this){
            externalDisplayListeners.erase(externalDisplayListeners.begin() + i);
            break;
        }
    }
}
