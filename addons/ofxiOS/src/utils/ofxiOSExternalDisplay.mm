//
//  ofxiOSExternalDisplay.cpp
//
//  Created by lukasz karluk on 22/03/12.
//  http://julapy.com
//

#include "ofxiOSExternalDisplay.h"
#include <TargetConditionals.h>

using std::vector;

#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)


#include "ofxiOSAppDelegate.h"

//-----------------------------------------------------------------------------------------
static vector<ofxiOSExternalDisplay *> externalDisplayListeners;

void ofxiOSExternalDisplay::alertExternalDisplayConnected(){
    for(int i = 0; i < externalDisplayListeners.size(); i++){
        externalDisplayListeners[i]->externalDisplayConnected();
    }
}

void ofxiOSExternalDisplay::alertExternalDisplayDisconnected(){
    for(int i = 0; i < externalDisplayListeners.size(); i++){
        externalDisplayListeners[i]->externalDisplayDisconnected();
    }
}

void ofxiOSExternalDisplay::alertExternalDisplayChanged(){
    for(int i = 0; i < externalDisplayListeners.size(); i++){
        externalDisplayListeners[i]->externalDisplayChanged();
    }
}

//-----------------------------------------------------------------------------------------
vector<ofxiOSExternalDisplayMode> ofxiOSExternalDisplay::getExternalDisplayModes(){
    vector<ofxiOSExternalDisplayMode> externalDislayModes;
    if([[UIScreen screens] count] == 1){ // no external displays connected.
        return externalDislayModes;
    }
    UIScreen * externalScreen;
    externalScreen = [[UIScreen screens] objectAtIndex:1];
    
    for(int i = 0; i < [[externalScreen availableModes] count]; i++){
        UIScreenMode * screenMode = [[externalScreen availableModes] objectAtIndex:i];
        externalDislayModes.push_back(ofxiOSExternalDisplayMode());
        externalDislayModes.back().width = screenMode.size.width;
        externalDislayModes.back().height = screenMode.size.height;
        externalDislayModes.back().pixelAspectRatio = screenMode.pixelAspectRatio;
    }
    return externalDislayModes;
}

bool ofxiOSExternalDisplay::displayOnExternalScreen(ofxiOSExternalDisplayMode externalDisplayMode){
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
           
            ofxiOSAppDelegate * appDelegate;
            appDelegate = [[UIApplication sharedApplication] delegate];
            
            bool bOk = [appDelegate displayOnScreenWithIndex:1 andScreenModeIndex:i];
            return bOk;
        }
    }
    
    return false;
}

bool ofxiOSExternalDisplay::displayOnExternalScreenWithPreferredDisplayMode(){
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
    
    ofxiOSAppDelegate * appDelegate;
    appDelegate = [[UIApplication sharedApplication] delegate];
    
    bool bOk = [appDelegate displayOnScreenWithIndex:1 andScreenModeIndex:i];
    return bOk;
}

bool ofxiOSExternalDisplay::displayOnDeviceScreen(){
    ofxiOSAppDelegate * appDelegate;
    appDelegate = [[UIApplication sharedApplication] delegate];
    
    bool bOk = [appDelegate displayOnScreenWithIndex:0 andScreenModeIndex:0];
    return bOk;
}

bool ofxiOSExternalDisplay::mirrorOn(){
    ofxiOSAppDelegate * appDelegate;
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

bool ofxiOSExternalDisplay::mirrorOff(){
    ofxiOSAppDelegate * appDelegate;
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
bool ofxiOSExternalDisplay::isDisplayingOnExternalScreen(){
    ofxiOSAppDelegate * appDelegate;
    appDelegate = [[UIApplication sharedApplication] delegate];
    return (appDelegate.currentScreenIndex == 1);
}

bool ofxiOSExternalDisplay::isDisplayingOnDeviceScreen(){
    ofxiOSAppDelegate * appDelegate;
    appDelegate = [[UIApplication sharedApplication] delegate];
    return (appDelegate.currentScreenIndex == 0);
}

bool ofxiOSExternalDisplay::isExternalScreenConnected(){
    return ([[UIScreen screens] count] > 1);
}

bool ofxiOSExternalDisplay::isMirroring(){
    if([[UIScreen screens] count] == 1){
        return false;
    }
    return ([[[UIScreen screens] objectAtIndex:1] mirroredScreen] == [UIScreen mainScreen]);
}

//-----------------------------------------------------------------------------------------
ofxiOSExternalDisplay::ofxiOSExternalDisplay(){
    externalDisplayListeners.push_back(this);
}

ofxiOSExternalDisplay::~ofxiOSExternalDisplay(){
    for(int i = 0; i < externalDisplayListeners.size(); i++){
        if(externalDisplayListeners[i]==this){
            externalDisplayListeners.erase(externalDisplayListeners.begin() + i);
            break;
        }
    }
}

#endif
