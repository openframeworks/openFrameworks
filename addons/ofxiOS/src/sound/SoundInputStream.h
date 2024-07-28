//
//  SoundInputStream.h
//  Created by Lukasz Karluk on 13/06/13.
//  http://julapy.com/blog
//

#pragma once

#import "SoundStream.h"
#include "ofxiOSConstants.h"
#if defined(TARGET_OF_IOS) && defined(OF_UI_KIT)
@interface SoundInputStream : SoundStream {

}

@end
#endif
