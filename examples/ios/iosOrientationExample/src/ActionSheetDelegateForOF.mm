//
//  ActionSheetDelegateForOF.m
//  Created by lukasz karluk on 6/06/12.
//

#import "ActionSheetDelegateForOF.h"

@implementation ActionSheetDelegateForOF {
    //
}

- (id)initWithApp:(testApp *)myApp {
    self = [super init];
    if(self) {
        app = myApp;
    }
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex {
    switch (buttonIndex) {
        case 0:
            app->rotateToPortrait();
            break;
        case 1:
            app->rotateToPortraitUpSideDown();
            break;
        case 2:
            app->rotateToLandscapeLeft();
            break;
        case 3:
            app->rotateToLandscapeRight();
            break;
        case 4:
            app->toggleAutoRotation();
            break;
        default:
            break;
    }
}

@end