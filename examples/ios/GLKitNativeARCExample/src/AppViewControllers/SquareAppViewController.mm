//
//  CustomAppViewController.m
//  Created by lukasz karluk on 8/02/12.
//

#import "SquareAppViewController.h"
#import "ofxiOSExtras.h"
#import "ofAppiOSWindow.h"

@implementation SquareAppViewController

- (id) initWithFrame:(CGRect)frame app:(ofxiOSApp *)app {
    
    ofxiOSGetOFWindow()->setOrientation( OF_ORIENTATION_DEFAULT );   //-- default portait orientation.    
    
    return self = [super initWithFrame:frame app:app sharegroup:nil];
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return NO;
}

@end
