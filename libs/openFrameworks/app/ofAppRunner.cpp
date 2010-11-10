#include "ofAppRunner.h"

//========================================================================
// static variables:

ofBaseApp	*				OFSAptr = NULL;
bool 						bMousePressed;
bool						bRightButton;
int							width, height;

ofAppBaseWindow *			window = NULL;

//========================================================================
// core events instance & arguments
#ifdef OF_USING_POCO
ofCoreEvents 				ofEvents;
ofEventArgs					voidEventArgs;
#endif

//========================================================================
// default windowing
#ifdef TARGET_OF_IPHONE
	#include "ofAppiPhoneWindow.h"
#else
	#include "ofAppGlutWindow.h"
#endif


//--------------------------------------
void ofSetupOpenGL(ofAppBaseWindow * windowPtr, int w, int h, int screenMode){
	window = windowPtr;
	window->setupOpenGL(w, h, screenMode);
}


//--------------------------------------
void ofSetupOpenGL(int w, int h, int screenMode){
	#ifdef TARGET_OF_IPHONE
		window = new ofAppiPhoneWindow();
	#else
		window = new ofAppGlutWindow();
	#endif

	window->setupOpenGL(w, h, screenMode);
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

	//------------------------
	// try to close rtAudio:
	ofSoundStreamClose();

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

	ofNotifyExit();

	if(OFSAptr)delete OFSAptr;
}

//--------------------------------------
void ofRunApp(ofBaseApp * OFSA){

	OFSAptr = OFSA;
	if(OFSAptr){
		OFSAptr->mouseX = 0;
		OFSAptr->mouseY = 0;
	}
	
	#ifdef TARGET_OSX 
		//this internally checks the executable path for osx
		ofSetDataPathRoot("../../../data/");
	#endif

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

	window->initializeWindow();

	ofSeedRandom();
	ofResetElapsedTimeCounter();

	window->runAppViaInfiniteLoop(OFSAptr);

}

//--------------------------------------
ofBaseApp * ofGetAppPtr(){
	return OFSAptr;
}

//--------------------------------------
int ofGetFrameNum(){
	return window->getFrameNum();
}

//--------------------------------------
float ofGetFrameRate(){
	return window->getFrameRate();
}

double ofGetLastFrameTime(){
	return window->getLastFrameTime();
}

//--------------------------------------
void ofSetFrameRate(int targetRate){

	window->setFrameRate(targetRate);
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
	window->hideCursor();
}

//--------------------------------------
void ofShowCursor(){
	window->showCursor();
}


//--------------------------------------
void ofSetWindowPosition(int x, int y){
	window->setWindowPosition(x,y);
}

//--------------------------------------
void ofSetWindowShape(int width, int height){
	window->setWindowShape(width, height);
}

//--------------------------------------
int ofGetWindowPositionX(){
	return (int)window->getWindowPosition().x;
}

//--------------------------------------
int ofGetWindowPositionY(){
	return (int)window->getWindowPosition().y;
}

//--------------------------------------
int ofGetScreenWidth(){
	return (int)window->getScreenSize().x;
}

//--------------------------------------
int ofGetScreenHeight(){
	return (int)window->getScreenSize().y;
}

//--------------------------------------------------
int ofGetWidth(){
	return (int)window->getWindowSize().x;
}
//--------------------------------------------------
int ofGetHeight(){
	return (int)window->getWindowSize().y;
}

//--------------------------------------
void ofSetWindowTitle(string title){
	window->setWindowTitle(title);
}

//----------------------------------------------------------
void ofEnableSetupScreen(){
	window->enableSetupScreen();
}

//----------------------------------------------------------
void ofDisableSetupScreen(){
	window->disableSetupScreen();
}

//--------------------------------------
void ofToggleFullscreen(){
	window->toggleFullscreen();
}

//--------------------------------------
void ofSetFullscreen(bool fullscreen){
	window->setFullscreen(fullscreen);
}

//--------------------------------------
int ofGetWindowMode(){
	return window->getWindowMode();
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
		GLint sync = bSync == true ? 1 : 0;
		CGLSetParameter (CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
	//--------------------------------------
	#endif
	//--------------------------------------

	//--------------------------------------
	#ifdef TARGET_LINUX
	//--------------------------------------
		//if (GLEW_GLX_SGI_swap_control)
		void (*swapInterval)(int)  = (void (*)(int)) glXGetProcAddress((const GLubyte*) "glXSwapIntervalSGI");
		if(!swapInterval)
			swapInterval = (void (*)(int)) glXGetProcAddress((const GLubyte*) "glXSwapIntervalMESA");

		if(swapInterval)
			swapInterval(bSync ? 1 : 0);
		//glXSwapIntervalSGI(bSync ? 1 : 0);
	//--------------------------------------
	#endif
	//--------------------------------------

}


