/*==============================================================================

    Copyright (c) 2010, 2011 ofxKinect Team

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
    
==============================================================================*/
#pragma once

/// \class ofxBase3DVideo
///
/// a base class for 3D video devices
class ofxBase3DVideo: public ofBaseVideo {

public:
    
    /// get the pixels of the most recent depth frame
    virtual ofPixels& getDepthPixels()=0;
    virtual const ofPixels& getDepthPixels() const=0;
    
    /// get the distance in millimeters to a given point as a float array
    virtual ofFloatPixels& getDistancePixels()=0;
    virtual const ofFloatPixels& getDistancePixels() const=0;
    
    /// get the grayscale depth texture
    virtual ofTexture& getDepthTexture()=0;
    virtual const ofTexture& getDepthTexture() const=0;
};
