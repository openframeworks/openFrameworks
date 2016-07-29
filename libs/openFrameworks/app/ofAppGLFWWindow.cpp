#include "ofAppGLFWWindow.h"
#include "ofEvents.h"

#include "ofBaseApp.h"
#include "ofGLRenderer.h"
#include "ofGLProgrammableRenderer.h"
#include "ofAppRunner.h"
#include "ofFileUtils.h"

#ifdef TARGET_LINUX
	#include "ofIcon.h"
	#include "ofImage.h"
	#define GLFW_EXPOSE_NATIVE_X11
	#ifndef TARGET_OPENGLES
		#define GLFW_EXPOSE_NATIVE_GLX
	#else
		#define GLFW_EXPOSE_NATIVE_EGL
	#endif
	#include <X11/extensions/Xrandr.h>
	#include "GLFW/glfw3native.h"
	#include <X11/Xatom.h>
	#include <xcb/xcb.h>
	#include <xcb/xcbext.h>
#elif defined(TARGET_OSX)
	#include <Cocoa/Cocoa.h>
	#define GLFW_EXPOSE_NATIVE_COCOA
	#define GLFW_EXPOSE_NATIVE_NSGL
	#include "GLFW/glfw3native.h"
#elif defined(TARGET_WIN32)
	#define GLFW_EXPOSE_NATIVE_WIN32
	#define GLFW_EXPOSE_NATIVE_WGL
	#include <GLFW/glfw3native.h>
#endif

//-------------------------------------------------------
ofAppGLFWWindow::ofAppGLFWWindow(){
	bEnableSetupScreen	= true;
	buttonInUse			= 0;
	buttonPressed		= false;
	bWindowNeedsShowing	= true;

	orientation 		= OF_ORIENTATION_DEFAULT;
	windowMode			= OF_WINDOW;

	ofAppPtr			= nullptr;

    pixelScreenCoordScale = 1;
	nFramesSinceWindowResized = 0;
	iconSet = false;
	windowP = nullptr;
	windowW = 0;
	windowH = 0;
	currentW = 0;
	currentH = 0;

	glfwSetErrorCallback(error_cb);
}

ofAppGLFWWindow::~ofAppGLFWWindow(){
	close();
}

void ofAppGLFWWindow::close(){
	if(windowP){
		glfwSetMouseButtonCallback( windowP, nullptr );
		glfwSetCursorPosCallback( windowP, nullptr );
		glfwSetCursorEnterCallback( windowP, nullptr );
		glfwSetKeyCallback( windowP, nullptr );
		glfwSetWindowSizeCallback( windowP, nullptr );
		glfwSetWindowCloseCallback( windowP, nullptr );
		glfwSetScrollCallback( windowP, nullptr );
		glfwSetDropCallback( windowP, nullptr );
		//hide the window before we destroy it stops a flicker on OS X on exit.
		glfwHideWindow(windowP);
		glfwDestroyWindow(windowP);
		windowP = nullptr;
		events().disable();
		bWindowNeedsShowing = true;
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::setNumSamples(int _samples){
	settings.numSamples=_samples;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setMultiDisplayFullscreen(bool bMultiFullscreen){
    settings.multiMonitorFullScreen = bMultiFullscreen;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setDoubleBuffering(bool doubleBuff){
	settings.doubleBuffering = doubleBuff;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setColorBits(int r, int g, int b){
	settings.redBits=r;
	settings.greenBits=g;
	settings.blueBits=b;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setAlphaBits(int a){
	settings.alphaBits=a;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setDepthBits(int depth){
	settings.depthBits=depth;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setStencilBits(int stencil){
	settings.stencilBits=stencil;
}

//------------------------------------------------------------
#ifdef TARGET_OPENGLES
void ofAppGLFWWindow::setup(const ofGLESWindowSettings & settings){
#else
void ofAppGLFWWindow::setup(const ofGLWindowSettings & settings){
#endif
	const ofGLFWWindowSettings * glSettings = dynamic_cast<const ofGLFWWindowSettings*>(&settings);
	if(glSettings){
		setup(*glSettings);
	}else{
		setup(ofGLFWWindowSettings(settings));
	}
}

void ofAppGLFWWindow::setup(const ofGLFWWindowSettings & _settings){
	if(windowP){
		ofLogError() << "window already setup, probably you are mixing old and new style setup";
		ofLogError() << "call only ofCreateWindow(settings) or ofSetupOpenGL(...)";
		ofLogError() << "calling window->setup() after ofCreateWindow() is not necesary and won't do anything";
		return;
	}
	settings = _settings;

	if(!glfwInit( )){
		ofLogError("ofAppGLFWWindow") << "couldn't init GLFW";
		return;
	}

//	ofLogNotice("ofAppGLFWWindow") << "WINDOW MODE IS " << screenMode;

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RED_BITS, settings.redBits);
	glfwWindowHint(GLFW_GREEN_BITS, settings.greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, settings.blueBits);
	glfwWindowHint(GLFW_ALPHA_BITS, settings.alphaBits);
	glfwWindowHint(GLFW_DEPTH_BITS, settings.depthBits);
	glfwWindowHint(GLFW_STENCIL_BITS, settings.stencilBits);
	glfwWindowHint(GLFW_STEREO, settings.stereo);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
#ifndef TARGET_OSX
	glfwWindowHint(GLFW_AUX_BUFFERS, settings.doubleBuffering?1:0);
#endif
	glfwWindowHint(GLFW_SAMPLES, settings.numSamples);
	glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
	glfwWindowHint(GLFW_DECORATED, settings.decorated);
	#ifdef TARGET_OPENGLES
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.glesVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		if(settings.glesVersion>=2){
			currentRenderer = shared_ptr<ofBaseRenderer>(new ofGLProgrammableRenderer(this));
		}else{
			currentRenderer = shared_ptr<ofBaseRenderer>(new ofGLRenderer(this));
		}
	#else
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.glVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.glVersionMinor);
		if((settings.glVersionMajor==3 && settings.glVersionMinor>=2) || settings.glVersionMajor>=4){
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}
		if(settings.glVersionMajor>=3){
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			currentRenderer = shared_ptr<ofBaseRenderer>(new ofGLProgrammableRenderer(this));
		}else{
			currentRenderer = shared_ptr<ofBaseRenderer>(new ofGLRenderer(this));
		}
	#endif

	GLFWwindow * sharedContext = nullptr;
	if(settings.shareContextWith){
		sharedContext = (GLFWwindow*)settings.shareContextWith->getWindowContext();
	}

	if(settings.windowMode==OF_GAME_MODE){
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		if(count>settings.monitor){
			windowP = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), monitors[settings.monitor], sharedContext);
		}else{
			ofLogError("ofAppGLFWWindow") << "couldn't find any monitors";
			return;
		}
	}else{
		windowP = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, sharedContext);
		if(!windowP){
			ofLogError("ofAppGLFWWindow") << "couldn't create GLFW window";
			return;
		}
		if(settings.windowMode==OF_FULLSCREEN){
			if(!settings.isPositionSet()){
				int count = 0;
				auto monitors = glfwGetMonitors(&count);
				if(count > 0){
					int x = 0, y = 0;
					settings.monitor = ofClamp(settings.monitor,0,count-1);
					glfwGetMonitorPos(monitors[settings.monitor],&x,&y);
					settings.setPosition(glm::vec2(x,y));
					setWindowPosition(settings.getPosition().x,settings.getPosition().y);
					#ifdef TARGET_OSX
						//for OS X we need to set this first as the window size affects the window positon
						auto mode = glfwGetVideoMode(monitors[settings.monitor]);
						settings.width = mode->width;
						settings.height = mode->height;
						setWindowShape(settings.width, settings.height);
					#endif
					setWindowPosition(settings.getPosition().x,settings.getPosition().y);
				}
			}else{
				setWindowPosition(settings.getPosition().x,settings.getPosition().y);
				#ifdef TARGET_OSX
					auto size = getScreenSize();
					settings.width = size.x;
					settings.height = size.y;
					setWindowShape(settings.width, settings.height);
				#endif
			}
		}else{
			if (settings.isPositionSet()) {
				setWindowPosition(settings.getPosition().x,settings.getPosition().y);
			}
		}
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
		if(settings.iconified){
			iconify(true);
		}
	}
	
	//don't try and show a window if its been requsted to be hidden
	bWindowNeedsShowing = settings.visible;

	glfwSetWindowUserPointer(windowP,this);

	glfwGetWindowSize( windowP, &settings.width, &settings.height );

    glfwMakeContextCurrent(windowP);

	glfwGetWindowSize(windowP, &currentW, &currentH );
	windowW = settings.width;
	windowH = settings.height;

    int framebufferW, framebufferH;
    glfwGetFramebufferSize(windowP, &framebufferW, &framebufferH);
    
    //this lets us detect if the window is running in a retina mode
    if( framebufferW != windowW ){
        pixelScreenCoordScale = framebufferW / windowW;
		
		auto position = getWindowPosition();
		setWindowShape(windowW, windowH);
		setWindowPosition(position.x, position.y);
	}

#ifndef TARGET_OPENGLES
    static bool inited = false;
    if(!inited){
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			ofLogError("ofAppRunner") << "couldn't init GLEW: " << glewGetErrorString(err);
			return;
		}
		inited = true;
    }
#endif

    ofLogVerbose() << "GL Version:" << glGetString(GL_VERSION);

    if(currentRenderer->getType()==ofGLProgrammableRenderer::TYPE){
#ifndef TARGET_OPENGLES
    	static_cast<ofGLProgrammableRenderer*>(currentRenderer.get())->setup(settings.glVersionMajor,settings.glVersionMinor);
#else
    	static_cast<ofGLProgrammableRenderer*>(currentRenderer.get())->setup(settings.glesVersion,0);
#endif
    }else{
    	static_cast<ofGLRenderer*>(currentRenderer.get())->setup();
    }

    setVerticalSync(true);
	glfwSetMouseButtonCallback(windowP, mouse_cb);
	glfwSetCursorPosCallback(windowP, motion_cb);
	glfwSetCursorEnterCallback(windowP, entry_cb);
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
	vector<unsigned long> buffer(length);
	buffer[0]=iconPixels.getWidth();
	buffer[1]=iconPixels.getHeight();
	for(int i=0;i<iconPixels.getWidth()*iconPixels.getHeight();i++){
		buffer[i+2]  = iconPixels[i*4+3]<<24;
		buffer[i+2] += iconPixels[i*4+0]<<16;
		buffer[i+2] += iconPixels[i*4+1]<<8;
		buffer[i+2] += iconPixels[i*4+2];
	}

	XChangeProperty(getX11Display(), getX11Window(), XInternAtom(getX11Display(), "_NET_WM_ICON", False), XA_CARDINAL, 32,
						 PropModeReplace,  (const unsigned char*)buffer.data(),  length);
	XFlush(getX11Display());
}
#endif

//--------------------------------------------
ofCoreEvents & ofAppGLFWWindow::events(){
	return coreEvents;
}

//--------------------------------------------
shared_ptr<ofBaseRenderer> & ofAppGLFWWindow::renderer(){
	return currentRenderer;
}

//--------------------------------------------
void ofAppGLFWWindow::update(){
	events().notifyUpdate();
	
	//show the window right before the first draw call.
	if( bWindowNeedsShowing && windowP ){
		glfwShowWindow(windowP);
		bWindowNeedsShowing = false;
		if(settings.windowMode==OF_FULLSCREEN){
			setFullscreen(true);
		}
	}
}

//--------------------------------------------
void ofAppGLFWWindow::draw(){
	currentRenderer->startRender();
	if( bEnableSetupScreen ) currentRenderer->setupScreen();

	events().notifyDraw();

	#ifdef TARGET_WIN32
	if (currentRenderer->getBackgroundAuto() == false){
		// on a PC resizing a window with this method of accumulation (essentially single buffering)
		// is BAD, so we clear on resize events.
		if (nFramesSinceWindowResized < 3){
			currentRenderer->clear();
		} else {
			if ( (events().getFrameNum() < 3 || nFramesSinceWindowResized < 3) && settings.doubleBuffering){
				glfwSwapBuffers(windowP);
			}else{
				glFlush();
			}
		}
	} else {
		if(settings.doubleBuffering){
		    glfwSwapBuffers(windowP);
		} else {
			glFlush();
		}
	}
	#else
		if (currentRenderer->getBackgroundAuto() == false){
			// in accum mode resizing a window is BAD, so we clear on resize events.
			if (nFramesSinceWindowResized < 3){
				currentRenderer->clear();
			}
		}
		if(settings.doubleBuffering){
		    glfwSwapBuffers(windowP);
		} else{
			glFlush();
		}
	#endif

	currentRenderer->finishRender();

	nFramesSinceWindowResized++;
}


//--------------------------------------------
void ofAppGLFWWindow::swapBuffers() {
	glfwSwapBuffers(windowP);
}

//--------------------------------------------
void ofAppGLFWWindow::startRender() {
	renderer()->startRender();
}

//--------------------------------------------
void ofAppGLFWWindow::finishRender() {
	renderer()->finishRender();
}

//--------------------------------------------
bool ofAppGLFWWindow::getWindowShouldClose(){
	return glfwWindowShouldClose(windowP);
}

//--------------------------------------------
void ofAppGLFWWindow::setWindowShouldClose(){
	glfwSetWindowShouldClose(windowP,1);
	events().notifyExit();
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowTitle(string title){
	glfwSetWindowTitle(windowP,title.c_str());
}

//------------------------------------------------------------
int ofAppGLFWWindow::getPixelScreenCoordScale(){
    return pixelScreenCoordScale;
}

//------------------------------------------------------------
glm::vec2 ofAppGLFWWindow::getWindowSize(){
	if(windowMode == OF_GAME_MODE)
	{
		const GLFWvidmode * desktopMode = glfwGetVideoMode(glfwGetWindowMonitor(windowP));
		if(desktopMode){
			return {desktopMode->width*pixelScreenCoordScale, desktopMode->height*pixelScreenCoordScale};
		}else{
			return {currentW*pixelScreenCoordScale, currentH*pixelScreenCoordScale};
		}
	}else{
		return {currentW*pixelScreenCoordScale, currentH*pixelScreenCoordScale};
	}
}

//------------------------------------------------------------
glm::vec2 ofAppGLFWWindow::getWindowPosition(){
	int x, y;
	glfwGetWindowPos(windowP, &x, &y);
    
    x *= pixelScreenCoordScale;
    y *= pixelScreenCoordScale;

	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return glm::vec2{x,y};
	}else{
		return glm::vec2(x,y); //NOTE: shouldn't this be (y,x) ??????
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
		bool bPointMatch = xW >= monitorRect.getMinX() && yW >= monitorRect.getMinY() && xW < monitorRect.getMaxX() && yW < monitorRect.getMaxY();
		//		if (monitorRect.inside(xW, yW)){
		if( bPointMatch ) {
			return iC;
			break;
		}
	}
	return 0;
}


//------------------------------------------------------------
glm::vec2 ofAppGLFWWindow::getScreenSize(){
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	if(count>0){
		int currentMonitor = getCurrentMonitor();
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[currentMonitor]);
		if(desktopMode){
			if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
				return {desktopMode->width*pixelScreenCoordScale, desktopMode->height*pixelScreenCoordScale};
			}else{
				return {desktopMode->height*pixelScreenCoordScale, desktopMode->width*pixelScreenCoordScale};
			}
		}
	}
	return glm::vec2();
}

//------------------------------------------------------------
int ofAppGLFWWindow::getWidth(){
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return currentW * pixelScreenCoordScale;
	}else{
		return currentH * pixelScreenCoordScale;
	}
}

//------------------------------------------------------------
int ofAppGLFWWindow::getHeight(){
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return currentH * pixelScreenCoordScale;
	}else{
		return currentW * pixelScreenCoordScale;
	}
}

//------------------------------------------------------------
GLFWwindow* ofAppGLFWWindow::getGLFWWindow(){
    return windowP;
}

//------------------------------------------------------------
ofWindowMode ofAppGLFWWindow::getWindowMode(){
	return windowMode;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowPosition(int x, int y){
    glfwSetWindowPos(windowP,x/pixelScreenCoordScale,y/pixelScreenCoordScale);
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowShape(int w, int h){
	if(windowMode == OF_WINDOW){
		windowW = w;
		windowH = h;
	}
	currentW = w/pixelScreenCoordScale;
	currentH = h/pixelScreenCoordScale;
	
	#ifdef TARGET_OSX
		auto pos = getWindowPosition();
		glfwSetWindowSize(windowP,currentW,currentH);
		if( pos != getWindowPosition() ){
			setWindowPosition(pos.x, pos.y);
		}
	#else
		glfwSetWindowSize(windowP,currentW,currentH);
	#endif
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
 
	ofWindowMode curWindowMode = windowMode;
 
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
	if(windowMode==OF_FULLSCREEN){
		int monitorCount;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
		if( settings.multiMonitorFullScreen && monitorCount > 1 ){
			// find the monitors at the edges of the virtual desktop
			int minx=numeric_limits<int>::max();
			int miny=numeric_limits<int>::max();
			int maxx=numeric_limits<int>::min();
			int maxy=numeric_limits<int>::min();
			int x,y,w,h;
			int monitorLeft=0, monitorRight=0, monitorTop=0, monitorBottom=0;
			for(int i = 0; i < monitorCount; i++){
				glfwGetMonitorPos(monitors[i],&x,&y);
				auto videoMode = glfwGetVideoMode(monitors[i]);
				w = videoMode->width;
				h = videoMode->height;
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
			currentW = maxx - minx;
			currentH = maxy - minx;
		}else{
			auto monitor = glfwGetWindowMonitor(windowP);
			if(monitor){
				auto videoMode = glfwGetVideoMode(monitor);
				if(videoMode){
					currentW = videoMode->width;
					currentH = videoMode->height;
				}
			}
		}
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
	setWindowShape(windowW, windowH);
 
#elif defined(TARGET_OSX)
	if( windowMode == OF_FULLSCREEN){
		//----------------------------------------------------
		[NSApp setPresentationOptions:NSApplicationPresentationHideMenuBar | NSApplicationPresentationHideDock];
		NSWindow * cocoaWindow = glfwGetCocoaWindow(windowP);
 
		[cocoaWindow setStyleMask:NSBorderlessWindowMask];
 
		int monitorCount;
        GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
 
		int currentMonitor = getCurrentMonitor();
		ofVec3f screenSize = getScreenSize();
		
		if( orientation == OF_ORIENTATION_90_LEFT || orientation == OF_ORIENTATION_90_RIGHT ){
			std::swap(screenSize.x, screenSize.y);
		}
		
		ofRectangle allScreensSpace;
		
		// save window shape before going fullscreen
		auto pos = getWindowPosition();
		windowRect.x = pos.x;
		windowRect.y = pos.y;
		windowRect.width = windowW;
		windowRect.height = windowH;
		
        if( settings.multiMonitorFullScreen && monitorCount > 1 ){
 
			//calc the sum Rect of all the monitors
			for(int i = 0; i < monitorCount; i++){
				const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[i]);
				int x, y;
				glfwGetMonitorPos(monitors[i], &x, &y);
				ofRectangle screen = ofRectangle( x, y, desktopMode->width, desktopMode->height );
				allScreensSpace = allScreensSpace.getUnion(screen);
			}
			//for OS X we need to set this first as the window size affects the window positon
			//need to account for the pixel density factor when we're getting the values from glfw
			setWindowShape(allScreensSpace.width*pixelScreenCoordScale, allScreensSpace.height*pixelScreenCoordScale);
			setWindowPosition(allScreensSpace.x, allScreensSpace.y);
 
        }else if (monitorCount > 1 && currentMonitor < monitorCount){
            int xpos;
			int ypos;
			glfwGetMonitorPos(monitors[currentMonitor], &xpos, &ypos);

			// Scale (if needed) to physical pixels size, since setWindowPosition
			// uses physical pixel dimensions. On HIDPI screens pixelScreenCoordScale
			// is likely to be 2, on "normal" screens pixelScreenCoordScale will be 1:
			xpos *= pixelScreenCoordScale;
			ypos *= pixelScreenCoordScale;
			
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
		
		// make sure to save current pos if not specified in settings
		if( settings.isPositionSet() ) {
			ofVec3f pos = getWindowPosition();
			settings.setPosition(ofVec2f(pos.x, pos.y));
		}
 
        //make sure the window is getting the mouse/key events
        [cocoaWindow makeFirstResponder:cocoaWindow.contentView];
 
	}else if( windowMode == OF_WINDOW ){
		
		// set window shape if started in fullscreen
		if(windowRect.width == 0 && windowRect.height == 0) {
			windowRect.x = getWindowPosition().x;
			windowRect.y = getWindowPosition().y;
			windowRect.width = getWindowSize().x;
			windowRect.height = getWindowSize().y;
		}
        
		[NSApp setPresentationOptions:NSApplicationPresentationDefault];
		NSWindow * cocoaWindow = glfwGetCocoaWindow(windowP);
		[cocoaWindow setStyleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask];
 
		setWindowShape(windowRect.width, windowRect.height);
 
		//----------------------------------------------------
		// if we have recorded the screen posion, put it there
		// if not, better to let the system do it (and put it where it wants)
		if (ofGetFrameNum() > 0){
			setWindowPosition(windowRect.x, windowRect.y);
		}
 
		//----------------------------------------------------
        //make sure the window is getting the mouse/key events
        [cocoaWindow makeFirstResponder:cocoaWindow.contentView];
	}
#elif defined(TARGET_WIN32)
    if( windowMode == OF_FULLSCREEN){
		// save window shape before going fullscreen
		auto pos = getWindowPosition();
		windowRect.x = pos.x;
		windowRect.y = pos.y;
		windowRect.width = windowW;
		windowRect.height = windowH;
 
		//----------------------------------------------------
		HWND hwnd = glfwGetWin32Window(windowP);
 
		SetWindowLong(hwnd, GWL_EXSTYLE, 0);
  		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
  		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
 
        float fullscreenW = getScreenSize().x;
        float fullscreenH = getScreenSize().y;
		
		if( orientation == OF_ORIENTATION_90_LEFT || orientation == OF_ORIENTATION_90_RIGHT ){
			std::swap(fullscreenW, fullscreenH);
		}
 
        int xpos = 0;
        int ypos = 0;
 
        if( settings.multiMonitorFullScreen ){

			int minX = 0;
			int maxX = 0;
			int minY = 0;
			int maxY = 0;
			int monitorCount;
			GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
			int tempXPos = 0;
			int tempYPos = 0;
			//lets find the total width of all the monitors
			//and we'll make the window height the height of the largest monitor.
			for(int i = 0; i < monitorCount; i++){
				const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[i]);
				glfwGetMonitorPos(monitors[i], &tempXPos, &tempYPos);
				minX = min(tempXPos,minX);
				minY = min(tempYPos,minY);
				maxX = max(maxX,tempXPos + desktopMode->width);
				maxY = max(maxY,tempYPos + desktopMode->height);

				xpos = min(xpos,tempXPos);
				ypos = min(ypos,tempYPos);
			}

			fullscreenW = maxX-minX;
			fullscreenH = maxY-minY;
        }else{
 
            int monitorCount;
            GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
            int currentMonitor = getCurrentMonitor();
            glfwGetMonitorPos(monitors[currentMonitor], &xpos, &ypos);
 
        }
 
		SetWindowPos(hwnd, HWND_TOPMOST, xpos, ypos, fullscreenW, fullscreenH, SWP_SHOWWINDOW);
		currentW = fullscreenW;
		currentH = fullscreenH;
 
	}else if( windowMode == OF_WINDOW ){
		// set window shape if started in fullscreen
		if(windowRect.width == 0 && windowRect.height == 0) {
			windowRect.x = getWindowPosition().x;
			windowRect.y = getWindowPosition().y;
			windowRect.width = getWindowSize().x;
			windowRect.height = getWindowSize().y;
		}

		HWND hwnd = glfwGetWin32Window(windowP);
 
  		DWORD EX_STYLE = WS_EX_OVERLAPPEDWINDOW;
		DWORD STYLE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_SIZEBOX;
 
	  	ChangeDisplaySettings(0, 0);
		SetWindowLong(hwnd, GWL_EXSTYLE, EX_STYLE);
		SetWindowLong(hwnd, GWL_STYLE, STYLE);
  		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
 
		//not sure why this is - but if we don't do this the window shrinks by 4 pixels in x and y
		//should look for a better fix.
		setWindowPosition(windowRect.x-2, windowRect.y-2);
		setWindowShape(windowRect.width+4, windowRect.height+4);
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
static void rotateMouseXY(ofOrientation orientation, int w, int h, double &x, double &y) {
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
ofAppGLFWWindow * ofAppGLFWWindow::setCurrent(GLFWwindow* windowP){
	ofAppGLFWWindow * instance = static_cast<ofAppGLFWWindow *>(glfwGetWindowUserPointer(windowP));
	shared_ptr<ofMainLoop> mainLoop = ofGetMainLoop();
	if(mainLoop){
		mainLoop->setCurrentWindow(instance);
	}
	instance->makeCurrent();
	return instance;
}

//------------------------------------------------------------
void ofAppGLFWWindow::mouse_cb(GLFWwindow* windowP_, int button, int state, int mods) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);
#ifdef TARGET_OSX
    //we do this as unlike glut, glfw doesn't report right click for ctrl click or middle click for alt click 
    if( instance->events().getKeyPressed(OF_KEY_CONTROL) && button == GLFW_MOUSE_BUTTON_LEFT){
        button = GLFW_MOUSE_BUTTON_RIGHT; 
    }
    if( instance->events().getKeyPressed(OF_KEY_ALT) && button == GLFW_MOUSE_BUTTON_LEFT){
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
		instance->events().notifyMousePressed(instance->events().getMouseX(), instance->events().getMouseY(), button);
		instance->buttonPressed=true;
	} else if (state == GLFW_RELEASE) {
		instance->events().notifyMouseReleased(instance->events().getMouseX(), instance->events().getMouseY(), button);
		instance->buttonPressed=false;
	}
	instance->buttonInUse = button;


}

//------------------------------------------------------------
void ofAppGLFWWindow::motion_cb(GLFWwindow* windowP_, double x, double y) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	rotateMouseXY(instance->orientation, instance->getWidth(), instance->getHeight(), x, y);

	if(!instance->buttonPressed){
		instance->events().notifyMouseMoved(x*instance->pixelScreenCoordScale, y*instance->pixelScreenCoordScale);
	}else{
		instance->events().notifyMouseDragged(x*instance->pixelScreenCoordScale, y*instance->pixelScreenCoordScale, instance->buttonInUse);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::entry_cb(GLFWwindow *windowP_, int entered) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	if(entered){
		instance->events().notifyMouseEntered(instance->events().getMouseX(), instance->events().getMouseY());
	}else{
		instance->events().notifyMouseExited(instance->events().getMouseX(), instance->events().getMouseY());
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::scroll_cb(GLFWwindow* windowP_, double x, double y) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	rotateMouseXY(instance->orientation, instance->getWidth(), instance->getHeight(), x, y);
	instance->events().notifyMouseScrolled(instance->events().getMouseX(), instance->events().getMouseY(), x, y);
}

//------------------------------------------------------------
void ofAppGLFWWindow::drop_cb(GLFWwindow* windowP_, int numFiles, const char** dropString) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	ofDragInfo drag;
	drag.position = {instance->events().getMouseX(), instance->events().getMouseY()};
	drag.files.resize(numFiles);
	for(int i=0; i<(int)drag.files.size(); i++){
		drag.files[i] = std::filesystem::path(dropString[i]).string();
	}
	instance->events().notifyDragEvent(drag);
}

//------------------------------------------------------------
void ofAppGLFWWindow::error_cb(int errorCode, const char* errorDescription){
	ofLogError("ofAppGLFWWindow") << errorCode << ": " << errorDescription;
}

//------------------------------------------------------------

unsigned long keycodeToUnicode(ofAppGLFWWindow * window, int keycode, int scancode, int modifier){
#ifdef TARGET_LINUX
	return XKeycodeToKeysym(window->getX11Display(), scancode, modifier);
#endif
#ifdef TARGET_WIN32
	static WCHAR buf[2];
	static BYTE keyboardState[256];
	GetKeyboardState( keyboardState );

	// Careful: keycode arrives translated into GLFW key codes,
	// but keycode needs to be a virtual key (VK_...) so we're 
	// in deep troble, since this information has been removed
	// by GLFW...
	//
	// The way around this is to ask the operating system
	// nicely to create a virtual key for us, based on 
	// the scancode and the currently bound keyboard layout.
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646306(v=vs.85).aspx
	//
	// create a "fake" virtual key 
	
	UINT fakeVirtualKey = MapVirtualKey( scancode, MAPVK_VSC_TO_VK_EX );

	int ret = ToUnicode( fakeVirtualKey, scancode, keyboardState, buf , 2, 0);

	if ( ret == 1 ){
		return buf[0]; 
	} else {
		return 0;
	}
#endif
#ifdef TARGET_OSX
	static UInt32 deadKeyState = 0;
	static UniChar characters[8];
	static UniCharCount characterCount = 0;

	typedef struct __TISInputSource*  TISInputSourceRef;
	typedef TISInputSourceRef (*pFnGetInputSource)(void); // define function pointer that may return a input source ref, no arguments
	typedef void* (*pFnGetInputSourceProperty)(TISInputSourceRef,CFStringRef);
	typedef UInt8 (*pFnGetKeyboardType)(void);
	
	static const CFBundleRef tisBundle = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.HIToolbox"));
	
	// We need to call some system methods, following GLFW's example
	// in their OS X version of ```_glfwPlatformGetKeyName```.
	//
	// We know these methods must be available, since GLFW uses them
	// internally.
	//
	// The most important method is ```UCKeyTranslate``` - everything
	// else here is just a royal preparation party to feed it with the
	// correct parameters.
	//
	// Since these methods are hidden deep within Carbon,
	// we have to first request function pointers to make
	// them callable.
	//
	// We do this only the first time, then we're re-using them,
	// that's why these elements are marked static, and static const.
	//
	static pFnGetInputSource         getInputSource         = (pFnGetInputSource)CFBundleGetFunctionPointerForName(tisBundle, CFSTR("TISCopyCurrentKeyboardLayoutInputSource"));
	static pFnGetKeyboardType        getKeyboardType        = (pFnGetKeyboardType)CFBundleGetFunctionPointerForName(tisBundle,CFSTR("LMGetKbdType"));
	static pFnGetInputSourceProperty getInputSourceProperty = (pFnGetInputSourceProperty)CFBundleGetFunctionPointerForName(tisBundle, CFSTR("TISGetInputSourceProperty"));
	
	static const TISInputSourceRef sourceRef = getInputSource(); // note that for the first time, this creates a copy on the heap, then we're re-using it.
	
	static const CFStringRef* kPropertyUnicodeKeyLayoutData = (CFStringRef*)CFBundleGetDataPointerForName(tisBundle, CFSTR("kTISPropertyUnicodeKeyLayoutData"));
	static const CFStringRef kTISPropertyUnicodeKeyLayoutData = * kPropertyUnicodeKeyLayoutData;
	static const CFDataRef UnicodeKeyLayoutData = (CFDataRef)getInputSourceProperty(sourceRef, kTISPropertyUnicodeKeyLayoutData);
	
	static const UCKeyboardLayout* pKeyboardLayout = (UCKeyboardLayout*)CFDataGetBytePtr(UnicodeKeyLayoutData);
	
	UInt32 mod_OSX = 0;
	{
		// We have to translate the GLFW modifier bitflags back to OS X,
		// so that SHIFT, CONTROL, etc can be taken into account when
		// calculating the unicode codepoint.
		
		if (modifier & GLFW_MOD_SHIFT)
			mod_OSX |= NSShiftKeyMask;
		if (modifier & GLFW_MOD_CONTROL)
			mod_OSX |= NSControlKeyMask;
		if (modifier & GLFW_MOD_ALT)
			mod_OSX |= NSAlternateKeyMask;
		if (modifier & GLFW_MOD_SUPER)
			mod_OSX |= NSCommandKeyMask;
	
		// This is really weird, but although OSX documentation says to do to the following:
		//		modifierKeyState = ((EventRecord.modifiers) >> 8) & 0xFF;
		// Bit-shifting by 16 bit seems to be necessary...
		//
		// (Tested using an Austrian Mac Keyboard).
		mod_OSX = (mod_OSX >> 16) & 0xFF;
	}
	
	// All this yak shaving was necessary to feed this diva of a function call:
	// https://developer.apple.com/library/mac/documentation/Carbon/Reference/Unicode_Utilities_Ref/index.html#//apple_ref/c/func/UCKeyTranslate
	
	if (noErr == UCKeyTranslate(pKeyboardLayout,
					   scancode,
					   kUCKeyActionDisplay,
					   mod_OSX,
					   getKeyboardType(),
					   kUCKeyTranslateNoDeadKeysBit,
					   &deadKeyState,
					   sizeof(characters) / sizeof(characters[0]),
					   &characterCount,
					   characters))
	{
		// if successful, first character contains codepoint
		return characters[0];
	} else {
		return 0;
	}
	
#endif
	return 0;
}

//------------------------------------------------------------
void ofAppGLFWWindow::keyboard_cb(GLFWwindow* windowP_, int keycode, int scancode, int action, int mods) {
	int key = 0;
	uint32_t codepoint = 0;
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	switch (keycode) {
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
			codepoint = keycodeToUnicode(instance, keycode, scancode, mods);
			key = codepoint;
			break;
	}

	if(action == GLFW_PRESS || action == GLFW_REPEAT){
		instance->events().notifyKeyPressed(key,keycode,scancode,codepoint);
	}else if (action == GLFW_RELEASE){
		instance->events().notifyKeyReleased(key,keycode,scancode,codepoint);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::resize_cb(GLFWwindow* windowP_,int w, int h) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	if(instance->windowMode == OF_WINDOW){
		instance->windowW = w * instance->pixelScreenCoordScale;
		instance->windowH = h * instance->pixelScreenCoordScale;
	}
	instance->currentW = w;
	instance->currentH = h;
	instance->events().notifyWindowResized(w*instance->pixelScreenCoordScale, h*instance->pixelScreenCoordScale);
	instance->nFramesSinceWindowResized = 0;
}

//--------------------------------------------
void ofAppGLFWWindow::exit_cb(GLFWwindow* windowP_){
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	instance->events().notifyExit();
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
void ofAppGLFWWindow::setClipboardString(const string& text) {
    glfwSetClipboardString(ofAppGLFWWindow::windowP, text.c_str());
}

//------------------------------------------------------------
string ofAppGLFWWindow::getClipboardString() {
	const char* clipboard = glfwGetClipboardString(ofAppGLFWWindow::windowP);

	if (clipboard) {
		return clipboard;
	} else {
		return "";
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::listVideoModes(){
	glfwInit();
	int numModes;
	const GLFWvidmode * vidModes = glfwGetVideoModes(nullptr, &numModes );
	for(int i=0; i<numModes; i++){
		ofLogNotice() << vidModes[i].width << " x " << vidModes[i].height
		<< vidModes[i].redBits+vidModes[i].greenBits+vidModes[i].blueBits << "bit";
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::listMonitors(){
	glfwInit();
	int count;
	const auto monitors = glfwGetMonitors(&count);
	for(int i = 0; i<count; i++){
		auto monitor = monitors[i];
		int w,h,x,y;
		glfwGetMonitorPhysicalSize(monitor,&w,&h);
		glfwGetMonitorPos(monitor,&x,&y);
		ofLogNotice() << i << ": " << glfwGetMonitorName(monitor) << ", physical size: " << w << "x" << h << "mm at " << x << ", " << y;
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



void ofAppGLFWWindow::makeCurrent(){
	glfwMakeContextCurrent(windowP);
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
