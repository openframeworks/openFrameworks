#pragma once

#include "ofConstants.h"

/// returns a shader for doing color space conversion from the given format
/// might return NULL in case the format is directly supported by a texture
ofShader * ofGetVideoShader(ofPixelFormat pixelFormat);

/// sets the corresponding texture planes and other variables needed to
/// render a video through a color space conversion shader, usually this
/// happens automatically when calling video.draw()
void ofSetVideoShaderUniforms(ofBaseVideoDraws & video, ofShader & shader);
