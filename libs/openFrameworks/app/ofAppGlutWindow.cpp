#include "ofAppGlutWindow.h"
#include "ofBaseApp.h"
#include "ofConstants.h"
#include "ofPixels.h"
#include "ofGLRenderer.h"

#ifdef TARGET_WIN32
	#if (_MSC_VER)
		#define GLUT_BUILDING_LIB
		#include "glut.h"
	#else
		#include <GL/glut.h>
		#include <GL/freeglut_ext.h>
	#endif
	#include <Shellapi.h>
#endif
#ifdef TARGET_OSX
    #include <OpenGL/OpenGL.h>
	#include "../../../libs/glut/lib/osx/GLUT.framework/Versions/A/Headers/glut.h"
    #include <Cocoa/Cocoa.h>
#endif
#ifdef TARGET_LINUX
	#include <GL/glut.h>
	#include "ofIcon.h"
	#include "ofImage.h"
	#include <X11/Xatom.h>
	#include <GL/freeglut_ext.h>
	#include <GL/glx.h>
#endif

using namespace std;

// glut works with static callbacks UGH, so we need static variables here:

static ofWindowMode windowMode;
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
static ofAppGlutWindow * instance;

#ifdef TARGET_WIN32

//------------------------------------------------

// this is to fix a bug with glut that doesn't properly close the app
// with window closing.  we grab the window procedure, store it, and parse windows messages,
// using the close and destroy messages and passing on the others...

//------------------------------------------------

static WNDPROC currentWndProc;
static HWND handle  = nullptr;

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

    // Finally, we destroy the HDROP handle so the extra memory
    // allocated by the application is released.
    DragFinish(hDrop);

	instance->events().notifyDragEvent(info);

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
	currentWndProc = (WNDPROC)GetWindowLongPtr(handle, GWLP_WNDPROC);

	//tell the window to now use our event handler!
	SetWindowLongPtr(handle, GWLP_WNDPROC, (LONG_PTR)winProc);
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
	instance = this;
	windowId = 0;
}

//lets you enable alpha blending using a display string like:
// "rgba double samples>=4 depth" ( mac )
// "rgb double depth alpha samples>=4" ( some pcs )
//------------------------------------------------------------
 void ofAppGlutWindow::setGlutDisplayString(string displayStr){
	displayString = displayStr;
 }

 //------------------------------------------------------------
void ofAppGlutWindow::setDoubleBuffering(bool _bDoubleBuffered){
	bDoubleBuffered = _bDoubleBuffered;
}

//------------------------------------------------------------
void ofAppGlutWindow::setup(const ofGLWindowSettings & settings){

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

	windowMode = settings.windowMode;
	bNewScreenMode = true;

	if (windowMode == OF_FULLSCREEN){
		glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
		windowId = glutCreateWindow("");

		requestedWidth  = settings.getWidth();
		requestedHeight = settings.getHeight();
	} else if (windowMode != OF_GAME_MODE){
		glutInitWindowSize(settings.getWidth(), settings.getHeight());
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
		sprintf( gameStr, "%dx%d:%d@%d", settings.getWidth(), settings.getHeight(), 32, 60 );

    	glutGameModeString(gameStr);

    	if (!glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)){
    		ofLogError("ofAppGlutWindow") << "setupOpenGL(): selected game mode format " << gameStr << " not available";
    	}
    	// start fullscreen game mode
    	glutEnterGameMode();
	}
	windowW = glutGet(GLUT_WINDOW_WIDTH);
	windowH = glutGet(GLUT_WINDOW_HEIGHT);

	currentRenderer = shared_ptr<ofBaseRenderer>(new ofGLRenderer(this));


#ifndef TARGET_OPENGLES
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		ofLogError("ofAppRunner") << "couldn't init GLEW: " << glewGetErrorString(err);
		return;
	}
#endif
	static_cast<ofGLRenderer*>(currentRenderer.get())->setup();
	setVerticalSync(true);


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
#ifdef TARGET_LINUX
	glutCloseFunc(exit_cb);
#endif

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
			GIMP_IMAGE_RUN_LENGTH_DECODE(iconPixels.getData(),ofIconDebug.rle_pixel_data,iconPixels.getWidth()*iconPixels.getHeight(),ofIconDebug.bytes_per_pixel);
		#else
			iconPixels.allocate(ofIcon.width,ofIcon.height,ofIcon.bytes_per_pixel);
			GIMP_IMAGE_RUN_LENGTH_DECODE(iconPixels.getData(),ofIcon.rle_pixel_data,iconPixels.getWidth()*iconPixels.getHeight(),ofIcon.bytes_per_pixel);
		#endif
		setWindowIcon(iconPixels);
    }
#endif
	if (settings.isPositionSet()) {
		setWindowPosition(settings.getPosition().x,settings.getPosition().y);
	}

#ifdef TARGET_OSX
	// The osx implementation of glut changes the cwd, this restores it
	// to wherever it was when the app was started
	ofRestoreWorkingDirectoryToDefault();
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
	for(size_t i=0;i<iconPixels.getWidth()*iconPixels.getHeight();i++){
		buffer[i+2] = iconPixels[i*4+3]<<24;
		buffer[i+2] += iconPixels[i*4]<<16;
		buffer[i+2] += iconPixels[i*4+1]<<8;
		buffer[i+2] += iconPixels[i*4];
	}

	XChangeProperty(m_display, m_window, XInternAtom(m_display, "_NET_WM_ICON", False), XA_CARDINAL, 32,
						 PropModeReplace,  (const unsigned char*)buffer,  length);
	delete[] buffer;
	XFlush(m_display);
}
#endif

//------------------------------------------------------------
void ofAppGlutWindow::update(){
	idle_cb();
}

//------------------------------------------------------------
void ofAppGlutWindow::draw(){
	display();
}

//------------------------------------------------------------
void ofAppGlutWindow::close(){
	events().notifyExit();
	events().disable();
#ifdef TARGET_LINUX
	glutLeaveMainLoop();
#else
	std::exit(0);
#endif
}

//------------------------------------------------------------
void ofAppGlutWindow::loop(){
	instance->events().notifySetup();
	instance->events().notifyUpdate();
	glutMainLoop();
}

//------------------------------------------------------------
void ofAppGlutWindow::setWindowTitle(string title){
	glutSetWindowTitle(title.c_str());
}

//------------------------------------------------------------
glm::vec2 ofAppGlutWindow::getWindowSize(){
	return {windowW, windowH};
}

//------------------------------------------------------------
glm::vec2 ofAppGlutWindow::getWindowPosition(){
	int x = glutGet(GLUT_WINDOW_X);
	int y = glutGet(GLUT_WINDOW_Y);
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return {x,y};
	}else{
		return {y,x};
	}
}

//------------------------------------------------------------
glm::vec2 ofAppGlutWindow::getScreenSize(){
	int width = glutGet(GLUT_SCREEN_WIDTH);
	int height = glutGet(GLUT_SCREEN_HEIGHT);
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return {width, height};
	}else{
		return {height, width};
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
		 CGDisplayHideCursor(0);
	#else
		glutSetCursor(GLUT_CURSOR_NONE);
	#endif
}

//------------------------------------------------------------
void ofAppGlutWindow::showCursor(){
	#if defined(TARGET_OSX) && defined(MAC_OS_X_VERSION_10_7)
		 CGDisplayShowCursor(0);
	#else
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	#endif
}

//------------------------------------------------------------
ofWindowMode ofAppGlutWindow::getWindowMode(){
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
ofCoreEvents & ofAppGlutWindow::events(){
	return coreEvents;
}

//------------------------------------------------------------
shared_ptr<ofBaseRenderer> & ofAppGlutWindow::renderer(){
	return currentRenderer;
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
					[NSApp setPresentationOptions:NSApplicationPresentationHideMenuBar | NSApplicationPresentationHideDock];
					#ifdef MAC_OS_X_VERSION_10_7 //needed for Lion as when the machine reboots the app is not at front level
						if( instance->events().getFrameNum() <= 10 ){  //is this long enough? too long?
							[[NSApplication sharedApplication] activateIgnoringOtherApps:YES];
						}
					#endif
				#endif

			}else if( windowMode == OF_WINDOW ){

				glutReshapeWindow(requestedWidth, requestedHeight);

				//----------------------------------------------------
				// if we have recorded the screen posion, put it there
				// if not, better to let the system do it (and put it where it wants)
				if (instance->events().getFrameNum() > 0){
					glutPositionWindow(nonFullScreenX,nonFullScreenY);
				}
				//----------------------------------------------------

				#ifdef TARGET_OSX
					[NSApp setPresentationOptions:NSApplicationPresentationDefault];
				#endif
			}
			bNewScreenMode = false;
		}
	}

	instance->currentRenderer->startRender();

	if( bEnableSetupScreen ) instance->currentRenderer->setupScreen();

	instance->events().notifyDraw();

    #ifdef TARGET_WIN32
    if (instance->currentRenderer->getBackgroundAuto() == false){
        // on a PC resizing a window with this method of accumulation (essentially single buffering)
        // is BAD, so we clear on resize events.
        if (nFramesSinceWindowResized < 3){
            instance->currentRenderer->clear();
        } else {
            if ( (instance->events().getFrameNum() < 3 || nFramesSinceWindowResized < 3) && bDoubleBuffered)    glutSwapBuffers();
            else  glFlush();
        }
    } else {
        if(bDoubleBuffered){
			glutSwapBuffers();
		} else{
			glFlush();
		}
    }
    #else
		if (instance->currentRenderer->getBackgroundAuto() == false){
			// in accum mode resizing a window is BAD, so we clear on resize events.
			if (nFramesSinceWindowResized < 3){
				instance->currentRenderer->clear();
			}
		}
		if(bDoubleBuffered){
			glutSwapBuffers();
		} else{
			glFlush();
		}
    #endif

	instance->currentRenderer->finishRender();

    nFramesSinceWindowResized++;

}

//------------------------------------------------------------
void ofAppGlutWindow::swapBuffers() {
	glutSwapBuffers();
}

//--------------------------------------------
void ofAppGlutWindow::startRender() {
	renderer()->startRender();
}

//--------------------------------------------
void ofAppGlutWindow::finishRender() {
	renderer()->finishRender();
}

//------------------------------------------------------------
static void rotateMouseXY(ofOrientation orientation, int w, int h, int &x, int &y) {
	int savedY;
	switch(orientation) {
		case OF_ORIENTATION_180:
			x = w - x;
			y = h - y;
			break;

		case OF_ORIENTATION_90_RIGHT:
			savedY = y;
			y = x;
			x = w-savedY;
			break;

		case OF_ORIENTATION_90_LEFT:
			savedY = y;
			y = h - x;
			x = savedY;
			break;

		case OF_ORIENTATION_DEFAULT:
		default:
			break;
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::mouse_cb(int button, int state, int x, int y) {
	rotateMouseXY(orientation, instance->getWidth(), instance->getHeight(), x, y);


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

	if (instance->events().getFrameNum() > 0){
		if (state == GLUT_DOWN) {
			instance->events().notifyMousePressed(x, y, button);
		} else if (state == GLUT_UP) {
			instance->events().notifyMouseReleased(x, y, button);
		}

		buttonInUse = button;
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::motion_cb(int x, int y) {
	rotateMouseXY(orientation, instance->getWidth(), instance->getHeight(), x, y);

	if (instance->events().getFrameNum() > 0){
		instance->events().notifyMouseDragged(x, y, buttonInUse);
	}

}

//------------------------------------------------------------
void ofAppGlutWindow::passive_motion_cb(int x, int y) {
	rotateMouseXY(orientation, instance->getWidth(), instance->getHeight(), x, y);

	if (instance->events().getFrameNum() > 0){
		instance->events().notifyMouseMoved(x, y);
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::dragEvent(char ** names, int howManyFiles, int dragX, int dragY){

	// TODO: we need position info on mac passed through
	ofDragInfo info;
	info.position.x = dragX;
	info.position.y = instance->getHeight()-dragY;

	for (int i = 0; i < howManyFiles; i++){
		string temp = string(names[i]);
		info.files.push_back(temp);
	}

	instance->events().notifyDragEvent(info);
}


//------------------------------------------------------------
void ofAppGlutWindow::idle_cb(void) {
	instance->events().notifyUpdate();

	glutPostRedisplay();
}


//------------------------------------------------------------
void ofAppGlutWindow::keyboard_cb(unsigned char key, int x, int y) {
	instance->events().notifyKeyPressed(key);
}

//------------------------------------------------------------
void ofAppGlutWindow::keyboard_up_cb(unsigned char key, int x, int y){
	instance->events().notifyKeyReleased(key);
}

//------------------------------------------------------
void ofAppGlutWindow::special_key_cb(int key, int x, int y) {
	instance->events().notifyKeyPressed(special_key_to_of(key));
}

//------------------------------------------------------------
void ofAppGlutWindow::special_key_up_cb(int key, int x, int y) {
	instance->events().notifyKeyReleased(special_key_to_of(key));
}

//------------------------------------------------------------
int ofAppGlutWindow::special_key_to_of(int key) {
	switch (key) {
	case GLUT_KEY_F1:
		return OF_KEY_F1;

	case GLUT_KEY_F2:
		return OF_KEY_F2;

	case GLUT_KEY_F3:
		return OF_KEY_F3;

	case GLUT_KEY_F4:
		return OF_KEY_F4;

	case GLUT_KEY_F5:
		return OF_KEY_F5;

	case GLUT_KEY_F6:
		return OF_KEY_F6;

	case GLUT_KEY_F7:
		return OF_KEY_F7;

	case GLUT_KEY_F8:
		return OF_KEY_F8;

	case GLUT_KEY_F9:
		return OF_KEY_F9;

	case GLUT_KEY_F10:
		return OF_KEY_F10;

	case GLUT_KEY_F11:
		return OF_KEY_F11;

	case GLUT_KEY_F12:
		return OF_KEY_F12;

	case GLUT_KEY_LEFT:
		return OF_KEY_LEFT;

	case GLUT_KEY_UP:
		return OF_KEY_UP;

	case GLUT_KEY_RIGHT:
		return OF_KEY_RIGHT;

	case GLUT_KEY_DOWN:
		return OF_KEY_DOWN;

	case GLUT_KEY_PAGE_UP:
		return OF_KEY_PAGE_UP;

	case GLUT_KEY_PAGE_DOWN:
		return OF_KEY_PAGE_DOWN;

	case GLUT_KEY_HOME:
		return OF_KEY_HOME;

	case GLUT_KEY_END:
		return OF_KEY_END;

	case GLUT_KEY_INSERT:
		return OF_KEY_INSERT;

	default:
		return 0;
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::resize_cb(int w, int h) {
	windowW = w;
	windowH = h;

	instance->events().notifyWindowResized(w, h);

	nFramesSinceWindowResized = 0;
}

//------------------------------------------------------------
void ofAppGlutWindow::entry_cb(int state) {
	if (state == GLUT_ENTERED){
		instance->events().notifyMouseEntered(instance->events().getMouseX(), instance->events().getMouseY());
	}else if (state == GLUT_LEFT){
		instance->events().notifyMouseExited(instance->events().getMouseX(), instance->events().getMouseY());
	}
}

//------------------------------------------------------------
void ofAppGlutWindow::exit_cb() {
	instance->events().notifyExit();
	instance->events().disable();
}
