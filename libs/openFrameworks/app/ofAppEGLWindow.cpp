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
#include "ofGLProgrammableRenderer.h"
#include <assert.h>


void ofGLReadyCallback();

// native events
struct udev*         udev;
struct udev_device*  dev;
struct udev_monitor* mon;
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

// TODO, make this match the upcoming additions to ofWindow
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

// from http://cantuna.googlecode.com/svn-history/r16/trunk/src/screen.cpp
#define CASE_STR(x,y) case x: str = y; break

static const char* eglErrorString(EGLint err) {
    string str;
    switch (err) {
        CASE_STR(EGL_SUCCESS, "no error");
        CASE_STR(EGL_NOT_INITIALIZED, "EGL not, or could not be, initialized");
        CASE_STR(EGL_BAD_ACCESS, "access violation");
        CASE_STR(EGL_BAD_ALLOC, "could not allocate resources");
        CASE_STR(EGL_BAD_ATTRIBUTE, "invalid attribute");
        CASE_STR(EGL_BAD_CONTEXT, "invalid context specified");
        CASE_STR(EGL_BAD_CONFIG, "invald frame buffer configuration specified");
        CASE_STR(EGL_BAD_CURRENT_SURFACE, "current window, pbuffer or pixmap surface is no longer valid");
        CASE_STR(EGL_BAD_DISPLAY, "invalid display specified");
        CASE_STR(EGL_BAD_SURFACE, "invalid surface specified");
        CASE_STR(EGL_BAD_MATCH, "bad argument match");
        CASE_STR(EGL_BAD_PARAMETER, "invalid paramater");
        CASE_STR(EGL_BAD_NATIVE_PIXMAP, "invalid NativePixmap");
        CASE_STR(EGL_BAD_NATIVE_WINDOW, "invalid NativeWindow");
        CASE_STR(EGL_CONTEXT_LOST, "APM event caused context loss");
        default: str = "unknown error " + err; break;
    }
    return str.c_str();
}


// X11 events
  #include <X11/XKBlib.h>


#ifdef TARGET_RASPBERRY_PI
  // TODO: remove these when they enter system headers
  // From : https://github.com/raspberrypi/userland/blob/master/interface/vmcs_host/vc_vchi_dispmanx.h
  #ifndef ELEMENT_CHANGE_LAYER
    #define ELEMENT_CHANGE_LAYER          (1<<0)
  #endif
  #ifndef ELEMENT_CHANGE_OPACITY
    #define ELEMENT_CHANGE_OPACITY        (1<<1)
  #endif
  #ifndef ELEMENT_CHANGE_DEST_RECT
    #define ELEMENT_CHANGE_DEST_RECT      (1<<2)
  #endif
  #ifndef ELEMENT_CHANGE_SRC_RECT
    #define ELEMENT_CHANGE_SRC_RECT       (1<<3)
  #endif
  #ifndef ELEMENT_CHANGE_MASK_RESOURCE
    #define ELEMENT_CHANGE_MASK_RESOURCE  (1<<4)
  #endif
  #ifndef ELEMENT_CHANGE_TRANSFORM
    #define ELEMENT_CHANGE_TRANSFORM      (1<<5)
  #endif
#endif


//-------------------------------------------------------------------------------------
ofAppEGLWindow::Settings::Settings() {
  eglWindowPreference = OF_APP_WINDOW_AUTO;
  eglWindowOpacity = 255;

  // these are usually set as default, but set them here just to be sure
  frameBufferAttributes[EGL_RED_SIZE]     = 8; // 8 bits for red
  frameBufferAttributes[EGL_GREEN_SIZE]   = 8; // 8 bits for green
  frameBufferAttributes[EGL_BLUE_SIZE]    = 8; // 8 bits for blue
  frameBufferAttributes[EGL_ALPHA_SIZE]   = 8; // 8 bits for alpha
  frameBufferAttributes[EGL_LUMINANCE_SIZE] = EGL_DONT_CARE; // 8 bits for alpha
  frameBufferAttributes[EGL_DEPTH_SIZE]   = 24; // 24 bits for depth
  frameBufferAttributes[EGL_STENCIL_SIZE] = 8; // 8 bits for stencil
  frameBufferAttributes[EGL_SAMPLES]      = 1;

  initialClearColor = ofColor(0.15 * 255, 0.15 * 255, 0.15 * 255, 255);

  screenNum = 0; /* 0 = LCD on the raspberry pi */
  layer = 0;
}

// TODO. we may not need these to be static, but we will
// leave it this way for now in case future EGL windows 
// use static callbacks (like glut)

//------------------------------------------------------------
ofAppEGLWindow::ofAppEGLWindow() {
  keyboardDetected = false;
  mouseDetected	= false;
  threadTimeout = ofThread::INFINITE_JOIN_TIMEOUT;
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
EGLDisplay ofAppEGLWindow::getEglDisplay() const {
  return eglDisplay;
}

//------------------------------------------------------------
EGLSurface ofAppEGLWindow::getEglSurface() const {
  return eglSurface;
}

//------------------------------------------------------------
EGLContext ofAppEGLWindow::getEglContext() const {
  return eglContext;
}

#ifndef TARGET_RASPBERRY_PI
//------------------------------------------------------------
Display* ofAppEGLWindow::getX11Display(){
	return x11Display;
}

//------------------------------------------------------------
Window ofAppEGLWindow::getX11Window(){
	return x11Window;
}
#endif
//------------------------------------------------------------
EGLConfig ofAppEGLWindow::getEglConfig() const {
  return eglConfig;
}

//------------------------------------------------------------
EGLint ofAppEGLWindow::getEglVersionMajor () const {
  return eglVersionMajor;
}

//------------------------------------------------------------
EGLint ofAppEGLWindow::getEglVersionMinor() const {
  return eglVersionMinor;
}

//------------------------------------------------------------
void ofAppEGLWindow::init(Settings _settings) {
    terminate      = false;

    windowMode      = OF_WINDOW;
    bNewScreenMode  = true;
    nFramesSinceWindowResized = 0;
    buttonInUse     = 0;
    bEnableSetupScreen  = true;
    eglDisplayString   = "";
    orientation     = OF_ORIENTATION_DEFAULT;

    //TODO: 2.0f is an arbitrary factor that makes mouse speed ok at 1024x768,
    // to be totally correct we might need to take into account screen size
    // and add acceleration
    mouseScaleX = 2.0f;
    mouseScaleY = 2.0f;

    isUsingX11      = false;
    isWindowInited  = false;
    isSurfaceInited = false;

    // APPLY SETTINGS
    settings = _settings;

    eglDisplay = NULL;
    eglSurface = NULL;
    eglContext = NULL;
    eglConfig  = NULL;
    eglVersionMinor = -1;
    eglVersionMinor = -1;
    glesVersion = 1;

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
            ofLogError("ofAppEGLWindow") << "init(): X11 window requested, but X11 is not available";
        }
    }

    ////////////////
    // TODO remove the following ifdef once x11 is accelerated on RPI
    #ifdef TARGET_RASPBERRY_PI
    if(isUsingX11) {
        isUsingX11 = false;
        ofLogWarning("ofAppEGLWindow") << "init(): X11 not availble on RPI yet, using a native window instead";
    }
    #endif
    ////////////////

    initNative();

    ofAddListener(ofEvents().exit, this, &ofAppEGLWindow::exit);
}

//------------------------------------------------------------
void ofAppEGLWindow::exit(ofEventArgs &e) {
  terminate = true; // TODO, it is unlikely that this will happen
  if(!isUsingX11) {
    destroyNativeEvents();
  }   

  // we got a terminate ... so clean up.
  destroySurface();
  destroyWindow();

  exitNative();
}

//------------------------------------------------------------
void ofAppEGLWindow::initNative() {
    #ifdef TARGET_RASPBERRY_PI
        initRPiNative();
    #endif
}

//------------------------------------------------------------
void ofAppEGLWindow::exitNative() {
    #ifdef TARGET_RASPBERRY_PI
        exitRPiNative();
    #endif
}

//------------------------------------------------------------
EGLNativeWindowType ofAppEGLWindow::getNativeWindow()  {
  if(!isWindowInited) {
    ofLogWarning("ofAppEGLWindow") << "getNativeDisplay(): window not initialized, returning NULL";
    return NULL;
  }

  if(isUsingX11) {
    return (EGLNativeWindowType)x11Window;
  } else {
        #ifdef TARGET_RASPBERRY_PI
    return (EGLNativeWindowType)&dispman_native_window;
        #else
    ofLogNotice("ofAppEGLWindow") << "getNativeWindow(): no native window type for this system, perhaps try X11?";
    return NULL;
        #endif
  }
}

//------------------------------------------------------------
EGLNativeDisplayType ofAppEGLWindow::getNativeDisplay() {
  if(!isWindowInited) {
    ofLogWarning("ofAppEGLWindow") << "getNativeDisplay(): window not initialized, returning NULL";
    return 0;
  }

  if(isUsingX11) {
    return (EGLNativeDisplayType)x11Display;
  } else {
        #ifdef TARGET_RASPBERRY_PI
    return (EGLNativeDisplayType)NULL;
        #else
    ofLogNotice("ofAppEGLWindow") << "getNativeDisplay(): no native window type for this system, perhaps try X11?";
    return 0;
        #endif
  }
}


void ofAppEGLWindow::setGLESVersion(int _glesVersion){
    glesVersion = _glesVersion;
}

//------------------------------------------------------------
void ofAppEGLWindow::setupOpenGL(int w, int h, int screenMode) {

    // we set this here, and if we need to make a fullscreen 
    // app, we do it during the first loop.
     windowMode = screenMode;
     bShowCursor = true;

    nonFullscreenWindowRect.set(0,0,w,h);
    nonFullscreenWindowRect.standardize();

    ofRectangle startRect = nonFullscreenWindowRect;
    bNewScreenMode = false;

    if(windowMode == OF_GAME_MODE) {
       ofLogWarning("ofAppEGLWindow") << "setupOpenGL(): OF_GAME_MODE not supported, using OF_WINDOW";
       startRect = nonFullscreenWindowRect;
    } else if(windowMode == OF_FULLSCREEN) {
       startRect = getScreenRect();
    }

    isWindowInited = createWindow(startRect);
    isSurfaceInited = createSurface();

    if(!isWindowInited) {
      ofLogError("ofAppEGLWindow")  << "setupOpenGL(): screen creation failed, window not inited";
    }
    
    setupPeripherals();

    ofLogNotice("ofAppEGLWindow")  << "setupOpenGL(): peripheral setup complete";
    ofGLReadyCallback();
}

//------------------------------------------------------------
void ofAppEGLWindow::setupPeripherals() {
    if(!isUsingX11) {
        // roll our own cursor!
        mouseCursor.allocate(mouse_cursor_data.width,mouse_cursor_data.height,OF_IMAGE_COLOR_ALPHA);
        MOUSE_CURSOR_RUN_LENGTH_DECODE(mouseCursor.getPixels(),mouse_cursor_data.rle_pixel_data,mouse_cursor_data.width*mouse_cursor_data.height,mouse_cursor_data.bpp);
        mouseCursor.update();
        ofLogNotice("ofAppEGLWindow") << "setupPeripherals(): peripheral setup complete";
        setupNativeEvents();
        ofLogNotice("ofAppEGLWindow") << "setupPeripherals(): native event setup complete";

    } else {
        ofLogError("ofAppEGLWindow") << "setupPeripherals(): peripherals not supported on X11";
    }
}

//------------------------------------------------------------
bool ofAppEGLWindow::createSurface() {

  EGLNativeWindowType nativeWindow = getNativeWindow();
  EGLNativeDisplayType display = getNativeDisplay();

  ofLogNotice("ofAppEGLWindow") << "createSurface(): setting up EGL Display";
    // get an EGL eglDisplay connection
    
    isSurfaceInited = false;

    EGLint result;

    if(display==0){
      eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    }else{
      eglDisplay = eglGetDisplay(display);
    }

    if(eglDisplay == EGL_NO_DISPLAY) {
      ofLogNotice("ofAppEGLWindow") << "createSurface(): eglGetDisplay returned: " << eglDisplay;
     return false;
    }else{
      ofLogNotice("ofAppEGLWindow") << "createSurface(): EGL Display correctly set";
    }

    // initialize the EGL eglDisplay connection
    result = eglInitialize(eglDisplay, 
                           &eglVersionMajor, 
                           &eglVersionMinor);

    if(result == EGL_BAD_DISPLAY) {
//  eglDisplay is not an EGL connection
        ofLogError("ofAppEGLWindow") << "createSurface(): eglInitialize returned EGL_BAD_DISPLAY";
        return false;
    } else if(result == EGL_NOT_INITIALIZED) {
        // eglDisplay cannot be intitialized
        ofLogError("ofAppEGLWindow") << "createSurface(): eglInitialize returned EGL_NOT_INITIALIZED";
        return false;
    } else if(result == EGL_FALSE) {
        // eglinitialize was not initialiezd
        ofLogError("ofAppEGLWindow") << "createSurface(): eglInitialize returned EGL_FALSE";
        return false;
    } else {
        // result == EGL_TRUE
        // success!
    }

    EGLint glesVersion;
    int glesVersionForContext;

    if(ofGetCurrentRenderer()) {
      ofLogNotice("ofAppEGLWindow") << "createSurface(): current renderer type: " << ofGetCurrentRenderer()->getType();
    } else {
      ofLogNotice("ofAppEGLWindow") << "createSurface(): no current renderer selected";
    }

    if(this->glesVersion==2){
      glesVersion = EGL_OPENGL_ES2_BIT;
      glesVersionForContext = 2;
        ofLogNotice("ofAppEGLWindow") << "createSurface(): GLES2 renderer detected";
    }else{
      glesVersion = EGL_OPENGL_ES_BIT;
      glesVersionForContext = 1;
      ofLogNotice("ofAppEGLWindow") << "createSurface(): default renderer detected";
    }
    
    ofEGLAttributeListIterator iter, iterEnd;
    int i;

    // each attribute has 2 values, and we need one extra for the EGL_NONE terminator
    EGLint attribute_list_framebuffer_config[settings.frameBufferAttributes.size() * 2 + 3];

    iter = settings.frameBufferAttributes.begin();
    iterEnd = settings.frameBufferAttributes.end();
    i = 0;
    for(; iter != iterEnd; iter++) {
        attribute_list_framebuffer_config[i++] = iter->first;
        attribute_list_framebuffer_config[i++] = iter->second;
    }
	attribute_list_framebuffer_config[i++] = EGL_RENDERABLE_TYPE;
	attribute_list_framebuffer_config[i++] = glesVersion; //openGL ES version
    attribute_list_framebuffer_config[i] = EGL_NONE; // add the terminator

    EGLint num_configs;

    // get an appropriate EGL frame buffer configuration
    // http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglChooseConfig.html
    result = eglChooseConfig(eglDisplay, 
                             attribute_list_framebuffer_config, 
                             &eglConfig, 
                             1, // we only want the first one.  if we want more, 
                                // we need to pass in an array.
                                // we are optimistic and don't give it more chances
                                // to find a good configuration
                             &num_configs);

    if(result == EGL_FALSE) {
        EGLint error = eglGetError();
        ofLogError("ofAppEGLWindow") << "createSurface(): error finding valid configuration based on settings: " << eglErrorString(error);
        return false;
    }

    if(num_configs <= 0 || eglConfig == NULL) {
        ofLogError("ofAppEGLWindow") << "createSurface(): no matching configs were found, num_configs: " << num_configs;
        return false;
    }


    // each attribute has 2 values, and we need one extra for the EGL_NONE terminator
    EGLint attribute_list_window_surface[settings.windowSurfaceAttributes.size() * 2 + 1];

    iter = settings.windowSurfaceAttributes.begin();
    iterEnd = settings.windowSurfaceAttributes.end();

    i = 0;
    for(; iter != iterEnd; iter++) {
        attribute_list_window_surface[i++] = iter->first;
        attribute_list_window_surface[i++] = iter->second;
    }
    attribute_list_window_surface[i] = EGL_NONE; // add the terminator

    // create a surface
    eglSurface = eglCreateWindowSurface( eglDisplay, // our display handle 
                                         eglConfig,    // our first config
                                         nativeWindow, // our native window
                                         attribute_list_window_surface); // surface attribute list
    
    if(eglSurface == EGL_NO_SURFACE) {
        EGLint error = eglGetError();
        switch(error) {
            case EGL_BAD_MATCH:
                ofLogError("ofAppEGLWindow") << "createSurface(): error creating surface: EGL_BAD_MATCH " << eglErrorString(error);  
                ofLogError("ofAppEGLWindow") << "createSurface(): check window and EGLConfig attributes to determine compatibility, ";
                ofLogError("ofAppEGLWindow") << "createSurface(): or verify that the EGLConfig supports rendering to a window";
                 break;
            case EGL_BAD_CONFIG:
                ofLogError("ofAppEGLWindow") << "createSurface(): error creating surface: EGL_BAD_CONFIG " << eglErrorString(error);
                ofLogError("ofAppEGLWindow") << "createSurface(): verify that provided EGLConfig is valid";
                 break;
            case EGL_BAD_NATIVE_WINDOW:
                ofLogError("ofAppEGLWindow") << "createSurface(): error creating surface: EGL_BAD_NATIVE_WINDOW " << eglErrorString(error);
                ofLogError("ofAppEGLWindow") << "createSurface(): verify that provided EGLNativeWindow is valid";
                 break;
            case EGL_BAD_ALLOC:
                ofLogError("ofAppEGLWindow") << "createSurface(): error creating surface: EGL_BAD_ALLOC " << eglErrorString(error);
                ofLogError("ofAppEGLWindow") << "createSurface(): not enough resources available";
                 break;
             default:
              ofLogError("ofAppEGLWindow") << "createSurface(): error creating surface: << " << error << eglErrorString(error);
           } 

        return false;
    }else{
        ofLogNotice("ofAppEGLWindow") << "createSurface(): surface created correctly";
    }

  // get an appropriate EGL frame buffer configuration
  result = eglBindAPI(EGL_OPENGL_ES_API);

  if(result == EGL_FALSE) {
      ofLogError("ofAppEGLWindow") << "createSurface(): error binding API: " << eglErrorString(eglGetError());
      return false;
  }else{
      ofLogNotice("ofAppEGLWindow") << "createSurface(): API bound correctly";
  }

  // create an EGL rendering eglContext  
  EGLint attribute_list_surface_context[] = {
    EGL_CONTEXT_CLIENT_VERSION, glesVersionForContext,
    EGL_NONE
  };

    eglContext = eglCreateContext(eglDisplay, 
                                  eglConfig, 
                                  EGL_NO_CONTEXT, 
                                  attribute_list_surface_context);

    if(eglContext == EGL_NO_CONTEXT) {
       EGLint error = eglGetError();
       if(error == EGL_BAD_CONFIG) {
            ofLogError("ofAppEGLWindow") << "createSurface(): error creating context: EGL_BAD_CONFIG " << eglErrorString(error);
            return false;
       } else {
            ofLogError("ofAppEGLWindow") << "createSurface(): error creating context: " << error << " " << eglErrorString(error);
            return false;
       }
    }

    // connect the eglContext to the eglSurface
    result = eglMakeCurrent(eglDisplay, 
                            eglSurface, // draw surface
                            eglSurface, // read surface
                            eglContext);

    if(eglContext == EGL_FALSE) {
        EGLint error = eglGetError();
        ofLogError("ofAppEGLWindow") << "createSurface(): couldn't making current surface: " << eglErrorString(error);
        return false;
    }

    // Set background color and clear buffers
    glClearColor(settings.initialClearColor.r / 255.0f,
                 settings.initialClearColor.g / 255.0f,
                 settings.initialClearColor.b / 255.0f,
                 settings.initialClearColor.a / 255.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    glClear( GL_DEPTH_BUFFER_BIT );

    ofLogNotice("ofAppEGLWindow") << "createSurface(): -----EGL-----";
    ofLogNotice("ofAppEGLWindow") << "createSurface(): EGL_VERSION_MAJOR = " << eglVersionMajor;
    ofLogNotice("ofAppEGLWindow") << "createSurface(): EGL_VERSION_MINOR = " << eglVersionMinor;
    ofLogNotice("ofAppEGLWindow") << "createSurface(): EGL_CLIENT_APIS = " << eglQueryString(eglDisplay, EGL_CLIENT_APIS);
    ofLogNotice("ofAppEGLWindow") << "createSurface(): EGL_VENDOR = "  << eglQueryString(eglDisplay, EGL_VENDOR);
    ofLogNotice("ofAppEGLWindow") << "createSurface(): EGL_VERSION = " << eglQueryString(eglDisplay, EGL_VERSION);
    ofLogNotice("ofAppEGLWindow") << "createSurface(): EGL_EXTENSIONS = " << eglQueryString(eglDisplay, EGL_EXTENSIONS);
    ofLogNotice("ofAppEGLWindow") << "createSurface(): GL_RENDERER = " << glGetString(GL_RENDERER);
    ofLogNotice("ofAppEGLWindow") << "createSurface(): GL_VERSION  = " << glGetString(GL_VERSION);
    ofLogNotice("ofAppEGLWindow") << "createSurface(): GL_VENDOR   = " << glGetString(GL_VENDOR);
    ofLogNotice("ofAppEGLWindow") << "createSurface(): -------------";

    isSurfaceInited = true;

    return true;
}

//------------------------------------------------------------
bool ofAppEGLWindow::destroySurface() {
    if(isSurfaceInited) {
        ofLogNotice("ofAppEGLWindow") << "destroySurface(): destroying EGL surface";
        eglSwapBuffers(eglDisplay, eglSurface);
        eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(eglDisplay, eglSurface);
        eglDestroyContext(eglDisplay, eglContext);
        eglTerminate(eglDisplay);
        isSurfaceInited = false;

        eglDisplay = NULL;
        eglSurface = NULL;
        eglContext = NULL;
        eglConfig  = NULL;
        eglVersionMinor = -1;
        eglVersionMinor = -1;
        
        return true;
    } else {
        ofLogError("ofAppEGLWindow") << "destroySurface(): attempted to destroy uninitialized window";
        return false;
    }
}

//------------------------------------------------------------
bool ofAppEGLWindow::destroyWindow() {
    if(isWindowInited) {
      if(isUsingX11) {
        // TODO: double check
        XDestroyWindow(x11Display,x11Window); // or XCloseWindow?
        XFree(x11Screen);
      } else {
          #ifdef TARGET_RASPBERRY_PI
              dispman_update = vc_dispmanx_update_start(0);
              if (dispman_element != DISPMANX_NO_HANDLE) {
                vc_dispmanx_element_remove(dispman_update, dispman_element);
                dispman_element = DISPMANX_NO_HANDLE;
              }

              vc_dispmanx_update_submit_sync(dispman_update);

              if (dispman_display != DISPMANX_NO_HANDLE) {
                vc_dispmanx_display_close(dispman_display);
                dispman_display = DISPMANX_NO_HANDLE;
              }
          #else
            ofLogNotice("ofAppEGLWindow") << "destroyWindow(): no native window type for this system, perhaps try X11?";
          #endif
      }

    } else {
        ofLogNotice("ofAppEGLWindow") << "destroyWindow(): destroying (uninited) native window (not implemented yet)";
    }

    return true;
}


//------------------------------------------------------------
void ofAppEGLWindow::initializeWindow() {
    nFramesSinceWindowResized = 0;
}

//------------------------------------------------------------
void ofAppEGLWindow::runAppViaInfiniteLoop(ofBaseApp *appPtr) {
   ofLogNotice("ofAppEGLWindow") << "runAppViaInfiniteLoop(): entering infinite loop";

    ofAppPtr = appPtr; // make a local copy
 
    ofNotifySetup();
    ofLogNotice("ofAppEGLWindow") << "runAppViaInfiniteLoop(): setting up notifications complete";
 
    // loop it!
    while (!terminate) {
      checkEvents();
      idle();
      display();
    }

    ofLogNotice("ofAppEGLWindow") << "runAppViaInfiniteLoop(): exiting infinite loop";
}

//------------------------------------------------------------
void ofAppEGLWindow::setupNativeEvents() {
  setupNativeUDev();
  setupNativeMouse();
  setupNativeKeyboard();
  startThread();
}

//------------------------------------------------------------
void ofAppEGLWindow::destroyNativeEvents() {
  destroyNativeUDev();
  destroyNativeMouse(); 
  destroyNativeKeyboard(); 
  waitForThread(true, threadTimeout);
}

//------------------------------------------------------------
void ofAppEGLWindow::setWindowRect(const ofRectangle& requestedWindowRect) {
    if(!isWindowInited) {
      ofLogError("ofAppEGLWindow") << "setWindowRect(): window not inited";
      return;
    }

    ofRectangle newRect = requestedWindowRect.getStandardized();

    if(newRect != currentWindowRect) {
        ofRectangle oldWindowRect = currentWindowRect;

        if(isUsingX11) {
          int ret = XMoveResizeWindow(x11Display, 
                                      x11Window,
                                      (int)newRect.x,
                                      (int)newRect.y,
                                      (unsigned int)newRect.width,
                                      (unsigned int)newRect.height);
          if(ret == BadValue) {
            ofLogError("ofAppEGLWindow") << "setWindowRect(): XMoveResizeWindow returned BadValue";
          } else if(ret == BadWindow) {
            ofLogError("ofAppEGLWindow") << "setWindowRect(): XMoveResizeWindow returned BadWindow";
          } else {
            // all is good
            currentWindowRect = newRect;
          }
        } else {
          #ifdef TARGET_RASPBERRY_PI

          VC_RECT_T dst_rect;
          dst_rect.x      = (int32_t)newRect.x;
          dst_rect.y      = (int32_t)newRect.y;
          dst_rect.width  = (int32_t)newRect.width;
          dst_rect.height = (int32_t)newRect.height;

          VC_RECT_T src_rect;
          src_rect.x      = 0;
          src_rect.y      = 0;
          src_rect.width  = (int32_t)newRect.width << 16;
          src_rect.height = (int32_t)newRect.height << 16;

          DISPMANX_UPDATE_HANDLE_T dispman_update = vc_dispmanx_update_start(0);
          
          vc_dispmanx_element_change_attributes(dispman_update,
                                                dispman_element,
                                                ELEMENT_CHANGE_SRC_RECT|ELEMENT_CHANGE_DEST_RECT, // we do both when resizing
                                                0, // layer (we aren't changing it here)
                                                0, // opactiy (we aren't changing it here)
                                                &dst_rect,
                                                &src_rect,
                                                0, // mask (we aren't changing it here)
          #ifdef USE_DISPMANX_TRANSFORM_T
                                                (DISPMANX_TRANSFORM_T)0);
          #else
                                                (VC_IMAGE_TRANSFORM_T)0);
          #endif

	  vc_dispmanx_update_submit_sync(dispman_update);

          // next time swapBuffers is called, it will be resized based on this eglwindow size data
          dispman_native_window.element = dispman_element;
          dispman_native_window.width   = (int32_t)newRect.width;
          dispman_native_window.height  = (int32_t)newRect.height; // don't forget!

          currentWindowRect = newRect;

          #else
            ofLogError("ofAppEGLWindow") << "createEGLWindow(): no native window type for this system, perhaps try X11?";
          #endif
        }

        if(oldWindowRect.width  != currentWindowRect.width ||
           oldWindowRect.height != currentWindowRect.height) {
            ofNotifyWindowResized(currentWindowRect.width,
                                  currentWindowRect.height);
            nFramesSinceWindowResized = 0;
        }
    }
}


//------------------------------------------------------------
bool ofAppEGLWindow::createWindow(const ofRectangle& requestedWindowRect) {
  if(isUsingX11) {
      return createX11NativeWindow(requestedWindowRect);
  } else {
    #ifdef TARGET_RASPBERRY_PI
      return createRPiNativeWindow(requestedWindowRect);
    #else
      ofLogError("ofAppEGLWindow") << "createEGLWindow(): no native window type for this system, perhaps try X11?";
      return false;
    #endif
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
void ofAppEGLWindow::setWindowTitle(string title) {
    ofLogNotice("ofAppEGLWindow") << "setWindowTitle(): not implemented";
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
  unsigned int screenWidth = 0;
  unsigned int screenHeight = 0;

  if(isUsingX11) {
    // TODO, there must be a way to get screensize if the window is not inited
    if(isWindowInited && x11Screen) {
      screenWidth  = XWidthOfScreen(x11Screen);
      screenHeight = XHeightOfScreen(x11Screen);
    } else {
        ofLogError("ofAppEGLWindow") << "getScreenSize(): tried to get display size but failed, x11Screen is not inited";
    }

  } else {
    #ifdef TARGET_RASPBERRY_PI
      int success = graphics_get_display_size(settings.screenNum, &screenWidth, &screenHeight);
      if(success < 0) {
        ofLogError("ofAppEGLWindow") << "getScreenSize(): tried to get display size but failed";
      }

    #else
      ofLogError("ofAppEGLWindow") << "getScreenSize(): no native window type for this system, perhaps try X11?";
    #endif

  }

  return ofPoint(screenWidth, screenHeight,0);
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
  if(!isWindowInited) {
    ofLogError("ofAppEGLWindow") << "setWindowPosition(): window not inited";
    return;
  }

  if(isUsingX11) {
      int ret = XMoveWindow(x11Display, 
                           x11Window,
                           x,
                           y);
    if(ret == BadValue) {
      ofLogError("ofAppEGLWindow") << "setWindowPosition(): XMoveWindow returned BadValue";
    } else if(ret == BadWindow) {
      ofLogError("ofAppEGLWindow") << "setWindowPosition(): XMoveWindow returned BadWindow";
    } else {
      currentWindowRect.x = x;
      currentWindowRect.y = y;
      nonFullscreenWindowRect = currentWindowRect;
    }
  } else {
  #ifdef TARGET_RASPBERRY_PI

    // keep it in bounds
    ofPoint screenSize = getScreenSize();
    x = ofClamp(x, 0, screenSize.x - currentWindowRect.width);
    y = ofClamp(y, 0, screenSize.y - currentWindowRect.height);

    VC_RECT_T dst_rect;
    dst_rect.x      = (int32_t)x;
    dst_rect.y      = (int32_t)y;
    dst_rect.width  = (int32_t)currentWindowRect.width;
    dst_rect.height = (int32_t)currentWindowRect.height;

    dispman_update = vc_dispmanx_update_start(0);
    
    vc_dispmanx_element_change_attributes(dispman_update,
                                          dispman_native_window.element,
                                          ELEMENT_CHANGE_DEST_RECT,
                                          0,
                                          0,
                                          &dst_rect,
                                          NULL,
                                          0,
    #ifdef USE_DISPMANX_TRANSFORM_T
                                          (DISPMANX_TRANSFORM_T)0);
    #else
                                          (VC_IMAGE_TRANSFORM_T)0);
    #endif

    vc_dispmanx_update_submit_sync(dispman_update);

    currentWindowRect.x = x;
    currentWindowRect.y = y;
    nonFullscreenWindowRect = currentWindowRect;

  #else
    ofLogError("ofAppEGLWindow") << "setWindowPosition(): no native window type for this system, perhaps try X11?";
  #endif
  }

}

//------------------------------------------------------------
void ofAppEGLWindow::setWindowShape(int w, int h){
    if(!isWindowInited) {
    ofLogError("ofAppEGLWindow") << "setWindowPosition(): window not inited";
    return;
  }

  if(isUsingX11) {
      int ret = XResizeWindow(x11Display, 
                           x11Window,
                           (unsigned int)w,
                           (unsigned int)h);
    if(ret == BadValue) {
      ofLogError("ofAppEGLWindow") << "setWindowPosition(): XMoveWindow returned BadValue";
    } else if(ret == BadWindow) {
      ofLogError("ofAppEGLWindow") << "setWindowPosition(): XMoveWindow returned BadWindow";
    } else {
      currentWindowRect.width = w;
      currentWindowRect.height = h;
      nonFullscreenWindowRect = currentWindowRect;
    }
  } else {
  #ifdef TARGET_RASPBERRY_PI
    setWindowRect(ofRectangle(currentWindowRect.x,currentWindowRect.y,w,h));
    nonFullscreenWindowRect = currentWindowRect;
  #else
    ofLogError("ofAppEGLWindow") << "setWindowPosition(): no native window type for this system, perhaps try X11?";
  #endif
  }
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
  ofNotifyUpdate();

}

//------------------------------------------------------------
void ofAppEGLWindow::display() {

  // take care of any requests for a new screen mode
  if (windowMode != OF_GAME_MODE && bNewScreenMode){
    if( windowMode == OF_FULLSCREEN){
      setWindowRect(getScreenRect());
    } else if( windowMode == OF_WINDOW ){
      setWindowRect(nonFullscreenWindowRect);
    }
    bNewScreenMode = false;
  }

  ///////////////////////////////////////////////////////////////////////////////////////
  // set viewport, clear the screen
 
  ofPtr<ofGLProgrammableRenderer> renderer = ofGetGLProgrammableRenderer();
  if(renderer){
   renderer->startRender();
  }

  ofViewport(0, 0, getWindowWidth(), getWindowHeight());    // used to be glViewport( 0, 0, width, height );
  
  float * bgPtr = ofBgColorPtr();
  bool bClearAuto = ofbClearBg();

  if ( bClearAuto == true || ofGetFrameNum() < 3){
    ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
  }

  if( bEnableSetupScreen ) ofSetupScreen(); // this calls into the current renderer (ofSetupScreenPerspective)

  ofNotifyDraw();
  
  if(!isUsingX11) {
    if(bShowCursor){
        
        GLboolean bIsDepthTestEnabled = GL_FALSE;
        glGetBooleanv(GL_DEPTH_TEST, &bIsDepthTestEnabled);

        if(bIsDepthTestEnabled == GL_TRUE) {
            glDisable(GL_DEPTH_TEST);
        }
        
        bool isUsingNormalizedTexCoords = ofGetUsingNormalizedTexCoords();
        if(isUsingNormalizedTexCoords) {
          ofDisableNormalizedTexCoords();
        }
        
        ofPushStyle();
        ofEnableAlphaBlending();
        ofDisableTextureEdgeHack();
        ofSetColor(255);
        mouseCursor.draw(ofGetMouseX(),ofGetMouseY());
        ofEnableTextureEdgeHack();
        //TODO: we need a way of querying the previous state of texture hack
        ofPopStyle();
        
        if(bIsDepthTestEnabled == GL_TRUE) {
            glEnable(GL_DEPTH_TEST);
        }

        if(isUsingNormalizedTexCoords) {
          ofEnableNormalizedTexCoords();
        }


    }
   }
 
  if(renderer) {
    renderer->finishRender();
  }
  
  EGLBoolean success = eglSwapBuffers(eglDisplay, eglSurface);
  if(!success) {
       GLint error = eglGetError();
       ofLogNotice("ofAppEGLWindow") << "display(): eglSwapBuffers failed: " << eglErrorString(error);
  }

  nFramesSinceWindowResized++;

}

//------------------------------------------------------------
ofRectangle ofAppEGLWindow::getScreenRect(){
  ofPoint screenSize = getScreenSize();
  return ofRectangle(0,0,screenSize.x,screenSize.y);
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
void ofAppEGLWindow::setupNativeUDev() {
 
    udev = udev_new(); // create new udev object
    if(!udev) {
        ofLogError("ofAppEGLWindow") << "setupNativeUDev(): couldn't create udev object";
    } else {
        ofLogNotice("ofAppEGLWindow") << "setupNativeUDev(): created udev object";
        // setup udev to monitor for input devices
        mon = udev_monitor_new_from_netlink(udev, "udev");
        // just listen for input devices
        udev_monitor_filter_add_match_subsystem_devtype(mon, "input", NULL);
        udev_monitor_enable_receiving(mon);
        // get the file descriptor for the mon (used w/ select);
        udev_fd = udev_monitor_get_fd(mon);
    }

    if(udev_fd < 0) {
        ofLogError("ofAppEGLWindow") << "setupNativeUDev(): did not create udev object, udev_fd < 0";
    }

}

//------------------------------------------------------------
void ofAppEGLWindow::destroyNativeUDev() {
    udev_unref(udev); // clean up
}


//------------------------------------------------------------
void ofAppEGLWindow::setupNativeMouse() {
    struct dirent **eps;
    int n = scandir("/dev/input/by-path/", &eps, filter_mouse, dummy_sort);

    // make sure that we found an appropriate entry
    if(n >= 0 && eps != 0 && eps[0] != 0) {
        string devicePathBuffer;
        devicePathBuffer.append("/dev/input/by-path/");
        devicePathBuffer.append(eps[0]->d_name);
        mouse_fd = open(devicePathBuffer.c_str(), O_RDONLY | O_NONBLOCK);
        ofLogNotice("ofAppEGLWindow") << "setupMouse(): mouse_fd= " <<  mouse_fd << " devicePath=" << devicePathBuffer;
    } else {
        ofLogNotice("ofAppEGLWindow") << "setupMouse(): unabled to find mouse";
    }

    if (mouse_fd >= 0) {
        char deviceNameBuffer[256] = "Unknown Device";
        ioctl(mouse_fd, EVIOCGNAME(sizeof(deviceNameBuffer)), deviceNameBuffer);
        ofLogNotice("ofAppEGLWindow") << "setupMouse(): mouse device name = " << deviceNameBuffer;
    } else {
        ofLogError("ofAppEGLWindow") << "setupMouse(): did not open mouse";
    }

    mb.mouseButtonState = 0;

    if(mouse_fd < 0) {
        ofLogError("ofAppEGLWindow") << "setupMouse(): did not open mouse, mouse_fd < 0";
    }else {
        mouseDetected = true;
	}


}

//------------------------------------------------------------
void ofAppEGLWindow::setupNativeKeyboard() {
    struct dirent **eps;
    int n = scandir("/dev/input/by-path/", &eps, filter_kbd, dummy_sort);

    // make sure that we found an appropriate entry
    if(n >= 0 && eps != 0 && eps[0] != 0) {
        string devicePathBuffer;
        devicePathBuffer.append("/dev/input/by-path/");
        devicePathBuffer.append(eps[0]->d_name);
        keyboard_fd = open(devicePathBuffer.c_str(), O_RDONLY | O_NONBLOCK);
        ofLogNotice("ofAppEGLWindow") << "setupKeyboard(): keyboard_fd= " <<  keyboard_fd << " devicePath=" << devicePathBuffer;
		
    } else {
        ofLogWarning("ofAppEGLWindow") << "setupKeyboard(): unabled to find keyboard";
    }

    if (keyboard_fd >= 0) {
        char deviceNameBuffer[256] = "Unknown Device";
        ioctl(keyboard_fd, EVIOCGNAME(sizeof(deviceNameBuffer)), deviceNameBuffer);
        ofLogNotice("ofAppEGLWindow") << "setupKeyboard(): keyboard device name = " << deviceNameBuffer;
    

        // save current terminal settings
        tcgetattr (STDIN_FILENO, &tc);
        ots = tc;
        // disable echo on our temporary settings
        tc.c_lflag &= ~ECHO;
        tc.c_lflag |= ECHONL;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &tc);

    } else {
        ofLogError("ofAppEGLWindow") << "setupKeyboard(): did not open keyboard";
    }

    kb.shiftPressed = false;
    kb.capsLocked = false;
    
    if(keyboard_fd < 0) {
        ofLogError("ofAppEGLWindow") << "setupKeyboard(): did not open keyboard, keyboard_fd < 0";
    }else {
        keyboardDetected = true;
	}
}

//------------------------------------------------------------
void ofAppEGLWindow::destroyNativeMouse() {
    if(mouse_fd >= 0) {
        // nothing to do
    }
}

//------------------------------------------------------------
void ofAppEGLWindow::destroyNativeKeyboard() {
    ofLogNotice("ofAppEGLWindow") << "destroyNativeKeyboard()";

    if (keyboard_fd >= 0) {
      tcsetattr (STDIN_FILENO, TCSAFLUSH, &ots);
    } else {
      ofLogNotice("ofAppEGLWindow") << "destroyNativeKeyboard(): unable to reset terminal";
    }
}


//------------------------------------------------------------
void ofAppEGLWindow::readNativeUDevEvents() {
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
            // TODO: finish auto connect
            ofLogNotice() << "Got device";
            ofLogNotice() << "   node: %s\n", udev_device_get_devnode(dev);
            ofLogNotice() << "   subsystem: %s\n", udev_device_get_subsystem(dev);
            ofLogNotice() << "   devtype: %s\n", udev_device_get_devtype(dev);
            ofLogNotice() << "   action: %s\n", udev_device_get_action(dev);
            udev_device_unref(dev);
        }
        else {
            ofLogNotice("ofAppEGLWindow") << "readNativeUDevEvents(): device returned by receive_device() is NULL";
        }                   
    }
}

//------------------------------------------------------------
void ofAppEGLWindow::readNativeKeyboardEvents() {
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
                    ofLogNotice("ofAppEGLWindow") << "readKeyboardEvents(): input_event.code is outside of our small range";
                }
            }
        } else if(ev.type == EV_MSC) {
            // EV_MSC events are used for input and output events that 
            // do not fall under other categories.
            // ofLogVerbose("ofAppEGLWindow") << "readKeyboardEvents(): EV_MSC";
        } else if(ev.type == EV_SYN ) {
            // EV_SYN Used as markers to separate events. Events may be 
            // separated in time or in space, such as with the multitouch protocol.
            // ofLogVerbose("ofAppEGLWindow") << "readKeyboardEvents(): EV_SYN";
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
}

//------------------------------------------------------------
void ofAppEGLWindow::readNativeMouseEvents() {
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
                    ofLogNotice("ofAppEGLWindow") << "readMouseEvents(): unknown mouse axis (perhaps it's the scroll wheel?)";
                    break;
             }

        } else if(ev.type == EV_KEY) {
            // only tracking three buttons now ...
            if(ev.code == BTN_LEFT) {
                if(ev.value == 0) { // release
                    mouseEvent.button = OF_MOUSE_BUTTON_LEFT;
                    mouseEvent.type = ofMouseEventArgs::Released;
                    mb.mouseButtonState &= ~MOUSE_BUTTON_LEFT_MASK;
                    pushMouseEvent = true;
                } else if(ev.value == 1) { // press
                    mb.mouseButtonState |= MOUSE_BUTTON_LEFT_MASK;
                    mouseEvent.type = ofMouseEventArgs::Pressed;
                    mouseEvent.button = OF_MOUSE_BUTTON_LEFT;
                    pushMouseEvent = true;
                } else { // unknown
                    ofLogNotice("ofAppEGLWindow") << "readMouseEvents(): EV_KEY : unknown ev.value = " << ev.value;
                }
            } else if(ev.code == BTN_MIDDLE) {
                if(ev.value == 0) { // release
                    mouseEvent.button = OF_MOUSE_BUTTON_MIDDLE;
                    mouseEvent.type = ofMouseEventArgs::Released;
                    mb.mouseButtonState &= ~MOUSE_BUTTON_MIDDLE_MASK;
                    pushMouseEvent = true;
                } else if(ev.value == 1) { // press
                    mb.mouseButtonState |= MOUSE_BUTTON_MIDDLE_MASK;
                    mouseEvent.type = ofMouseEventArgs::Pressed;
                    mouseEvent.button = OF_MOUSE_BUTTON_MIDDLE;
                    pushMouseEvent = true;
                } else { // unknown
                    ofLogNotice("ofAppEGLWindow") << "readMouseEvents(): EV_KEY : unknown ev.value = " << ev.value;
                }
            } else if(ev.code == BTN_RIGHT) {
                if(ev.value == 0) { // release
                    mouseEvent.button = OF_MOUSE_BUTTON_RIGHT;
                    mouseEvent.type = ofMouseEventArgs::Released;
                    mb.mouseButtonState &= ~MOUSE_BUTTON_RIGHT_MASK;
                    pushMouseEvent = true;
                } else if(ev.value == 1) { // press
                    mb.mouseButtonState |= MOUSE_BUTTON_RIGHT_MASK;
                    mouseEvent.type = ofMouseEventArgs::Pressed;
                    mouseEvent.button = OF_MOUSE_BUTTON_RIGHT;
                    pushMouseEvent = true;
                } else {
                    ofLogNotice("ofAppEGLWindow") << "readMouseEvents(): EV_KEY : unknown ev.value = " << ev.value;
                }
            } else {
                ofLogNotice("ofAppEGLWindow") << "readMouseEvents(): EV_KEY : unknown ev.code = " << ev.code;
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

            //ofLogVerbose("ofAppEGLWindow") << "readMouseEvents(): EV_SYN";
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

}

#ifdef TARGET_RASPBERRY_PI
//------------------------------------------------------------
void ofAppEGLWindow::initRPiNative() {
    bcm_host_init();

    memset(&dispman_native_window, 0x0, sizeof(EGL_DISPMANX_WINDOW_T));
    dispman_element = DISPMANX_NO_HANDLE;
    dispman_display = DISPMANX_NO_HANDLE;
    dispman_update  = DISPMANX_NO_HANDLE;
    memset(&dispman_clamp, 0x0, sizeof(DISPMANX_CLAMP_T));
    dispman_transform = DISPMANX_NO_ROTATE;
    memset(&dispman_alpha, 0x0, sizeof(VC_DISPMANX_ALPHA_T)); // zero dispman_alpha

}

//------------------------------------------------------------
void ofAppEGLWindow::exitRPiNative() {
    bcm_host_deinit();
}

//------------------------------------------------------------
bool ofAppEGLWindow::createRPiNativeWindow(const ofRectangle& requestedWindowRect){

  ofRectangle screenRect = getScreenRect();

  // make sure our requested window rectangle does not exceed the native 
  // screen size, or start outside of it.
  ofRectangle windowRect = screenRect.getIntersection(requestedWindowRect);

  ofLogNotice("ofAppEGLWindow") << "setupRPiNativeWindow(): screenRect: " << screenRect.width << "x" << screenRect.height;
  ofLogNotice("ofAppEGLWindow") << "setupRPiNativeWindow(): windowRect: " << windowRect.width << "x" << windowRect.height;

//////////////////////////
    VC_RECT_T dst_rect;

    dst_rect.x      = (int32_t)windowRect.x;
    dst_rect.y      = (int32_t)windowRect.y;
    dst_rect.width  = (int32_t)windowRect.width;
    dst_rect.height = (int32_t)windowRect.height;

    VC_RECT_T src_rect;

    src_rect.x      = 0;
    src_rect.y      = 0;
    src_rect.width  = dst_rect.width << 16;
    src_rect.height = dst_rect.height << 16;

    memset(&dispman_alpha, 0x0, sizeof(VC_DISPMANX_ALPHA_T)); // zero dispman_alpha
    dispman_alpha.flags = DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS;
    dispman_alpha.opacity = ofClamp(settings.eglWindowOpacity,0,255);
    dispman_alpha.mask = 0;

    memset(&dispman_clamp, 0x0, sizeof(DISPMANX_CLAMP_T));

    // there are other values for dispman_transform, but they do not seem to have an effect
    dispman_transform = DISPMANX_NO_ROTATE;

    // get the zero display
    dispman_display = vc_dispmanx_display_open(settings.screenNum);
    
    // begin the display manager interaction
    dispman_update  = vc_dispmanx_update_start( 0 );

    // add a "display manager element" with our parameters so 
    // that it can fill in the structures.  we will pass this
    // filled dispman_element to our native window, which will
    // be used to construct the EGL surface, etc.
    dispman_element = vc_dispmanx_element_add ( dispman_update, 
                                                dispman_display,
                                                settings.layer, // layer 
                                                &dst_rect, // dst rect
                                                (DISPMANX_RESOURCE_HANDLE_T)0, // src
                                                &src_rect, // src rect
                                                DISPMANX_PROTECTION_NONE, // ? 
                                                &dispman_alpha, // alpha
                                                &dispman_clamp, // clamp
                                                dispman_transform // transform
                                                );

    if(dispman_element == DISPMANX_NO_HANDLE) {
      ofLogError("ofAppEGLWindow") << "setupRPiNativeWindow(): dispman_element == DISPMANX_NO_HANDLE";
      return false;
    } else if(dispman_element == (unsigned)DISPMANX_INVALID) {
      ofLogError("ofAppEGLWindow") << "setupRPiNativeWindow(): dispman_element == DISPMANX_INVALID";
      return false;
    }

    // set dispman_native_window to zero
    memset(&dispman_native_window, 0x0, sizeof(EGL_DISPMANX_WINDOW_T));
    dispman_native_window.element = dispman_element;
    dispman_native_window.width   = (int32_t)windowRect.width;
    dispman_native_window.height  = (int32_t)windowRect.height;

    // set background to black (not required)
    vc_dispmanx_display_set_background(dispman_update, dispman_display, 0x00, 0x00, 0x00);

    // finished with display manager update, so sync
    vc_dispmanx_update_submit_sync( dispman_update );

    currentWindowRect = windowRect;

    return true;
}
#endif

//------------------------------------------------------------
// X11 BELOW
//------------------------------------------------------------
bool ofAppEGLWindow::createX11NativeWindow(const ofRectangle& requestedWindowRect){

  // X11 variables
  x11Window      = 0;
  x11Display     = 0;
  x11ScreenNum   = 0; // TODO: settings.screenNum?
  x11Screen      = 0;
  XVisualInfo*  x11Visual   = 0; // TODO does this need to be deleted?
  Colormap      x11Colormap = 0;

  /*
    Step 0 - Create a NativeWindowType that we can use it for OpenGL ES output
  */
  Window               sRootWindow;
  XSetWindowAttributes sWA;
  unsigned int         ui32Mask;
  int                  i32Depth;
  
  //ofRectangle screenRect = getScreenRect();

  // make sure our requested window rectangle does not exceed the native 
  // screen size, or start outside of it.
  ofRectangle windowRect = requestedWindowRect.getStandardized();//screenRect.getIntersection(requestedWindowRect);

  // Initializes the display and screen
  x11Display = XOpenDisplay( 0 );
  if (!x11Display) {
    ofLogError("ofAppEGLWindow") << "unable to open X display";
    return false;
  }
 
  x11ScreenNum = XDefaultScreen( x11Display );

  x11Screen = XDefaultScreenOfDisplay(x11Display);

  // Gets the window parameters
  sRootWindow = RootWindow(x11Display, x11ScreenNum);
  i32Depth = DefaultDepth(x11Display, x11ScreenNum);
  x11Visual = new XVisualInfo(); // TODO does this need to be deleted?

  XMatchVisualInfo( x11Display, 
                    x11ScreenNum, 
                    i32Depth, 
                    TrueColor, 
                    x11Visual);

  if (!x11Visual) {
    ofLogError("ofAppEGLWindow") << "unable to acquire XVisualInfo";
    return false;
  }

  x11Colormap = XCreateColormap( x11Display, sRootWindow, x11Visual->visual, AllocNone );

  delete x11Visual; // TODO : is this ok?

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
                            (int)windowRect.x, (int)windowRect.y, // Specify the x and y coordinates, 
                            // which are the top-left outside corner 
                            // of the window's borders and are relative 
                            // to the inside of the parent window's borders. 
                            (unsigned int)windowRect.width, (unsigned int)windowRect.height, // Specify the width and height, which are the 
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

  // check success?  
  currentWindowRect = windowRect;

  return true;
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
