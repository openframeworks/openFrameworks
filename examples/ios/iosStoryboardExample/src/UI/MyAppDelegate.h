//
//  MyAppDelegate.h
//  Created by lukasz karluk on 11/02/14.
//  http://julapy.com
//  Note:
//      If the app is not compiling, try removing the storyboard reference
//      from the xcode project and adding it back to the project.
//      Set the deployment target in project / General / Deployment Target >= 5.1

#import "ofxiOSAppDelegate.h"

@interface MyAppDelegate : ofxiOSAppDelegate {
    //
}

@property (nonatomic, retain) UINavigationController* navigationController;

@end
