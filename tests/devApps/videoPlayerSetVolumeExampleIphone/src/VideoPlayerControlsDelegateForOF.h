//
//  VideoPlayerControlsDelegateForOF.h
//  moviePlayerExample
//
//  Created by lukasz karluk on 6/06/12.
//

#import <Foundation/Foundation.h>
#import "VideoPlayerControls.h"
#import "ofApp.h"

/**
 *  this is a bit of obj-c that will help connect the OF app with the VideoPlayerControls UI.
 *  all this is doing is re-routing UI events from the controls to the OF app.
 */

@interface VideoPlayerControlsDelegateForOF : NSObject <VideoPlayerControlsDelegate> {
    ofApp * app;
}

- (id)initWithApp:(ofApp *)app;
- (void)playPressed;
- (void)pausePressed;
- (void)scrubBegin;
- (void)scrubToPosition:(float)position;
- (void)scrubEnd;
- (void)loadPressed;
- (void)unloadPressed;
- (void)loopOnPressed;
- (void)loopOffPressed;
- (void)nativeOnPressed;
- (void)nativeOffPressed;
- (void)muteOnPressed;
- (void)muteOffPressed;

@end
