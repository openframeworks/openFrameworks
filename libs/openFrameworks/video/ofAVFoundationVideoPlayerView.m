//
//  ofAVFoundationVideoPlayerView.m
//  Created by Lukasz Karluk on 06/07/14.
//	Merged with code by Sam Kronick, James George and Elie Zananiri.
//

#import "ofAVFoundationVideoPlayerView.h"

//---------------------------------------------------------- video player view.
@implementation ofAVFoundationVideoPlayerView

+ (Class)layerClass {
	return [AVPlayerLayer class];
}


- (AVPlayer*)player {
	return [(AVPlayerLayer*)[self layer] player];
}


- (void)setPlayer:(AVPlayer*)player {
	[(AVPlayerLayer*)[self layer] setPlayer:player];
}

//----------------------------------------------------------
- (id)initVideoPlayerView {
    /**
     *  initialise video player view to full screen by default.
     *  later the view frame can be changed if need be.
     */
    
#ifdef TARGET_IOS
    
    CGRect playerViewFrame = [UIScreen mainScreen].bounds;
    self = [super initWithFrame:playerViewFrame];
    if(self != NULL) {
        self.backgroundColor = [UIColor blackColor];
    }
    return self;
    
#elif defined(TARGET_OSX)
    
    NSRect playerViewFrame = [NSScreen mainScreen].visibleFrame;
    self = [super initWithFrame:playerViewFrame];
    if(self != NULL) {
        //
    }
    return self;

#endif
}

//----------------------------------------------------------
- (void)dealloc {
    self.player = nil;
    [super dealloc];
}

//----------------------------------------------------------
- (void)setVideoPosition:(CGPoint)position {
#ifdef TARGET_IOS
    
    CGRect playerViewFrame = self.frame;
    playerViewFrame.origin = position;
    self.frame = playerViewFrame;
    
#elif defined(TARGET_OSX)
    
    NSRect playerViewFrame = self.frame;
    playerViewFrame.origin.x = position.x;
    playerViewFrame.origin.y = position.y;
    self.frame = playerViewFrame;
    
#endif
}

- (void)setVideoSize:(CGSize)size {
#ifdef TARGET_IOS
    
    CGRect playerViewFrame = self.frame;
    playerViewFrame.size = size;
    self.frame = playerViewFrame;
    
#elif defined(TARGET_OSX)
    
    NSRect playerViewFrame = self.frame;
    playerViewFrame.size.width = size.width;
    playerViewFrame.size.height = size.height;
    self.frame = playerViewFrame;
    
#endif
}

@end
