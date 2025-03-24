//
//  VideoPlayerControls.h
//  moviePlayerExample
//
//  Created by lukasz karluk on 21/05/12.
//

#import <UIKit/UIKit.h>
#import "VideoPlayerControlsDelegateForOF.h"

@protocol VideoPlayerControlsDelegate <NSObject>
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

@interface VideoPlayerControls : UIViewController {
    ofApp * app;
    id<VideoPlayerControlsDelegate> delegate;
}

- (void)setPlay:(BOOL)bPlay;
- (void)setNative:(BOOL)bNative;
- (void)setLoop:(BOOL)bLoop;
- (void)setMute:(BOOL)bMute;
- (void)setLoad:(BOOL)bLoad;
- (void)setNewFrame:(BOOL)bNewFrame;
- (void)setPosition:(float)position;
- (void)setTimeInSeconds:(NSTimeInterval)seconds;

@end
