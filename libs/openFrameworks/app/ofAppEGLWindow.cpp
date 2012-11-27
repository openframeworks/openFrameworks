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
#include "ofAppEGLWindow.h"

#include "ofGraphics.h" // used in runAppViaInfiniteLoop()
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofFileUtils.h"
#include <assert.h>

#define MOUSE_CURSOR_RUN_LENGTH_DECODE(image_buf, rle_data, size, bpp) do \
{ unsigned int __bpp; unsigned char *__ip; const unsigned char *__il, *__rd; \
  __bpp = (bpp); __ip = (image_buf); __il = __ip + (size) * __bpp; \
  __rd = (rle_data); if (__bpp > 3) { /* RGBA */ \
    while (__ip < __il) { unsigned int __l = *(__rd++); \
      if (__l & 128) { __l = __l - 128; \
        do { memcpy (__ip, __rd, 4); __ip += 4; } while (--__l); __rd += 4; \
      } else { __l *= 4; memcpy (__ip, __rd, __l); \
               __ip += __l; __rd += __l; } } \
  } else { /* RGB */ \
    while (__ip < __il) { unsigned int __l = *(__rd++); \
      if (__l & 128) { __l = __l - 128; \
        do { memcpy (__ip, __rd, 3); __ip += 3; } while (--__l); __rd += 3; \
      } else { __l *= 3; memcpy (__ip, __rd, __l); \
               __ip += __l; __rd += __l; } } \
  } } while (0)
static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bpp; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 rle_pixel_data[382 + 1];
} mouse_cursor_data = {
  12, 19, 4,
  "\1\0\0\0\377\213\377\377\377\0\202\0\0\0\377\212\377\377\377\0\3\0\0\0\377"
  "\377\377\377\377\0\0\0\377\211\377\377\377\0\1\0\0\0\377\202\377\377\377"
  "\377\1\0\0\0\377\210\377\377\377\0\1\0\0\0\377\203\377\377\377\377\1\0\0"
  "\0\377\207\377\377\377\0\1\0\0\0\377\204\377\377\377\377\1\0\0\0\377\206"
  "\377\377\377\0\1\0\0\0\377\205\377\377\377\377\1\0\0\0\377\205\377\377\377"
  "\0\1\0\0\0\377\206\377\377\377\377\1\0\0\0\377\204\377\377\377\0\1\0\0\0"
  "\377\207\377\377\377\377\1\0\0\0\377\203\377\377\377\0\1\0\0\0\377\210\377"
  "\377\377\377\1\0\0\0\377\202\377\377\377\0\1\0\0\0\377\211\377\377\377\377"
  "\3\0\0\0\377\377\377\377\0\0\0\0\377\212\377\377\377\377\202\0\0\0\377\206"
  "\377\377\377\377\206\0\0\0\377\203\377\377\377\377\1\0\0\0\377\202\377\377"
  "\377\377\1\0\0\0\377\204\377\377\377\0\1\0\0\0\377\202\377\377\377\377\3"
  "\0\0\0\377\377\377\377\0\0\0\0\377\202\377\377\377\377\1\0\0\0\377\203\377"
  "\377\377\0\3\0\0\0\377\377\377\377\377\0\0\0\377\202\377\377\377\0\1\0\0"
  "\0\377\202\377\377\377\377\1\0\0\0\377\203\377\377\377\0\202\0\0\0\377\204"
  "\377\377\377\0\1\0\0\0\377\202\377\377\377\377\1\0\0\0\377\210\377\377\377"
  "\0\1\0\0\0\377\202\377\377\377\377\1\0\0\0\377\211\377\377\377\0\202\0\0"
  "\0\377\203\377\377\377\0",
};

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

cout << "ofAppEGLWindow constructor " << endl;
}

//------------------------------------------------------------
ofAppEGLWindow::~ofAppEGLWindow() {

}

//------------------------------------------------------------
bool ofAppEGLWindow::setupRPiNativeWindow(int w, int h, int screenMode){

#ifdef TARGET_RASPBERRY_PI
  bcm_host_init();

  //boolean force HDMI vs. composite

  int32_t success = 0;

  uint32_t sw;
  uint32_t sh;

  // create an EGL window surface
  // IF SCREENMODE==FULLSCREEN
  success = graphics_get_display_size(0 /* LCD */, &sw, &sh);

    cout << "succes=" << success << endl;

  if(success < 0) {
    cout << "tried to get display, but failed." << endl;
    return false;
  }

  cout << "   REQUESTED SCREEN SIZE w=" << w << " and  h=" << h << endl;
  cout << "HARDWARE SCREEN SIZE IS sw=" << sw << " and sh=" << sh << endl;

  if(screenMode == OF_WINDOW) {
    sw = MIN(sw,w);
    sh = MIN(sh,h);
  } else {
    // OF_FULLSCREEN and GAME take the screen size 
  }

    cout << "CREATING A SCREEN THAT IS w=" << sw << " and h=" << sh << endl;


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
  
  bool ret = setupEGL(&nativeWindow,NULL);
  if(ret){
  	screenRect.x = 0;
	screenRect.y = 0;
    screenRect.width = nativeWindow.width;
    screenRect.height = nativeWindow.height;
  }
  return ret;
    
#else
  return false;
#endif
    
}

//------------------------------------------------------------
bool ofAppEGLWindow::setupX11NativeWindow(int w, int h, int screenMode){
#ifndef TARGET_RASPBERRY_PI

	// X11 variables
	Window				x11Window	= 0;
	Display*			x11Display	= 0;
	long				x11Screen	= 0;
	XVisualInfo*		x11Visual	= 0;
	Colormap			x11Colormap	= 0;
	/*
		Step 0 - Create a NativeWindowType that we can use it for OpenGL ES output
	*/
	Window					sRootWindow;
    XSetWindowAttributes	sWA;
	unsigned int			ui32Mask;
	int						i32Depth;
	
	// Initializes the display and screen
	x11Display = XOpenDisplay( 0 );
	if (!x11Display)
	{
		ofLogError()<< "Error: Unable to open X display";
		return false;
	}
	x11Screen = XDefaultScreen( x11Display );

	// Gets the window parameters
	sRootWindow = RootWindow(x11Display, x11Screen);
	i32Depth = DefaultDepth(x11Display, x11Screen);
	x11Visual = new XVisualInfo;
	XMatchVisualInfo( x11Display, x11Screen, i32Depth, TrueColor, x11Visual);
	if (!x11Visual)
	{
		ofLogError()<< "Error: Unable to acquire visual";
		return false;
	}
    x11Colormap = XCreateColormap( x11Display, sRootWindow, x11Visual->visual, AllocNone );
    sWA.colormap = x11Colormap;

    // Add to these for handling other events
    sWA.event_mask = StructureNotifyMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask;
    ui32Mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;

	// Creates the X11 window
    x11Window = XCreateWindow( x11Display, RootWindow(x11Display, x11Screen), 0, 0, w, h,
								 0, CopyFromParent, InputOutput, CopyFromParent, ui32Mask, &sWA);
	XMapWindow(x11Display, x11Window);
	XFlush(x11Display);
	
	bool ret = setupEGL((NativeWindowType)x11Window,(EGLNativeDisplayType*)x11Display);
	if(ret){
		screenRect.x = 0;
		screenRect.y = 0;
		screenRect.width = w;
		screenRect.height = h;
	}
	return ret;
#else
  	return false;
#endif

}


//------------------------------------------------------------
void ofAppEGLWindow::setupOpenGL(int w, int h, int screenMode) {
     
	 cout << "in ofAppEGLWINDOW: setupOpenGL" << endl;

     windowMode = screenMode;
     bNewScreenMode = true;

     if(windowMode == OF_GAME_MODE) {
         ofLogWarning("ofAppEGLWindow") << "OF_GAME_MODE not supported.";
     }

     //windowW = requestedWidth  = getWindowWidth();
     //windowH = requestedHeight = getWindowHeight();



	#ifdef TARGET_RASPBERRY_PI
		bool success = setupRPiNativeWindow(w,h,screenMode);
	#else 
		bool success = setupX11NativeWindow(w,h,screenMode);
	#endif

    if(!success) {
      cout << "CREATED screen failed " << w << " x " << h << endl;
    } else {
      cout << "CREATED SCREEN WITH SIZE " << w << " x " << h << endl;
    }

    nonFullscreenWindowRect = screenRect;
    currentWindowRect = screenRect;
    
    mouseCursor.allocate(mouse_cursor_data.width,mouse_cursor_data.height,OF_IMAGE_COLOR_ALPHA);
    MOUSE_CURSOR_RUN_LENGTH_DECODE(mouseCursor.getPixels(),mouse_cursor_data.rle_pixel_data,mouse_cursor_data.width*mouse_cursor_data.height,mouse_cursor_data.bpp);
    mouseCursor.update();
	bShowCursor = true;

}

//------------------------------------------------------------
bool ofAppEGLWindow::setupEGL(NativeWindowType nativeWindow, EGLNativeDisplayType * display)
{

    EGLBoolean result;
    EGLint num_config;
    EGLConfig config;

	ofLogNotice("ofAppEGLWindow::setupEGL") << "setting EGL Display";
    // get an EGL eglDisplay connection
    
    if(display==NULL){
    	ofLogNotice("ofAppEGLWindow::setupEGL") << "setting default Display";
    	eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    }else{
		ofLogNotice("ofAppEGLWindow::setupEGL") << "setting argument Display";
    	eglDisplay = eglGetDisplay(*display);
    }

    if(eglDisplay == EGL_NO_DISPLAY) {
		  ofLogError("ofAppEGLWindow::setupEGL") << "eglGetDisplay returned: " << eglDisplay;
		  return false;
    }else{
    	ofLogNotice("ofAppEGLWindow::setupEGL") << "EGL Display correctly set";
    }

    EGLint eglVersionMajor = 0;
    EGLint eglVersionMinor = 0;

    // initialize the EGL eglDisplay connection
	ofLogNotice("ofAppEGLWindow::setupEGL") << "eglInitialize";
    result = eglInitialize(eglDisplay, &eglVersionMajor, &eglVersionMinor);

    if(result == EGL_BAD_DISPLAY) {
//  eglDisplay is not an EGL connection
      ofLogError("ofAppEGLWindow::setupEGL") << "eglInitialize returned EGL_BAD_DISPLAY";
      return false;
    } else if(result == EGL_NOT_INITIALIZED) {
      // eglDisplay cannot be intitialized
      ofLogError("ofAppEGLWindow::setupEGL") << "eglInitialize returned EGL_NOT_INITIALIZED";
      return false;
    } else if(result == EGL_FALSE) {
      // eglinitialize was not initialiezd
      ofLogError("ofAppEGLWindow::setupEGL") << "eglInitialize returned EGL_FALSE";
      return false;
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
	ofLogNotice("ofAppEGLWindow::setupEGL") << "eglChooseConfig";
    result = eglChooseConfig(eglDisplay, 
                             attribute_list, 
                             &config, 
                             1, 
                             &num_config);

    assert(EGL_FALSE != result);

	ofLogNotice("ofAppEGLWindow::setupEGL") << "eglCreateWindowSurface";
    eglSurface = eglCreateWindowSurface( eglDisplay, config, nativeWindow, NULL );
    assert(eglSurface != EGL_NO_SURFACE);
    
    // create an EGL rendering eglContext
	ofLogNotice("ofAppEGLWindow::setupEGL") << "eglCreateContext";
    eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, NULL);
    assert(eglContext != EGL_NO_CONTEXT);


    // connect the eglContext to the eglSurface
	ofLogNotice("ofAppEGLWindow::setupEGL") << "eglMakeCurrent";
    result = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    assert(EGL_FALSE != result);

    // Set background color and clear buffers
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    glClear( GL_DEPTH_BUFFER_BIT );

    //ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer));

    return true;
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

    // loop it!
    infiniteLoop();
    
    // we got a terminate ... so clean up.
    destroyEGL();
}

//------------------------------------------------------------
void ofAppEGLWindow::infiniteLoop() {
	startThread();
    while (!terminate) {
    	callMouseEvents();	
    	idle();
    	display();
    }
}

//------------------------------------------------------------
void ofAppEGLWindow::callMouseEvents(){
	static queue<ofMouseEventArgs> copy;
	lock();
	copy = mouseEvents;
	unlock();
	while(!copy.empty()){
		ofNotifyMouseEvent(copy.front());
		copy.pop();
	}
}

//------------------------------------------------------------
void ofAppEGLWindow::hideCursor(){
	bShowCursor = false;
}

//------------------------------------------------------------
void ofAppEGLWindow::showCursor(){
	bShowCursor = true;
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
  
  if(bShowCursor){
  	ofBlendMode blend = ofGetStyle().blendingMode;
  	ofEnableAlphaBlending();
  	mouseCursor.draw(ofGetMouseX(),ofGetMouseY());
  	ofEnableBlendMode(blend);
  }

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


//------------------------------------------------------------
ofRectangle ofAppEGLWindow::getScreenRect(){
	return 	currentWindowRect;
}

//------------------------------------------------------------
ofRectangle ofAppEGLWindow::requestNewWindowRect(const ofRectangle& rect){
	setWindowPosition(rect.x,rect.y);
	setWindowShape(rect.width,rect.height);
	return getScreenRect();
}

//------------------------------------------------------------
void ofAppEGLWindow::threadedFunction(){
	getPocoThread().setOSPriority(Poco::Thread::getMinOSPriority());
	ofFile mouseFile("/dev/input/mouse0",ofFile::ReadOnly);    
	const int XSIGN = 1<<4, YSIGN = 1<<5;
    struct {char buttons, dx, dy; } m;
    ofMouseEventArgs mouseEvent;
    bool pushMouseEvent;
    bool mousePressed=false;
	while(isThreadRunning()){
		pushMouseEvent=false;
		while(1){
			mouseFile.read((char*)&m,sizeof(m));
	        if (m.buttons&8) {
	        	break; // This bit should always be set
	        }
	        mouseFile.read(&m.buttons,1); //try to sync
        	ofSleepMillis(5);
	    }
        if (m.buttons&3){
        	mouseEvent.button = m.buttons&3;
        	if(mousePressed){
        		mouseEvent.type = ofMouseEventArgs::Dragged;
        	}else{
        		mouseEvent.type = ofMouseEventArgs::Pressed;
        		pushMouseEvent = true;
        	}
        	mousePressed = true;
        }else{
        	if(mousePressed){
        		mouseEvent.type = ofMouseEventArgs::Released;
        		pushMouseEvent = true;
        	}else{
        		mouseEvent.type = ofMouseEventArgs::Moved;
        	}
        	mousePressed = false;
        }
        if(m.dx!=0 || m.dy!=0){
        	mouseEvent.x+=m.dx*2;
        	mouseEvent.y-=m.dy*2;
        	pushMouseEvent = true;
        }
        if (m.buttons&XSIGN){
        	mouseEvent.x-=256*2;
        	pushMouseEvent = true;
        }
        if (m.buttons&YSIGN){
        	mouseEvent.y+=256*2;
        	pushMouseEvent = true;
        }
        mouseEvent.x = ofClamp(mouseEvent.x,0,currentWindowRect.width);
        mouseEvent.y = ofClamp(mouseEvent.y,0,currentWindowRect.height);
        if(pushMouseEvent){
			lock();
		    mouseEvents.push(mouseEvent);
		    unlock();
		}
        ofSleepMillis(50);
	}
}
