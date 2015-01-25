//
//  ofAVFoundationVideoPlayerView.h
//  Created by Lukasz Karluk on 06/07/14.
//	Merged with code by Sam Kronick, James George and Elie Zananiri.
//

//----------------------------------------------------------
#include <TargetConditionals.h>
#if (TARGET_OS_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
    #define TARGET_IOS
#else
    #define TARGET_OSX
#endif

//----------------------------------------------------------
#ifdef TARGET_IOS
    #import <UIKit/UIKit.h>
    #define BaseView UIView
#elif defined(TARGET_OSX)
    #import <AppKit/AppKit.h>
    #define BaseView NSView
#endif

#import <AVFoundation/AVFoundation.h>

//---------------------------------------------------------- video player view.
@interface ofAVFoundationVideoPlayerView : BaseView {
    //
}
@property (nonatomic, retain) AVPlayer * player;

- (id)initVideoPlayerView;
- (void)setVideoPosition:(CGPoint)position;
- (void)setVideoSize:(CGSize)size;

@end
