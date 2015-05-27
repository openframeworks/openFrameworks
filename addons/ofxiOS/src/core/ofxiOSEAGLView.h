//
//  ofxiOSEAGLView.h
//  iOS+OFLib
//
//  Created by lukasz karluk on 5/07/12.
//

#import <UIKit/UIKit.h>
#import "EAGLView.h"

class ofxiOSApp;
class ofVec3f;
class ofAppiOSWindow;

@interface ofxiOSEAGLView : EAGLView {

@protected
    NSMutableDictionary	* activeTouches;
    ofxiOSApp * app;
    ofAppiOSWindow * window;
    
	ofVec3f * screenSize;   // because ofVec3f is forward declared,
	ofVec3f * windowSize;   // these values have to be pointers.
	ofVec3f * windowPos;
}

@property (readonly, nonatomic, getter=getScreenSize) ofVec3f * screenSize;
@property (readonly, nonatomic, getter=getWindowSize) ofVec3f * windowSize;
@property (readonly, nonatomic, getter=getWindowPosition) ofVec3f * windowPos;

+ (ofxiOSEAGLView *) getInstance;

- (id)initWithFrame:(CGRect)frame andApp:(ofxiOSApp *)app;
- (void)setup;
- (void)updateDimensions;
- (void)destroy;
- (CGPoint)orientateTouchPoint:(CGPoint)touchPoint;

@end

#define ofxiPhoneEAGLView ofxiOSEAGLView
