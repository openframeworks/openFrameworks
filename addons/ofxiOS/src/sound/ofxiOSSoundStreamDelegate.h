//
//  ofxiOSSoundStreamDelegate.h
//  Created by Lukasz Karluk on 14/06/13.
//  http://julapy.com/blog
//

#import "SoundStream.h"

class ofBaseSoundInput;
class ofBaseSoundOutput;

@interface ofxiOSSoundStreamDelegate : NSObject <SoundStreamDelegate> {
    ofBaseSoundInput * soundInputApp;
    ofBaseSoundOutput * soundOutputApp;
}

- (id)initWithSoundInputApp:(ofBaseSoundInput *)app;
- (id)initWithSoundOutputApp:(ofBaseSoundOutput *)app;


@end

#define ofxiPhoneSoundStreamDelegate ofxiOSSoundStreamDelegate