//
//  ofxiOSEAGLView.h
//  iOS+OFLib
//
//  Created by lukasz karluk on 5/07/12.
//

#import <UIKit/UIKit.h>
#import "EAGLView.h"

class ofBaseApp;

@interface ofxiOSEAGLView : EAGLView {

@protected
    NSMutableDictionary	* activeTouches;
    ofBaseApp * app;
    
	double timeNow;
    double timeThen;
    double fps;
    double lastFrameTime;
	int	nFrameCount;
	float frameRate;
}

@property (readonly, nonatomic, getter=getLastFrameTime) double lastFrameTime;
@property (readonly, nonatomic, getter=getFrameNum) int nFrameCount;
@property (readonly, nonatomic, getter=getFrameRate) float frameRate;

+ (ofxiOSEAGLView *) getInstance;

- (id)initWithFrame:(CGRect)frame andApp:(ofBaseApp *)app;
- (void)setup;

@end
