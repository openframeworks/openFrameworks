//
//  VideoPlayerControlsDelegateForOF.m
//  moviePlayerExample
//
//  Created by lukasz karluk on 6/06/12.
//

#import "VideoPlayerControlsDelegateForOF.h"

@implementation VideoPlayerControlsDelegateForOF {
    //
}

- (id)initWithApp:(testApp *)myApp {
    self = [super init];
    if(self) {
        app = myApp;
    }
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)playPressed {
    app->playPressed();
}

- (void)pausePressed {
    app->pausePressed();
}

- (void)scrubBegin {
    app->scrubBegin();
}

- (void)scrubToPosition:(float)position {
    app->scrubToPosition(position);
}

- (void)scrubEnd {
    app->scrubEnd();
}

- (void)loadPressed {
    app->loadPressed();
}

- (void)unloadPressed {
    app->unloadPressed();
}

- (void)loopOnPressed {
    app->loopOnPressed();
}

- (void)loopOffPressed {
    app->loopOffPressed();
}

- (void)nativeOnPressed {
    app->nativeOnPressed();
}

- (void)nativeOffPressed {
    app->nativeOffPressed();
}

- (void)muteOnPressed {
    app->muteOnPressed();
}

- (void)muteOffPressed {
    app->muteOffPressed();
}

@end