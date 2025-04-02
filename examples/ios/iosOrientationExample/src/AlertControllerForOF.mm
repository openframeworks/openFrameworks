//
//  AlertControllerForOF.cpp
//  iosOrientationExample
//
//  Created by azuremous on 23/01/24
//

#import "AlertControllerForOF.h"

@implementation AlertControllerForOF {
	
}

- (id)initWithApp:(ofApp *)myApp {
	self = [super init];
	if(self) {
		app = myApp;
	}
	return self;
}

- (void)dealloc {

}

- (void)showRotationOptions {
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:nil
                                                                             message:@"Choose rotation"
                                                                      preferredStyle:UIAlertControllerStyleActionSheet];
    
    UIAlertAction *portraitAction = [UIAlertAction actionWithTitle:@"Portrait"
                                                             style:UIAlertActionStyleDefault
                                                           handler:^(UIAlertAction *action) {
        self->app->rotateToPortrait();
    }];
    [alertController addAction:portraitAction];
    
    UIAlertAction *portraitUpsideDownAction = [UIAlertAction actionWithTitle:@"Portrait Upside Down"
                                                                       style:UIAlertActionStyleDefault
                                                                     handler:^(UIAlertAction *action) {
        self->app->rotateToPortraitUpSideDown();
    }];
    [alertController addAction:portraitUpsideDownAction];
    
    UIAlertAction *landscapeLeftAction = [UIAlertAction actionWithTitle:@"Landscape Left"
                                                                  style:UIAlertActionStyleDefault
                                                                handler:^(UIAlertAction *action) {
        self->app->rotateToLandscapeLeft();
    }];
    
    [alertController addAction:landscapeLeftAction];
    
    UIAlertAction *landscapeRightAction = [UIAlertAction actionWithTitle:@"Landscape Right"
                                                                   style:UIAlertActionStyleDefault
                                                                 handler:^(UIAlertAction *action) {
        self->app->rotateToLandscapeRight();
    }];
    [alertController addAction:landscapeRightAction];
    
    UIAlertAction *autoRotationAction = [UIAlertAction actionWithTitle:@"Toggle Auto Rotation"
                                                                 style:UIAlertActionStyleDefault
                                                               handler:^(UIAlertAction *action) {
        self->app->toggleAutoRotation();

    }];
    [alertController addAction:autoRotationAction];
    
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"Cancel"
                                                           style:UIAlertActionStyleCancel
                                                         handler:nil];
    [alertController addAction:cancelAction];
    
    if ([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad) {
        UIView * sourceView = ofxiOSGetGLParentView();
        if (alertController.popoverPresentationController) {
            alertController.popoverPresentationController.sourceView = sourceView;
            alertController.popoverPresentationController.sourceRect = CGRectMake(sourceView.bounds.size.width/2, sourceView.bounds.size.height/2, 0, 0);
            alertController.popoverPresentationController.permittedArrowDirections = 0;
        }
    }
    
    [ofxiOSGetUIWindow().rootViewController presentViewController:alertController animated:YES completion:nil];
}


@end
