//
//  ofxiPhoneViewController.h
//  Created by lukasz karluk on 12/12/11.
//

#import <UIKit/UIKit.h>

class ofBaseApp;
@class ofxiOSEAGLView;

@interface ofxiPhoneViewController : UIViewController

@property (nonatomic, retain) ofxiOSEAGLView * glView;

- (id)initWithFrame:(CGRect)frame app:(ofBaseApp *)app;

@end
