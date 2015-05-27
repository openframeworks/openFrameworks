//
//  ActionSheetDelegateForOF.h
//  emptyExample
//  Created by Lukasz Karluk on 17/07/13.
//

#import <UIKit/UIKit.h>
#import "ofApp.h"

/**
 *  this is a bit of obj-c that will help connect the OF app with the UIActionSheet.
 *  all this is doing is re-routing UI events from the UIActionSheet to the OF app.
 */

@interface ActionSheetDelegateForOF : NSObject <UIActionSheetDelegate> {
    ofApp * app;
}

- (id)initWithApp:(ofApp *)app;

@end
