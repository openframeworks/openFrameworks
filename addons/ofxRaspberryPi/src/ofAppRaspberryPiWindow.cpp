/*==============================================================================

 Copyright (c) 2011, 2012 Christopher Baker <http://christopherbaker.net>

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

    Modified by Philip Whitfield (undef.ch)

 ==============================================================================*/

#pragma oncecd bin


#include "ofAppRaspberryPiWindow.h"


//------------------------------------------------------------
ofAppRaspberryPiWindow::ofAppRaspberryPiWindow() { };
//------------------------------------------------------------
ofAppRaspberryPiWindow::~ofAppRaspberryPiWindow(){ };

//------------------------------------------------------------
void ofAppRaspberryPiWindow::setupOpenGL(int w, int h, int screenMode)
{

    // raspberry pi host init
    bcm_host_init();

    // call the super class method
    ofEGLWindow::setupOpenGL(w,h,screenMode);

    // choose our renderer
    ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer));
}

// //------------------------------------------------------------
// void ofAppRaspberryPiWindow::_getMouse() {}
// //------------------------------------------------------------
// void ofAppRaspberryPiWindow::_getKeyboard() {}
// //------------------------------------------------------------
// void ofAppRaspberryPiWindow::_getJoystick() {}