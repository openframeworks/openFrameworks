//
//  ofxiOSSoundStreamDelegate.h
//  Created by Lukasz Karluk on 14/06/13.
//  http://julapy.com/blog
//

#pragma once

#import "SoundStream.h"
#include <functional>

class ofSoundBuffer;

@interface ofxiOSSoundStreamDelegate : NSObject <SoundStreamDelegate>

- (instancetype)initWithSoundInputFn:(std::function<void(ofSoundBuffer &)>)input;
- (instancetype)initWithSoundOutputFn:(std::function<void(ofSoundBuffer &)>)output;
- (void)setInput:(std::function<void(ofSoundBuffer &)>)input;
- (void)setOutput:(std::function<void(ofSoundBuffer &)>)output;

@end

#define ofxiPhoneSoundStreamDelegate ofxiOSSoundStreamDelegate
