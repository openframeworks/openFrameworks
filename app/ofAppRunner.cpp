#include "ofAppRunner.h"

//========================================================================
// static variables:
ofBaseApp	*				OFSAptr;
int							windowMode;
bool 						bMousePressed;
bool						bRightButton;
int							width, height;
static float 				ofFrameRate;


//========================================================================
// core events instance:
ofCoreEvents 				ofEvents;

//========================================================================
// core events arguments:
ofEventArgs					voidEventArgs;
ofMouseEventArgs 			mouseEventArgs;
ofKeyEventArgs 				keyEventArgs;
ofResizeEventArgs 			resizeEventArgs;


//========================================================================
// callbacks:
#include "ofAppGlutGlue.h"


//--------------------------------------
void ofSetupOpenGL(int w, int h, int screenMode){
	int argc = 1;
	char *argv = "openframeworks";
	char **vptr = &argv;
	glutInit(&argc, vptr);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
	windowMode = screenMode;


	if (windowMode != OF_GAME_MODE){
		glutInitWindowSize(w, h);
		glutCreateWindow("");


		ofBackground(200,200,200);		// default bg color
		ofSetColor(0xFFFFFF); 			// default draw color
										// used to be black, but
										// black + texture = black
										// so maybe grey bg
										// and "white" fg color
										// as default works the best...

		requestedWidth  = glutGet(GLUT_WINDOW_WIDTH);
		requestedHeight = glutGet(GLUT_WINDOW_HEIGHT);
	} else {
		glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
    	// w x h, 32bit pixel depth, 60Hz refresh rate
		char gameStr[64];
		sprintf( gameStr, "%dx%d:%d@%d", w, h, 32, 60 );

    	glutGameModeString(gameStr);

    	if (!glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)){
    		printf("game mode error: selected format (%s) not available \n", gameStr);
    	}
    	// start fullscreen game mode
    	glutEnterGameMode();
	}
}

//----------------------- 	gets called when the app exits
// 							currently looking at who to turn off
//							at the end of the application

void ofExitCallback();
void ofExitCallback(){


	//------------------------
	// try to close FMOD:
	ofSoundPlayer::closeFmod();
	//------------------------
	// try to close quicktime, for non-linux systems:

	#if defined( TARGET_OSX ) || defined( TARGET_WIN32 )
	closeQuicktime();
	#endif


	//------------------------
	// try to close freeImage:
	ofCloseFreeImage();
	//------------------------
	// try to close free type:
	// ....

	#ifdef WIN32_HIGH_RES_TIMING
		timeEndPeriod(1);
	#endif

	if(OFSAptr)
		OFSAptr->exit();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.exit, voidEventArgs );
	#endif
}

//--------------------------------------
void ofRunApp(ofBaseApp * OFSA){

	OFSAptr = OFSA;
	if(OFSAptr){
	OFSAptr->mouseX = 0;
	OFSAptr->mouseY = 0;
	}

	atexit(ofExitCallback);

	#ifdef WIN32_HIGH_RES_TIMING
		timeBeginPeriod(1);		// ! experimental, sets high res time
								// you need to call timeEndPeriod.
								// if you quit the app other than "esc"
								// (ie, close the console, kill the process, etc)
								// at exit wont get called, and the time will
								// remain high res, that could mess things
								// up on your system.
								// info here:http://www.geisswerks.com/ryan/FAQS/timing.html

	#endif


	bFrameRateSet = false;

	//----------------------
	// setup the callbacks
	glutMouseFunc(mouse_cb);
	glutMotionFunc(motion_cb);
	glutPassiveMotionFunc(passive_motion_cb);
	glutIdleFunc(idle_cb);
	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard_cb);
	glutKeyboardUpFunc(keyboard_up_cb);
	glutSpecialFunc(special_key_cb);
	glutSpecialUpFunc(special_key_up_cb);

	glutReshapeFunc(resize_cb);


	//----------------------

	ofSeedRandom();

	// 		seed the random generator
	// 		(users can seed it to a value in setup
	// 		if they want it fixed);

	if(OFSAptr)
		OFSAptr->setup();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.setup, voidEventArgs );
	#endif
    
	glutMainLoop();
}

//--------------------------------------
float ofGetFrameRate(){
	return ofFrameRate;
}

//--------------------------------------
void ofSetFrameRate(int targetRate){
	// given this FPS, what is the amount of millis per frame
	// that should elapse?

	// --- > f / s

	if (targetRate == 0){
		bFrameRateSet = false;
		return;
	}

	bFrameRateSet 			= true;
	float durationOfFrame 	= 1.0f / (float)targetRate;
	millisForFrame 			= (int)(1000.0f * durationOfFrame);
}

//--------------------------------------
void ofSleepMillis(int millis){
	#ifdef TARGET_WIN32
		Sleep(millis);			//windows sleep in milliseconds
	#else
		usleep(millis * 1000);	//mac sleep in microseconds - cooler :)
	#endif
}

//--------------------------------------
void ofHideCursor(){
	#ifdef TARGET_OSX
		CGDisplayHideCursor(kCGDirectMainDisplay);
	#else 
		glutSetCursor(GLUT_CURSOR_NONE);
	#endif
}

//--------------------------------------
void ofShowCursor(){
	#ifdef TARGET_OSX
		CGDisplayShowCursor(kCGDirectMainDisplay);
	#else 
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	#endif
}


//--------------------------------------
void ofSetWindowPosition(int x, int y){
	glutPositionWindow(x,y);
}

//--------------------------------------
void ofSetWindowShape(int width, int height){
	glutReshapeWindow(width, height);
	// this is useful, esp if we are in the first frame (setup):
	requestedWidth  = width;
	requestedHeight = height;
}

//--------------------------------------
int ofGetWindowPositionX(){
	return glutGet(GLUT_WINDOW_X);
}

//--------------------------------------
int ofGetWindowPositionY(){
	return glutGet(GLUT_WINDOW_Y);
}

//--------------------------------------
int ofGetScreenWidth(){
	return glutGet(GLUT_SCREEN_WIDTH);
}

//--------------------------------------
int ofGetScreenHeight(){
	return glutGet(GLUT_SCREEN_HEIGHT);
}

//--------------------------------------------------
int ofGetWidth(){
	return glutGet(GLUT_WINDOW_WIDTH);
}
//--------------------------------------------------
int ofGetHeight(){
	return glutGet(GLUT_WINDOW_HEIGHT);
}

//--------------------------------------
void ofSetWindowTitle(string title){
	glutSetWindowTitle(title.c_str());
}

//----------------------------------------------------------
void ofEnableSetupScreen(){
	enableSetupScreen = true;
}

//----------------------------------------------------------
void ofDisableSetupScreen(){
	enableSetupScreen = false;
}

//--------------------------------------
void ofToggleFullscreen(){

	if (windowMode == OF_GAME_MODE) return;

	newScreenMode = true;
	if (windowMode == OF_WINDOW){
		windowMode = OF_FULLSCREEN;
	} else {
		windowMode = OF_WINDOW;
	}
}

//--------------------------------------
void ofSetFullscreen(bool fullscreen){

	if (windowMode == OF_GAME_MODE) return;

	if(fullscreen && windowMode==OF_WINDOW){
		windowMode    = OF_FULLSCREEN;
		newScreenMode = true;
	}
	else if(!fullscreen && windowMode==OF_FULLSCREEN){
		windowMode    = OF_WINDOW;
		newScreenMode = true;
	}
}

//--------------------------------------
int ofGetWindowMode(){
	return windowMode;
}

//--------------------------------------
void ofSetVerticalSync(bool bSync){
	//----------------------------
	#ifdef TARGET_WIN32
	//----------------------------
		if (bSync) {
			if (GLEE_WGL_EXT_swap_control) wglSwapIntervalEXT (1);
		} else {
			if (GLEE_WGL_EXT_swap_control) wglSwapIntervalEXT (0);
		}
	//----------------------------
	#endif
	//----------------------------

	//--------------------------------------
	#ifdef TARGET_OSX
	//--------------------------------------
		long sync = bSync == true ? 1 : 0;
		CGLSetParameter (CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
	//--------------------------------------
	#endif
	//--------------------------------------


	// linux ofSetVerticalSync needed -- anyone want to help w/ this?
	// http://www.inb.uni-luebeck.de/~boehme/xvideo_sync.html
	// glXGetVideoSyncSGI(&count); // but needs to be at the end of every "draw?
	// also, see this:
   	// glXWaitVideoSyncSGI(2,0,&count);
}


