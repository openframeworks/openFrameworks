#include "ofAppGLFWWindow.h"
#include "ofEvents.h"

#include "ofBaseApp.h"
#include "ofMain.h"
#include "ofProgrammableGLRenderer.h"

#define USE_PROGRAMMABLE_GL

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
	bNewScreenMode		= false;
	samples				= 0;
	nFramesForFPS		= 0;
	timeNow				= 0;
	timeThen			= 0;
	frameRate			= 0;
	orientation = OF_ORIENTATION_DEFAULT;

//	 OF_KEY_MODIFIER = 0x0000;
//	 OF_KEY_RETURN	= 294;


}

void ofAppGLFWWindow::setFSAASamples(int _samples){
	samples=_samples;
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
	
#ifdef USE_PROGRAMMABLE_GL
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	
	windowP = glfwCreateWindow(w, h, "GLFW Window", NULL, NULL);
	
	
	
//	if (windowMode == OF_WINDOW){
//
//		
//		result = glfwOpenWindow(
//		          w, h,          // Width and height of window
//		          8, 8, 8,           // Number of red, green, and blue bits for color buffer
//		          8,                 // Number of bits for alpha buffer
//		          32,                // Number of bits for depth buffer (Z-buffer)
//		          0,                 // Number of bits for stencil buffer
//		          GLFW_WINDOW        // We want a desktop window (could be GLFW_FULLSCREEN)
//		      );
//	}else if(windowMode == OF_FULLSCREEN){
//		result = glfwOpenWindow(
//				  getScreenSize().x, getScreenSize().y,          // Width and height of window
//				  8, 8, 8,           // Number of red, green, and blue bits for color buffer
//				  8,                 // Number of bits for alpha buffer
//				  32,                // Number of bits for depth buffer (Z-buffer)
//				  0,                 // Number of bits for stencil buffer
//				  GLFW_FULLSCREEN        // We want a desktop window (could be GLFW_FULLSCREEN)
//			  );
//		showCursor();
//	}else if(windowMode == OF_GAME_MODE){
//		result = glfwOpenWindow(
//				  w, h,          // Width and height of window
//				  8, 8, 8,           // Number of red, green, and blue bits for color buffer
//				  8,                 // Number of bits for alpha buffer
//				  32,                // Number of bits for depth buffer (Z-buffer)
//				  0,                 // Number of bits for stencil buffer
//				  GLFW_FULLSCREEN        // We want a desktop window (could be GLFW_FULLSCREEN)
//			  );
//		showCursor();
//	}
//	else
//	{
//		printf("**** invalid windowMode\n");
//	}
//	
//	if ( result != GL_TRUE )
//	{
//		printf("**** failed to open glfw window\n");
//	}
//	else
//		printf("*--- opened glfw window\n");

	setVerticalSync(false);
	// Set window title
//	glfwSetWindowTitle( " " );

//	glfwEnable( windowP, GLFW_KEY_REPEAT );

	requestedHeight = requestedHeight < 1 ? 1 : requestedHeight;
	glfwGetWindowSize( windowP, &requestedWidth, &requestedHeight );
	

	nonFullScreenW = ofGetWidth();
	nonFullScreenH = ofGetHeight();

    glfwMakeContextCurrent(windowP);
	
	// glewExperimental = true;
	glewInit();
	
	fprintf(stdout,"Vendor:   %s\n",   (char*)glGetString(GL_VENDOR));
	fprintf(stdout,"Renderer: %s\n",   (char*)glGetString(GL_RENDERER));
	fprintf(stdout,"Version:  %s\n",   (char*)glGetString(GL_VERSION));
	fprintf(stdout,"GLSL:     %s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));


	glfwGetWindowSize(windowP, &windowW, &windowH );
		
    setWindowPosition(100, 50);
	
}

//--------------------------------------------
int ofAppGLFWWindow::exit_cb(GLFWwindow* windowP_){
	OF_EXIT_APP(0);
	return 0;
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

		if ( bNewScreenMode ){
			changeMode();
			bNewScreenMode = false;
		}

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

void ofAppGLFWWindow::changeMode(){
	//glfwToggleFullscreen();

	if( windowMode == OF_FULLSCREEN){

		nonFullScreenW = ofGetWidth();
		nonFullScreenH = ofGetHeight();

		//----------------------------------------------------
		ofAppGLFWWindow::setWindowShape(getScreenSize().x, getScreenSize().y);
		ofAppGLFWWindow::setWindowPosition(0,0);

		#ifdef TARGET_OSX
//	tig: this doesn't compile
		// SetSystemUIMode(kUIModeAllHidden,NULL);
		#endif

	}else if( windowMode == OF_WINDOW ){

		ofAppGLFWWindow::setWindowShape(nonFullScreenW, nonFullScreenH);

		//----------------------------------------------------
		// if we have recorded the screen posion, put it there
		// if not, better to let the system do it (and put it where it wants)
		if (nFrameCount > 0){
			ofAppGLFWWindow::setWindowPosition(nonFullScreenX,nonFullScreenY);
		}
		//----------------------------------------------------

		#ifdef TARGET_OSX
		// tig:this doesn't compile
//			SetSystemUIMode(kUIModeNormal,NULL);
		#endif
	}
}

void ofAppGLFWWindow::setWindowTitle(string title){
	glfwSetWindowTitle(windowP,title.c_str());
}
//------------------------------------------------------------
ofPoint ofAppGLFWWindow::getWindowSize(){
	if(windowMode == OF_FULLSCREEN)
	{
		GLFWvidmode desktopMode;
		desktopMode = glfwGetVideoMode(glfwGetWindowMonitor(windowP));
		return ofVec3f(desktopMode.width, desktopMode.height,0);
	}else{
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
	GLFWvidmode desktopMode;
	desktopMode = glfwGetVideoMode(glfwGetWindowMonitor(windowP));

	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return ofVec3f(desktopMode.width, desktopMode.height,0);
	}else{
		return ofVec3f(desktopMode.height, desktopMode.width,0);
	}
}

int ofAppGLFWWindow::getWidth(){
	if(windowMode == OF_FULLSCREEN)
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
	if(windowMode == OF_FULLSCREEN)
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
//	#ifdef TARGET_OSX
//		CGDisplayHideCursor(kCGDirectMainDisplay);
//	#else
//		glfwDisable( windowP, GLFW_MOUSE_CURSOR );
//	#endif
};

void ofAppGLFWWindow::showCursor(){
//	#ifdef TARGET_OSX
//		//THIS HANGS ON OSX
//		//CGDisplayShowCursor(kCGDirectMainDisplay);
//	#else
//		glfwEnable( GLFW_MOUSE_CURSOR );
//	#endif
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
	if (windowMode == OF_GAME_MODE) return;

	if(fullscreen && windowMode==OF_WINDOW){
		windowMode    = OF_FULLSCREEN;
		bNewScreenMode = true;
	}
	else if(!fullscreen && windowMode==OF_FULLSCREEN){
		windowMode    = OF_WINDOW;
		bNewScreenMode = true;
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::toggleFullscreen(){
	if (windowMode == OF_GAME_MODE) return;

	bNewScreenMode = true;
	if (windowMode == OF_WINDOW){
		windowMode = OF_FULLSCREEN;
	} else {
		windowMode = OF_WINDOW;
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
	static ofEventArgs voidEventArgs;

	//	thanks to jorge for the fix:
	//	http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate

	if(ofAppPtr)
		ofAppPtr->update();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents().update, voidEventArgs );
	#endif

}

//--------------------------------------------
// callbacks


void ofAppGLFWWindow::idle_cb(){
	//	thanks to jorge for the fix:
	//	http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate
	
	/*if (nFrameCount != 0 && bFrameRateSet == true){
		diffMillis = ofGetElapsedTimeMillis() - prevMillis;
		if (diffMillis > millisForFrame){
			; // we do nothing, we are already slower than target frame
		} else {
			int waitMillis = millisForFrame - diffMillis;
#ifdef TARGET_WIN32
			Sleep(waitMillis);         //windows sleep in milliseconds
#else
			usleep(waitMillis * 1000);   //mac sleep in microseconds - cooler :)
#endif
		}
	}
	prevMillis = ofGetElapsedTimeMillis(); // you have to measure here
	
    // -------------- fps calculation:
	// theo - now moved from display to idle_cb
	// discuss here: http://github.com/openframeworks/openFrameworks/issues/labels/0062#issue/187
	//
	//
	// theo - please don't mess with this without letting me know.
	// there was some very strange issues with doing ( timeNow-timeThen ) producing different values to: double diff = timeNow-timeThen;
	// http://www.openframeworks.cc/forum/viewtopic.php?f=7&t=1892&p=11166#p11166
	
	timeNow = ofGetElapsedTimef();
	double diff = timeNow-timeThen;
	if( diff  > 0.00001 ){
		fps			= 1.0 / diff;
		frameRate	*= 0.9f;
		frameRate	+= 0.1f*fps;
	}
	lastFrameTime	= diff;
	timeThen		= timeNow;*/
  	// --------------
	
	ofNotifyUpdate();

	// glutPostRedisplay();

}

void ofAppGLFWWindow::display(void){
	glfwMakeContextCurrent(windowP);

	if(!ofGLIsFixedPipeline()) ofGetProgrammableGLRenderer()->startRender();
	// set viewport, clear the screen
	ofViewport();
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();

	// I don't know why, I need more than one frame at the start in fullscreen mode
	// also, in non-fullscreen mode, windows/intel graphics, this bClearAuto just fails.
	// I seem to have 2 buffers, alot of flickering
	// and don't accumulate the way I expect.
	// with this line:   if ((bClearAuto == true) || nFrameCount < 3){
	// we do nFrameCount < 3, so that the buffers are cleared at the start of the app
	// or else we have video memory garbage to draw on to...

	glDrawBuffer( GL_BACK );
	
	// printf("nFrameCount %i \n", nFrameCount);
	
	
	// glDrawBuffer( GL_FRONT );

	
	
	if ( bClearAuto == true ){
		glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glClear( GL_COLOR_BUFFER_BIT );
	}

	glDrawBuffer( GL_BACK );
	

	if( bEnableSetupScreen )ofSetupScreen();


	ofNotifyDraw();


	if(!ofGLIsFixedPipeline()) ofGetProgrammableGLRenderer()->finishRender();

	// Swap front and back buffers (we use a double buffered display)
    glfwSwapBuffers(windowP);
	glfwPollEvents();

}


//------------------------------------------------------------
void ofAppGLFWWindow::exitApp(){
//	--  This is already happening in ofAppRunner

//	static ofEventArgs voidEventArgs;
//	if(ofAppPtr)ofAppPtr->exit();
//
//	#ifdef OF_USING_POCO
//		ofNotifyEvent( ofEvents().exit, voidEventArgs );
//	#endif

	ofLog(OF_LOG_VERBOSE,"GLFW app is being terminated!");

	// Terminate GLFW
	glfwTerminate();

	OF_EXIT_APP(0);
}

//------------------------------------------------------------
static void rotateMouseXY(ofOrientation orientation, int &x, int &y) {
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
void ofAppGLFWWindow::motion_cb(GLFWwindow* windowP_, int x, int y) {
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

	if(state == GLFW_PRESS){
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
		//glfwSetWindowRefreshCallback(idle_cb);
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
