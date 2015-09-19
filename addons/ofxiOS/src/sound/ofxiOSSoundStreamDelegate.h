//
//  ofxiOSSoundStreamDelegate.h
//  Created by Lukasz Karluk on 14/06/13.
//  http://julapy.com/blog
//

#import "SoundStream.h"

class ofBaseSoundInput;
class ofBaseSoundOutput;

@interface ofxiOSSoundStreamDelegate : NSObject <SoundStreamDelegate>

- (id)initWithSoundInputApp:(ofBaseSoundInput *)app;
- (id)initWithSoundOutputApp:(ofBaseSoundOutput *)app;
- (void)setInput:(ofBaseSoundInput *)input;
- (void)setOutput:(ofBaseSoundOutput *)output;

@end

#define ofxiPhoneSoundStreamDelegate ofxiOSSoundStreamDelegate