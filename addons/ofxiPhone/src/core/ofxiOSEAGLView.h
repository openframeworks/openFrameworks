//
//  ofxiOSEAGLView.h
//  iOS+OFLib
//
//  Created by lukasz karluk on 5/07/12.
//

#import <UIKit/UIKit.h>
#import "EAGLView.h"

class ofxiPhoneApp;
class ofVec3f;

@interface ofxiOSEAGLView : EAGLView {

@protected
    NSMutableDictionary	* activeTouches;
    ofxiPhoneApp * app;
    
	double timeNow;
    double timeThen;
    double fps;
    double lastFrameTime;
	int	nFrameCount;
	float frameRate;
    
	ofVec3f * screenSize;   // because ofVec3f is forward declared,
	ofVec3f * windowSize;   // these values have to be pointers.
	ofVec3f * windowPos;
}

@property (readonly, nonatomic, getter=getLastFrameTime) double lastFrameTime;
@property (readonly, nonatomic, getter=getFrameNum) int nFrameCount;
@property (readonly, nonatomic, getter=getFrameRate) float frameRate;

@property (readonly, nonatomic, getter=getScreenSize) ofVec3f * screenSize;
@property (readonly, nonatomic, getter=getWindowSize) ofVec3f * windowSize;
@property (readonly, nonatomic, getter=getWindowPosition) ofVec3f * windowPos;

+ (ofxiOSEAGLView *) getInstance;

- (id)initWithFrame:(CGRect)frame andApp:(ofxiPhoneApp *)app;
- (void)setup;
- (void)destroy;

@end
