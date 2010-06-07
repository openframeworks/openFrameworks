#include "ofAppGlutWindow.h"
#include "ofBaseApp.h"
#include "ofMain.h"

// glut works with static callbacks UGH, so we need static variables here:

int				windowMode;
bool			bNewScreenMode;
float			timeNow, timeThen, fps;
int				nFramesForFPS;
int				nFrameCount;
int				buttonInUse;
bool			bEnableSetupScreen;

bool			bFrameRateSet;
int 			millisForFrame;
int 			prevMillis;
int 			diffMillis;

float 			frameRate;

double			lastFrameTime;

int				requestedWidth;
int				requestedHeight;
int 			nonFullScreenX;
int 			nonFullScreenY;
int				windowW;
int				windowH;
int				mouseX, mouseY;
ofBaseApp *		ofAppPtr;

int             nFramesSinceWindowResized;

#ifdef TARGET_WIN32

//------------------------------------------------

// this is to fix a bug with glut that doesn't properly close the app
// with window closing.  we grab the window procedure, store it, and parse windows messages,
// using the close and destroy messages and passing on the others...

//------------------------------------------------

static WNDPROC currentWndProc;
static HWND handle  = NULL;

static LRESULT CALLBACK winProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam){

   //we catch close and destroy messages
   //and send them to OF

   switch(Msg){

      case WM_CLOSE:
         OF_EXIT_APP(0);
      break;
      case WM_DESTROY:
         OF_EXIT_APP(0);
         break;
      default:
         return CallWindowProc(currentWndProc, handle, Msg, wParam, lParam);
      break;
    }

    return 0;
}

//--------------------------------------
static void fixCloseWindowOnWin32(){

	//get the HWND
	handle = WindowFromDC(wglGetCurrentDC());

	//store the current message event handler for the window
	currentWndProc = (WNDPROC)GetWindowLongPtr(handle, GWL_WNDPROC);

	//tell the window to now use our event handler!
	SetWindowLongPtr(handle, GWL_WNDPROC, (long)winProc);
}

#endif




//----------------------------------------------------------
ofAppGlutWindow::ofAppGlutWindow(){
	timeNow				= 0;
	timeThen			= 0;
	fps					= 60.0; //give a realistic starting value - win32 issues
	frameRate			= 60.0;
	windowMode			= OF_WINDOW;
	bNewScreenMode		= true;
	nFramesForFPS		= 0;
	nFramesSinceWindowResized = 0;
	nFrameCount			= 0;
	buttonInUse			= 0;
	bEnableSetupScreen	= true;
	bFrameRateSet		= false;
	millisForFrame		= 0;
	prevMillis			= 0;
	diffMillis			= 0;
	requestedWidth		= 0;
	requestedHeight		= 0;
	nonFullScreenX		= -1;
	nonFullScreenY		= -1;
	mouseX				= 0;
	mouseY				= 0;
	lastFrameTime		= 0.0;
	displayString		= "";

}

//lets you enable alpha blending using a display string like:
// "rgba double samples>=4 depth" ( mac )
// "rgb double depth alpha samples>=4" ( some pcs )
//------------------------------------------------------------
 void ofAppGlutWindow::setGlutDisplayString(string displayStr){
	displayString = displayStr;
 }

//------------------------------------------------------------
void ofAppGlutWindow::setupOpenGL(int w, int h, int screenMode){

	int argc = 1;
	char *argv = (char*)"openframeworks";
	char **vptr = &argv;
	glutInit(&argc, vptr);

	if( displayString != ""){
		glutInitDisplayString( displayString.c_str() );
	}else{
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
	}

	windowMode = screenMode;
	bNewScreenMode = true;

	if (windowMode != OF_GAME_MODE){
		glutInitWindowSize(w, h);
		glutCreateWindow("");

		//Default colors etc are now in ofGraphics - ofSetupGraphicDefaults
		ofSetupGraphicDefaults();

		/*
		ofBackground(200,200,200);		// default bg color
		ofSetColor(0xFFFFFF); 			// default draw color
		// used to be black, but
		// black + texture = black
		// so maybe grey bg
		// and "white" fg color
		// as default works the best...
		*/

		requestedWidth  = glutGet(GLUT_WINDOW_WIDTH);
		requestedHeight = glutGet(GLUT_WINDOW_HEIGHT);
	} else {
		if( displayString != ""){
			glutInitDisplayString( displayString.c_str() );
		}else{
			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
		}

    	// w x h, 32bit pixel depth, 60Hz refresh rate
		char gameStr[64];
		sprintf( gameStr, "%dx%d:%d@%d", w, h, 32, 60 );

    	glutGameModeString(gameStr);

    	if (!glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)){
    		ofLog(OF_LOG_ERROR,"game mode error: selected format (%s) not available \n", gameStr);
    	}
    	// start fullscreen game mode
    	glutEnterGameMode();
	}
	windowW = glutGet(GLUT_WINDOW_WIDTH);
	windowH = glutGet(GLUT_WINDOW_HEIGHT);
}

//------------------------------------------------------------
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

    nFramesSinceWindowResized = 0;

    #ifdef TARGET_WIN32
        //----------------------
        // this is specific to windows (respond properly to close / destroy)
        fixCloseWindowOnWin32();
    #endif

}

//------------------------------------------------------------
void ofAppGlutWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
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


	glutMainLoop();
}

//------------------------------------------------------------
void ofAppGlutWindow::exitApp(){

//  -- This already exists in ofExitCallback

//	static ofEventArgs voidEventArgs;
//
//	if(ofAppPtr)ofAppPtr->exit();
//
//	#ifdef OF_USING_POCO
//		ofNotifyEvent( ofEvents.exit, voidEventArgs );
//	#endif

	ofLog(OF_LOG_VERBOSE,"GLUT OF app is being terminated!");

	OF_EXIT_APP(0);
}

//------------------------------------------------------------
float ofAppGlutWindow::getFrameRate(){
	return frameRate;
}

//------------------------------------------------------------
double ofAppGlutWindow::getLastFrameTime(){
	return lastFrameTime;
}

//------------------------------------------------------------
int ofAppGlutWindow::getFrameNum(){
	return nFrameCount;
}

//------------------------------------------------------------
void ofAppGlutWindow::setWindowTitle(string title){
	glutSetWindowTitle(title.c_str());
}

//------------------------------------------------------------
ofPoint ofAppGlutWindow::getWindowSize(){
	return ofPoint(windowW, windowH,0);
}

//------------------------------------------------------------
ofPoint ofAppGlutWindow::getWindowPosition(){
	int x = glutGet(GLUT_WINDOW_X);
	int y = glutGet(GLUT_WINDOW_Y);
	return ofPoint(x,y,0);
}

//------------------------------------------------------------
ofPoint ofAppGlutWindow::getScreenSize(){
	int width = glutGet(GLUT_SCREEN_WIDTH);
	int height = glutGet(GLUT_SCREEN_HEIGHT);
	return ofPoint(width, height,0);
}

//------------------------------------------------------------
void ofAppGlutWindow::setWindowPosition(int x, int y){
	glutPositionWindow(x,y);
}

//------------------------------------------------------------
void ofAppGlutWindow::setWindowShape(int w, int h){
	glutReshapeWindow(w, h);
	// this is useful, esp if we are in the first frame (setup):
	requestedWidth  = w;
	requestedHeight = h;
}

//------------------------------------------------------------
void ofAppGlutWindow::hideCursor(){
	glutSetCursor(GLUT_CURSOR_NONE);
}

//------------------------------------------------------------
void ofAppGlutWindow::showCursor(){
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

//------------------------------------------------------------
void ofAppGlutWindow::setFrameRate(float targetRate){
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

	frameRate				= targetRate;

}

//------------------------------------------------------------
int ofAppGlutWindow::getWindowMode(){
	return windowMode;
}

//------------------------------------------------------------
void ofAppGlutWindow::toggleFullscreen(){
	if( windowMode == OF_GAME_MODE)return;

	if( windowMode == OF_WINDOW ){
		windowMode = OF_FULLSCREEN;
	}else{
		windowMode = OF_WINDOW;
	}

	bNewScreenMode = true;
}

//------------------------------------------------------------
void ofAppGlutWindow::setFullscreen(bool fullscreen){
    if( windowMode == OF_GAME_MODE)return;

    if(fullscreen && windowMode != OF_FULLSCREEN){
        bNewScreenMode  = true;
        windowMode      = OF_FULLSCREEN;
    }else if(!fullscreen && windowMode != OF_WINDOW) {
        bNewScreenMode  = true;
        windowMode      = OF_WINDOW;
    }
}

//------------------------------------------------------------
void ofAppGlutWindow::enableSetupScreen(){
	bEnableSetupScreen = true;
}

//------------------------------------------------------------
void ofAppGlutWindow::disableSetupScreen(){
	bEnableSetupScreen = false;
}

//------------------------------------------------------------
void ofAppGlutWindow::display(void){
	static ofEventArgs voidEventArgs;

	//--------------------------------
	// when I had "glutFullScreen()"
	// in the initOpenGl, I was gettings a "heap" allocation error
	// when debugging via visual studio.  putting it here, changes that.
	// maybe it's voodoo, or I am getting rid of the problem
	// by removing something unrelated, but everything seems
	// to work if I put fullscreen on the first frame of display.

	if (windowMode != OF_GAME_MODE){
		if ( bNewScreenMode ){
			if( windowMode == OF_FULLSCREEN){

				//----------------------------------------------------
				// before we go fullscreen, take a snapshot of where we are:
				nonFullScreenX = glutGet(GLUT_WINDOW_X);
				nonFullScreenY = glutGet(GLUT_WINDOW_Y);
				//----------------------------------------------------

				glutFullScreen();

				#ifdef TARGET_OSX
					SetSystemUIMode(kUIModeAllHidden,NULL);
				#endif

			}else if( windowMode == OF_WINDOW ){

				glutReshapeWindow(requestedWidth, requestedHeight);

				//----------------------------------------------------
				// if we have recorded the screen posion, put it there
				// if not, better to let the system do it (and put it where it wants)
				if (nFrameCount > 0){
					glutPositionWindow(nonFullScreenX,nonFullScreenY);
				}
				//----------------------------------------------------

				#ifdef TARGET_OSX
					SetSystemUIMode(kUIModeNormal,NULL);
				#endif
			}
			bNewScreenMode = false;
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

    // to do non auto clear on PC for now - we do something like "single" buffering --
    // it's not that pretty but it work for the most part

    #ifdef TARGET_WIN32
    if (bClearAuto == false){
        glDrawBuffer (GL_FRONT);
    }
    #endif

	if ( bClearAuto == true || nFrameCount < 3){
		glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if( bEnableSetupScreen )ofSetupScreen();


	if(ofAppPtr)
		ofAppPtr->draw();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.draw, voidEventArgs );
	#endif


    #ifdef TARGET_WIN32
    if (bClearAuto == false){
        // on a PC resizing a window with this method of accumulation (essentially single buffering)
        // is BAD, so we clear on resize events.
        if (nFramesSinceWindowResized < 3){
            glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        } else {
            if (nFrameCount < 3 || nFramesSinceWindowResized < 3)    glutSwapBuffers();
            else                                                     glFlush();
        }
    } else {
        glutSwapBuffers();
    }
    #else
		if (bClearAuto == false){
			// in accum mode resizing a window is BAD, so we clear on resize events.
			if (nFramesSinceWindowResized < 3){
				glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
		}
        glutSwapBuffers();
    #endif

    nFramesSinceWindowResized++;

	//fps calculation moved to idle_cb as we were having fps speedups when heavy drawing was occuring
	//wasn't reflecting on the actual app fps which was in reality slower. 
	//could be caused by some sort of deferred drawing? 

	nFrameCount++;		// increase the overall frame count

	//setFrameNum(nFrameCount); // get this info to ofUtils for people to access

}

//------------------------------------------------------------
void ofAppGlutWindow::mouse_cb(int button, int state, int x, int y) {
	static ofMouseEventArgs mouseEventArgs;

	if (nFrameCount > 0){
		if(ofAppPtr){
		ofAppPtr->mouseX = x;
		ofAppPtr->mouseY = y;
		}

		if (state == GLUT_DOWN) {
			if(ofAppPtr)
				ofAppPtr->mousePressed(x,y,button);

			#ifdef OF_USING_POCO
				mouseEventArgs.x = x;
				mouseEventArgs.y = y;
				mouseEventArgs.button = button;
				ofNotifyEvent( ofEvents.mousePressed, mouseEventArgs );
			#endif
		} else if (state == GLUT_UP) {
			if(ofAppPtr){
				ofAppPtr->mouseReleased(x,y,button);
				ofAppPtr->mouseReleased();
			}

			#ifdef OF_USING_POCO
				mouseEventArgs.x = x;
				mouseEventArgs.y = y;
				mouseEventArgs.button = button;
				ofNotifyEvent( ofEvents.mouseReleased, mouseEventArgs );
			#endif
		}
		buttonInUse = button;
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::motion_cb(int x, int y) {
	static ofMouseEventArgs mouseEventArgs;

	if (nFrameCount > 0){
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
void ofAppGlutWindow::passive_motion_cb(int x, int y) {
	static ofMouseEventArgs mouseEventArgs;

	if (nFrameCount > 0){
		if(ofAppPtr){
		ofAppPtr->mouseX = x;
		ofAppPtr->mouseY = y;
			ofAppPtr->mouseMoved(x,y);
		}

		#ifdef OF_USING_POCO
			mouseEventArgs.x = x;
			mouseEventArgs.y = y;
			ofNotifyEvent( ofEvents.mouseMoved, mouseEventArgs );
		#endif
	}
}


//------------------------------------------------------------
void ofAppGlutWindow::idle_cb(void) {
	static ofEventArgs voidEventArgs;

	//	thanks to jorge for the fix:
	//	http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate

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
	 timeThen		= timeNow;
  	// --------------	

	if(ofAppPtr)
		ofAppPtr->update();

		#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.update, voidEventArgs);
		#endif

	glutPostRedisplay();
}


//------------------------------------------------------------
void ofAppGlutWindow::keyboard_cb(unsigned char key, int x, int y) {
	static ofKeyEventArgs keyEventArgs;

	if(ofAppPtr)
		ofAppPtr->keyPressed(key);

	#ifdef OF_USING_POCO
		keyEventArgs.key = key;
		ofNotifyEvent( ofEvents.keyPressed, keyEventArgs );
	#endif

	if (key == OF_KEY_ESC){				// "escape"
		exitApp();
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::keyboard_up_cb(unsigned char key, int x, int y) {
	static ofKeyEventArgs keyEventArgs;

	if(ofAppPtr)
		ofAppPtr->keyReleased(key);

	#ifdef OF_USING_POCO
		keyEventArgs.key = key;
		ofNotifyEvent( ofEvents.keyReleased, keyEventArgs );
	#endif
}

//------------------------------------------------------
void ofAppGlutWindow::special_key_cb(int key, int x, int y) {
	static ofKeyEventArgs keyEventArgs;

	if(ofAppPtr)
		ofAppPtr->keyPressed(key | OF_KEY_MODIFIER);

	#ifdef OF_USING_POCO
		keyEventArgs.key = (key | OF_KEY_MODIFIER);
		ofNotifyEvent( ofEvents.keyPressed, keyEventArgs );
	#endif
}

//------------------------------------------------------------
void ofAppGlutWindow::special_key_up_cb(int key, int x, int y) {
	static ofKeyEventArgs keyEventArgs;

	if(ofAppPtr)
		ofAppPtr->keyReleased(key | OF_KEY_MODIFIER);

	#ifdef OF_USING_POCO
		keyEventArgs.key = (key | OF_KEY_MODIFIER);
		ofNotifyEvent( ofEvents.keyReleased, keyEventArgs );
	#endif
}

//------------------------------------------------------------
void ofAppGlutWindow::resize_cb(int w, int h) {
	static ofResizeEventArgs resizeEventArgs;

	windowW = w;
	windowH = h;

	if(ofAppPtr)
		ofAppPtr->windowResized(w,h);

	#ifdef OF_USING_POCO
		resizeEventArgs.width = w;
		resizeEventArgs.height = h;
		ofNotifyEvent( ofEvents.windowResized, resizeEventArgs );
	#endif

	nFramesSinceWindowResized = 0;
}
