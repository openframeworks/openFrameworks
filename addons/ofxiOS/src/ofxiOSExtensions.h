//
//  ofxiOSExtensions.h
//  iOS+OFLib
//
//  Created by lukasz karluk on 5/07/12.
//

#pragma once
#include "ofiOSConstants.h"
#if defined(TARGET_OF_IOS)
void ofReloadGLResources();
void ofReloadAllImageTextures();
void ofReloadAllFontTextures();
void ofUnloadAllFontTextures();
void ofRegenerateAllVbos();
#endif
