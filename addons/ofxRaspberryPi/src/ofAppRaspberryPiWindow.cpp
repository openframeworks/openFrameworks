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
//#include "ofAppEGLWindow.h"
#include <assert.h>

//------------------------------------------------------------
ofAppRaspberryPiWindow::ofAppRaspberryPiWindow() { };
//------------------------------------------------------------
ofAppRaspberryPiWindow::~ofAppRaspberryPiWindow(){ };

//------------------------------------------------------------
void ofAppRaspberryPiWindow::setupOpenGL(int w, int h, int screenMode) {

    // raspberry pi host init
    // TODO: this may be called elsewhere in the core ...


    bcm_host_init();

    //boolean force HDMI vs. composite

    int32_t success = 0;

    uint32_t sw;
    uint32_t sh;

    // create an EGL window surface
    // IF SCREENMODE==FULLSCREEN
    success = graphics_get_display_size(0 /* LCD */, &sw, &sh);
    assert( success >= 0 );

cout << "   REQUESTED SCREEN SIZE w=" << w << " and  h=" << h << endl;
cout << "HARDWARE SCREEN SIZE IS sw=" << sw << " and sh=" << sh << endl;



//////////////////////////
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = sw;
    dst_rect.height = sh;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = sw << 16;
    src_rect.height = sh << 16;

    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;


    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );

    dispman_element = vc_dispmanx_element_add ( dispman_update, 
                                                dispman_display,
                                                0/*layer*/, 
                                                &dst_rect, 
                                                0/*src*/,
                                                &src_rect, 
                                                DISPMANX_PROTECTION_NONE, 
                                                0 /*alpha*/, 
                                                0/*clamp*/, 
                                                (DISPMANX_TRANSFORM_T)0/*transform*/
                                                );

    nativeWindow.element = dispman_element;
    nativeWindow.width = sw;
    nativeWindow.height = sh;
    vc_dispmanx_update_submit_sync( dispman_update );


// //////////////////////////









    // call the super class method
    ofAppEGLWindow::setupEGL(&nativeWindow);

    // call the super class opengl setup
    //ofEGLWindow::setupOpenGL(sw, sh, screenMode);

    cout << "CREATED SCREEN WITH SIZE " << sw << " x " << sh << endl;


    // TEMPORARY

    screenRect.x = 0;
    screenRect.y = 0;
    screenRect.width = sw;
    screenRect.height = sh;

    nonFullscreenWindowRect = screenRect;
    currentWindowRect = screenRect;


    // choose our renderer
    ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer));
}

// //------------------------------------------------------------
// void ofAppRaspberryPiWindow::_getMouse() {}
// //------------------------------------------------------------
// void ofAppRaspberryPiWindow::_getKeyboard() {}
// //------------------------------------------------------------
// void ofAppRaspberryPiWindow::_getJoystick() {}