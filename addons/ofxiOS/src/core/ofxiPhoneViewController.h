//
//  ofxiPhoneViewController.h
//  Created by lukasz karluk on 12/12/11.
//

#import <UIKit/UIKit.h>

class ofxiPhoneApp;
@class ofxiOSEAGLView;

@interface ofxiPhoneViewController : UIViewController

@property (nonatomic, retain) ofxiOSEAGLView * glView;
@property (nonatomic, readonly) UIInterfaceOrientation currentInterfaceOrientation;

- (id)initWithFrame:(CGRect)frame app:(ofxiPhoneApp *)app;

- (void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
                            animated:(BOOL)animated;

@end
