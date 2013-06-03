#include "ofAppGLFWWindow.h"
#include "ofEvents.h"

#include "ofBaseApp.h"
#include "ofMain.h"
#include "ofProgrammableGLRenderer.h"

#ifdef TARGET_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include "GL/glfw3native.h"
#elif defined(TARGET_OSX)
#include <Cocoa/Cocoa.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#include "GL/glfw3native.h"
#elif defined(TARGET_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include "GL/glfw3native.h"
#endif

//========================================================================
// static variables:

static int			buttonInUse;
static bool			buttonPressed;
static ofBaseApp *	ofAppPtr;

static int			nFramesForFPS ;
static float		timeNow;
static float		timeThen;

static int			windowW;
static int			windowH;

GLFWwindow* ofAppGLFWWindow::windowP = NULL;

//-------------------------------------------------------
ofAppGLFWWindow::ofAppGLFWWindow():ofAppBaseWindow(){
	ofLog(OF_LOG_VERBOSE,"creating glfw window");
	bEnableSetupScreen	= true;
	nFrameCount			= 0;
	buttonInUse			= 0;

	nonFullScreenX		= 0;
	nonFullScreenY		= 0;
	nonFullScreenW		= 0;
	nonFullScreenH		= 0;

	millisForFrame		= 0;
	prevMillis			= 0;
	diffMillis			= 0;
	bFrameRateSet		= false;
	frameRate			= 0;
	samples				= 0;
	nFramesForFPS		= 0;
	timeNow				= 0;
	timeThen			= 0;
	frameRate			= 0;
	orientation = OF_ORIENTATION_DEFAULT;

	glVersionMinor=glVersionMajor=-1;


}

void ofAppGLFWWindow::setFSAASamples(int _samples){
	samples=_samples;
}


void ofAppGLFWWindow::setOpenGLVersion(int major, int minor){
	glVersionMajor = major;
	glVersionMinor = minor;
}

void ofAppGLFWWindow::setupOpenGL(int w, int h, int screenMode){

	requestedWidth = w;
	requestedHeight = h;


	if(!glfwInit( )){
		ofLog(OF_LOG_ERROR,"cannot init GLFW");
		return;
	}

//	printf("WINDOW MODE IS %i", screenMode);

	windowMode = screenMode;

	glfwWindowHint( GLFW_SAMPLES, samples );
	// targets					default
	// GLFW_REFRESH_RATE			0
	// GLFW_ACCUM_RED_BITS;			0
	// GLFW_ACCUM_GREEN_BITS;		0
	// GLFW_ACCUM_BLUE_BITS;		0
	// GLFW_ACCUM_ALPHA_BITS;		0
	// GLFW_AUX_BUFFERS;			0
	// GLFW_STEREO;					0
	// GLFW_WINDOW_NO_RESIZE;		0
	// GLFW_FSAA_SAMPLES;			0
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 0);

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
		#ifdef TARGET_WIN32
		HWND hwnd = glfwGetWin32Window(windowP);
		lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		lStyle = GetWindowLong(hwnd, GWL_STYLE);
		#endif // TARGET_WIN32
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
	glfwSetScrollCallback(windowP, scroll_cb);}


//--------------------------------------------
void ofAppGLFWWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	ofAppPtr = appPtr;

	glfwMakeContextCurrent(windowP);

	ofNotifySetup();
	ofNotifyUpdate();

	while(true){
		if (nFrameCount != 0 && bFrameRateSet == true){
			diffMillis = ofGetElapsedTimeMillis() - prevMillis;
			if (diffMillis > millisForFrame){
				; // we do nothing, we are already slower than target frame
			} else {
				int waitMillis = millisForFrame - diffMillis;
				ofSleepMillis(waitMillis);
			}
		}


		unsigned long long newMillis = ofGetElapsedTimeMillis(); // you have to measure here
		lastFrameTime = (newMillis-prevMillis)/1000.;
		prevMillis = newMillis;

		idle();
		display();

		//	thanks to jorge for the fix:
		//	http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate
		timeNow = ofGetElapsedTimef();
		double diff = timeNow-timeThen;
		if( diff  > 0.00001 ){
			fps			= 1.0 / diff;
			frameRate	*= 0.9f;
			frameRate	+= 0.1f*fps;
		}
		//lastFrameTime	= diff;
		timeThen		= timeNow;

		nFrameCount++;

	}
}

void ofAppGLFWWindow::setWindowTitle(string title){
	glfwSetWindowTitle(windowP,title.c_str());
}
//------------------------------------------------------------
ofPoint ofAppGLFWWindow::getWindowSize(){
	if(windowMode == OF_GAME_MODE)
	{
		GLFWvidmode desktopMode;
		desktopMode = glfwGetVideoMode(glfwGetWindowMonitor(windowP));
		return ofVec3f(desktopMode.width, desktopMode.height,0);
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
		GLFWvidmode desktopMode;
		desktopMode = glfwGetVideoMode(monitors[0]);
		if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
			return ofVec3f(desktopMode.width, desktopMode.height,0);
		}else{
			return ofVec3f(desktopMode.height, desktopMode.width,0);
		}
	}else{
		return ofPoint(0,0);
	}
}

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

int	ofAppGLFWWindow::getWindowMode(){
	return windowMode;
}

void ofAppGLFWWindow::setWindowPosition(int x, int y){
	glfwSetWindowPos(windowP,x,y);
	nonFullScreenX=x;
	nonFullScreenY=y;
}

void ofAppGLFWWindow::setWindowShape(int w, int h){
	glfwSetWindowSize(windowP,w,h);
	// this is useful, esp if we are in the first frame (setup):
	requestedWidth  = w;
	requestedHeight = h;
}

void ofAppGLFWWindow::hideCursor(){
	glfwSetInputMode(windowP,GLFW_CURSOR_MODE,GLFW_CURSOR_HIDDEN);
};

void ofAppGLFWWindow::showCursor(){
	glfwSetInputMode(windowP,GLFW_CURSOR_MODE,GLFW_CURSOR_NORMAL);
};

void ofAppGLFWWindow::setFrameRate(float targetRate){
	// given this FPS, what is the amount of millis per frame
	// that should elapse?

	// --- > f / s

	bFrameRateSet 			= true;
	float durationOfFrame 	= 1.0f / (float)targetRate;
	millisForFrame 			= (int)(1000.0f * durationOfFrame);

	frameRate = targetRate;

};

void ofAppGLFWWindow::enableSetupScreen(){
	bEnableSetupScreen = true;
};

void ofAppGLFWWindow::disableSetupScreen(){
	bEnableSetupScreen = false;
};

//------------------------------------------------------------
int ofAppGLFWWindow::getFrameNum(){
	return nFrameCount;
}

//------------------------------------------------------------
float ofAppGLFWWindow::getFrameRate(){
	return fps;
}

//------------------------------------------------------------
double ofAppGLFWWindow::getLastFrameTime() {
	return lastFrameTime;
}

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
		if (nFrameCount > 0){
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

void ofAppGLFWWindow::setOrientation(ofOrientation orientation){
	this->orientation = orientation;
}

ofOrientation ofAppGLFWWindow::getOrientation(){
	return orientation;
}

//------------------------------------------------------------
void ofAppGLFWWindow::idle(void) {
	ofNotifyUpdate();

}

void ofAppGLFWWindow::display(void){
	glfwMakeContextCurrent(windowP);

	if(!ofGLIsFixedPipeline()) ofGetProgrammableGLRenderer()->startRender();
	// set viewport, clear the screen
	ofViewport();
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();

	if ( bClearAuto == true ){
		glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	if( bEnableSetupScreen )ofSetupScreen();


	ofNotifyDraw();


	if(!ofGLIsFixedPipeline()) ofGetProgrammableGLRenderer()->finishRender();

	// Swap front and back buffers (we use a double buffered display)
    glfwSwapBuffers(windowP);
	glfwPollEvents();

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
void ofAppGLFWWindow::mouse_cb(GLFWwindow* windowP_, int button, int state) {
	ofLog(OF_LOG_VERBOSE,"button: %i",button);

	if (state == GLFW_PRESS) {
		ofNotifyMousePressed(ofAppPtr->mouseX, ofAppPtr->mouseY, button);
		buttonPressed=true;
	} else if (state == GLFW_RELEASE) {
		ofNotifyMouseReleased(ofAppPtr->mouseX, ofAppPtr->mouseY, button);
		buttonPressed=false;
	}
	buttonInUse = button;


}

//------------------------------------------------------------
void ofAppGLFWWindow::motion_cb(GLFWwindow* windowP_, double x, double y) {
	rotateMouseXY(ofGetOrientation(), x, y);

	if(!buttonPressed){
		ofNotifyMouseMoved(x, y);
	}else{
		ofNotifyMouseDragged(x, y, buttonInUse);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::scroll_cb(GLFWwindow* windowP_, double x, double y) {
	// ofSendMessage("scroll|"+ofToString(x,5) + "|" + ofToString(y,5));
}


//------------------------------------------------------------
void ofAppGLFWWindow::keyboard_cb(GLFWwindow* windowP_, int key, int state) {

	ofLog(OF_LOG_VERBOSE,"key: %i, state: %i",key,state);

	switch (key) {
		case GLFW_KEY_ESC:
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
		case GLFW_KEY_PAGEUP:
			key = OF_KEY_PAGE_UP;
			break;
		case GLFW_KEY_PAGEDOWN:
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
		case GLFW_KEY_ENTER:
			key = OF_KEY_RETURN;
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

	if(state == GLFW_PRESS || state == GLFW_REPEAT){
		ofNotifyKeyPressed(key);
		if (key == OF_KEY_ESC){				// "escape"
			exitApp();
		}
	}else{
	 if (state == GLFW_RELEASE) ofNotifyKeyReleased(key);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::resize_cb(GLFWwindow* windowP_,int w, int h) {
	if(ofAppPtr)ofAppPtr->windowResized(w,h);
	windowW = w;
	windowH = h;
	#ifdef OF_USING_POCO
		static ofResizeEventArgs resizeEventArgs;

		resizeEventArgs.width = w;
		resizeEventArgs.height = h;
		ofNotifyEvent( ofEvents().windowResized, resizeEventArgs );
	#endif
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
//	GLFWvidmode vidModes[100];
//	int numModes = glfwGetVideoModes(NULL, NULL );
//	for(int i=0; i<numModes; i++){
//		printf("%i x %i %ibits",vidModes[i].Width,vidModes[i].Height,vidModes[i].RedBits+vidModes[i].GreenBits+vidModes[i].BlueBits);
//	}
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowIconified(){
	return glfwGetWindowParam(windowP, GLFW_ICONIFIED);
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowActive(){
//	return glfwGetWindowParam(GLFW_ACTIVE);
	return true;
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowResizeable(){
	return !glfwGetWindowParam(windowP, GLFW_RESIZABLE);
}

//------------------------------------------------------------

void ofAppGLFWWindow::iconify(bool bIconify){
	if(bIconify)
			glfwIconifyWindow(windowP);
	else
		glfwRestoreWindow(windowP);
}
//#endif
