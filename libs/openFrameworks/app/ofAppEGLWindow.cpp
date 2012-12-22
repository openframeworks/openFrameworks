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
#include "ofGLES2Renderer.h"
#include <assert.h>

// native events
struct udev         *udev;
struct udev_device  *dev;
struct udev_monitor *mon;
static int  udev_fd     = -1;

static int  keyboard_fd = -1; // defaults to 0 ie console
static int  mouse_fd    = -1; // defaults to 0 

// minimal map
const int lowercase_map[] = {
    0,  0,  '1',  '2',  '3',  '4',  '5', '6',  '7', '8', '9', '0',
    '-', '=', '\b', '\t', 'q',  'w',  'e', 'r',  't', 'y', 'u', 'i',
    'o', 'p', '[',  ']',  '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
    'j', 'k', 'l',  ';',  '\'',  '\n', 0,  '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm',  ',',  '.',  '/',  0,  '*',  0,  ' ', 0,  0,
    0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  0, 0,  0, 0,
    0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  0, 0,  0, 0,
    0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  '\r'
 
};

// minimal keyboard map
const int uppercase_map[] = {
    0,  0,  '!',  '@',  '#',  '$',  '%', '^',  '&', '*', '(', ')',
    '_', '+', '\b', '\t', 'Q',  'W',  'E', 'R',  'T', 'Y', 'U', 'I',
    'O', 'P', '{',  '}',  '\n', 0,   'A', 'S',  'D', 'F', 'G', 'H',
    'J', 'K', 'L',  ':',  '"', '\n', 0,  '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M',  '<',  '>',  '?',  0,  '*',  0,  ' ', 0,  0,
    0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  0, 0,  0, 0,
    0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  0, 0,  0, 0,
    0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  '\r'
};

// keep track of a few things ...
typedef struct {
    bool shiftPressed;
    bool capsLocked;
} KeyboardState;

static KeyboardState kb;

static struct termios tc;
static struct termios ots;

typedef struct {
    int   mouseButtonState;
} MouseState;

#define MOUSE_BUTTON_LEFT_MASK        1
#define MOUSE_BUTTON_MIDDLE_MASK 1 << 1
#define MOUSE_BUTTON_RIGHT_MASK  2 << 1

static MouseState mb;

static int string_ends_with(const char *str, const char *suffix) {
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

static int dummy_sort(const struct dirent **a,const struct dirent **b) {
    return 1; // dummy sort
}

static int filter_kbd(const struct dirent *d) {
    if(d->d_type != DT_DIR && string_ends_with(d->d_name,"event-kbd")) {
        return 1;
    } else {
        return 0;
    }
}

static int filter_mouse(const struct dirent *d) {
    if(d->d_type != DT_DIR && string_ends_with(d->d_name,"event-mouse")) {
        return 1;
    } else {
        return 0;
    }
}

// native
#define MOUSE_CURSOR_RUN_LENGTH_DECODE(image_buf, rle_data, size, bpp) do \
{ unsigned int __bpp; unsigned char *__ip; const unsigned char *__il, *__rd; \
  __bpp = (bpp); __ip = (image_buf); __il = __ip + (size) * __bpp; \
  __rd = (rle_data); \
    while (__ip < __il) { unsigned int __l = *(__rd++); \
      if (__l & 128) { __l = __l - 128; \
        do { memcpy (__ip, __rd, 4); __ip += 4; } while (--__l); __rd += 4; \
      } else { __l *= 4; memcpy (__ip, __rd, __l); \
               __ip += __l; __rd += __l; } } \
} while (0)
static const struct {
  unsigned int   width;
  unsigned int   height;
  unsigned int   bpp; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char  rle_pixel_data[382 + 1];
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


// X11 events
  #include <X11/XKBlib.h>


//-------------------------------------------------------------------------------------
ofAppEGLWindow::Settings::Settings() {
  eglWindowPreference = OF_APP_WINDOW_AUTO;
  eglWindowOpacity = 255;

  eglRedSize   = 8; // 8 bits for red
  eglGreenSize = 8; // 8 bits for green
  eglBlueSize  = 8; // 8 bits for blue
  eglAlphaSize = 8; // 8 bits for alpha

  eglSurfaceType = EGL_WINDOW_BIT; // default eglSurface type

  initialClearColor = ofColor(0.15 * 255, 0.15 * 255, 0.15 * 255, 255);

}

// TODO. we may not need these to be static, but we will
// leave it this way for now in case future EGL windows 
// use static callbacks (like glut)

//------------------------------------------------------------
ofAppEGLWindow::ofAppEGLWindow() {
  init();
}

//------------------------------------------------------------
ofAppEGLWindow::ofAppEGLWindow(Settings _settings) {
  init(_settings);
}

//------------------------------------------------------------
ofAppEGLWindow::~ofAppEGLWindow() {
  ofRemoveListener(ofEvents().exit, this, &ofAppEGLWindow::exit);
}

//------------------------------------------------------------
void ofAppEGLWindow::init(Settings _settings) {
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

    //TODO: 2.0f is an arbitrary factor that makes mouse speed ok at 1024x768,
    // to be totally correct we might need to take into account screen size
    // and add acceleration
    mouseScaleX = 2.0f;
    mouseScaleY = 2.0f;

    isUsingX11 = false;

    // APPLY SETTINGS
    settings = _settings;

    ofAddListener(ofEvents().exit, this, &ofAppEGLWindow::exit);
}

//------------------------------------------------------------
void ofAppEGLWindow::exit(ofEventArgs &e) {
  terminate = true; // TODO, it is unlikely that this will happen
  if(!isUsingX11) {
    destroyNativeEvents();
  }   

  // we got a terminate ... so clean up.
  destroyEGL();
}

//------------------------------------------------------------
void ofAppEGLWindow::setupOpenGL(int w, int h, int screenMode) {

     windowMode = screenMode;
     bNewScreenMode = true;

     if(windowMode == OF_GAME_MODE) {
         ofLogWarning("ofAppEGLWindow") << "OF_GAME_MODE not supported.";
     }

     //windowW = requestedWidth  = getWindowWidth();
     //windowH = requestedHeight = getWindowHeight();

	   bool success = setupNativeWindow(w,h,screenMode);

    if(!success) {
      ofLogNotice("ofAppEGLWindow::setupOpenGL")  << "CREATED screen failed " << w << " x " << h;
    } else {
      ofLogNotice("ofAppEGLWindow::setupOpenGL")  << "CREATED SCREEN WITH SIZE " << w << " x " << h;
    }

    nonFullscreenWindowRect = screenRect;
    currentWindowRect = screenRect;
    
    setupPeripherals();

  	bShowCursor = true;

}

//------------------------------------------------------------
void ofAppEGLWindow::setupPeripherals() {
    if(!isUsingX11) {
        // roll our own cursor!
        mouseCursor.allocate(mouse_cursor_data.width,mouse_cursor_data.height,OF_IMAGE_COLOR_ALPHA);
        MOUSE_CURSOR_RUN_LENGTH_DECODE(mouseCursor.getPixels(),mouse_cursor_data.rle_pixel_data,mouse_cursor_data.width*mouse_cursor_data.height,mouse_cursor_data.bpp);
        mouseCursor.update();
    } else {
        ofLogError("ofAppEGLWindow") << "Peripherals not supported on X11";
    }
}

//------------------------------------------------------------
bool ofAppEGLWindow::setupNativeWindow(int w, int h, int screenMode) {
    // X11 check
    // char * pDisplay;
    // pDisplay = getenv ("DISPLAY");
    // bool bIsX11Available = (pDisplay != NULL);

    bool bIsX11Available = getenv("DISPLAY") != NULL;

    if(settings.eglWindowPreference == OF_APP_WINDOW_AUTO) {
        if(bIsX11Available) {
            isUsingX11 = true;
        } else {
            isUsingX11 = false;
        }
    } else if(settings.eglWindowPreference == OF_APP_WINDOW_NATIVE) {
        isUsingX11 = false;
    } else if(settings.eglWindowPreference == OF_APP_WINDOW_X11) {
        isUsingX11 = true;
        if(!bIsX11Available) {
            isUsingX11 = false;
            ofLogError("ofAppEGLWindow") << "X11 Window requested, but X11 is not available.";
        }
    }

    ////////////////
    // TODO remove the following ifdef once x11 is accelerated on RPI
    #ifdef TARGET_RASPBERRY_PI
    if(isUsingX11) {
        isUsingX11 = false;
        ofLogWarning("ofAppEGLWindow") << "I'm sorry, I know you wanted X11, but it's not availble on RPI yet.  Using a native window instead.";
    }
    #endif
    ////////////////


    if(isUsingX11) {
        return setupX11NativeWindow(w,h,screenMode);
    } else {
        #ifdef TARGET_RASPBERRY_PI
          return setupRPiNativeWindow(w,h,screenMode);
        #else
          ofLogError("ofAppEGLWindow") << "There is no native window type for this system. Perhaps try X11?";
          return false;
        #endif
    }
}

//------------------------------------------------------------
bool ofAppEGLWindow::setupEGL(NativeWindowType nativeWindow, EGLNativeDisplayType * display) {

  EGLBoolean result;
  EGLint num_config;
  EGLConfig config;

	ofLogNotice("ofAppEGLWindow::setupEGL") << "setting EGL Display";
    // get an EGL eglDisplay connection
    
    if(display==NULL){
    	eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    }else{
    	eglDisplay = eglGetDisplay(*display);
    }

    if(eglDisplay == EGL_NO_DISPLAY) {
	   ofLogError("ofAppEGLWindow::setupEGL") << "eglGetDisplay returned: " << eglDisplay;
	   return false;
    }else{
    	ofLogNotice("ofAppEGLWindow::setupEGL") << "EGL Display correctly set.";
    }

    EGLint eglVersionMajor = 0;
    EGLint eglVersionMinor = 0;

    // initialize the EGL eglDisplay connection
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
    EGLint glesVersion;
    int glesVersionForContext;

    if(ofGetCurrentRenderer()) {
      ofLogNotice("ofAppEGLWindow::setupEGL") << "ofGetCurrentRenderer()->getType()=" << ofGetCurrentRenderer()->getType();
    } else {
      ofLogNotice("ofAppEGLWindow::setupEGL") << "No current render selected.";
    }

    if(ofGetCurrentRenderer() && ofGetCurrentRenderer()->getType()=="GLES2"){
        glesVersion = EGL_OPENGL_ES2_BIT;
	   	glesVersionForContext = 2;
        ofLogNotice("ofAppEGLWindow::setupEGL") << "GLES2 Renderer detected.";
    }else{
		  glesVersion = EGL_OPENGL_ES_BIT;
		  glesVersionForContext = 1;
      ofLogNotice("ofAppEGLWindow::setupEGL") << "Default Renderer detected.";
    }
    
    EGLint attribute_list[] = {
		    EGL_RED_SIZE,        settings.eglRedSize, // 8 bits for red
		    EGL_GREEN_SIZE,      settings.eglGreenSize, // 8 bits for green
		    EGL_BLUE_SIZE,       settings.eglBlueSize, // 8 bits for blue
		    EGL_ALPHA_SIZE,      settings.eglAlphaSize, // 8 bits for alpha
		    EGL_SURFACE_TYPE,    settings.eglSurfaceType, // default eglSurface type
		    EGL_RENDERABLE_TYPE, glesVersion, //openGL ES version
		    EGL_NONE // attribute list is termintated with EGL_NONE
		};

    // get an appropriate EGL frame buffer configuration
    result = eglChooseConfig(eglDisplay, 
                             attribute_list, 
                             &config, 
                             1, 
                             &num_config);

    assert(EGL_FALSE != result);
    
    eglSurface = eglCreateWindowSurface( eglDisplay, config, nativeWindow, NULL );
    assert(eglSurface != EGL_NO_SURFACE);
    
    
	// get an appropriate EGL frame buffer configuration
	//result = eglBindAPI(EGL_OPENGL_ES_API);
	//assert(EGL_FALSE != result);
    
    // create an EGL rendering eglContext
    
    EGLint contextAttribList[] = 
	{
		EGL_CONTEXT_CLIENT_VERSION, glesVersionForContext,
		EGL_NONE
	};

    eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, contextAttribList);
    assert(eglContext != EGL_NO_CONTEXT);


    // connect the eglContext to the eglSurface
    result = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    assert(EGL_FALSE != result);

    // Set background color and clear buffers
    glClearColor(settings.initialClearColor.r / 255.0f,
                 settings.initialClearColor.g / 255.0f,
                 settings.initialClearColor.b / 255.0f,
                 settings.initialClearColor.a / 255.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    glClear( GL_DEPTH_BUFFER_BIT );

    //ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer));
    
    if(glesVersionForContext==2){
  		ofLogNotice("ofAppEGLWindow::setupEGL") << "OpenGL ES version " << glGetString(GL_VERSION) << endl;
  		ofGLES2Renderer* renderer = (ofGLES2Renderer*)ofGetCurrentRenderer().get();
  		renderer->setup();
    }

    ofLogNotice("ofAppEGLWindow::setupEGL") << "-----EGL-----";
    ofLogNotice("ofAppEGLWindow::setupEGL") << "EGL_VERSION_MAJOR = " << eglVersionMajor;
    ofLogNotice("ofAppEGLWindow::setupEGL") << "EGL_VERSION_MINOR = " << eglVersionMinor;
    ofLogNotice("ofAppEGLWindow::setupEGL") << "EGL_CLIENT_APIS = " << eglQueryString(eglDisplay, EGL_CLIENT_APIS);
    ofLogNotice("ofAppEGLWindow::setupEGL") << "EGL_VENDOR = "  << eglQueryString(eglDisplay, EGL_VENDOR);
    ofLogNotice("ofAppEGLWindow::setupEGL") << "EGL_VERSION = " << eglQueryString(eglDisplay, EGL_VERSION);
    ofLogNotice("ofAppEGLWindow::setupEGL") << "EGL_EXTENSIONS = " << eglQueryString(eglDisplay, EGL_EXTENSIONS);
    ofLogNotice("ofAppEGLWindow::setupEGL") << "GL_RENDERER = " << glGetString(GL_RENDERER);
    ofLogNotice("ofAppEGLWindow::setupEGL") << "GL_VERSION  = " << glGetString(GL_VERSION);
    ofLogNotice("ofAppEGLWindow::setupEGL") << "GL_VENDOR   = " << glGetString(GL_VENDOR);
    ofLogNotice("ofAppEGLWindow::setupEGL") << "-------------";


    return true;
}

//------------------------------------------------------------
void ofAppEGLWindow::destroyEGL() {
    ofLogNotice("ofAppEGLWindow::destroyEGL") << "Destroying EGL window.";
    eglMakeCurrent( eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
    eglDestroySurface( eglDisplay, eglSurface );
    eglDestroyContext( eglDisplay, eglContext );
    eglTerminate( eglDisplay );
}

//------------------------------------------------------------
void ofAppEGLWindow::initializeWindow() {
    nFramesSinceWindowResized = 0;
}

//------------------------------------------------------------
void ofAppEGLWindow::runAppViaInfiniteLoop(ofBaseApp *appPtr) {
    ofAppPtr = appPtr; // make a local copy
   
    ofNotifySetup();

    if(!isUsingX11) {
        setupNativeEvents();
    }

    // loop it!
    while (!terminate) {
      checkEvents();  
      idle();
      display();
    }
}

//------------------------------------------------------------
bool ofAppEGLWindow::setupNativeEvents() {
  setupNativeUDev();
  setupNativeMouse();
  setupNativeKeyboard();
  startThread();
}

//------------------------------------------------------------
bool ofAppEGLWindow::destroyNativeEvents() {
  destroyNativeUDev();
  destroyNativeMouse(); 
  destroyNativeKeyboard(); 
  waitForThread(true);
}

//------------------------------------------------------------
void ofAppEGLWindow::setWindowRect(const ofRectangle& requestedWindowRect) {
    if(requestedWindowRect != currentWindowRect) {
        ofRectangle oldWindowRect = currentWindowRect;

        currentWindowRect = requestNewWindowRect(requestedWindowRect);
        
        if(oldWindowRect != currentWindowRect) {
            ofNotifyWindowResized(currentWindowRect.width,currentWindowRect.height);
            nFramesSinceWindowResized = 0;
        }
    }
}

//------------------------------------------------------------
int ofAppEGLWindow::getWindowWidth() {
    return currentWindowRect.width;
}

//------------------------------------------------------------
int ofAppEGLWindow::getWindowHeight() {
    return currentWindowRect.height;
}

//------------------------------------------------------------
void ofAppEGLWindow::checkEvents(){

    if(isUsingX11) {
        while(1){
            XEvent event;
            if (::XCheckWindowEvent(x11Display, x11Window, -1, &event)){
                handleX11Event(event);
            }else if (::XCheckTypedEvent(x11Display, ClientMessage, &event)){
                handleX11Event(event);
            }else{
                break;
            }
        }    
    } else {
        static queue<ofMouseEventArgs> mouseEventsCopy;
        lock();
        mouseEventsCopy = mouseEvents;
        while(!mouseEvents.empty()){
          mouseEvents.pop();
        }
        unlock();
        while(!mouseEventsCopy.empty()){
          ofNotifyMouseEvent(mouseEventsCopy.front());
          mouseEventsCopy.pop();
        }

        // KEYBOARD EVENTS
        static queue<ofKeyEventArgs> keyEventsCopy;
        lock();
        keyEventsCopy = keyEvents;
        while(!keyEvents.empty()){
          keyEvents.pop();
        }
        unlock();
        while(!keyEventsCopy.empty()){
          ofNotifyKeyEvent(keyEventsCopy.front());
          keyEventsCopy.pop();
        }
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
  if( windowMode == OF_GAME_MODE) return;

  if( windowMode == OF_WINDOW ){
    setFullscreen(true);
  }else{
    setFullscreen(false);
  }

}

//------------------------------------------------------------
void ofAppEGLWindow::setFullscreen(bool fullscreen){
    if( windowMode == OF_GAME_MODE) return;

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
 

	if(ofGetCurrentRenderer()->getType()=="GLES2"){
		ofGLES2Renderer* renderer = (ofGLES2Renderer*)ofGetCurrentRenderer().get();
		renderer->startRender();
	}

  ofViewport(0, 0, getWindowWidth(), getWindowHeight());    // used to be glViewport( 0, 0, width, height );
  
  float * bgPtr = ofBgColorPtr();
  bool bClearAuto = ofbClearBg();

  if ( bClearAuto == true || nFrameCount < 3){
    ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
  }

  if( bEnableSetupScreen ) ofSetupScreen(); // this calls into the current renderer (ofSetupScreenPerspective)

  ofNotifyDraw();
  
  if(!isUsingX11) {
    if(bShowCursor){
        ofPushStyle();
        ofEnableAlphaBlending();
        ofDisableTextureEdgeHack();
        ofSetColor(255);
        mouseCursor.draw(ofGetMouseX(),ofGetMouseY());
        ofEnableTextureEdgeHack();
        //TODO: we need a way of querying the previous state of texture hack
        ofPopStyle();
    }
   }
 
	if(ofGetCurrentRenderer()->getType()=="GLES2"){
		ofGLES2Renderer* renderer = (ofGLES2Renderer*)ofGetCurrentRenderer().get();
		renderer->finishRender();
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
void ofAppEGLWindow::setVerticalSync(bool enabled){
	eglSwapInterval(eglDisplay, enabled ? 1 : 0);
}

//------------------------------------------------------------
void ofAppEGLWindow::threadedFunction(){
    // set the thread to low priority
    getPocoThread().setOSPriority(Poco::Thread::getMinOSPriority());

    // TODO: a way to setup mouse and keyboard if 
    // they are not plugged in upon start
    // This can be done with our udev device callbacks

    while(isThreadRunning()) {
        readNativeUDevEvents();
        readNativeMouseEvents();
        readNativeKeyboardEvents();

        // sleep briefly
        ofSleepMillis(20);
	}
}
 
//------------------------------------------------------------
// PLATFORM SPECIFIC RPI
//------------------------------------------------------------

//------------------------------------------------------------
bool ofAppEGLWindow::setupNativeUDev() {
 
    udev = udev_new(); // create new udev object
    if(!udev) {
        ofLogError("ofAppEGLWindow") << "setupUDev() : Can't create udev object.";
    } else {
        ofLogNotice("ofAppEGLWindow") << "setupUDev() : Created udev object.";
        // setup udev to monitor for input devices
        mon = udev_monitor_new_from_netlink(udev, "udev");
        // just listen for input devices
        udev_monitor_filter_add_match_subsystem_devtype(mon, "input", NULL);
        udev_monitor_enable_receiving(mon);
        // get the file descriptor for the mon (used w/ select);
        udev_fd = udev_monitor_get_fd(mon);
    }

    return udev_fd > -1;
}

//------------------------------------------------------------
bool ofAppEGLWindow::destroyNativeUDev() {
    udev_unref(udev); // clean up
}


//------------------------------------------------------------
bool ofAppEGLWindow::setupNativeMouse() {
    struct dirent **eps;
    int n = scandir("/dev/input/by-path/", &eps, filter_mouse, dummy_sort);

    // make sure that we found an appropriate entry
    if(n >= 0 && eps != 0 && eps[0] != 0) {
        char devicePathBuffer[256];
        sprintf(devicePathBuffer,"/dev/input/by-path/%s\0",eps[0]->d_name);
        mouse_fd = open(devicePathBuffer, O_RDONLY | O_NONBLOCK);
        ofLogNotice("ofAppEGLWindow") << "setupMouse() : mouse_fd= " <<  mouse_fd << " devicePath=" << devicePathBuffer;
    } else {
        ofLogNotice("ofAppEGLWindow") << "setupMouse() : Unabled to find mouse.";
    }

    if (mouse_fd >= 0) {
        char deviceNameBuffer[256] = "Unknown Device";
        ioctl(mouse_fd, EVIOCGNAME(sizeof(deviceNameBuffer)), deviceNameBuffer);
        ofLogNotice("ofAppEGLWindow") << "setupMouse() : mouse device name = " << deviceNameBuffer;
    } else {
        ofLogError("ofAppEGLWindow") << "setupMouse() : did not open mouse.";
    }

    mb.mouseButtonState = 0;

    return mouse_fd > -1;
}

//------------------------------------------------------------
bool ofAppEGLWindow::setupNativeKeyboard() {
    struct dirent **eps;
    int n = scandir("/dev/input/by-path/", &eps, filter_kbd, dummy_sort);

    // make sure that we found an appropriate entry
    if(n >= 0 && eps != 0 && eps[0] != 0) {
        char devicePathBuffer[256];
        sprintf(devicePathBuffer,"/dev/input/by-path/%s\0",eps[0]->d_name);
        keyboard_fd=open(devicePathBuffer, O_RDONLY | O_NONBLOCK);
        ofLogNotice("ofAppEGLWindow") << "setupKeyboard() : keyboard_fd= " <<  mouse_fd << " devicePath=" << devicePathBuffer;
    } else {
        ofLogWarning("ofAppEGLWindow") << "setupKeyboard() : Unabled to find keyboard.";
    }

    if (keyboard_fd >= 0) {
        char deviceNameBuffer[256] = "Unknown Device";
        ioctl(keyboard_fd, EVIOCGNAME(sizeof(deviceNameBuffer)), deviceNameBuffer);
        ofLogNotice("ofAppEGLWindow") << "setupKeyboard() : keyboard device name = " << deviceNameBuffer;
    

        // save current terminal settings
        tcgetattr (STDIN_FILENO, &tc);
        ots = tc;
        // disable echo on our temporary settings
        tc.c_lflag &= ~ECHO;
        tc.c_lflag |= ECHONL;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &tc);

    } else {
        ofLogError("ofAppEGLWindow") << "setupKeyboard() : did not open keyboard.";
    }

    kb.shiftPressed = false;
    kb.capsLocked = false;
    
    return keyboard_fd > -1;
}

//------------------------------------------------------------
bool ofAppEGLWindow::destroyNativeMouse() {
    if(mouse_fd >= 0) {
        // nothing to do
    }
}

//------------------------------------------------------------
bool ofAppEGLWindow::destroyNativeKeyboard() {
    ofLogNotice("ofAppEGLWindow") << "destroyNativeKeyboard()";

    if (keyboard_fd >= 0) {
      tcsetattr (STDIN_FILENO, TCSAFLUSH, &ots);
    } else {
      ofLogNotice("ofAppEGLWindow") << "destroyNativeKeyboard() : unable to reset terminal";
    }
}


//------------------------------------------------------------
bool ofAppEGLWindow::readNativeUDevEvents() {
    // look for devices being attatched / detatched

    fd_set fds;
    struct timeval tv;
    int ret;

    FD_ZERO(&fds);
    FD_SET(udev_fd, &fds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    ret = select(udev_fd+1, &fds, NULL, NULL, &tv);

    /* Check if our file descriptor has received data. */
    if (ret > 0 && FD_ISSET(udev_fd, &fds)) {
        /* Make the call to receive the device.
           select() ensured that this will not block. */
        dev = udev_monitor_receive_device(mon);
        if (dev) {
            printf("Got Device\n");
            printf("   Node: %s\n", udev_device_get_devnode(dev));
            printf("   Subsystem: %s\n", udev_device_get_subsystem(dev));
            printf("   Devtype: %s\n", udev_device_get_devtype(dev));
            printf("   Action: %s\n", udev_device_get_action(dev));
            udev_device_unref(dev);
        }
        else {
            printf("No Device from receive_device(). An error occured.\n");
        }                   
    }
}

//------------------------------------------------------------
bool ofAppEGLWindow::readNativeKeyboardEvents() {
    // http://www.diegm.uniud.it/loghi/CE2/kbd.pdf
    // http://cgit.freedesktop.org/~whot/evtest/plain/evtest.c
    // https://strcpy.net/b/archives/2010/11/17/abusing_the_linux_input_subsystem/index.html
    struct input_event ev;
    char key = 0;

    int nBytesRead = read(keyboard_fd, &ev,sizeof(struct input_event));

    static ofKeyEventArgs keyEvent;
    bool pushKeyEvent = false;

    while(nBytesRead >= 0) {

        if (ev.type==EV_KEY) {
            if(ev.value == 0) {
                // key released
                keyEvent.type = ofKeyEventArgs::Released;
            } else if(ev.value == 1) {
                // key pressed
                keyEvent.type = ofKeyEventArgs::Pressed;
            } else if(ev.value == 2) {
                // key repeated
                keyEvent.type = ofKeyEventArgs::Pressed;
            } else {
                // unknown ev.value
            }

            bool shouldMapKey = false;

            switch (ev.code) {
                case KEY_RIGHTSHIFT:
                case KEY_LEFTSHIFT:
                    kb.shiftPressed = ev.value;
                    break;
                case KEY_RIGHTCTRL:
                case KEY_LEFTCTRL:
                    break;
            case KEY_CAPSLOCK:
                if (ev.value == 1) {
                    if (kb.capsLocked) {
                        kb.capsLocked = 0;
                    } else {
                        kb.capsLocked = 1;
                    }
                }
                break;

            case KEY_ESC:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_ESC;
                break;
            case KEY_BACKSPACE:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_BACKSPACE;
                break;
            case KEY_DELETE:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_DEL;
                break;
            case KEY_F1:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F1;
                break;
            case KEY_F2:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F2;
                break;
            case KEY_F3:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F3;
                break;
            case KEY_F4:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F4;
                break;
            case KEY_F5:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F5;
                break;
            case KEY_F6:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F6;
                break;
            case KEY_F7:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F7;
                break;
            case KEY_F8:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F8;
                break;
            case KEY_F9:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F9;
                break;
            case KEY_F10:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F10;
                break;
            case KEY_F11:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F11;
                break;
            case KEY_F12:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_F12;
                break;
            case KEY_LEFT:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_LEFT;
                break;
            case KEY_UP:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_UP;
                break;
            case KEY_RIGHT:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_RIGHT;
                break;
            case KEY_DOWN:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_DOWN;
                break;
            case KEY_PAGEUP:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_PAGE_UP;
                break;
            case KEY_PAGEDOWN:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_PAGE_DOWN;
                break;
            case KEY_HOME:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_HOME;
                break;
            case KEY_END:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_END;
                break;
            case KEY_INSERT:
                pushKeyEvent = true;
                keyEvent.key = OF_KEY_INSERT;
                break;

            default:
            // VERY RUDIMENTARY KEY MAPPING WITH MAPS ABOVE
                if(ev.code < sizeof(lowercase_map)) {
                    if (kb.shiftPressed) {
                        key = uppercase_map[ev.code];
                        if (kb.capsLocked) keyEvent.key = tolower(key);
                        keyEvent.key = key;
                        pushKeyEvent = true;
                    } else {
                        key = lowercase_map[ev.code];
                        if (kb.capsLocked) key = toupper(key);
                        keyEvent.key = key;
                        pushKeyEvent = true;
                    }
                } else {
                    ofLogNotice("ofAppEGLWindow") << "readKeyboardEvents() : input_event.code is outside of our small range.";
                }
            }
        } else if(ev.type == EV_MSC) {
            // EV_MSC events are used for input and output events that 
            // do not fall under other categories.
            // ofLogVerbose("ofAppEGLWindow") << "readKeyboardEvents() : EV_MSC";
        } else if(ev.type == EV_SYN ) {
            // EV_SYN Used as markers to separate events. Events may be 
            // separated in time or in space, such as with the multitouch protocol.
            // ofLogVerbose("ofAppEGLWindow") << "readKeyboardEvents() : EV_SYN";
        } else {
            // unhandled type
        }

        // do we have a mouse svent to push?
        if(pushKeyEvent){
            lock();
            keyEvents.push(keyEvent);
            unlock();
            pushKeyEvent = false;
        }

        nBytesRead = read(keyboard_fd, &ev,sizeof(struct input_event));
    }

    return true;
}

//------------------------------------------------------------
bool ofAppEGLWindow::readNativeMouseEvents() {
    // http://cgit.freedesktop.org/~whot/evtest/plain/evtest.c
    struct input_event ev;

    static ofMouseEventArgs mouseEvent;

    bool pushMouseEvent = false;

    int nBytesRead = read(mouse_fd, &ev,sizeof(struct input_event));

    bool axisValuePending = false;

    while(nBytesRead >= 0) {

        if(ev.type == EV_REL || ev.type == EV_ABS) {
            int axis = ev.code;
            int amount = ev.value;

            switch(axis) {
                case 0:
                    if(ev.type == EV_REL) {
                        mouseEvent.x += amount * mouseScaleX; 
                    } else {
                        mouseEvent.x = amount * mouseScaleX;
                    }

                    mouseEvent.x = ofClamp(mouseEvent.x, 0, currentWindowRect.width);
                    axisValuePending = true;
                    break;
                case 1:
                    if(ev.type == EV_REL) {
                        mouseEvent.y += amount * mouseScaleY; 
                    } else {
                        mouseEvent.y = amount * mouseScaleY;
                    }

                    mouseEvent.y = ofClamp(mouseEvent.y, 0, currentWindowRect.height);
                    axisValuePending = true;
                    break;
                default:
                    ofLogNotice("ofAppEGLWindow") << "readMouseEvents() : Unknown mouse axis (perhaps it's the scroll wheel?)";
                    break;
             }

        } else if(ev.type == EV_KEY) {
            // only tracking three buttons now ...
            if(ev.code == BTN_LEFT) {
                if(ev.value == 0) { // release
                    mouseEvent.button = mb.mouseButtonState;
                    mouseEvent.type = ofMouseEventArgs::Released;
                    mb.mouseButtonState &= ~MOUSE_BUTTON_LEFT_MASK;
                    pushMouseEvent = true;
                } else if(ev.value == 1) { // press
                    mb.mouseButtonState |= MOUSE_BUTTON_LEFT_MASK;
                    mouseEvent.type = ofMouseEventArgs::Pressed;
                    mouseEvent.button = mb.mouseButtonState;
                    pushMouseEvent = true;
                } else { // unknown
                    ofLogNotice("ofAppEGLWindow") << "readMouseEvents() : EV_KEY : Unknown ev.value = " << ev.value;
                }
            } else if(ev.code == BTN_MIDDLE) {
                if(ev.value == 0) { // release
                    mouseEvent.button = mb.mouseButtonState;
                    mouseEvent.type = ofMouseEventArgs::Released;
                    mb.mouseButtonState &= ~MOUSE_BUTTON_MIDDLE_MASK;
                    pushMouseEvent = true;
                } else if(ev.value == 1) { // press
                    mb.mouseButtonState |= MOUSE_BUTTON_MIDDLE_MASK;
                    mouseEvent.type = ofMouseEventArgs::Pressed;
                    mouseEvent.button = mb.mouseButtonState;
                    pushMouseEvent = true;
                } else { // unknown
                    ofLogNotice("ofAppEGLWindow") << "readMouseEvents() : EV_KEY : Unknown ev.value = " << ev.value;
                }
            } else if(ev.code == BTN_RIGHT) {
                if(ev.value == 0) { // release
                    mouseEvent.button = mb.mouseButtonState;
                    mouseEvent.type = ofMouseEventArgs::Released;
                    mb.mouseButtonState &= ~MOUSE_BUTTON_RIGHT_MASK;
                    pushMouseEvent = true;
                } else if(ev.value == 1) { // press
                    mb.mouseButtonState |= MOUSE_BUTTON_RIGHT_MASK;
                    mouseEvent.type = ofMouseEventArgs::Pressed;
                    mouseEvent.button = mb.mouseButtonState;
                    pushMouseEvent = true;
                } else {
                    ofLogNotice("ofAppEGLWindow") << "readMouseEvents() : EV_KEY : Unknown ev.value = " << ev.value;
                }
            } else {
                ofLogNotice("ofAppEGLWindow") << "readMouseEvents() : EV_KEY : Unknown ev.code = " << ev.code;
            }
            // not sure why we are getting that event here
        } else if(ev.type == EV_MSC) {
            // EV_MSC events are used for input and output events that 
            // do not fall under other categories.
            // ofLogVerbose("ofAppEGLWindow") << "readMouseEvents() : EV_MSC";
        } else if(ev.type == EV_SYN ) {
            // EV_SYN Used as markers to separate events. Events may be 
            // separated in time or in space, such as with the multitouch protocol.

            // EV_SYN events are sent when axis value (one or a pair) are changed
            if(axisValuePending) {
                // TODO, this state doesn't make as much sense when the mouse is not dragging
                if(mb.mouseButtonState > 0) {
                    // dragging (what if dragging w/ more than one button?)
                    mouseEvent.type = ofMouseEventArgs::Dragged;
                } else {
                    // just moving
                    mouseEvent.type = ofMouseEventArgs::Moved;
                }
                
                mouseEvent.button = mb.mouseButtonState;

                pushMouseEvent = true;
                axisValuePending = false;
            }

            //ofLogVerbose("ofAppEGLWindow") << "readMouseEvents() : EV_SYN";
        } else {
            // unhandled type
        }

        // do we have a mouse event to push?
        if(pushMouseEvent){
            // lock the thread for a moment while we copy the data
            lock();
            mouseEvents.push(mouseEvent);
            unlock();
            pushMouseEvent = false;
        }

        nBytesRead = read(mouse_fd, &ev,sizeof(struct input_event));
    }

    return true;

}


#ifdef TARGET_RASPBERRY_PI
//------------------------------------------------------------
bool ofAppEGLWindow::setupRPiNativeWindow(int w, int h, int screenMode){

  bcm_host_init();

  //boolean force HDMI vs. composite

  int32_t success = 0;

  uint32_t sw;
  uint32_t sh;

  // create an EGL window surface
  // IF SCREENMODE==FULLSCREEN
  success = graphics_get_display_size(0 /* LCD */, &sw, &sh);

  if(success < 0) {
    ofLogError("ofAppEGLWindow") << "setupRPiNativeWindow() : tried to get display, but failed.";
    return false;
  }

  ofLogNotice("ofAppEGLWindow::setupRPiNativeWindow") << "Requested Screen Size w=" << w << " and  h=" << h;
  ofLogNotice("ofAppEGLWindow::setupRPiNativeWindow") << "Hardware Screen Size sw=" << sw << " and  sh=" << sh;

  if(screenMode == OF_WINDOW) {
    sw = MIN(sw,w);
    sh = MIN(sh,h);
  } else {
    // OF_FULLSCREEN and GAME take the screen size 
  }

  ofLogNotice("ofAppEGLWindow::setupRPiNativeWindow") << "Final Screen Size sw=" << sw << " and  sh=" << sh;


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

    VC_DISPMANX_ALPHA_T rpiNativeWindowAlpha;
    rpiNativeWindowAlpha.flags = DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS;
    rpiNativeWindowAlpha.opacity = ofClamp(settings.eglWindowOpacity,0,255);
    rpiNativeWindowAlpha.mask = 0;

    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update  = vc_dispmanx_update_start( 0 );

    dispman_element = vc_dispmanx_element_add ( dispman_update, 
                                              dispman_display,
                                              0/*layer*/, 
                                              &dst_rect, 
                                              0/*src*/,
                                              &src_rect, 
                                              DISPMANX_PROTECTION_NONE, 
                                              &rpiNativeWindowAlpha, 
                                              0/*clamp*/, 
                                              (DISPMANX_TRANSFORM_T)0/*transform*/
                                              );

    rpiNativeWindow.element = dispman_element;
    rpiNativeWindow.width = sw;
    rpiNativeWindow.height = sh;

    vc_dispmanx_update_submit_sync( dispman_update );

    bool ret = setupEGL(&rpiNativeWindow,NULL);

    if(ret){
        screenRect.x = 0;
        screenRect.y = 0;
        screenRect.width = rpiNativeWindow.width;
        screenRect.height = rpiNativeWindow.height;
    }

    return ret;    
}
#endif

//------------------------------------------------------------
// X11 BELOW
//------------------------------------------------------------
bool ofAppEGLWindow::setupX11NativeWindow(int w, int h, int screenMode){

  // X11 variables
  x11Window      = 0;
  x11Display     = 0;
  long x11Screen = 0;
  XVisualInfo*  x11Visual   = 0;
  Colormap      x11Colormap = 0;
  /*
    Step 0 - Create a NativeWindowType that we can use it for OpenGL ES output
  */
  Window               sRootWindow;
  XSetWindowAttributes sWA;
  unsigned int         ui32Mask;
  int                  i32Depth;
  
  // Initializes the display and screen
  x11Display = XOpenDisplay( 0 );
  if (!x11Display) {
    ofLogError("ofAppEGLWindow")<< "Unable to open X display.";
    return false;
  }
 
  x11Screen = XDefaultScreen( x11Display );

  // Gets the window parameters
  sRootWindow = RootWindow(x11Display, x11Screen);
  i32Depth = DefaultDepth(x11Display, x11Screen);
  x11Visual = new XVisualInfo;

  XMatchVisualInfo( x11Display, x11Screen, i32Depth, TrueColor, x11Visual);

  if (!x11Visual) {
    ofLogError("ofAppEGLWindow") << "Unable to acquire visual.";
    return false;
  }

  x11Colormap = XCreateColormap( x11Display, sRootWindow, x11Visual->visual, AllocNone );

  // set the colormap window attribuet
  sWA.colormap = x11Colormap;

  // Add to these for handling other events
  sWA.event_mask = 0;
  sWA.event_mask |= StructureNotifyMask;
  sWA.event_mask |= ExposureMask;
  sWA.event_mask |= ButtonPressMask;
  sWA.event_mask |= ButtonReleaseMask;
  sWA.event_mask |= PointerMotionMask;
  sWA.event_mask |= KeyPressMask;
  sWA.event_mask |= KeyReleaseMask;
 
  // setup background pixel attributes
  ui32Mask = 0;
  ui32Mask |= CWBackPixel;
  ui32Mask |= CWBorderPixel; 
  ui32Mask |= CWEventMask; 
  ui32Mask |= CWColormap;

  // Creates the X11 window
  x11Window = XCreateWindow(x11Display, // Specifies the connection to the X server.
                            sRootWindow, // Specifies the parent window.
                            0, 0, // Specify the x and y coordinates, 
                            // which are the top-left outside corner 
                            // of the window's borders and are relative 
                            // to the inside of the parent window's borders. 
                            w, h, // Specify the width and height, which are the 
                            // created window's inside dimensions and do 
                            // not include the created window's borders.
                            0, // Specifies the width of the created 
                            // window's border in pixels.
                            CopyFromParent, // Specifies the window's depth. 
                            // A depth of CopyFromParent means 
                            // the depth is taken from the parent. 
                            InputOutput, // Specifies the created window's class. 
                            // You can pass InputOutput, InputOnly, 
                            // or CopyFromParent. A class of CopyFromParent 
                            // means the class is taken from the parent.
                            CopyFromParent, // Specifies the visual type. 
                            // A visual of CopyFromParent means the visual type 
                            // is taken from the parent.
                            ui32Mask, // Specifies which window attributes are 
                            // defined in the attributes argument. This mask is
                            // the bitwise inclusive OR of the valid attribute 
                            // mask bits. If valuemask is zero, the attributes 
                            // are ignored and are not referenced.
                            &sWA //Specifies the background pixel value of the window.
                            );

  XMapWindow(x11Display, x11Window);
  XFlush(x11Display);
  
  bool ret = setupEGL((NativeWindowType)x11Window,(EGLNativeDisplayType*)x11Display);

  if(ret){
    // TODO, update this, or make X11 windows query the window directly
    // i.e. it should be possible to only use screenRect with nonx11 setups.
    screenRect.x = 0;
    screenRect.y = 0;
    screenRect.width = w;
    screenRect.height = h;
  }

  return ret;
}

//------------------------------------------------------------
static KeySym KeyCodeToKeySym(Display * display, KeyCode keycode, unsigned int event_mask) {
    KeySym keysym = NoSymbol;

    //Get the map
    XkbDescPtr keyboard_map = XkbGetMap(display, XkbAllClientInfoMask, XkbUseCoreKbd);
    if (keyboard_map) {
        //What is diff between XkbKeyGroupInfo and XkbKeyNumGroups?
        unsigned char info = XkbKeyGroupInfo(keyboard_map, keycode);
        unsigned int num_groups = XkbKeyNumGroups(keyboard_map, keycode);

        //Get the group
        unsigned int group = 0x00;
        switch (XkbOutOfRangeGroupAction(info)) {
            case XkbRedirectIntoRange:
                /* If the RedirectIntoRange flag is set, the four least significant
                 * bits of the groups wrap control specify the index of a group to
                 * which all illegal groups correspond. If the specified group is
                 * also out of range, all illegal groups map to Group1.
                 */
                group = XkbOutOfRangeGroupInfo(info);
                if (group >= num_groups) {
                    group = 0;
                }
            break;

            case XkbClampIntoRange:
                /* If the ClampIntoRange flag is set, out-of-range groups correspond
                 * to the nearest legal group. Effective groups larger than the
                 * highest supported group are mapped to the highest supported group;
                 * effective groups less than Group1 are mapped to Group1 . For
                 * example, a key with two groups of symbols uses Group2 type and
                 * symbols if the global effective group is either Group3 or Group4.
                 */
                group = num_groups - 1;
            break;

            case XkbWrapIntoRange:
                /* If neither flag is set, group is wrapped into range using integer
                 * modulus. For example, a key with two groups of symbols for which
                 * groups wrap uses Group1 symbols if the global effective group is
                 * Group3 or Group2 symbols if the global effective group is Group4.
                 */
            default:
                if (num_groups != 0) {
                    group %= num_groups;
                }
            break;
        }

        XkbKeyTypePtr key_type = XkbKeyKeyType(keyboard_map, keycode, group);
        unsigned int active_mods = event_mask & key_type->mods.mask;

        int i, level = 0;
        for (i = 0; i < key_type->map_count; i++) {
            if (key_type->map[i].active && key_type->map[i].mods.mask == active_mods) {
                level = key_type->map[i].level;
            }
        }

        keysym = XkbKeySymEntry(keyboard_map, keycode, level, group);
        XkbFreeClientMap(keyboard_map, XkbAllClientInfoMask, true);
    }

    return keysym;
}

//------------------------------------------------------------
void ofAppEGLWindow::handleX11Event(const XEvent& event){
    static ofMouseEventArgs mouseEvent;
    static ofKeyEventArgs keyEvent;
    switch (event.type){
        case KeyPress:
        case KeyRelease: 
            {
                KeySym key = KeyCodeToKeySym(x11Display,event.xkey.keycode,event.xkey.state);
                keyEvent.key = key;
                if (event.type == KeyPress) {
                    keyEvent.type = ofKeyEventArgs::Pressed;
                    if(key == 65307){
                        keyEvent.key = OF_KEY_ESC;
                    }
                } else if (event.type == KeyRelease){
                    keyEvent.type = ofKeyEventArgs::Released;
                }
                
                ofNotifyKeyEvent(keyEvent);
            }
            break;
        case ButtonPress:
        case ButtonRelease:
            mouseEvent.x = static_cast<float>(event.xbutton.x);
            mouseEvent.y = static_cast<float>(event.xbutton.y);
            mouseEvent.button = event.xbutton.button;
            if (event.type == ButtonPress){
                mouseEvent.type = ofMouseEventArgs::Pressed;
            } else {
                mouseEvent.type = ofMouseEventArgs::Released;
            }
            
            ofNotifyMouseEvent(mouseEvent);
            break;
        case MotionNotify:
            //cout << "motion notify" << endl;
            mouseEvent.x = static_cast<float>(event.xmotion.x);
            mouseEvent.y = static_cast<float>(event.xmotion.y);
            mouseEvent.button = event.xbutton.button;
            if(ofGetMousePressed()) {
                mouseEvent.type = ofMouseEventArgs::Dragged;
            } else {
                mouseEvent.type = ofMouseEventArgs::Moved;
            }
            
            ofNotifyMouseEvent(mouseEvent);
            break;
        case ConfigureNotify:
            currentWindowRect.x = event.xconfigure.x;
            currentWindowRect.y = event.xconfigure.y;
            currentWindowRect.width = event.xconfigure.width;
            currentWindowRect.height = event.xconfigure.height;
            nonFullscreenWindowRect = currentWindowRect;
            ofNotifyWindowResized(event.xconfigure.width,event.xconfigure.height);
            break;
  /*case ClientMessage:
  {
    if (event.xclient.message_type == wmProtocols_ &&
    event.xclient.format == 32 &&
    event.xclient.data.l[0] == (long) wmDeleteWindow_)
    {
    if (listener())
    {
      if (listener()->onClose(wrapper() ? *wrapper() : *(WindowInterface*)this))
        isShuttingDown_ = true;
    }
    else
    {
      isShuttingDown_ = true;
    }
    }
    break;
  }*/
  }
}
