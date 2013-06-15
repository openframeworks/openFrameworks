#include "ofAppGLFWWindow.h"
#include "ofEvents.h"

#include "ofBaseApp.h"
#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"

#ifdef TARGET_LINUX
#include "ofIcon.h"
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include "glfw3native.h"
#include <X11/Xatom.h>
#elif defined(TARGET_OSX)
#include <Cocoa/Cocoa.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#include "GLFW/glfw3native.h"
#elif defined(TARGET_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include "GL/glfw3native.h"
#endif

//========================================================================
// static variables:

ofBaseApp *	ofAppGLFWWindow::ofAppPtr;
ofAppGLFWWindow	* ofAppGLFWWindow::instance;
GLFWwindow* ofAppGLFWWindow::windowP = NULL;



//-------------------------------------------------------
ofAppGLFWWindow::ofAppGLFWWindow():ofAppBaseWindow(){
	ofLog(OF_LOG_VERBOSE,"creating glfw window");
	bEnableSetupScreen	= true;
	buttonInUse			= 0;
	buttonPressed		= false;

	nonFullScreenX		= 0;
	nonFullScreenY		= 0;
	nonFullScreenW		= 0;
	nonFullScreenH		= 0;

	samples				= 0;
	rBits=gBits=bBits=aBits = 8;
	depthBits			= 24;
	stencilBits			= 0;

	orientation 		= OF_ORIENTATION_DEFAULT;

	requestedWidth		= 0;
	requestedHeight		= 0;
	windowMode			= OF_WINDOW;

	windowW				= 0;
	windowH				= 0;
	bDoubleBuffered		= true;

	ofAppPtr			= NULL;
	instance			= this;

	glVersionMinor=glVersionMajor=-1;
	nFramesSinceWindowResized = 0;

	iconSet = false;

}


//------------------------------------------------------------
void ofAppGLFWWindow::setNumSamples(int _samples){
	samples=_samples;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setDoubleBuffering(bool doubleBuff){
	bDoubleBuffered = doubleBuff;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setColorBits(int r, int g, int b){
	rBits=r;
	gBits=g;
	bBits=b;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setAlphaBits(int a){
	aBits=a;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setDepthBits(int depth){
	depthBits=depth;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setStencilBits(int stencil){
	stencilBits=stencil;
}



//------------------------------------------------------------
void ofAppGLFWWindow::setOpenGLVersion(int major, int minor){
	glVersionMajor = major;
	glVersionMinor = minor;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setupOpenGL(int w, int h, int screenMode){

	requestedWidth = w;
	requestedHeight = h;


	if(!glfwInit( )){
		ofLog(OF_LOG_ERROR,"cannot init GLFW");
		return;
	}

//	printf("WINDOW MODE IS %i", screenMode);

	windowMode = screenMode;

	glfwWindowHint(GLFW_RED_BITS, rBits);
	glfwWindowHint(GLFW_GREEN_BITS, gBits);
	glfwWindowHint(GLFW_BLUE_BITS, bBits);
	glfwWindowHint(GLFW_ALPHA_BITS, aBits);
	glfwWindowHint(GLFW_DEPTH_BITS, depthBits);
	glfwWindowHint(GLFW_STENCIL_BITS, stencilBits);
#ifndef TARGET_OSX
	glfwWindowHint(GLFW_AUX_BUFFERS,bDoubleBuffered?1:0);
#endif
	glfwWindowHint(GLFW_SAMPLES,samples);

	if(glVersionMinor!=-1 && glVersionMajor!=-1){
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
		if(glVersionMajor>=3){
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		}
		#ifdef TARGET_OPENGLES
		glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_ES_API);
		#endif
	}

	if(windowMode==OF_GAME_MODE){
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		if(count>0){
			windowP = glfwCreateWindow(w, h, "", monitors[0], NULL);
		}else{
			ofLogError() << "can't find any monitor";
			return;
		}
	}else{
		windowP = glfwCreateWindow(w, h, "", NULL, NULL);
		if(windowMode==OF_FULLSCREEN){
			setFullscreen(true);
		}
	}
    if(!windowP) {
        ofLogError() << "error creating GLFW window";
        return;
    }

	setVerticalSync(false);
	// Set window title
//	glfwSetWindowTitle( " " );

//	glfwEnable( windowP, GLFW_KEY_REPEAT );

	requestedHeight = requestedHeight < 1 ? 1 : requestedHeight;
	glfwGetWindowSize( windowP, &requestedWidth, &requestedHeight );


	nonFullScreenW = w;
	nonFullScreenH = h;

    glfwMakeContextCurrent(windowP);

    glfwGetWindowSize(windowP, &windowW, &windowH );


}

//--------------------------------------------
void ofAppGLFWWindow::exit_cb(GLFWwindow* windowP_){
	OF_EXIT_APP(0);
}

//--------------------------------------------
void ofAppGLFWWindow::initializeWindow(){
	 //----------------------
	 // setup the callbacks

	glfwSetMouseButtonCallback(windowP, mouse_cb);
	glfwSetCursorPosCallback(windowP, motion_cb);
	glfwSetKeyCallback(windowP, keyboard_cb);
	glfwSetWindowSizeCallback(windowP, resize_cb);
	glfwSetWindowCloseCallback(windowP, exit_cb);
	glfwSetScrollCallback(windowP, scroll_cb);

#ifdef TARGET_LINUX
    if(!iconSet){
		ofPixels iconPixels;
		#ifdef DEBUG
			iconPixels.allocate(ofIconDebug.width,ofIconDebug.height,ofIconDebug.bytes_per_pixel);
			GIMP_IMAGE_RUN_LENGTH_DECODE(iconPixels.getPixels(),ofIconDebug.rle_pixel_data,iconPixels.getWidth()*iconPixels.getHeight(),ofIconDebug.bytes_per_pixel);
		#else
			iconPixels.allocate(ofIcon.width,ofIcon.height,ofIcon.bytes_per_pixel);
			GIMP_IMAGE_RUN_LENGTH_DECODE(iconPixels.getPixels(),ofIcon.rle_pixel_data,iconPixels.getWidth()*iconPixels.getHeight(),ofIcon.bytes_per_pixel);
		#endif
		setWindowIcon(iconPixels);
    }
#endif
}

#ifdef TARGET_LINUX
//------------------------------------------------------------
void ofAppGLFWWindow::setWindowIcon(const string & path){
    ofPixels iconPixels;
	ofLoadImage(iconPixels,path);
	setWindowIcon(iconPixels);
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowIcon(const ofPixels & iconPixels){
	iconSet = true;

	GLXDrawable m_window = glfwGetX11Window(windowP);
	Display* m_display = glfwGetX11Display();

	int attributes[40];
	int i=0;
	attributes[i++] = GLX_RGBA;
	attributes[i++] = GLX_DOUBLEBUFFER;
	attributes[i++] = GLX_RED_SIZE; attributes[i++] = 1;
	attributes[i++] = GLX_BLUE_SIZE; attributes[i++] = 1;
	attributes[i++] = GLX_GREEN_SIZE; attributes[i++] = 1;
	attributes[i++] = GLX_DEPTH_SIZE; attributes[i++] = 1;
	attributes[i] = None;

	XVisualInfo * m_visual = glXChooseVisual(m_display, DefaultScreen(m_display), attributes);
	XWMHints *xwmhints = XAllocWMHints();
	XImage *x_image, *mask_image;
	Pixmap icon_pixmap, mask_pixmap;
	icon_pixmap = XCreatePixmap(m_display, m_window, iconPixels.getWidth(), iconPixels.getHeight(), 24);
	mask_pixmap = XCreatePixmap(m_display, m_window, iconPixels.getHeight(), iconPixels.getHeight(), 1);
	GC gc_icon = XCreateGC(m_display, icon_pixmap, 0, NULL);
	GC gc_mask = XCreateGC(m_display, mask_pixmap, 0, NULL);

	x_image = XCreateImage( m_display, m_visual->visual, 24, ZPixmap, 0, NULL, iconPixels.getWidth(), iconPixels.getHeight(), 32, 0 );
	mask_image = XCreateImage( m_display, m_visual->visual, 1, ZPixmap, 0, NULL, iconPixels.getWidth(), iconPixels.getHeight(), 8, 0);

	x_image->data = (char *)malloc(x_image->bytes_per_line * iconPixels.getHeight());
	mask_image->data = (char *)malloc( mask_image->bytes_per_line * iconPixels.getHeight());

	/* copy the OF icon into the XImage */
	int px, py;
	for (px=0; px<iconPixels.getWidth(); px++) {
		for (py=0; py<iconPixels.getHeight(); py++) {
			/* mask out pink */
			int i = py*iconPixels.getWidth()*4+px*4;
			XPutPixel(x_image, px, py, (iconPixels[i]<<16)+(iconPixels[i+1]<<8)+iconPixels[i+2] );
			XPutPixel(mask_image, px, py, iconPixels[i+3] );
		}
	}

	XPutImage(m_display, icon_pixmap, gc_icon, x_image, 0, 0, 0, 0, iconPixels.getWidth(), iconPixels.getHeight());
	XPutImage(m_display, mask_pixmap, gc_mask, mask_image, 0, 0, 0, 0, iconPixels.getWidth(), iconPixels.getHeight());

	// Now the pixmap is ok to assign to the window as a hint
	xwmhints->icon_pixmap = icon_pixmap;
	xwmhints->icon_mask = mask_pixmap;
	XFreeGC (m_display, gc_icon);
	XFreeGC (m_display, gc_mask);
	XDestroyImage( x_image ); /* frees x_image->data too */
	XDestroyImage( mask_image );

	xwmhints->initial_state = NormalState;
	xwmhints->input= True;
	xwmhints->flags= InputHint|IconPixmapHint|IconMaskHint|StateHint;
	XSetWMHints(m_display, m_window, xwmhints );
	XFree(xwmhints);
}
#endif

//--------------------------------------------
void ofAppGLFWWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	ofAppPtr = appPtr;

	glfwMakeContextCurrent(windowP);

	ofNotifySetup();
	while(true){
		ofNotifyUpdate();
		display();
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::display(void){
	if(ofGetGLProgrammableRenderer()){
		ofGetGLProgrammableRenderer()->startRender();
	}

	// set viewport, clear the screen
	ofViewport();		// used to be glViewport( 0, 0, width, height );
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();

	// to do non auto clear on PC for now - we do something like "single" buffering --
	// it's not that pretty but it work for the most part

	#ifdef TARGET_WIN32
	if (bClearAuto == false){
		glDrawBuffer (GL_FRONT);
	}
	#endif

	if ( bClearAuto == true ){
		ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
	}

	if( bEnableSetupScreen )ofSetupScreen();

	ofNotifyDraw();

	#ifdef TARGET_WIN32
	if (bClearAuto == false){
		// on a PC resizing a window with this method of accumulation (essentially single buffering)
		// is BAD, so we clear on resize events.
		if (nFramesSinceWindowResized < 3){
			ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
		} else {
			if ( (ofGetFrameNum() < 3 || nFramesSinceWindowResized < 3) && bDoubleBuffered)    glfwSwapBuffers(windowP);
			else                                                     glFlush();
		}
	} else {
		if(bDoubleBuffered){
		    glfwSwapBuffers(windowP);
		} else {
			glFlush();
		}
	}
	#else
		if (bClearAuto == false){
			// in accum mode resizing a window is BAD, so we clear on resize events.
			if (nFramesSinceWindowResized < 3){
				ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
			}
		}
		if(bDoubleBuffered){
		    glfwSwapBuffers(windowP);
		} else{
			glFlush();
		}
	#endif

	if(ofGetGLProgrammableRenderer()){
		ofGetGLProgrammableRenderer()->finishRender();
	}

	nFramesSinceWindowResized++;
	glfwPollEvents();

}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowTitle(string title){
	glfwSetWindowTitle(windowP,title.c_str());
}

//------------------------------------------------------------
ofPoint ofAppGLFWWindow::getWindowSize(){
	if(windowMode == OF_GAME_MODE)
	{
		const GLFWvidmode * desktopMode = glfwGetVideoMode(glfwGetWindowMonitor(windowP));
		if(desktopMode){
			return ofVec3f(desktopMode->width, desktopMode->height,0);
		}else{
			return ofPoint(windowW,windowH);
		}
	}else{
	    glfwGetWindowSize(windowP,&windowW,&windowH);
		return ofPoint(windowW,windowH);
	}
}

//------------------------------------------------------------
ofPoint ofAppGLFWWindow::getWindowPosition(){
	glfwGetWindowPos(windowP, &nonFullScreenX, &nonFullScreenY);
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return ofPoint(nonFullScreenX,nonFullScreenY,0);
	}else{
		return ofPoint(nonFullScreenY,nonFullScreenX,0);
	}
}

//------------------------------------------------------------
ofPoint ofAppGLFWWindow::getScreenSize(){
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	if(count>0){
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[0]);
		if(desktopMode){
			if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
				return ofVec3f(desktopMode->width, desktopMode->height,0);
			}else{
				return ofPoint(0,0);
			}
		}else{
			return ofPoint(0,0);
		}
	}else{
		return ofPoint(0,0);
	}
}

//------------------------------------------------------------
int ofAppGLFWWindow::getWidth(){
	if(windowMode == OF_GAME_MODE)
	{
		return getScreenSize().x;
	}
	else {
		if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
			return windowW;
		}else{
			return windowH;
		}
	}

}

//------------------------------------------------------------
int ofAppGLFWWindow::getHeight()
{
	if(windowMode == OF_GAME_MODE)
	{
		return getScreenSize().y;
	}
	else {
		if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
			return windowH;
		}else{
			return windowW;
		}
	}
}

//------------------------------------------------------------
int	ofAppGLFWWindow::getWindowMode(){
	return windowMode;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowPosition(int x, int y){
	glfwSetWindowPos(windowP,x,y);
	nonFullScreenX=x;
	nonFullScreenY=y;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowShape(int w, int h){
	glfwSetWindowSize(windowP,w,h);
	// this is useful, esp if we are in the first frame (setup):
	requestedWidth  = w;
	requestedHeight = h;
}

//------------------------------------------------------------
void ofAppGLFWWindow::hideCursor(){
	glfwSetInputMode(windowP,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
};

//------------------------------------------------------------
void ofAppGLFWWindow::showCursor(){
	glfwSetInputMode(windowP,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
};

//------------------------------------------------------------
void ofAppGLFWWindow::enableSetupScreen(){
	bEnableSetupScreen = true;
};

//------------------------------------------------------------
void ofAppGLFWWindow::disableSetupScreen(){
	bEnableSetupScreen = false;
};

//------------------------------------------------------------
void ofAppGLFWWindow::setFullscreen(bool fullscreen){
	if (fullscreen)
		windowMode = OF_FULLSCREEN;
	else
		windowMode = OF_WINDOW;

#ifdef TARGET_LINUX
#include <X11/Xatom.h>

	Window nativeWin = glfwGetX11Window(windowP);
	Display* display = glfwGetX11Display();


	Atom m_net_state= XInternAtom(display, "_NET_WM_STATE", false);
	Atom m_net_fullscreen= XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", false);

	XEvent xev;

	xev.xclient.type = ClientMessage;
	xev.xclient.serial = 0;
	xev.xclient.send_event = True;
	xev.xclient.window = nativeWin;
	xev.xclient.message_type = m_net_state;
	xev.xclient.format = 32;

	if (fullscreen)
		xev.xclient.data.l[0] = 1;
	else
		xev.xclient.data.l[0] = 0;

	xev.xclient.data.l[1] = m_net_fullscreen;
	xev.xclient.data.l[2] = 0;
	xev.xclient.data.l[3] = 0;
	xev.xclient.data.l[4] = 0;
	XSendEvent(display, RootWindow(display, DefaultScreen(display)),
	           False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
#elif defined(TARGET_OSX)
	if( windowMode == OF_FULLSCREEN){
        nonFullScreenX = getWindowPosition().x;
        nonFullScreenY = getWindowPosition().y;
		nonFullScreenW = getWindowSize().x;
		nonFullScreenH = getWindowSize().y;

		//----------------------------------------------------
		SetSystemUIMode(kUIModeAllHidden,NULL);
		NSWindow * cocoaWindow = glfwGetCocoaWindow(windowP);
		[cocoaWindow setStyleMask:NSBorderlessWindowMask];

		setWindowShape(getScreenSize().x, getScreenSize().y);
		setWindowPosition(0,0);

		//[cocoaWindow makeFirstResponder: cocoaWindow];

	}else if( windowMode == OF_WINDOW ){
		SetSystemUIMode(kUIModeNormal,NULL);
		NSWindow * cocoaWindow = glfwGetCocoaWindow(windowP);
		[cocoaWindow setStyleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask];

		setWindowShape(nonFullScreenW, nonFullScreenH);

		//----------------------------------------------------
		// if we have recorded the screen posion, put it there
		// if not, better to let the system do it (and put it where it wants)
		if (ofGetFrameNum() > 0){
			setWindowPosition(nonFullScreenX,nonFullScreenY);
		}

		//[cocoaWindow makeFirstResponder: cocoaWindow];
		//----------------------------------------------------

	}
#elif defined(TARGET_WIN32)
    if( windowMode == OF_FULLSCREEN){
        nonFullScreenX = getWindowPosition().x;
        nonFullScreenY = getWindowPosition().y;
		nonFullScreenW = getWindowSize().x;
		nonFullScreenH = getWindowSize().y;

		//----------------------------------------------------
		HWND hwnd = glfwGetWin32Window(windowP);
        SetWindowLong(hwnd, GWL_STYLE,
                  lStyle & ~(WS_CAPTION | WS_THICKFRAME));
        SetWindowLong(hwnd, GWL_EXSTYLE,
                  lExStyle & ~(WS_EX_DLGMODALFRAME |
                  WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

        SetWindowPos(hwnd, NULL, 0, 0,
                   getScreenSize().x, getScreenSize().y,
                   SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}else if( windowMode == OF_WINDOW ){
		HWND hwnd = glfwGetWin32Window(windowP);
        SetWindowLong(hwnd, GWL_STYLE, lStyle);
        SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
        setWindowShape(nonFullScreenW,nonFullScreenH);
        setWindowPosition(nonFullScreenX,nonFullScreenY);
		//----------------------------------------------------
	}
#endif
}

//------------------------------------------------------------
void ofAppGLFWWindow::toggleFullscreen(){
	if (windowMode == OF_GAME_MODE) return;


	if (windowMode == OF_WINDOW){
		setFullscreen(true);
	} else {
		setFullscreen(false);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::setOrientation(ofOrientation orientation){
	this->orientation = orientation;
}

//------------------------------------------------------------
ofOrientation ofAppGLFWWindow::getOrientation(){
	return orientation;
}

//------------------------------------------------------------
void ofAppGLFWWindow::exitApp(){
	ofLog(OF_LOG_VERBOSE,"GLFW app is being terminated!");

	// Terminate GLFW
	glfwTerminate();

	OF_EXIT_APP(0);
}

//------------------------------------------------------------
static void rotateMouseXY(ofOrientation orientation, double &x, double &y) {
	int savedY;
	switch(orientation) {
		case OF_ORIENTATION_180:
			x = ofGetWidth() - x;
			y = ofGetHeight() - y;
			break;

		case OF_ORIENTATION_90_RIGHT:
			savedY = y;
			y = x;
			x = ofGetWidth()-savedY;
			break;

		case OF_ORIENTATION_90_LEFT:
			savedY = y;
			y = ofGetHeight() - x;
			x = savedY;
			break;

		case OF_ORIENTATION_DEFAULT:
		default:
			break;
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::mouse_cb(GLFWwindow* windowP_, int button, int state, int mods) {
	ofLog(OF_LOG_VERBOSE,"button: %i",button);

	if (state == GLFW_PRESS) {
		ofNotifyMousePressed(ofAppPtr->mouseX, ofAppPtr->mouseY, button);
		instance->buttonPressed=true;
	} else if (state == GLFW_RELEASE) {
		ofNotifyMouseReleased(ofAppPtr->mouseX, ofAppPtr->mouseY, button);
		instance->buttonPressed=false;
	}
	instance->buttonInUse = button;


}

//------------------------------------------------------------
void ofAppGLFWWindow::motion_cb(GLFWwindow* windowP_, double x, double y) {
	rotateMouseXY(ofGetOrientation(), x, y);

	if(!instance->buttonPressed){
		ofNotifyMouseMoved(x, y);
	}else{
		ofNotifyMouseDragged(x, y, instance->buttonInUse);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::scroll_cb(GLFWwindow* windowP_, double x, double y) {
	// ofSendMessage("scroll|"+ofToString(x,5) + "|" + ofToString(y,5));
}


//------------------------------------------------------------
void ofAppGLFWWindow::keyboard_cb(GLFWwindow* windowP_, int key, int scancode, int action, int mods) {

	ofLog(OF_LOG_VERBOSE,"key: %i, state: %i",key,action);

	switch (key) {
		case GLFW_KEY_ESCAPE:
			key = OF_KEY_ESC;
			break;
		case GLFW_KEY_F1:
			key = OF_KEY_F1;
			break;
		case GLFW_KEY_F2:
			key = OF_KEY_F2;
			break;
		case GLFW_KEY_F3:
			key = OF_KEY_F3;
			break;
		case GLFW_KEY_F4:
			key = OF_KEY_F4;
			break;
		case GLFW_KEY_F5:
			key = OF_KEY_F5;
			break;
		case GLFW_KEY_F6:
			key = OF_KEY_F6;
			break;
		case GLFW_KEY_F7:
			key = OF_KEY_F7;
			break;
		case GLFW_KEY_F8:
			key = OF_KEY_F8;
			break;
		case GLFW_KEY_F9:
			key = OF_KEY_F9;
			break;
		case GLFW_KEY_F10:
			key = OF_KEY_F10;
			break;
		case GLFW_KEY_F11:
			key = OF_KEY_F11;
			break;
		case GLFW_KEY_F12:
			key = OF_KEY_F12;
			break;
		case GLFW_KEY_LEFT:
			key = OF_KEY_LEFT;
			break;
		case GLFW_KEY_RIGHT:
			key = OF_KEY_RIGHT;
			break;
		case GLFW_KEY_UP:
			key = OF_KEY_UP;
			break;
		case GLFW_KEY_DOWN:
			key = OF_KEY_DOWN;
			break;
		case GLFW_KEY_PAGE_UP:
			key = OF_KEY_PAGE_UP;
			break;
		case GLFW_KEY_PAGE_DOWN:
			key = OF_KEY_PAGE_DOWN;
			break;
		case GLFW_KEY_HOME:
			key = OF_KEY_HOME;
			break;
		case GLFW_KEY_END:
			key = OF_KEY_END;
			break;
		case GLFW_KEY_INSERT:
			key = OF_KEY_INSERT;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			key = OF_KEY_LEFT_SHIFT;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			key = OF_KEY_LEFT_CONTROL;
			break;
		case GLFW_KEY_LEFT_ALT:
			key = OF_KEY_LEFT_ALT;
			break;
		case GLFW_KEY_LEFT_SUPER:
			key = OF_KEY_LEFT_SUPER;
			break;
		case GLFW_KEY_RIGHT_SHIFT:
			key = OF_KEY_RIGHT_SHIFT;
			break;
		case GLFW_KEY_RIGHT_CONTROL:
			key = OF_KEY_RIGHT_CONTROL;
			break;
		case GLFW_KEY_RIGHT_ALT:
			key = OF_KEY_RIGHT_ALT;
			break;
		case GLFW_KEY_RIGHT_SUPER:
			key = OF_KEY_RIGHT_SUPER;
			break;
		default:
			break;
	}

	//GLFW defaults to uppercase - OF users are used to lowercase
	//if we are uppercase make lowercase
	// a better approach would be to check if shift keys are held down - and apply based on that
	if( key >= 65 && key <= 90 ){
		key += 32;
	}

	if(action == GLFW_PRESS || action == GLFW_REPEAT){
		ofNotifyKeyPressed(key);
		if (key == OF_KEY_ESC){				// "escape"
			exitApp();
		}
	}else if (action == GLFW_RELEASE){
		ofNotifyKeyReleased(key);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::resize_cb(GLFWwindow* windowP_,int w, int h) {
	if(ofAppPtr)ofAppPtr->windowResized(w,h);
	instance->windowW = w;
	instance->windowH = h;
	#ifdef OF_USING_POCO
		static ofResizeEventArgs resizeEventArgs;

		resizeEventArgs.width = w;
		resizeEventArgs.height = h;
		ofNotifyEvent( ofEvents().windowResized, resizeEventArgs );
	#endif

	instance->nFramesSinceWindowResized = 0;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setVerticalSync(bool bVerticalSync){
	if(bVerticalSync){
		glfwSwapInterval( 1);
	}else{
		glfwSwapInterval(0);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::listVideoModes(){
	int numModes;
	const GLFWvidmode * vidModes = glfwGetVideoModes(NULL, &numModes );
	for(int i=0; i<numModes; i++){
		printf("%i x %i %ibits",vidModes[i].width,vidModes[i].height,vidModes[i].redBits+vidModes[i].greenBits+vidModes[i].blueBits);
	}
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowIconified(){
	return glfwGetWindowAttrib(windowP, GLFW_ICONIFIED);
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowActive(){
//	return glfwGetWindowParam(GLFW_ACTIVE);
	return true;
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowResizeable(){
	return !glfwGetWindowAttrib(windowP, GLFW_RESIZABLE);
}

//------------------------------------------------------------
void ofAppGLFWWindow::iconify(bool bIconify){
	if(bIconify)
			glfwIconifyWindow(windowP);
	else
		glfwRestoreWindow(windowP);
}
