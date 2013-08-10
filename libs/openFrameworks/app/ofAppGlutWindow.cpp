#include "ofAppGlutWindow.h"
#include "ofBaseApp.h"
#include "ofEvents.h"
#include "ofUtils.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofConstants.h"
#include "ofGLProgrammableRenderer.h"

#ifdef TARGET_WIN32
	#define GLUT_BUILDING_LIB
	#include "glut.h"
#endif
#ifdef TARGET_OSX
    #include <OpenGL/OpenGL.h>
	#include "../../../libs/glut/lib/osx/GLUT.framework/Versions/A/Headers/glut.h"
	#include <Carbon/Carbon.h>
#endif
#ifdef TARGET_LINUX
	#include <GL/glut.h>
	#include "ofIcon.h"
	#include "ofImage.h"
	#include <X11/Xatom.h>
#endif

void ofGLReadyCallback();


// glut works with static callbacks UGH, so we need static variables here:

static int			windowMode;
static bool			bNewScreenMode;
static int			buttonInUse;
static bool			bEnableSetupScreen;
static bool			bDoubleBuffered; 

static int			requestedWidth;
static int			requestedHeight;
static int 			nonFullScreenX;
static int 			nonFullScreenY;
static int			windowW;
static int			windowH;
static int          nFramesSinceWindowResized;
static ofOrientation	orientation;
static ofBaseApp *  ofAppPtr;

#ifdef TARGET_WIN32

//------------------------------------------------

// this is to fix a bug with glut that doesn't properly close the app
// with window closing.  we grab the window procedure, store it, and parse windows messages,
// using the close and destroy messages and passing on the others...

//------------------------------------------------

static WNDPROC currentWndProc;
static HWND handle  = NULL;

// This function takes in a wParam from the WM_DROPFILES message and
// prints all the files to a message box.

void HandleFiles(WPARAM wParam)
{
    // DragQueryFile() takes a LPWSTR for the name so we need a TCHAR string
    TCHAR szName[MAX_PATH];

    // Here we cast the wParam as a HDROP handle to pass into the next functions
    HDROP hDrop = (HDROP)wParam;

	POINT pt;
	DragQueryPoint(hDrop, &pt);
	//ofLogNotice("ofAppGlutWindow") << "drag point: " << pt.x << pt.y;

	ofDragInfo info;
	info.position.x = pt.x;
	info.position.y = pt.y;


    // This functions has a couple functionalities.  If you pass in 0xFFFFFFFF in
    // the second parameter then it returns the count of how many filers were drag
    // and dropped.  Otherwise, the function fills in the szName string array with
    // the current file being queried.
    int count = DragQueryFile(hDrop, 0xFFFFFFFF, szName, MAX_PATH);

	#ifdef _MSC_VER
    // Here we go through all the files that were drag and dropped then display them
    for(int i = 0; i < count; i++)
    {
        // Grab the name of the file associated with index "i" in the list of files dropped.
        // Be sure you know that the name is attached to the FULL path of the file.
        DragQueryFile(hDrop, i, szName, MAX_PATH);

		wchar_t * s =  (wchar_t*)szName;
		char dfault = '?';
        const std::locale& loc = std::locale();
		std::ostringstream stm;
		while( *s != L'\0' ) {
			stm << std::use_facet< std::ctype<wchar_t> >( loc ).narrow( *s++, dfault );
		}
		info.files.push_back(string(stm.str()));

			//toUTF8(udispName, dispName);

        // Bring up a message box that displays the current file being processed
        //MessageBox(GetForegroundWindow(), szName, L"Current file received", MB_OK);
    }
#else

    HDROP hdrop = (HDROP)(wParam);
	int index, length;
	count = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);
	for (index=0; index<count; ++index) {
	  length = DragQueryFile(hdrop, index, NULL, 0);
	  if (length > 0) {
	    TCHAR* lpstr = new TCHAR[length+1];
	    DragQueryFile(hdrop, index, lpstr, length+1);
	    string temp = lpstr;
	    info.files.push_back(temp);
	    delete[] lpstr;
	  }
	}

	#endif

    // Finally, we destroy the HDROP handle so the extra memory
    // allocated by the application is released.
    DragFinish(hDrop);

	ofAppPtr->dragEvent(info);

}


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
	  case WM_DROPFILES:

            // Call our function we created to display all the files.
            // We pass the wParam because it's the HDROP handle.
            HandleFiles(wParam);
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

	// enable drag and drop of files.
	DragAcceptFiles (handle, TRUE);

	//store the current message event handler for the window
	currentWndProc = (WNDPROC)GetWindowLongPtr(handle, GWL_WNDPROC);

	//tell the window to now use our event handler!
	SetWindowLongPtr(handle, GWL_WNDPROC, (long)winProc);
}

#endif




//----------------------------------------------------------
ofAppGlutWindow::ofAppGlutWindow(){
	windowMode			= OF_WINDOW;
	bNewScreenMode		= true;
	nFramesSinceWindowResized = 0;
	buttonInUse			= 0;
	bEnableSetupScreen	= true;
	requestedWidth		= 0;
	requestedHeight		= 0;
	nonFullScreenX		= -1;
	nonFullScreenY		= -1;
	displayString		= "";
	orientation			= OF_ORIENTATION_DEFAULT;
	bDoubleBuffered = true; // LIA
	iconSet = false;

}

//lets you enable alpha blending using a display string like:
// "rgba double samples>=4 depth" ( mac )
// "rgb double depth alpha samples>=4" ( some pcs )
//------------------------------------------------------------
 void ofAppGlutWindow::setGlutDisplayString(string displayStr){
	displayString = displayStr;
 }


void ofAppGlutWindow::setDoubleBuffering(bool _bDoubleBuffered){ 
	bDoubleBuffered = _bDoubleBuffered;
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
		if(bDoubleBuffered){  
			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
		}else{
			glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH | GLUT_ALPHA );
		}
	}

	windowMode = screenMode;
	bNewScreenMode = true;

	if (windowMode == OF_FULLSCREEN){
		glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
		glutCreateWindow("");
		
		requestedWidth  = w;
		requestedHeight = h;
	} else if (windowMode != OF_GAME_MODE){
		glutInitWindowSize(w, h);
		glutCreateWindow("");

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
    		ofLogError("ofAppGlutWindow") << "setupOpenGL(): selected game mode format " << gameStr << " not available";
    	}
    	// start fullscreen game mode
    	glutEnterGameMode();
	}
	windowW = glutGet(GLUT_WINDOW_WIDTH);
	windowH = glutGet(GLUT_WINDOW_HEIGHT);

	ofGLReadyCallback();
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
	glutEntryFunc(entry_cb);

#ifdef TARGET_OSX
	glutDragEventFunc(dragEvent);
#endif

    nFramesSinceWindowResized = 0;

    #ifdef TARGET_WIN32
        //----------------------
        // this is specific to windows (respond properly to close / destroy)
        fixCloseWindowOnWin32();
    #endif

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
void ofAppGlutWindow::setWindowIcon(const string & path){
    ofPixels iconPixels;
	ofLoadImage(iconPixels,path);
	setWindowIcon(iconPixels);
}

//------------------------------------------------------------
void ofAppGlutWindow::setWindowIcon(const ofPixels & iconPixels){
	iconSet = true;
	Display *m_display = glXGetCurrentDisplay();
	GLXDrawable m_window = glXGetCurrentDrawable();
	iconSet = true;
	int length = 2+iconPixels.getWidth()*iconPixels.getHeight();
	unsigned long * buffer = new unsigned long[length];
	buffer[0]=iconPixels.getWidth();
	buffer[1]=iconPixels.getHeight();
	for(int i=0;i<iconPixels.getWidth()*iconPixels.getHeight();i++){
		buffer[i+2] = iconPixels[i*4+3]<<24;
		buffer[i+2] += iconPixels[i*4]<<16;
		buffer[i+2] += iconPixels[i*4+1]<<8;
		buffer[i+2] += iconPixels[i*4];
	}

	XChangeProperty(m_display, m_window, XInternAtom(m_display, "_NET_WM_ICON", False), XA_CARDINAL, 32,
						 PropModeReplace,  (const unsigned char*)buffer,  length);
	XFlush(m_display);
}
#endif

//------------------------------------------------------------
void ofAppGlutWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	ofAppPtr = appPtr;

	ofNotifySetup();
	ofNotifyUpdate();

	glutMainLoop();
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
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return ofPoint(x,y,0);
	}else{
		return ofPoint(y,x,0);
	}
}

//------------------------------------------------------------
ofPoint ofAppGlutWindow::getScreenSize(){
	int width = glutGet(GLUT_SCREEN_WIDTH);
	int height = glutGet(GLUT_SCREEN_HEIGHT);
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return ofPoint(width, height,0);
	}else{
		return ofPoint(height, width,0);
	}
}

//------------------------------------------------------------
int ofAppGlutWindow::getWidth(){
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return windowW;
	}
	return windowH;
}

//------------------------------------------------------------
int ofAppGlutWindow::getHeight(){
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return windowH;
	}
	return windowW;
}

//------------------------------------------------------------
void ofAppGlutWindow::setOrientation(ofOrientation orientationIn){
	orientation = orientationIn;
}

//------------------------------------------------------------
ofOrientation ofAppGlutWindow::getOrientation(){
	return orientation;
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
	#if defined(TARGET_OSX) && defined(MAC_OS_X_VERSION_10_7)
		 CGDisplayHideCursor(NULL);
	#else
		glutSetCursor(GLUT_CURSOR_NONE);
	#endif
}

//------------------------------------------------------------
void ofAppGlutWindow::showCursor(){
	#if defined(TARGET_OSX) && defined(MAC_OS_X_VERSION_10_7)
		 CGDisplayShowCursor(NULL);
	#else
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	#endif
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
void ofAppGlutWindow::setVerticalSync(bool bSync){
	//----------------------------
	#ifdef TARGET_WIN32
	//----------------------------
		if (bSync) {
			if (WGL_EXT_swap_control) {
				wglSwapIntervalEXT (1);
			}
		} else {
			if (WGL_EXT_swap_control) {
				wglSwapIntervalEXT (0);
			}
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
		void (*swapIntervalExt)(Display *,GLXDrawable, int)	 = (void (*)(Display *,GLXDrawable, int)) glXGetProcAddress((const GLubyte*) "glXSwapIntervalEXT");
		if(swapIntervalExt){
			Display *dpy = glXGetCurrentDisplay();
			GLXDrawable drawable = glXGetCurrentDrawable();
			if (drawable) {
				swapIntervalExt(dpy, drawable, bSync ? 1 : 0);
				return;
			}
		}
		void (*swapInterval)(int) = (void (*)(int)) glXGetProcAddress((const GLubyte*) "glXSwapIntervalSGI");
		if(!swapInterval) {
			swapInterval = (void (*)(int)) glXGetProcAddress((const GLubyte*) "glXSwapIntervalMESA");
		}
		if(swapInterval) {
			swapInterval(bSync ? 1 : 0);
		}
	//--------------------------------------
	#endif
	//--------------------------------------
}

//------------------------------------------------------------
void ofAppGlutWindow::display(void){

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
					#ifdef MAC_OS_X_VERSION_10_7 //needed for Lion as when the machine reboots the app is not at front level
						if( ofGetFrameNum() <= 10 ){  //is this long enough? too long?
							ProcessSerialNumber psn;							
							OSErr err = GetCurrentProcess( &psn );
							if ( err == noErr ){
								SetFrontProcess( &psn );
							}
						}
					#endif
				#endif

			}else if( windowMode == OF_WINDOW ){

				glutReshapeWindow(requestedWidth, requestedHeight);

				//----------------------------------------------------
				// if we have recorded the screen posion, put it there
				// if not, better to let the system do it (and put it where it wants)
				if (ofGetFrameNum() > 0){
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


	ofPtr<ofGLProgrammableRenderer> renderer = ofGetGLProgrammableRenderer();
	if(renderer){
		renderer->startRender();
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

	if ( bClearAuto == true || ofGetFrameNum() < 3){
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
            if ( (ofGetFrameNum() < 3 || nFramesSinceWindowResized < 3) && bDoubleBuffered)    glutSwapBuffers();
            else                                                     glFlush();
        }
    } else {
        if(bDoubleBuffered){
			glutSwapBuffers();
		} else{
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
			glutSwapBuffers();
		} else{
			glFlush();
		}
    #endif

	if(renderer){
		renderer->finishRender();
	}

    nFramesSinceWindowResized++;

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
void ofAppGlutWindow::mouse_cb(int button, int state, int x, int y) {
	rotateMouseXY(orientation, x, y);
    

	switch(button){
	case GLUT_LEFT_BUTTON:
		button = OF_MOUSE_BUTTON_LEFT;
		break;
	case GLUT_RIGHT_BUTTON:
		button = OF_MOUSE_BUTTON_RIGHT;
		break;
	case GLUT_MIDDLE_BUTTON:
		button = OF_MOUSE_BUTTON_MIDDLE;
		break;
	}
    
	if (ofGetFrameNum() > 0){
		if (state == GLUT_DOWN) {
			ofNotifyMousePressed(x, y, button);
		} else if (state == GLUT_UP) {
			ofNotifyMouseReleased(x, y, button);
		}

		buttonInUse = button;
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::motion_cb(int x, int y) {
	rotateMouseXY(orientation, x, y);

	if (ofGetFrameNum() > 0){
		ofNotifyMouseDragged(x, y, buttonInUse);
	}

}

//------------------------------------------------------------
void ofAppGlutWindow::passive_motion_cb(int x, int y) {
	rotateMouseXY(orientation, x, y);

	if (ofGetFrameNum() > 0){
		ofNotifyMouseMoved(x, y);
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::dragEvent(char ** names, int howManyFiles, int dragX, int dragY){

	// TODO: we need position info on mac passed through
	ofDragInfo info;
	info.position.x = dragX;
	info.position.y = ofGetHeight()-dragY;

	for (int i = 0; i < howManyFiles; i++){
		string temp = string(names[i]);
		info.files.push_back(temp);
	}

	ofNotifyDragEvent(info);
}


//------------------------------------------------------------
void ofAppGlutWindow::idle_cb(void) {

	ofNotifyUpdate();

	glutPostRedisplay();
}


//------------------------------------------------------------
void ofAppGlutWindow::keyboard_cb(unsigned char key, int x, int y) {
	ofNotifyKeyPressed(key);
}

//------------------------------------------------------------
void ofAppGlutWindow::keyboard_up_cb(unsigned char key, int x, int y){
	ofNotifyKeyReleased(key);
}

//------------------------------------------------------
void ofAppGlutWindow::special_key_cb(int key, int x, int y) {
	ofNotifyKeyPressed(key | OF_KEY_MODIFIER);
}

//------------------------------------------------------------
void ofAppGlutWindow::special_key_up_cb(int key, int x, int y) {
	ofNotifyKeyReleased(key | OF_KEY_MODIFIER);
}

//------------------------------------------------------------
void ofAppGlutWindow::resize_cb(int w, int h) {
	windowW = w;
	windowH = h;

	ofNotifyWindowResized(w, h);

	nFramesSinceWindowResized = 0;
}

void ofAppGlutWindow::entry_cb( int state ) {
	
	ofNotifyWindowEntry( state );
	
}
