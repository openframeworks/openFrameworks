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

#include "ofAppEGLWindow.h"

#include "ofGraphics.h" // used in runAppViaInfiniteLoop()
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofFileUtils.h"
#include <assert.h>

// TODO. we may not need these to be static, but we will
// leave it this way for now in case future EGL windows 
// use static callbacks (like glut)

//------------------------------------------------------------
ofAppEGLWindow::ofAppEGLWindow() {
    terminate      = false;

    timeNow         = 0;
    timeThen        = 0;
    fps             = 60.0; //give a realistic starting value - win32 issues
    frameRate       = 60.0;
    windowMode      = OF_WINDOW;
    bNewScreenMode  = true;
    nFramesForFPS   = 0;
    nFramesSinceWindowResized = 0;
    nFrameCount     = 0;
    buttonInUse     = 0;
    bEnableSetupScreen  = true;
    bFrameRateSet   = false;
    millisForFrame    = 0;
    prevMillis      = 0;
    diffMillis      = 0;
    // requestedWidth    = 0;
    // requestedHeight   = 0;
    // nonFullScreenX    = -1;
    // nonFullScreenY    = -1;
    lastFrameTime   = 0.0;
    eglDisplayString   = "";
    orientation     = OF_ORIENTATION_DEFAULT;
}

//------------------------------------------------------------
ofAppEGLWindow::~ofAppEGLWindow() {

}

// //------------------------------------------------------------
// void ofAppEGLWindow::setupOpenGL(int w, int h, int screenMode) {
//     cout << "in ofAppEGLWINDOW: setupOpenGL" << endl;

//     windowMode = screenMode;
//     bNewScreenMode = true;

//     if(windowMode == OF_GAME_MODE) {
//         ofLogWarning("ofAppEGLWindow") << "OF_GAME_MODE not supported.";
//     }

//     //windowW = requestedWidth  = getWindowWidth();
//     //windowH = requestedHeight = getWindowHeight();

// }

//------------------------------------------------------------
void ofAppEGLWindow::setupEGL(NativeWindowType nativeWindow)
{

    EGLBoolean result;
    EGLint num_config;
    EGLConfig config;

    // get an EGL eglDisplay connection
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if(eglDisplay == EGL_NO_DISPLAY) {
      ofLogError("ofAppEGLWindow::setupEGL") << "eglGetDisplay returned: " << eglDisplay;
      return;
    }

    EGLint eglVersionMajor = 0;
    EGLint eglVersionMinor = 0;

    // initialize the EGL eglDisplay connection
    result = eglInitialize(eglDisplay, &eglVersionMajor, &eglVersionMinor);

    if(result == EGL_BAD_DISPLAY) {
//  eglDisplay is not an EGL connection
      ofLogError("ofAppEGLWindow::setupEGL") << "eglInitialize returned EGL_BAD_DISPLAY";
      return;
    } else if(result == EGL_NOT_INITIALIZED) {
      // eglDisplay cannot be intitialized
      ofLogError("ofAppEGLWindow::setupEGL") << "eglInitialize returned EGL_NOT_INITIALIZED";
      return;
    } else if(result == EGL_FALSE) {
      // eglinitialize was not initialiezd
      ofLogError("ofAppEGLWindow::setupEGL") << "eglInitialize returned EGL_FALSE";
      return;
    } else {
      // result == EGL_TRUE
      // success!
    }

    // TODO -- give the ability to send in this list when setting up.
    static const EGLint attribute_list[] =
    {
        EGL_RED_SIZE,   8, // 8 bits for red
        EGL_GREEN_SIZE, 8, // 8 bits for green
        EGL_BLUE_SIZE,  8, // 8 bits for blue
        EGL_ALPHA_SIZE, 8, // 8 bits for alpha
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT, // default eglSurface type
        EGL_NONE // attribute list is termintated with EGL_NONE
    };

    // get an appropriate EGL frame buffer configuration
    result = eglChooseConfig(eglDisplay, 
                             attribute_list, 
                             &config, 
                             1, 
                             &num_config);

    assert(EGL_FALSE != result);

    // create an EGL rendering eglContext
    eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, NULL);

    assert(eglContext != EGL_NO_CONTEXT);

    eglSurface = eglCreateWindowSurface( eglDisplay, config, nativeWindow, NULL );
    assert(eglSurface != EGL_NO_SURFACE);

    // connect the eglContext to the eglSurface
    result = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    assert(EGL_FALSE != result);

    // Set background color and clear buffers
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    glClear( GL_DEPTH_BUFFER_BIT );

    //ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer));
}

//------------------------------------------------------------
void ofAppEGLWindow::destroyEGL() {
    eglMakeCurrent( eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
    eglDestroySurface( eglDisplay, eglSurface );
    eglDestroyContext( eglDisplay, eglContext );
    eglTerminate( eglDisplay );
}

//------------------------------------------------------------
void ofAppEGLWindow::initializeWindow()
{
    nFramesSinceWindowResized = 0;
}

//------------------------------------------------------------
void ofAppEGLWindow::runAppViaInfiniteLoop(ofBaseApp *appPtr) {
    ofAppPtr = appPtr; // make a local copy
   
    ofNotifySetup();
    ofNotifyUpdate();

    // loop it!
    infiniteLoop();
    
    // we got a terminate ... so clean up.
    destroyEGL();
}

//------------------------------------------------------------
void ofAppEGLWindow::infiniteLoop() {
    while (!terminate) {
      idle();
      display();
    }
}

//------------------------------------------------------------
void ofAppEGLWindow::hideCursor(){
    ofLogNotice("ofAppEGLWindow") << "hideCursor() not implemented.";
}

//------------------------------------------------------------
void ofAppEGLWindow::showCursor(){
    ofLogNotice("ofAppEGLWindow") << "showCursor() not implemented.";
}

//------------------------------------------------------------
void ofAppEGLWindow::setFrameRate(float targetRate){
  // given this FPS, what is the amount of millis per frame
  // that should elapse?

  // --- > f / s

  if (targetRate == 0){
    bFrameRateSet = false;
    return;
  }

  bFrameRateSet       = true;
  float durationOfFrame   = 1.0f / (float)targetRate;
  millisForFrame      = (int)(1000.0f * durationOfFrame);

  frameRate       = targetRate;

}

//------------------------------------------------------------
void ofAppEGLWindow::setWindowTitle(string title) {
    ofLogNotice("ofAppEGLWindow") << "setWindowTitle() not implemented.";
}

//------------------------------------------------------------
ofPoint ofAppEGLWindow::getWindowSize(){
  return ofPoint(currentWindowRect.width, currentWindowRect.height,0);
}

//------------------------------------------------------------
ofPoint ofAppEGLWindow::getWindowPosition(){
  return currentWindowRect.getPosition();
}

//------------------------------------------------------------
ofPoint ofAppEGLWindow::getScreenSize(){
  return ofPoint(screenRect.width, screenRect.height,0);
}

//------------------------------------------------------------
int ofAppEGLWindow::getWidth(){
  if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
    return currentWindowRect.width;
  }
  return currentWindowRect.height;
}

//------------------------------------------------------------
int ofAppEGLWindow::getHeight(){
  if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
    return currentWindowRect.height;
  }
  return currentWindowRect.width;
}

//------------------------------------------------------------
void ofAppEGLWindow::setOrientation(ofOrientation orientationIn){
  orientation = orientationIn;
}

//------------------------------------------------------------
ofOrientation ofAppEGLWindow::getOrientation(){
  return orientation;
}

//------------------------------------------------------------
bool ofAppEGLWindow::doesHWOrientation() {
    return false;
}

//------------------------------------------------------------
void ofAppEGLWindow::setWindowPosition(int x, int y){
// todo -- set window rect, etc, but if we are in fullscreen mode,
    // we won't change it.
  setWindowRect(ofRectangle(x,y,currentWindowRect.width,currentWindowRect.height));
}

//------------------------------------------------------------
void ofAppEGLWindow::setWindowShape(int w, int h){
  // todo -- set window rect, etc, but if we are in fullscreen mode,
 // we won't change it.
  setWindowRect(ofRectangle(currentWindowRect.x,currentWindowRect.y,w,h));
}



//------------------------------------------------------------
int ofAppEGLWindow::getWindowMode(){
  return windowMode;
}

//------------------------------------------------------------
void ofAppEGLWindow::toggleFullscreen(){
  if( windowMode == OF_GAME_MODE)return;

  if( windowMode == OF_WINDOW ){
    setFullscreen(true);
  }else{
    setFullscreen(false);
  }

}

//------------------------------------------------------------
void ofAppEGLWindow::setFullscreen(bool fullscreen){
    if( windowMode == OF_GAME_MODE)return;

    if(fullscreen && windowMode != OF_FULLSCREEN){
        bNewScreenMode  = true;
        windowMode      = OF_FULLSCREEN;
    }else if(!fullscreen && windowMode != OF_WINDOW) {
        bNewScreenMode  = true;
        windowMode      = OF_WINDOW;
    }
}

//------------------------------------------------------------
void ofAppEGLWindow::enableSetupScreen(){
  bEnableSetupScreen = true;
}

//------------------------------------------------------------
void ofAppEGLWindow::disableSetupScreen(){
  bEnableSetupScreen = false;
}

//------------------------------------------------------------
void ofAppEGLWindow::idle() {
  //  thanks to jorge for the fix:
  //  http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate

  if (nFrameCount != 0 && bFrameRateSet == true){
    diffMillis = ofGetElapsedTimeMillis() - prevMillis;
    if (diffMillis > millisForFrame){
      ; // we do nothing, we are already slower than target frame
    } else {
      int waitMillis = millisForFrame - diffMillis;
      usleep(waitMillis * 1000);   //mac sleep in microseconds - cooler :)
    }
  }
  prevMillis = ofGetElapsedTimeMillis(); // you have to measure here

  timeNow = ofGetElapsedTimef();
  double diff = timeNow-timeThen;
  if( diff  > 0.00001 ){
    fps     = 1.0 / diff;
    frameRate *= 0.9f;
    frameRate += 0.1f*fps;
   }
   lastFrameTime  = diff;
   timeThen   = timeNow;
    // --------------

  ofNotifyUpdate();

  postIdle();
}

//------------------------------------------------------------
void ofAppEGLWindow::display() {

  // take care of any requests for a new screen mode
  if (windowMode != OF_GAME_MODE){
    if ( bNewScreenMode ){
      if( windowMode == OF_FULLSCREEN){
        setWindowRect(getScreenRect());
      } else if( windowMode == OF_WINDOW ){
        setWindowRect(nonFullscreenWindowRect);
      }
      bNewScreenMode = false;
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////
  // set viewport, clear the screen
 

  ofViewport(0, 0, getWindowWidth(), getWindowHeight());    // used to be glViewport( 0, 0, width, height );
  
  float * bgPtr = ofBgColorPtr();
  bool bClearAuto = ofbClearBg();

  if ( bClearAuto == true || nFrameCount < 3){
    ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
  }

  if( bEnableSetupScreen ) ofSetupScreen(); // this calls into the current renderer (ofSetupScreenPerspective)

  ofNotifyDraw();

  eglSwapBuffers(eglDisplay, eglSurface);

  nFramesSinceWindowResized++;

  nFrameCount++;    // increase the overall frame count

  postDisplay();
}

//------------------------------------------------------------
float ofAppEGLWindow::getFrameRate(){
  return frameRate;
}

//------------------------------------------------------------
double ofAppEGLWindow::getLastFrameTime(){
  return lastFrameTime;
}

//------------------------------------------------------------
int ofAppEGLWindow::getFrameNum(){
  return nFrameCount;
}


