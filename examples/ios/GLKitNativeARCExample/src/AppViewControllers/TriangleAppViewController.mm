#import "TriangleAppViewController.h"
#import "ofxiOSExtras.h"
#import "ofAppiOSWindow.h"

@implementation TriangleAppViewController

- (id) initWithFrame:(CGRect)frame app:(ofxiOSApp *)app sharegroup:(EAGLSharegroup *)sharegroup{
    
    ofxiOSGetOFWindow()->setOrientation( OF_ORIENTATION_DEFAULT );   //-- default portait orientation.    
    
    return self = [super initWithFrame:frame app:app sharegroup:sharegroup];
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return NO;
}

@end
