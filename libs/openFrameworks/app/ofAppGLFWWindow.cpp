#include "ofAppGLFWWindow.h"
#include "ofEvents.h"

#include "ofBaseApp.h"
#include "ofGLProgrammableRenderer.h"
#include "ofAppRunner.h"

#ifdef TARGET_LINUX
	#include "ofIcon.h"
	#include "ofImage.h"
	#define GLFW_EXPOSE_NATIVE_X11
	#ifndef TARGET_OPENGLES
		#define GLFW_EXPOSE_NATIVE_GLX
	#else
		#define GLFW_EXPOSE_NATIVE_EGL
	#endif
	#include "GLFW/glfw3native.h"
	#include <X11/Xatom.h>
	#include "Poco/URI.h"
#elif defined(TARGET_OSX)
	#include <Cocoa/Cocoa.h>
	#include <Carbon/Carbon.h>
	#define GLFW_EXPOSE_NATIVE_COCOA
	#define GLFW_EXPOSE_NATIVE_NSGL
	#include "GLFW/glfw3native.h"
#elif defined(TARGET_WIN32)
	#define GLFW_EXPOSE_NATIVE_WIN32
	#define GLFW_EXPOSE_NATIVE_WGL
	#include <GLFW/glfw3native.h>
#endif

//========================================================================
// static variables:

ofBaseApp *	ofAppGLFWWindow::ofAppPtr;
ofAppGLFWWindow	* ofAppGLFWWindow::instance;
GLFWwindow* ofAppGLFWWindow::windowP = NULL;

void ofGLReadyCallback();

//-------------------------------------------------------
ofAppGLFWWindow::ofAppGLFWWindow():ofAppBaseWindow(){
	ofLogVerbose("ofAppGLFWWindow") << "creating GLFW window";
	bEnableSetupScreen	= true;
	buttonInUse			= 0;
	buttonPressed		= false;
    bMultiWindowFullscreen  = false;

	nonFullScreenX		= 0;
	nonFullScreenY		= 0;
	nonFullScreenW		= 0;
	nonFullScreenH		= 0;

	samples				= 0;
	rBits=gBits=bBits=aBits = 8;
	depthBits			= 24;
	stencilBits			= 0;

	orientation 		= OF_ORIENTATION_DEFAULT;

	requestedWidth		= 0;
	requestedHeight		= 0;
	windowMode			= OF_WINDOW;

	windowW				= 0;
	windowH				= 0;
	bDoubleBuffered		= true;

	ofAppPtr			= NULL;
	instance			= this;

	glVersionMinor=glVersionMajor=-1;
	nFramesSinceWindowResized = 0;
    
    //default to 4 times antialiasing. 
    setNumSamples(4);
	iconSet = false;

}


//------------------------------------------------------------
void ofAppGLFWWindow::setNumSamples(int _samples){
	samples=_samples;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setMultiDisplayFullscreen(bool bMultiFullscreen){
    bMultiWindowFullscreen = bMultiFullscreen; 
}

//------------------------------------------------------------
void ofAppGLFWWindow::setDoubleBuffering(bool doubleBuff){
	bDoubleBuffered = doubleBuff;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setColorBits(int r, int g, int b){
	rBits=r;
	gBits=g;
	bBits=b;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setAlphaBits(int a){
	aBits=a;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setDepthBits(int depth){
	depthBits=depth;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setStencilBits(int stencil){
	stencilBits=stencil;
}



//------------------------------------------------------------
void ofAppGLFWWindow::setOpenGLVersion(int major, int minor){
	glVersionMajor = major;
	glVersionMinor = minor;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setupOpenGL(int w, int h, int screenMode){

	requestedWidth = w;
	requestedHeight = h;


	if(!glfwInit( )){
		ofLogError("ofAppGLFWWindow") << "couldn't init GLFW";
		return;
	}

//	ofLogNotice("ofAppGLFWWindow") << "WINDOW MODE IS " << screenMode;

	int requestedMode = screenMode;

	glfwWindowHint(GLFW_RED_BITS, rBits);
	glfwWindowHint(GLFW_GREEN_BITS, gBits);
	glfwWindowHint(GLFW_BLUE_BITS, bBits);
	glfwWindowHint(GLFW_ALPHA_BITS, aBits);
	glfwWindowHint(GLFW_DEPTH_BITS, depthBits);
	glfwWindowHint(GLFW_STENCIL_BITS, stencilBits);
#ifdef TARGET_LINUX
	// start the window hidden so we can set the icon before it shows
	glfwWindowHint(GLFW_VISIBLE,GL_FALSE);
#endif
#ifndef TARGET_OSX
	glfwWindowHint(GLFW_AUX_BUFFERS,bDoubleBuffered?1:0);
#endif
	glfwWindowHint(GLFW_SAMPLES,samples);

	if(glVersionMinor!=-1 && glVersionMajor!=-1){
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
		if(glVersionMajor>=3){
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		}
		#ifdef TARGET_OPENGLES
		glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_ES_API);
		#endif
	}

	if(requestedMode==OF_GAME_MODE){
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		if(count>0){
			windowP = glfwCreateWindow(w, h, "", monitors[0], NULL);
		}else{
			ofLogError("ofAppGLFWWindow") << "couldn't find any monitors";
			return;
		}
	}else{
		windowP = glfwCreateWindow(w, h, "", NULL, NULL);
		if(!windowP){
			ofLogError("ofAppGLFWWindow") << "couldn't create GLFW window";
		}
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
				glfwShowWindow(windowP);
			}
		#endif
		if(requestedMode==OF_FULLSCREEN){
			setFullscreen(true);
		}
	}
    if(!windowP) {
        ofLogError("ofAppGLFWWindow") << "couldn't create window";
        return;
    }

	windowMode = requestedMode;

	setVerticalSync(false);
	// Set window title
//	glfwSetWindowTitle( " " );

//	glfwEnable( windowP, GLFW_KEY_REPEAT );

	requestedHeight = requestedHeight < 1 ? 1 : requestedHeight;
	glfwGetWindowSize( windowP, &requestedWidth, &requestedHeight );


	nonFullScreenW = w;
	nonFullScreenH = h;

    glfwMakeContextCurrent(windowP);

    glfwGetWindowSize(windowP, &windowW, &windowH );
    ofGLReadyCallback();

}

//--------------------------------------------
void ofAppGLFWWindow::exit_cb(GLFWwindow* windowP_){
	OF_EXIT_APP(0);
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
	glfwSetScrollCallback(windowP, scroll_cb);
	glfwSetDropCallback(windowP, drop_cb);

}

#ifdef TARGET_LINUX
//------------------------------------------------------------
void ofAppGLFWWindow::setWindowIcon(const string & path){
    ofPixels iconPixels;
	ofLoadImage(iconPixels,path);
	setWindowIcon(iconPixels);
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowIcon(const ofPixels & iconPixels){
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

	XChangeProperty(getX11Display(), getX11Window(), XInternAtom(getX11Display(), "_NET_WM_ICON", False), XA_CARDINAL, 32,
						 PropModeReplace,  (const unsigned char*)buffer,  length);
	XFlush(getX11Display());
}
#endif

//--------------------------------------------
void ofAppGLFWWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	ofAppPtr = appPtr;

	glfwMakeContextCurrent(windowP);

	ofNotifySetup();
	while(true){
		ofNotifyUpdate();
		display();
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::display(void){

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

	if ( bClearAuto == true ){
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
			if ( (ofGetFrameNum() < 3 || nFramesSinceWindowResized < 3) && bDoubleBuffered)    glfwSwapBuffers(windowP);
			else                                                     glFlush();
		}
	} else {
		if(bDoubleBuffered){
		    glfwSwapBuffers(windowP);
		} else {
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
		    glfwSwapBuffers(windowP);
		} else{
			glFlush();
		}
	#endif

	if(renderer){
		renderer->finishRender();
	}

	nFramesSinceWindowResized++;
	glfwPollEvents();

}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowTitle(string title){
	glfwSetWindowTitle(windowP,title.c_str());
}

//------------------------------------------------------------
ofPoint ofAppGLFWWindow::getWindowSize(){
	if(windowMode == OF_GAME_MODE)
	{
		const GLFWvidmode * desktopMode = glfwGetVideoMode(glfwGetWindowMonitor(windowP));
		if(desktopMode){
			return ofVec3f(desktopMode->width, desktopMode->height,0);
		}else{
			return ofPoint(windowW,windowH);
		}
	}else{
	    glfwGetWindowSize(windowP,&windowW,&windowH);
		return ofPoint(windowW,windowH);
	}
}

//------------------------------------------------------------
ofPoint ofAppGLFWWindow::getWindowPosition(){
    int x, y; 
	glfwGetWindowPos(windowP, &x, &y);
    
    if( windowMode == OF_WINDOW ){
        nonFullScreenX = x; 
        nonFullScreenY = y; 
    }   
    
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return ofPoint(x,y,0);
	}else{
		return ofPoint(x,y,0);
	}
}

//------------------------------------------------------------

int ofAppGLFWWindow::getCurrentMonitor(){
	int numberOfMonitors;
	GLFWmonitor** monitors = glfwGetMonitors(&numberOfMonitors);

	int xW;	int yW;
	glfwGetWindowPos(windowP, &xW, &yW);
	
	for (int iC=0; iC < numberOfMonitors; iC++){
		int xM; int yM;
		glfwGetMonitorPos(monitors[iC], &xM, &yM);
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[iC]);
		ofRectangle monitorRect(xM, yM, desktopMode->width, desktopMode->height);
		if (monitorRect.inside(xW, yW)){
			return iC;
			break;
		}
	}
	return 0;
}


//------------------------------------------------------------
ofPoint ofAppGLFWWindow::getScreenSize(){
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	if(count>0){
		int currentMonitor = getCurrentMonitor();
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[currentMonitor]);
		if(desktopMode){
			if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
				return ofVec3f(desktopMode->width, desktopMode->height,0);
			}else{
				return ofPoint(0,0);
			}
		}else{
			return ofPoint(0,0);
		}
	}else{
		return ofPoint(0,0);
	}
}

//------------------------------------------------------------
int ofAppGLFWWindow::getWidth(){
	if(windowMode == OF_GAME_MODE)
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

//------------------------------------------------------------
int ofAppGLFWWindow::getHeight()
{
	if(windowMode == OF_GAME_MODE)
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

//------------------------------------------------------------
int	ofAppGLFWWindow::getWindowMode(){
	return windowMode;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowPosition(int x, int y){
    glfwSetWindowPos(windowP,x,y);
    
    if( windowMode == OF_WINDOW ){
        nonFullScreenX=x;
        nonFullScreenY=y;
    }
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowShape(int w, int h){
	glfwSetWindowSize(windowP,w,h);
	// this is useful, esp if we are in the first frame (setup):
	requestedWidth  = w;
	requestedHeight = h;
}

//------------------------------------------------------------
void ofAppGLFWWindow::hideCursor(){
	glfwSetInputMode(windowP,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
};

//------------------------------------------------------------
void ofAppGLFWWindow::showCursor(){
	glfwSetInputMode(windowP,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
};

//------------------------------------------------------------
void ofAppGLFWWindow::enableSetupScreen(){
	bEnableSetupScreen = true;
};

//------------------------------------------------------------
void ofAppGLFWWindow::disableSetupScreen(){
	bEnableSetupScreen = false;
};

//------------------------------------------------------------
void ofAppGLFWWindow::setFullscreen(bool fullscreen){
 
    int curWindowMode  = windowMode;
 
  if (fullscreen){
		windowMode = OF_FULLSCREEN;
	}else{
		windowMode = OF_WINDOW;
    }
 
    //we only want to change window mode if the requested window is different to the current one.
    bool bChanged = windowMode != curWindowMode;
    if( !bChanged ){
        return;
    }
 
#ifdef TARGET_LINUX
#include <X11/Xatom.h>
 
    Window nativeWin = glfwGetX11Window(windowP);
	Display* display = glfwGetX11Display();
	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
 
	if( bMultiWindowFullscreen && monitorCount > 1 ){
		// find the monitors at the edges of the virtual desktop
		int minx=numeric_limits<int>::max();
		int miny=numeric_limits<int>::max();
		int maxx=numeric_limits<int>::min();
		int maxy=numeric_limits<int>::min();
		int x,y,w,h;
		int monitorLeft=0, monitorRight=0, monitorTop=0, monitorBottom=0;
        for(int i = 0; i < monitorCount; i++){
            glfwGetMonitorPos(monitors[i],&x,&y);
            glfwGetMonitorPhysicalSize(monitors[i],&w,&h);
            if(x<minx){
            	monitorLeft = i;
            	minx = x;
            }
            if(y<miny){
            	monitorTop = i;
            	miny = y;
            }
            if(x+w>maxx){
            	monitorRight = i;
            	maxx = x+w;
            }
            if(y+h>maxy){
            	monitorBottom = i;
            	maxy = y+h;
            }
 
        }
 
        // send fullscreen_monitors event with the edges monitors
		Atom m_net_fullscreen_monitors= XInternAtom(display, "_NET_WM_FULLSCREEN_MONITORS", false);
 
		XEvent xev;
 
		xev.xclient.type = ClientMessage;
		xev.xclient.serial = 0;
		xev.xclient.send_event = True;
		xev.xclient.window = nativeWin;
		xev.xclient.message_type = m_net_fullscreen_monitors;
		xev.xclient.format = 32;
 
		xev.xclient.data.l[0] = monitorTop;
		xev.xclient.data.l[1] = monitorBottom;
		xev.xclient.data.l[2] = monitorLeft;
		xev.xclient.data.l[3] = monitorRight;
		xev.xclient.data.l[4] = 1;
		XSendEvent(display, RootWindow(display, DefaultScreen(display)),
				   False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
 
	}
 
	// send fullscreen event
	Atom m_net_state= XInternAtom(display, "_NET_WM_STATE", false);
	Atom m_net_fullscreen= XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", false);
 
	XEvent xev;
 
	xev.xclient.type = ClientMessage;
	xev.xclient.serial = 0;
	xev.xclient.send_event = True;
	xev.xclient.window = nativeWin;
	xev.xclient.message_type = m_net_state;
	xev.xclient.format = 32;
 
	if (fullscreen)
		xev.xclient.data.l[0] = 1;
	else
		xev.xclient.data.l[0] = 0;
 
	xev.xclient.data.l[1] = m_net_fullscreen;
	xev.xclient.data.l[2] = 0;
	xev.xclient.data.l[3] = 0;
	xev.xclient.data.l[4] = 0;
	XSendEvent(display, RootWindow(display, DefaultScreen(display)),
			   False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
 
	// tell the window manager to bypass composition for this window in fullscreen for speed
	// it'll probably help solving vsync issues
	Atom m_bypass_compositor = XInternAtom(display, "_NET_WM_BYPASS_COMPOSITOR", False);
	unsigned long value = fullscreen ? 1 : 0;
	XChangeProperty(display, nativeWin, m_bypass_compositor, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&value, 1);
 
	XFlush(display);
 
#elif defined(TARGET_OSX)
	if( windowMode == OF_FULLSCREEN){
        nonFullScreenX = getWindowPosition().x;
        nonFullScreenY = getWindowPosition().y;
 
		nonFullScreenW = getWindowSize().x;
		nonFullScreenH = getWindowSize().y;
 
		//----------------------------------------------------
		SetSystemUIMode(kUIModeAllHidden,NULL);
		NSWindow * cocoaWindow = glfwGetCocoaWindow(windowP);
 
		[cocoaWindow setStyleMask:NSBorderlessWindowMask];
 
		int monitorCount;
        GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
 
		int currentMonitor = getCurrentMonitor();
		ofVec3f screenSize = getScreenSize();
 
		ofRectangle allScreensSpace;
 
        if( bMultiWindowFullscreen && monitorCount > 1 ){
 
			//calc the sum Rect of all the monitors
			for(int i = 0; i < monitorCount; i++){
				const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[i]);
				int x, y;
				glfwGetMonitorPos(monitors[i], &x, &y);
				ofRectangle screen = ofRectangle( x, y, desktopMode->width, desktopMode->height );
				allScreensSpace = allScreensSpace.getUnion(screen);
			}
			//for OS X we need to set this first as the window size affects the window positon
			setWindowShape(allScreensSpace.width, allScreensSpace.height);
			setWindowPosition(allScreensSpace.x, allScreensSpace.y);
 
        }else if (monitorCount > 1 && currentMonitor < monitorCount){
            int xpos;
			int ypos;
			glfwGetMonitorPos(monitors[currentMonitor], &xpos, &ypos);
 
            //we do this as setWindowShape affects the position of the monitor
            //normally we would just call setWindowShape first, but on multi monitor you see the window bleed onto the second monitor as it first changes shape and is then repositioned.
            //this first moves it over in X, does the screen resize and then by calling it again its set correctly in y.
			setWindowPosition(xpos, ypos);
            setWindowShape(screenSize.x, screenSize.y);
			setWindowPosition(xpos, ypos);
		}else{
            //for OS X we need to set this first as the window size affects the window positon
            setWindowShape(screenSize.x, screenSize.y);
			setWindowPosition(0,0);
		}
 
        //make sure the window is getting the mouse/key events
        [cocoaWindow makeFirstResponder:cocoaWindow.contentView];
 
	}else if( windowMode == OF_WINDOW ){
		SetSystemUIMode(kUIModeNormal,NULL);
		NSWindow * cocoaWindow = glfwGetCocoaWindow(windowP);
		[cocoaWindow setStyleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask];
 
		setWindowShape(nonFullScreenW, nonFullScreenH);
 
		//----------------------------------------------------
		// if we have recorded the screen posion, put it there
		// if not, better to let the system do it (and put it where it wants)
		if (ofGetFrameNum() > 0){
			setWindowPosition(nonFullScreenX,nonFullScreenY);
		}
 
		//----------------------------------------------------
        //make sure the window is getting the mouse/key events
        [cocoaWindow makeFirstResponder:cocoaWindow.contentView];
	}
#elif defined(TARGET_WIN32)
    if( windowMode == OF_FULLSCREEN){
        nonFullScreenX = getWindowPosition().x;
        nonFullScreenY = getWindowPosition().y;
		nonFullScreenW = getWindowSize().x;
		nonFullScreenH = getWindowSize().y;
 
		//----------------------------------------------------
		HWND hwnd = glfwGetWin32Window(windowP);
 
		SetWindowLong(hwnd, GWL_EXSTYLE, 0);
  		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
  		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
 
        float fullscreenW = getScreenSize().x;
        float fullscreenH = getScreenSize().y;
 
        int xpos = 0;
        int ypos = 0;
 
        if( bMultiWindowFullscreen ){
 
            float totalWidth = 0.0;
            float maxHeight  = 0.0;
            int monitorCount;
            GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
 
            //lets find the total width of all the monitors
            //and we'll make the window height the height of the largest monitor.
            for(int i = 0; i < monitorCount; i++){
                const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[i]);
                totalWidth += desktopMode->width;
                if( i == 0 || desktopMode->height > maxHeight ){
                    maxHeight = desktopMode->height;
                }
            }
 
            fullscreenW = totalWidth;
            fullscreenH = maxHeight;
        }else{
 
            int monitorCount;
            GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
            int currentMonitor = getCurrentMonitor();
            glfwGetMonitorPos(monitors[currentMonitor], &xpos, &ypos);
 
        }
 
        SetWindowPos(hwnd, HWND_TOPMOST, xpos, ypos, fullscreenW, fullscreenH, SWP_SHOWWINDOW);
 
	}else if( windowMode == OF_WINDOW ){
 
		HWND hwnd = glfwGetWin32Window(windowP);
 
  		DWORD EX_STYLE = WS_EX_OVERLAPPEDWINDOW;
		DWORD STYLE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
 
	  	ChangeDisplaySettings(0, 0);
		SetWindowLong(hwnd, GWL_EXSTYLE, EX_STYLE);
		SetWindowLong(hwnd, GWL_STYLE, STYLE);
  		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
 
		//not sure why this is - but if we don't do this the window shrinks by 4 pixels in x and y
		//should look for a better fix.
		setWindowPosition(nonFullScreenX-2, nonFullScreenY-2);
		setWindowShape(nonFullScreenW+4, nonFullScreenH+4);
	}
#endif
}

//------------------------------------------------------------
void ofAppGLFWWindow::toggleFullscreen(){
	if (windowMode == OF_GAME_MODE) return;


	if (windowMode == OF_WINDOW){
		setFullscreen(true);
	} else {
		setFullscreen(false);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::setOrientation(ofOrientation orientation){
	this->orientation = orientation;
}

//------------------------------------------------------------
ofOrientation ofAppGLFWWindow::getOrientation(){
	return orientation;
}

//------------------------------------------------------------
void ofAppGLFWWindow::exitApp(){
	ofLogVerbose("ofAppGLFWWindow") << "terminating GLFW app!";

	// Terminate GLFW
	glfwTerminate();

	OF_EXIT_APP(0);
}

//------------------------------------------------------------
static void rotateMouseXY(ofOrientation orientation, double &x, double &y) {
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
void ofAppGLFWWindow::mouse_cb(GLFWwindow* windowP_, int button, int state, int mods) {
	ofLogVerbose("ofAppGLFWWindow") << "mouse button: " << button;

#ifdef TARGET_OSX
    //we do this as unlike glut, glfw doesn't report right click for ctrl click or middle click for alt click 
    if( ofGetKeyPressed(OF_KEY_CONTROL) && button == GLFW_MOUSE_BUTTON_LEFT){
        button = GLFW_MOUSE_BUTTON_RIGHT; 
    }
    if( ofGetKeyPressed(OF_KEY_ALT) && button == GLFW_MOUSE_BUTTON_LEFT){
        button = GLFW_MOUSE_BUTTON_MIDDLE; 
    }
#endif

	switch(button){
	case GLFW_MOUSE_BUTTON_LEFT:
		button = OF_MOUSE_BUTTON_LEFT;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		button = OF_MOUSE_BUTTON_RIGHT;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		button = OF_MOUSE_BUTTON_MIDDLE;
		break;
	}

	if (state == GLFW_PRESS) {
		ofNotifyMousePressed(ofGetMouseX(), ofGetMouseY(), button);
		instance->buttonPressed=true;
	} else if (state == GLFW_RELEASE) {
		ofNotifyMouseReleased(ofGetMouseX(), ofGetMouseY(), button);
		instance->buttonPressed=false;
	}
	instance->buttonInUse = button;


}

//------------------------------------------------------------
void ofAppGLFWWindow::motion_cb(GLFWwindow* windowP_, double x, double y) {
	rotateMouseXY(ofGetOrientation(), x, y);

	if(!instance->buttonPressed){
		ofNotifyMouseMoved(x, y);
	}else{
		ofNotifyMouseDragged(x, y, instance->buttonInUse);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::scroll_cb(GLFWwindow* windowP_, double x, double y) {
	// ofSendMessage("scroll|"+ofToString(x,5) + "|" + ofToString(y,5));
}

//------------------------------------------------------------
void ofAppGLFWWindow::drop_cb(GLFWwindow* windowP_, const char* dropString) {
	string drop = dropString;
	ofDragInfo drag;
	drag.position.set(ofGetMouseX(),ofGetMouseY());
	drag.files = ofSplitString(drop,"\n",true);
#ifdef TARGET_LINUX
	for(int i=0; i<(int)drag.files.size(); i++){
		drag.files[i] = Poco::URI(drag.files[i]).getPath();
	}
#endif
	ofNotifyDragEvent(drag);
}

//------------------------------------------------------------
void ofAppGLFWWindow::keyboard_cb(GLFWwindow* windowP_, int key, int scancode, int action, int mods) {

	ofLogVerbose("ofAppGLFWWindow") << "key: " << key << " state: " << action;

	switch (key) {
		case GLFW_KEY_ESCAPE:
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
		case GLFW_KEY_PAGE_UP:
			key = OF_KEY_PAGE_UP;
			break;
		case GLFW_KEY_PAGE_DOWN:
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
		case GLFW_KEY_LEFT_SHIFT:
			key = OF_KEY_LEFT_SHIFT;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			key = OF_KEY_LEFT_CONTROL;
			break;
		case GLFW_KEY_LEFT_ALT:
			key = OF_KEY_LEFT_ALT;
			break;
		case GLFW_KEY_LEFT_SUPER:
			key = OF_KEY_LEFT_SUPER;
			break;
		case GLFW_KEY_RIGHT_SHIFT:
			key = OF_KEY_RIGHT_SHIFT;
			break;
		case GLFW_KEY_RIGHT_CONTROL:
			key = OF_KEY_RIGHT_CONTROL;
			break;
		case GLFW_KEY_RIGHT_ALT:
			key = OF_KEY_RIGHT_ALT;
			break;
		case GLFW_KEY_RIGHT_SUPER:
			key = OF_KEY_RIGHT_SUPER;
            break;
		case GLFW_KEY_BACKSPACE:
			key = OF_KEY_BACKSPACE;
			break;
		case GLFW_KEY_DELETE:
			key = OF_KEY_DEL;
			break;
		case GLFW_KEY_ENTER:
			key = OF_KEY_RETURN;
			break;
		case GLFW_KEY_KP_ENTER:
			key = OF_KEY_RETURN;
			break;   
		case GLFW_KEY_TAB:
			key = OF_KEY_TAB;
			break;   
		default:
			break;
	}

	//GLFW defaults to uppercase - OF users are used to lowercase
    //we look and see if shift is being held to toggle upper/lowecase 
	if( key >= 65 && key <= 90 && !ofGetKeyPressed(OF_KEY_SHIFT) ){
		key += 32;
	}

	if(action == GLFW_PRESS || action == GLFW_REPEAT){
		ofNotifyKeyPressed(key);
		if (key == OF_KEY_ESC){				// "escape"
			exitApp();
		}
	}else if (action == GLFW_RELEASE){
		ofNotifyKeyReleased(key);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::resize_cb(GLFWwindow* windowP_,int w, int h) {
	instance->windowW = w;
	instance->windowH = h;
	ofNotifyWindowResized(w, h);

	instance->nFramesSinceWindowResized = 0;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setVerticalSync(bool bVerticalSync){
	if(bVerticalSync){
		glfwSwapInterval( 1);
	}else{
		glfwSwapInterval(0);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::listVideoModes(){
	int numModes;
	const GLFWvidmode * vidModes = glfwGetVideoModes(NULL, &numModes );
	for(int i=0; i<numModes; i++){
		ofLogNotice() << vidModes[i].width << " x " << vidModes[i].height
		<< vidModes[i].redBits+vidModes[i].greenBits+vidModes[i].blueBits << "bit";
	}
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowIconified(){
	return glfwGetWindowAttrib(windowP, GLFW_ICONIFIED);
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowActive(){
//	return glfwGetWindowParam(GLFW_ACTIVE);
	return true;
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowResizeable(){
	return !glfwGetWindowAttrib(windowP, GLFW_RESIZABLE);
}

//------------------------------------------------------------
void ofAppGLFWWindow::iconify(bool bIconify){
	if(bIconify)
			glfwIconifyWindow(windowP);
	else
		glfwRestoreWindow(windowP);
}


#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)
Display* ofAppGLFWWindow::getX11Display(){
	return glfwGetX11Display();
}

Window ofAppGLFWWindow::getX11Window(){
	return glfwGetX11Window(windowP);
}
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
GLXContext ofAppGLFWWindow::getGLXContext(){
	return glfwGetGLXContext(windowP);
}
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
EGLDisplay ofAppGLFWWindow::getEGLDisplay(){
	return glfwGetEGLDisplay();
}

EGLContext ofAppGLFWWindow::getEGLContext(){
	return glfwGetEGLContext(windowP);
}

EGLSurface ofAppGLFWWindow::getEGLSurface(){
	return glfwGetEGLSurface(windowP);
}
#endif

#if defined(TARGET_OSX)
void * ofAppGLFWWindow::getNSGLContext(){
	return glfwGetNSGLContext(windowP);
}

void * ofAppGLFWWindow::getCocoaWindow(){
	return glfwGetCocoaWindow(windowP);
}
#endif

#if defined(TARGET_WIN32)
HGLRC ofAppGLFWWindow::getWGLContext(){
	return glfwGetWGLContext(windowP);
}

HWND ofAppGLFWWindow::getWin32Window(){
	return glfwGetWin32Window(windowP);
}

#endif
