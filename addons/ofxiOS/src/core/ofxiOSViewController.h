//
//  ofxiOSViewController.h
//  Created by lukasz karluk on 12/12/11.
//

#import <UIKit/UIKit.h>

class ofxiOSApp;
@class ofxiOSEAGLView;

@interface ofxiOSViewController : UIViewController

@property (nonatomic, retain) ofxiOSEAGLView * glView;

- (id)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app;

- (UIInterfaceOrientation)currentInterfaceOrientation;
- (void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
                            animated:(BOOL)animated;

@end

#define ofxPhoneViewController ofxiOSViewController