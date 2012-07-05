//
//  ofxiPhoneViewController.h
//  Created by lukasz karluk on 12/12/11.
//

#import <UIKit/UIKit.h>

class ofBaseApp;
@class ofxiOS_EAGLView;

@interface ofxiPhoneViewController : UIViewController

@property (nonatomic, retain) ofxiOS_EAGLView * glView;

- (id)initWithFrame:(CGRect)frame app:(ofBaseApp *)app;

@end
