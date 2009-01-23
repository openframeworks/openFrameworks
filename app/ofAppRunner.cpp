#include "ofAppRunner.h"

//--------------------------------------
void ofSetupOpenGL(int w, int h, int screenMode){

	ofAppWindow->setupOpenGL(w, h, screenMode);

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

	//---------------------
	// close the app window
	ofAppWindow->exit();

}

//--------------------------------------
void ofRunApp(ofBaseApp * OFSA){

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

	ofAppWindow->initializeWindow();

	ofSeedRandom();

	ofAppWindow->runAppViaInfiniteLoop(OFSA);

}

//--------------------------------------
float ofGetFrameRate(){
	return ofAppWindow->getFrameRate();
}

//--------------------------------------
void ofSetFrameRate(int targetRate){

	ofAppWindow->setFrameRate(targetRate);
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
	ofAppWindow->hideCursor();
}

//--------------------------------------
void ofShowCursor(){
	ofAppWindow->showCursor();
}


//--------------------------------------
void ofSetWindowPosition(int x, int y){
	ofAppWindow->setWindowPosition(x,y);
}

//--------------------------------------
void ofSetWindowShape(int width, int height){
	ofAppWindow->setWindowShape(width, height);
}

//--------------------------------------
int ofGetWindowPositionX(){
	return (int)ofAppWindow->getWindowPosition().x;
}

//--------------------------------------
int ofGetWindowPositionY(){
	return (int)ofAppWindow->getWindowPosition().y;
}

//--------------------------------------
int ofGetScreenWidth(){
	return (int)ofAppWindow->getScreenSize().x;
}

//--------------------------------------
int ofGetScreenHeight(){
	return (int)ofAppWindow->getScreenSize().y;
}

//--------------------------------------------------
int ofGetWidth(){
	return (int)ofAppWindow->getWindowSize().x;
}
//--------------------------------------------------
int ofGetHeight(){
	return (int)ofAppWindow->getWindowSize().y;
}

//--------------------------------------
void ofSetWindowTitle(string title){
	ofAppWindow->setWindowTitle(title);
}

//----------------------------------------------------------
void ofEnableSetupScreen(){
	ofAppWindow->enableSetupScreen();
}

//----------------------------------------------------------
void ofDisableSetupScreen(){
	ofAppWindow->disableSetupScreen();
}

//--------------------------------------
void ofToggleFullscreen(){
	ofAppWindow->toggleFullscreen();
}

//--------------------------------------
void ofSetFullscreen(bool fullscreen){

	ofAppWindow->setFullscreen(fullscreen);
}

//--------------------------------------
int ofGetWindowMode(){
	return ofAppWindow->getWindowMode();
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
	//--------------------------------------
	#ifdef TARGET_LINUX
	//--------------------------------------
		ofAppWindow->setVerticalSync(bSync);
	//--------------------------------------
	#endif
	//--------------------------------------
}


