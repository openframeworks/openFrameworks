//
//  CustomAppViewController.m
//  Created by lukasz karluk on 8/02/12.
//

#import "ImageAppViewController.h"
#include "ofxiOSExtras.h"
#include "ofAppiOSWindow.h"

@implementation ImageAppViewController

- (id) initWithFrame:(CGRect)frame app:(ofxiOSApp *)app {
    
    ofxiOSGetOFWindow()->setOrientation( OF_ORIENTATION_DEFAULT );   //-- default portait orientation.    
    
    return self = [super initWithFrame:frame app:app];
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return NO;
}

@end
