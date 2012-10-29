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
#include "bcm_host.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofxRaspberryPi.h"
//#include "ofxRaspberryPiUtils.h"
#include "ofFileUtils.h"
#include <assert.h>
#include "ofGLRenderer.h"

static bool paused=true;
static bool surfaceDestroyed=false;

static int  sWindowWidth  = 480;
static int  sWindowHeight = 800;

static bool bSetupScreen = true;

static float frameRate = 60;

static int frames = 0;
static unsigned long onesec = 0;
static unsigned long previousFrameMicros = 0;
static int nFrameCount = 0;
static float targetRate = 60;
static unsigned long oneFrameTime = 0;
static bool bFrameRateSet = false;

static double			lastFrameTime;

static ofOrientation orientation = OF_ORIENTATION_DEFAULT;

static ofMutex mutex;
static bool threadedTouchEvents = false;

//static ofAppAndroidWindow window;

static EGLDisplay display;
static EGLSurface surface;
static EGLContext context;
static uint32_t screen_width;
static uint32_t screen_height;
static EGL_DISPMANX_WINDOW_T nativewindow;

//------------------------------------------------------------
void ofAppRaspberryPiWindow::setupOpenGL(int w, int h, int screenMode){
	bcm_host_init();
int32_t success = 0;
   EGLBoolean result;
   EGLint num_config;

   static EGL_DISPMANX_WINDOW_T nativewindow;

   DISPMANX_ELEMENT_HANDLE_T dispman_element;
   DISPMANX_DISPLAY_HANDLE_T dispman_display;
   DISPMANX_UPDATE_HANDLE_T dispman_update;
   VC_RECT_T dst_rect;
   VC_RECT_T src_rect;

   static const EGLint attribute_list[] =
   {
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_ALPHA_SIZE, 8,
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_NONE
   };

   EGLConfig config;

   // get an EGL display connection
   display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
   assert(display!=EGL_NO_DISPLAY);

   // initialize the EGL display connection
   result = eglInitialize(display, NULL, NULL);
   assert(EGL_FALSE != result);

   // get an appropriate EGL frame buffer configuration
   result = eglChooseConfig(display, attribute_list, &config, 1, &num_config);
   assert(EGL_FALSE != result);

   // create an EGL rendering context
   context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
   assert(context!=EGL_NO_CONTEXT);

   // create an EGL window surface
   success = graphics_get_display_size(0 /* LCD */, &screen_width, &screen_height);
   assert( success >= 0 );

   dst_rect.x = 0;
   dst_rect.y = 0;
   dst_rect.width = screen_width;
   dst_rect.height = screen_height;

   src_rect.x = 0;
   src_rect.y = 0;
   src_rect.width = screen_width << 16;
   src_rect.height = screen_height << 16;

   dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
   dispman_update = vc_dispmanx_update_start( 0 );

   dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
      0/*layer*/, &dst_rect, 0/*src*/,
      &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);

   nativewindow.element = dispman_element;
   nativewindow.width = screen_width;
   nativewindow.height = screen_height;
   vc_dispmanx_update_submit_sync( dispman_update );

   surface = eglCreateWindowSurface( display, config, &nativewindow, NULL );
   assert(surface != EGL_NO_SURFACE);

   // connect the context to the surface
   result = eglMakeCurrent(display, surface, surface, context);
   assert(EGL_FALSE != result);

   // Set background color and clear buffers
   glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
   glClear( GL_COLOR_BUFFER_BIT );
   glClear( GL_DEPTH_BUFFER_BIT );

   cout << "CREATED SCREEN WITH SIZE " << screen_width << " x " << screen_height << endl;

   ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer));
}

//------------------------------------------------------------
void ofAppRaspberryPiWindow::initializeWindow() {

}

//------------------------------------------------------------
void ofAppRaspberryPiWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	appPtr->setup();
	while (true){
		appPtr->update();
		ofNotifyUpdate();

		//ofViewport(0, 0, screen_width, screen_height);
		float * bgPtr = ofBgColorPtr();
		bool bClearAuto = ofbClearBg();
		if ( bClearAuto == true || nFrameCount < 3){
			ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
		}

		ofSetupScreenPerspective(screen_width, screen_height, OF_ORIENTATION_DEFAULT);
		appPtr->draw();
		ofNotifyDraw();

	 	eglSwapBuffers(display, surface);
		nFrameCount++;
	}
	eglMakeCurrent( display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	eglDestroySurface( display, surface );
	eglDestroyContext( display, context );
	eglTerminate( display );
}

//------------------------------------------------------------
int	ofAppRaspberryPiWindow::getWidth(){ return screen_width; }
//------------------------------------------------------------
int	ofAppRaspberryPiWindow::getHeight(){ return screen_height; }
//------------------------------------------------------------
int ofAppRaspberryPiWindow::getFrameNum(){return nFrameCount; }
//------------------------------------------------------------
bool ofAppRaspberryPiWindow::_getMouse() {
    static int fd = -1;
    const int width=state->screen_width, height=state->screen_height;
    static int x=800, y=400;
    const int XSIGN = 1<<4, YSIGN = 1<<5;
    if (fd<0) {
       fd = open("/dev/input/mouse0",O_RDONLY|O_NONBLOCK);
    }
    if (fd>=0) {
        struct {char buttons, dx, dy; } m;
        while (1) {
           int bytes = read(fd, &m, sizeof m);
           if (bytes < (int)sizeof m) goto _exit;
           if (m.buttons&8) {
              break; // This bit should always be set
           }
           read(fd, &m, 1); // Try to sync up again
        }
        if (m.buttons&3)
           return m.buttons&3;
        x+=m.dx;
        y+=m.dy;
        if (m.buttons&XSIGN)
           x-=256;
        if (m.buttons&YSIGN)
           y-=256;
        if (x<0) x=0;
        if (y<0) y=0;
        if (x>width) x=width;
        if (y>height) y=height;
   }
_exit:
   if (outx) *outx = x;
   if (outy) *outy = y;
   return 0;
}
//------------------------------------------------------------
bool ofAppRaspberryPiWindow::_getKeyboard() {}
//------------------------------------------------------------
bool ofAppRaspberryPiWindow::_getJoystick() {}