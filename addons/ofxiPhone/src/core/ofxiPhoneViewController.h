//
//  ofxiPhoneViewController.h
//  Created by lukasz karluk on 12/12/11.
//

#import <UIKit/UIKit.h>

class ofxiPhoneApp;
@class ofxiOSEAGLView;

@interface ofxiPhoneViewController : UIViewController

@property (nonatomic, retain) ofxiOSEAGLView * glView;

- (id)initWithFrame:(CGRect)frame app:(ofxiPhoneApp *)app;

- (UIInterfaceOrientation) getInterfaceOrientation;
- (void)rotateToInterfaceOrientationPortrait:(BOOL)animated;
- (void)rotateToInterfaceOrientationPortraitUpsideDown:(BOOL)animated;
- (void)rotateToInterfaceOrientationLandscapeLeft:(BOOL)animated;
- (void)rotateToInterfaceOrientationLandscapeRight:(BOOL)animated;
- (void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
                            animated:(BOOL)animated;

@end
