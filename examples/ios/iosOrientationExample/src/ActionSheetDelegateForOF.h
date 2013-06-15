//
//  ActionSheetDelegateForOF.h
//  Created by lukasz karluk on 6/06/12.
//

#import <UIKit/UIKit.h>
#import "testApp.h"

/**
 *  this is a bit of obj-c that will help connect the OF app with the UIActionSheet.
 *  all this is doing is re-routing UI events from the UIActionSheet to the OF app.
 */

@interface ActionSheetDelegateForOF : NSObject <UIActionSheetDelegate> {
    testApp * app;
}

- (id)initWithApp:(testApp *)app;

@end
