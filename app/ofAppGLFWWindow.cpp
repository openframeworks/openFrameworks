#include "ofAppGLFWWindow.h"
#include "ofEvents.h"
#include "glfw.h"

#include "ofBaseApp.h"
#include "ofMain.h"

//========================================================================
// static variables:

static int			buttonInUse;
static bool			buttonPressed;
static ofBaseApp *	ofAppPtr;

static int			nFramesForFPS ;
static float		timeNow;
static float		timeThen;
static float		frameRate;

//-------------------------------------------------------
ofAppGLFWWindow::ofAppGLFWWindow():ofAppBaseWindow(){
	ofLog(OF_VERBOSE,"creating glfw window");
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
		ofLog(OF_ERROR,"cannot init GLFW");
		return;
	}

	printf("WINDOW MODE IS %i", screenMode);

	windowMode = screenMode;

	glfwOpenWindowHint( GLFW_FSAA_SAMPLES, samples );
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

	if (windowMode == OF_WINDOW){
		glfwOpenWindow(
		          w, h,          // Width and height of window
		          8, 8, 8,           // Number of red, green, and blue bits for color buffer
		          8,                 // Number of bits for alpha buffer
		          24,                // Number of bits for depth buffer (Z-buffer)
		          0,                 // Number of bits for stencil buffer
		          GLFW_WINDOW        // We want a desktop window (could be GLFW_FULLSCREEN)
		      );
	}else if(windowMode == OF_FULLSCREEN){
		glfwOpenWindow(
				  getScreenSize().x, getScreenSize().y,          // Width and height of window
				  8, 8, 8,           // Number of red, green, and blue bits for color buffer
				  8,                 // Number of bits for alpha buffer
				  24,                // Number of bits for depth buffer (Z-buffer)
				  0,                 // Number of bits for stencil buffer
				  GLFW_FULLSCREEN        // We want a desktop window (could be GLFW_FULLSCREEN)
			  );
		showCursor();
	}else if(windowMode == OF_GAME_MODE){
		glfwOpenWindow(
				  w, h,          // Width and height of window
				  8, 8, 8,           // Number of red, green, and blue bits for color buffer
				  8,                 // Number of bits for alpha buffer
				  24,                // Number of bits for depth buffer (Z-buffer)
				  0,                 // Number of bits for stencil buffer
				  GLFW_FULLSCREEN        // We want a desktop window (could be GLFW_FULLSCREEN)
			  );
		showCursor();
	}
	setVerticalSync(false);
	// Set window title
	glfwSetWindowTitle( " " );

	glfwEnable( GLFW_KEY_REPEAT );

	ofBackground(200,200,200);		// default bg color
	ofSetColor(0xFFFFFF); 			// default draw color
	// used to be black, but
	// black + texture = black
	// so maybe grey bg
	// and "white" fg color
	// as default works the best...

	requestedHeight = requestedHeight < 1 ? 1 : requestedHeight;
	glfwGetWindowSize( &requestedWidth, &requestedHeight );

	nonFullScreenW = ofGetWidth();
	nonFullScreenH = ofGetHeight();

	setWindowPosition(50, 50);

}

//--------------------------------------------
void ofAppGLFWWindow::initializeWindow(){
	 //----------------------
	 // setup the callbacks

	 glfwSetMouseButtonCallback(mouse_cb);
	 glfwSetMousePosCallback(motion_cb);
	 glfwSetKeyCallback(keyboard_cb);
	 glfwSetWindowSizeCallback(resize_cb);
	 glfwSetWindowCloseCallback(exit_cb);

}

//--------------------------------------------
int ofAppGLFWWindow::exit_cb(){
	return 0;
}

//--------------------------------------------
void ofAppGLFWWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	static ofEventArgs voidEventArgs;

	ofAppPtr = appPtr;

	if(ofAppPtr){
		ofAppPtr->setup();
		ofAppPtr->update();
	}

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.setup, voidEventArgs );
		ofNotifyEvent( ofEvents.update, voidEventArgs );
	#endif


	while(true){
		if (nFrameCount != 0 && bFrameRateSet == true){
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

		idle();

		if ( bNewScreenMode ){
			changeMode();
			bNewScreenMode = false;
		}

		display();

		//	thanks to jorge for the fix:
		//	http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate
		timeNow = ofGetElapsedTimef();
		if( (timeNow-timeThen) > 0.05f || nFramesForFPS == 0 ) {
			double fps = (double)nFramesForFPS / (timeNow-timeThen);
			timeThen = timeNow;
			nFramesForFPS = 0;
			frameRate = 0.9f * frameRate + 0.1f * fps;
		}
		nFramesForFPS++;

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
			SetSystemUIMode(kUIModeAllHidden,NULL);
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
			SetSystemUIMode(kUIModeNormal,NULL);
		#endif
	}
}

void ofAppGLFWWindow::setWindowTitle(string title){
	glfwSetWindowTitle(title.c_str());
}

ofPoint ofAppGLFWWindow::getWindowSize(){
	int width;
	int height;
	glfwGetWindowSize( &width, &height );
	return ofPoint(width, height,0);
}

ofPoint ofAppGLFWWindow::getWindowPosition(){
	/*int x = glutGet(GLUT_WINDOW_X);
	int y = glutGet(GLUT_WINDOW_Y);*/

	return ofPoint(nonFullScreenX,nonFullScreenY,0);
}

ofPoint ofAppGLFWWindow::getScreenSize(){
	GLFWvidmode desktopMode;
	glfwGetDesktopMode(&desktopMode);
	return ofPoint(desktopMode.Width, desktopMode.Height,0);
}

int	ofAppGLFWWindow::getWindowMode(){
	return windowMode;
}

void ofAppGLFWWindow::setWindowPosition(int x, int y){
	glfwSetWindowPos(x,y);
	nonFullScreenX=x;
	nonFullScreenY=y;
}

void ofAppGLFWWindow::setWindowShape(int w, int h){
	glfwSetWindowSize(w,h);
	// this is useful, esp if we are in the first frame (setup):
	requestedWidth  = w;
	requestedHeight = h;
}

void ofAppGLFWWindow::hideCursor(){
//	#ifdef TARGET_OSX
//		CGDisplayHideCursor(kCGDirectMainDisplay);
//	#else
		glfwDisable( GLFW_MOUSE_CURSOR );
//	#endif
};

void ofAppGLFWWindow::showCursor(){
//	#ifdef TARGET_OSX
//		//THIS HANGS ON OSX
//		//CGDisplayShowCursor(kCGDirectMainDisplay);
//	#else
		glfwEnable( GLFW_MOUSE_CURSOR );
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
	return frameRate;
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


//------------------------------------------------------------
void ofAppGLFWWindow::idle(void) {
	static ofEventArgs voidEventArgs;

	//	thanks to jorge for the fix:
	//	http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate

	if(ofAppPtr)
		ofAppPtr->update();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.update, voidEventArgs );
	#endif

}

//--------------------------------------------
// callbacks

void ofAppGLFWWindow::display(void){
	static ofEventArgs voidEventArgs;

	int width, height;

	width  = getWindowSize().x;
	height = getWindowSize().y;

	height = height > 0 ? height : 1;
	// set viewport, clear the screen
	glViewport( 0, 0, width, height );
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();

	// I don't know why, I need more than one frame at the start in fullscreen mode
	// also, in non-fullscreen mode, windows/intel graphics, this bClearAuto just fails.
	// I seem to have 2 buffers, alot of flickering
	// and don't accumulate the way I expect.
	// with this line:   if ((bClearAuto == true) || nFrameCount < 3){
	// we do nFrameCount < 3, so that the buffers are cleared at the start of the app
	// or else we have video memory garbage to draw on to...

	#ifdef TARGET_WIN32
		//windows doesn't get accumulation in window mode
		if ((bClearAuto == true || windowMode == OF_WINDOW) || nFrameCount < 3){
	#else
		//mac and linux does :)
		if ( bClearAuto == true || nFrameCount < 3){
	#endif
		glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if( bEnableSetupScreen )ofSetupScreen();

	if(ofAppPtr)
		ofAppPtr->draw();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.draw, voidEventArgs );
	#endif

	// Swap front and back buffers (we use a double buffered display)
	glfwSwapBuffers();

}


//------------------------------------------------------------
void ofAppGLFWWindow::exitApp(){
//	--  This is already happening in ofAppRunner

//	static ofEventArgs voidEventArgs;
//	if(ofAppPtr)ofAppPtr->exit();
//
//	#ifdef OF_USING_POCO
//		ofNotifyEvent( ofEvents.exit, voidEventArgs );
//	#endif

	ofLog(OF_VERBOSE,"GLFW app is being terminated!");

	// Terminate GLFW
	glfwTerminate();

	OF_EXIT_APP(0);
}


//------------------------------------------------------------
void ofAppGLFWWindow::mouse_cb(int button, int state) {
	ofLog(OF_VERBOSE,"button: %i",button);
	static ofMouseEventArgs	mouseEventArgs;

	if (state == GLFW_PRESS) {
		if(ofAppPtr)
			ofAppPtr->mousePressed(ofAppPtr->mouseX,ofAppPtr->mouseY,button);

		#ifdef OF_USING_POCO
			mouseEventArgs.x = ofAppPtr->mouseX;
			mouseEventArgs.y = ofAppPtr->mouseY;
			mouseEventArgs.button = button;
			ofNotifyEvent( ofEvents.mousePressed, mouseEventArgs );
		#endif

		buttonPressed=true;
	} else if (state == GLFW_RELEASE) {
		if(ofAppPtr){
			ofAppPtr->mouseReleased(ofAppPtr->mouseX,ofAppPtr->mouseY,button);
			ofAppPtr->mouseReleased();
		}

		#ifdef OF_USING_POCO
			mouseEventArgs.x = ofAppPtr->mouseX;
			mouseEventArgs.y = ofAppPtr->mouseY;
			mouseEventArgs.button = button;
			ofNotifyEvent( ofEvents.mouseReleased, mouseEventArgs );
		#endif

		buttonPressed=false;
	}
	buttonInUse = button;


}

//------------------------------------------------------------
void ofAppGLFWWindow::motion_cb(int x, int y) {
	static ofMouseEventArgs	mouseEventArgs;

	if(!buttonPressed){
		if(ofAppPtr){
			ofAppPtr->mouseX = x;
			ofAppPtr->mouseY = y;
			ofAppPtr->mouseMoved(x,y);
		}

		#ifdef OF_USING_POCO
			mouseEventArgs.x = x;
			mouseEventArgs.y = y;
			mouseEventArgs.button = -1;
			ofNotifyEvent( ofEvents.mouseMoved, mouseEventArgs );
		#endif

	}else{
		if(ofAppPtr){
			ofAppPtr->mouseX = x;
			ofAppPtr->mouseY = y;
			ofAppPtr->mouseDragged(x,y,buttonInUse);
		}

		#ifdef OF_USING_POCO
			mouseEventArgs.x = x;
			mouseEventArgs.y = y;
			mouseEventArgs.button = buttonInUse;
			ofNotifyEvent( ofEvents.mouseDragged, mouseEventArgs );
		#endif

	}
}


//------------------------------------------------------------
void ofAppGLFWWindow::keyboard_cb(int key, int state) {
	static ofKeyEventArgs keyEventArgs;

	ofLog(OF_VERBOSE,"key: %i, state: %i",key,state);

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
		if(ofAppPtr)ofAppPtr->keyPressed(key);

		#ifdef OF_USING_POCO
			keyEventArgs.key = key;
			ofNotifyEvent( ofEvents.keyPressed, keyEventArgs );
		#endif

		if (key == OF_KEY_ESC){				// "escape"
			exitApp();
		}
	}else{
		if(ofAppPtr)ofAppPtr->keyReleased(key);

		#ifdef OF_USING_POCO
			keyEventArgs.key = key;
			ofNotifyEvent( ofEvents.keyReleased, keyEventArgs );
		#endif
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::resize_cb(int w, int h) {
	if(ofAppPtr)ofAppPtr->resized(w,h);

	#ifdef OF_USING_POCO
		static ofResizeEventArgs resizeEventArgs;

		resizeEventArgs.width = w;
		resizeEventArgs.height = h;
		ofNotifyEvent( ofEvents.resize, resizeEventArgs );
	#endif
}

//------------------------------------------------------------
void ofAppGLFWWindow::setVerticalSync(bool bVerticalSync){
	if(bVerticalSync){
		//glfwSetWindowRefreshCallback(idle_cb);
		glfwSwapInterval(1);
	}else{
		glfwSwapInterval(0);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::listVideoModes(){
	GLFWvidmode vidModes[100];
	int numModes = glfwGetVideoModes( vidModes, 100 );
	for(int i=0; i<numModes; i++){
		printf("%i x %i %ibits",vidModes[i].Width,vidModes[i].Height,vidModes[i].RedBits+vidModes[i].GreenBits+vidModes[i].BlueBits);
	}
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowIconified(){
	return glfwGetWindowParam(GLFW_ICONIFIED);
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowActive(){
	return glfwGetWindowParam(GLFW_ACTIVE);

}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowResizeable(){
	return !glfwGetWindowParam(GLFW_WINDOW_NO_RESIZE);

}
void ofAppGLFWWindow::iconify(bool bIconify){
	if(bIconify)
		glfwIconifyWindow();
	else
		glfwRestoreWindow();
}
//#endif
