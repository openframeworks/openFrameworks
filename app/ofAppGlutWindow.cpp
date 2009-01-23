#include "ofAppGlutWindow.h"

#ifdef OF_WINDOWING_GLUT

#include "ofBaseApp.h"
#include "ofMain.h"
#include "ofEvents.h"

ofAppGlutWindow * ofGlutWindow = new ofAppGlutWindow;
ofAppBaseWindow * ofAppWindow  = ofGlutWindow;


//========================================================================
// core events instance & arguments
ofCoreEvents & ofEvents = ofGlutWindow->appEvents;

//----------------------------------------------------------
ofAppGlutWindow::ofAppGlutWindow(){
	bEnableSetupScreen	= true;
	nFrameCount			= 0;
	buttonInUse			= 0;
	nonFullScreenX		= -1;
	nonFullScreenY		= -1;
}


void ofAppGlutWindow::setupOpenGL(int w, int h, int screenMode){

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
    		ofLog(OF_ERROR,"game mode error: selected format (%s) not available \n", gameStr);
    	}
    	// start fullscreen game mode
    	glutEnterGameMode();
	}
}


void ofAppGlutWindow::initializeWindow(){


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


}

void ofAppGlutWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){

	ofAppPtr = appPtr;

	if(ofAppPtr){
		ofAppPtr->setup();
		ofAppPtr->update();
	}

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.setup, voidEventArgs );
		ofNotifyEvent( ofEvents.update, voidEventArgs );
	#endif


	glutMainLoop();
}


void ofAppGlutWindow::setWindowTitle(string title){
	glutSetWindowTitle(title.c_str());
}

ofPoint ofAppGlutWindow::getWindowSize(){
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	return ofPoint(width, height,0);
}

ofPoint ofAppGlutWindow::getWindowPosition(){
	int x = glutGet(GLUT_WINDOW_X);
	int y = glutGet(GLUT_WINDOW_Y);
	return ofPoint(x,y,0);
}

ofPoint ofAppGlutWindow::getScreenSize(){
	int width = glutGet(GLUT_SCREEN_WIDTH);
	int height = glutGet(GLUT_SCREEN_HEIGHT);
	return ofPoint(width, height,0);
}

int	ofAppGlutWindow::getWindowMode(){
	return windowMode;
}

float ofAppGlutWindow::getFrameRate(){
	return frameRate;
}

void ofAppGlutWindow::setWindowPosition(int x, int y){
	glutPositionWindow(x,y);
}

void ofAppGlutWindow::setWindowShape(int w, int h){
	glutReshapeWindow(w, h);
	// this is useful, esp if we are in the first frame (setup):
	requestedWidth  = w;
	requestedHeight = h;
}

void ofAppGlutWindow::hideCursor(){
	#ifdef TARGET_OSX
		CGDisplayHideCursor(kCGDirectMainDisplay);
	#else
		glutSetCursor(GLUT_CURSOR_NONE);
	#endif
};

void ofAppGlutWindow::showCursor(){
	#ifdef TARGET_OSX
		CGDisplayShowCursor(kCGDirectMainDisplay);
	#else
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	#endif
};

void ofAppGlutWindow::setFrameRate(float targetRate){
	// given this FPS, what is the amount of millis per frame
	// that should elapse?

	// --- > f / s

	float durationOfFrame 	= 1.0f / (float)targetRate;
	int millisForFrame 			= (int)(1000.0f * durationOfFrame);

	frameRate = targetRate;

	glutIdleFunc(NULL);
	glutTimerFunc(millisForFrame, timer_cb, millisForFrame);
};

void ofAppGlutWindow::enableSetupScreen(){
	bEnableSetupScreen = true;
};

void ofAppGlutWindow::disableSetupScreen(){
	bEnableSetupScreen = false;
};

//------------------------------------------------------------
int ofAppGlutWindow::getFrameNum(){
	return nFrameCount;
}

//------------------------------------------------------------
void ofAppGlutWindow::setFullscreen(bool fullscreen){
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
void ofAppGlutWindow::toggleFullscreen(){
	if (windowMode == OF_GAME_MODE) return;

	bNewScreenMode = true;
	if (windowMode == OF_WINDOW){
		windowMode = OF_FULLSCREEN;
	} else {
		windowMode = OF_WINDOW;
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::timer_cb(int value) {
	glutTimerFunc(value, timer_cb, value);
	idle_cb();
}

//------------------------------------------------------------
void ofAppGlutWindow::idle_cb(void) {

	//	thanks to jorge for the fix:
	//	http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate

	if(ofGlutWindow->ofAppPtr)
		ofGlutWindow->ofAppPtr->update();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.update, ofGlutWindow->voidEventArgs );
	#endif

	glutPostRedisplay();

}

//--------------------------------------------
// callbacks

void ofAppGlutWindow::display(void){

	//--------------------------------
	// when I had "glutFullScreen()"
	// in the initOpenGl, I was gettings a "heap" allocation error
	// when debugging via visual studio.  putting it here, changes that.
	// maybe it's voodoo, or I am getting rid of the problem
	// by removing something unrelated, but everything seems
	// to work if I put fullscreen on the first frame of display.

	if (ofGlutWindow->windowMode != OF_GAME_MODE){
		if ( ofGlutWindow->bNewScreenMode ){
			if( ofGlutWindow->windowMode == OF_FULLSCREEN){

				//----------------------------------------------------
				// before we go fullscreen, take a snapshot of where we are:
				ofGlutWindow->nonFullScreenX = glutGet(GLUT_WINDOW_X);
				ofGlutWindow->nonFullScreenY = glutGet(GLUT_WINDOW_Y);
				//----------------------------------------------------

				glutFullScreen();

				#ifdef TARGET_OSX
					SetSystemUIMode(kUIModeAllHidden,NULL);
				#endif

			}else if( ofGlutWindow->windowMode == OF_WINDOW ){

				ofGlutWindow->setWindowPosition(ofGlutWindow->nonFullScreenX,ofGlutWindow->nonFullScreenY);

				//----------------------------------------------------
				// if we have recorded the screen posion, put it there
				// if not, better to let the system do it (and put it where it wants)
				if (ofGlutWindow->nFrameCount > 0){
					ofGlutWindow->setWindowShape(ofGlutWindow->requestedWidth, ofGlutWindow->requestedHeight);
				}
				//----------------------------------------------------

				#ifdef TARGET_OSX
					SetSystemUIMode(kUIModeNormal,NULL);
				#endif
			}
			ofGlutWindow->bNewScreenMode = false;
		}
	}

	int width, height;

	width  = ofGetWidth();
	height = ofGetHeight();

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
		if ( bClearAuto == true || ofGlutWindow->nFrameCount < 3){
	#endif
		glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if( ofGlutWindow->bEnableSetupScreen )ofSetupScreen();

	if(ofGlutWindow->ofAppPtr)
		ofGlutWindow->ofAppPtr->draw();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.draw, ofGlutWindow->voidEventArgs );
	#endif

  	glutSwapBuffers();

  	// -------------- fps calculation:
  	ofGlutWindow->timeNow = ofGetElapsedTimef();
	if( (ofGlutWindow->timeNow-ofGlutWindow->timeThen) > 0.05f || ofGlutWindow->nFramesForFPS == 0 ) {
		ofGlutWindow->fps = (double)ofGlutWindow->nFramesForFPS / (ofGlutWindow->timeNow-ofGlutWindow->timeThen);
		ofGlutWindow->timeThen = ofGlutWindow->timeNow;
		ofGlutWindow->nFramesForFPS = 0;
		ofGlutWindow->frameRate = 0.9f * ofGlutWindow->frameRate + 0.1f * ofGlutWindow->fps;
  	}
	ofGlutWindow->nFramesForFPS++;
  	// --------------

	ofGlutWindow->nFrameCount++;		// increase the overall frame count

}


//------------------------------------------------------------
void ofAppGlutWindow::exit(){
	if(ofGlutWindow->ofAppPtr)
		ofGlutWindow->ofAppPtr->exit();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.exit, voidEventArgs );
	#endif
}


//------------------------------------------------------------
void ofAppGlutWindow::mouse_cb(int button, int state, int x, int y) {
	if (ofGlutWindow->nFrameCount > 0){
		if(ofGlutWindow->ofAppPtr){
			ofGlutWindow->ofAppPtr->mouseX = x;
			ofGlutWindow->ofAppPtr->mouseY = y;
		}

		if (state == GLUT_DOWN) {
			if(ofGlutWindow->ofAppPtr)
				ofGlutWindow->ofAppPtr->mousePressed(x,y,button);

			#ifdef OF_USING_POCO
				ofGlutWindow->mouseEventArgs.x = x;
				ofGlutWindow->mouseEventArgs.y = y;
				ofGlutWindow->mouseEventArgs.button = button;
				ofNotifyEvent( ofEvents.mousePressed, ofGlutWindow->mouseEventArgs );
			#endif
		} else if (state == GLUT_UP) {
			if(ofGlutWindow->ofAppPtr){
				ofGlutWindow->ofAppPtr->mouseReleased(x,y,button);
				ofGlutWindow->ofAppPtr->mouseReleased();
			}

			#ifdef OF_USING_POCO
				ofGlutWindow->mouseEventArgs.x = x;
				ofGlutWindow->mouseEventArgs.y = y;
				ofGlutWindow->mouseEventArgs.button = button;
				ofNotifyEvent( ofEvents.mouseReleased, ofGlutWindow->mouseEventArgs );
			#endif
		}
		ofGlutWindow->buttonInUse = button;
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::motion_cb(int x, int y) {

	if (ofGlutWindow->nFrameCount > 0){
		if(ofGlutWindow->ofAppPtr){
			ofGlutWindow->ofAppPtr->mouseX = x;
			ofGlutWindow->ofAppPtr->mouseY = y;
			ofGlutWindow->ofAppPtr->mouseDragged(x,y,ofGlutWindow->buttonInUse);
		}

		#ifdef OF_USING_POCO
			ofGlutWindow->mouseEventArgs.x = x;
			ofGlutWindow->mouseEventArgs.y = y;
			ofGlutWindow->mouseEventArgs.button = ofGlutWindow->buttonInUse;
			ofNotifyEvent( ofEvents.mouseDragged, ofGlutWindow->mouseEventArgs );
		#endif
	}

}

//------------------------------------------------------------
void ofAppGlutWindow::passive_motion_cb(int x, int y) {
	if (ofGlutWindow->nFrameCount > 0){
		if(ofGlutWindow->ofAppPtr){
			ofGlutWindow->ofAppPtr->mouseX = x;
			ofGlutWindow->ofAppPtr->mouseY = y;
			ofGlutWindow->ofAppPtr->mouseMoved(x,y);
		}

		#ifdef OF_USING_POCO
			ofGlutWindow->mouseEventArgs.x = x;
			ofGlutWindow->mouseEventArgs.y = y;
			ofNotifyEvent( ofEvents.mouseMoved, ofGlutWindow->mouseEventArgs );
		#endif
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::keyboard_cb(unsigned char key, int x, int y) {
	if(ofGlutWindow->ofAppPtr)
		ofGlutWindow->ofAppPtr->keyPressed(key);

	#ifdef OF_USING_POCO
		ofGlutWindow->keyEventArgs.key = key;
		ofNotifyEvent( ofEvents.keyPressed, ofGlutWindow->keyEventArgs );
	#endif

	if (key == 27){				// "escape"
		OF_EXIT_APP(0);
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::keyboard_up_cb(unsigned char key, int x, int y) {
	if(ofGlutWindow->ofAppPtr)
		ofGlutWindow->ofAppPtr->keyReleased(key);

	#ifdef OF_USING_POCO
		ofGlutWindow->keyEventArgs.key = key;
		ofNotifyEvent( ofEvents.keyReleased, ofGlutWindow->keyEventArgs );
	#endif
}

//------------------------------------------------------
void ofAppGlutWindow::special_key_cb(int key, int x, int y) {
	if(ofGlutWindow->ofAppPtr)
		ofGlutWindow->ofAppPtr->keyPressed(key | OF_KEY_MODIFIER);

	#ifdef OF_USING_POCO
		ofGlutWindow->keyEventArgs.key = (key | OF_KEY_MODIFIER);
		ofNotifyEvent( ofEvents.keyPressed, ofGlutWindow->keyEventArgs );
	#endif
}

//------------------------------------------------------------
void ofAppGlutWindow::special_key_up_cb(int key, int x, int y) {
	if(ofGlutWindow->ofAppPtr)
		ofGlutWindow->ofAppPtr->keyReleased(key | OF_KEY_MODIFIER);

	#ifdef OF_USING_POCO
		ofGlutWindow->keyEventArgs.key = (key | OF_KEY_MODIFIER);
		ofNotifyEvent( ofEvents.keyReleased, ofGlutWindow->keyEventArgs );
	#endif
}

//------------------------------------------------------------
void ofAppGlutWindow::resize_cb(int w, int h) {
	if(ofGlutWindow->ofAppPtr)
		ofGlutWindow->ofAppPtr->resized(w,h);

	#ifdef OF_USING_POCO
		ofGlutWindow->resizeEventArgs.width = w;
		ofGlutWindow->resizeEventArgs.height = h;
		ofNotifyEvent( ofEvents.resize, ofGlutWindow->resizeEventArgs );
	#endif
}

#endif
