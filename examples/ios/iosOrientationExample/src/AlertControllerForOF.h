//
//  AlertControllerForOF.h
//  iosOrientationExample
//
//  Created by azuremous on 23/01/24
//

#import <UIKit/UIKit.h>
#import "ofApp.h"

/**
 This is a piece of Objective-C code that will assist in linking the OF app with the UIAlertController. All it does is reroute UI events from the UIAlertController to the OF app.
 */

@interface AlertControllerForOF : NSObject {
    ofApp * app;
}

- (id)initWithApp:(ofApp *)app;
- (void)showRotationOptions;
@end
