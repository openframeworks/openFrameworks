void display(void);
void mouse_cb(int button, int state, int x, int y);
void motion_cb(int x, int y);
void passive_motion_cb(int x, int y);
void idle_cb(void);
void keyboard_cb(unsigned char key, int x, int y);
void keyboard_up_cb(unsigned char key, int x, int y);
void special_key_cb(int key, int x, int y) ;
void special_key_up_cb(int key, int x, int y) ;
void resize_cb(int w, int h);


//--------------------------------
static float timeNow, timeThen, fps;
static int nFramesForFPS;
static int nFrameCount = 0;
static int buttonInUse = 0;
static bool enableSetupScreen	= true;

static bool 	bFrameRateSet;
int 			millisForFrame;
int 			prevMillis;
int 			diffMillis;

static bool		newScreenMode = true;
int				requestedWidth;
int				requestedHeight;
int 			nonFullScreenX = -1;
int 			nonFullScreenY = -1;


//---------------------------------

//------------------------------------------------------------
void display(void){

	//--------------------------------
	// when I had "glutFullScreen()"
	// in the initOpenGl, I was gettings a "heap" allocation error
	// when debugging via visual studio.  putting it here, changes that.
	// maybe it's voodoo, or I am getting rid of the problem
	// by removing something unrelated, but everything seems
	// to work if I put fullscreen on the first frame of display.

	if (windowMode != OF_GAME_MODE){
		if ( newScreenMode ){
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
			newScreenMode = false;
		}
	}

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
		if ( bClearAuto == true || nFrameCount < 3){
	#endif
		glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if( enableSetupScreen )ofSetupScreen();

	if(OFSAptr)
		OFSAptr->draw();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.draw, voidEventArgs );
	#endif
    
  	glutSwapBuffers();

  	// -------------- fps calculation:
	timeNow = ofGetElapsedTimef();
	if( (timeNow-timeThen) > 0.05f || nFramesForFPS == 0 ) {
 		fps = (double)nFramesForFPS / (timeNow-timeThen);
      	timeThen = timeNow;
		nFramesForFPS = 0;
		ofFrameRate = 0.9f * ofFrameRate + 0.1f * fps;
  	}
  	nFramesForFPS++;
  	// --------------

	nFrameCount++;		// increase the overall frame count

	setFrameNum(nFrameCount); // get this info to ofUtils for people to access

}


//------------------------------------------------------------
void mouse_cb(int button, int state, int x, int y) {
	if (nFrameCount > 0){
		if(OFSAptr){
		OFSAptr->mouseX = x;
		OFSAptr->mouseY = y;
		}

		if (state == GLUT_DOWN) {
			if(OFSAptr)
				OFSAptr->mousePressed(x,y,button);

			#ifdef OF_USING_POCO
				mouseEventArgs.x = x;
				mouseEventArgs.y = y;
				mouseEventArgs.button = button;
				ofNotifyEvent( ofEvents.mousePressed, mouseEventArgs );
			#endif
		} else if (state == GLUT_UP) {
			if(OFSAptr)
				OFSAptr->mouseReleased(x,y,button);

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
void motion_cb(int x, int y) {

	if (nFrameCount > 0){
		if(OFSAptr){
		OFSAptr->mouseX = x;
		OFSAptr->mouseY = y;
			OFSAptr->mouseDragged(x,y,buttonInUse);
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
void passive_motion_cb(int x, int y) {
	if (nFrameCount > 0){
		if(OFSAptr){
		OFSAptr->mouseX = x;
		OFSAptr->mouseY = y;
			OFSAptr->mouseMoved(x,y);
		}

		#ifdef OF_USING_POCO
			mouseEventArgs.x = x;
			mouseEventArgs.y = y;
			ofNotifyEvent( ofEvents.mouseMoved, mouseEventArgs );
		#endif
	}
}

//------------------------------------------------------------
void idle_cb(void) {

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
	
	if(OFSAptr)
		OFSAptr->update();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.update, voidEventArgs );
	#endif
    
	glutPostRedisplay();
}


//------------------------------------------------------------
void keyboard_cb(unsigned char key, int x, int y) {
	if(OFSAptr)
		OFSAptr->keyPressed(key);

	#ifdef OF_USING_POCO
		keyEventArgs.key = key;
		ofNotifyEvent( ofEvents.keyPressed, keyEventArgs );
	#endif

	if (key == 27){				// "escape"
		OF_EXIT_APP(0);
	}
}

//------------------------------------------------------------
void keyboard_up_cb(unsigned char key, int x, int y) {
	if(OFSAptr)
		OFSAptr->keyReleased(key);

	#ifdef OF_USING_POCO
		keyEventArgs.key = key;
		ofNotifyEvent( ofEvents.keyReleased, keyEventArgs );
	#endif
}

//------------------------------------------------------
void special_key_cb(int key, int x, int y) {
	if(OFSAptr)
		OFSAptr->keyPressed(key | OF_KEY_MODIFIER);

	#ifdef OF_USING_POCO
		keyEventArgs.key = (key | OF_KEY_MODIFIER);
		ofNotifyEvent( ofEvents.keyPressed, keyEventArgs );
	#endif
}

//------------------------------------------------------------
void special_key_up_cb(int key, int x, int y) {
	if(OFSAptr)
		OFSAptr->keyReleased(key | OF_KEY_MODIFIER);

	#ifdef OF_USING_POCO
		keyEventArgs.key = (key | OF_KEY_MODIFIER);
		ofNotifyEvent( ofEvents.keyReleased, keyEventArgs );
	#endif
}

//------------------------------------------------------------
void resize_cb(int w, int h) {
	if(OFSAptr)
		OFSAptr->resized(w,h);

	#ifdef OF_USING_POCO
		resizeEventArgs.width = w;
		resizeEventArgs.height = h;
		ofNotifyEvent( ofEvents.resize, resizeEventArgs );
	#endif
}
