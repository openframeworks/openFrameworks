#include "ofAppGLFWWindow.h"

#if defined(TARGET_GLFW_WINDOW)
#include "ofGLProgrammableRenderer.h"
#include "ofGLRenderer.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef TARGET_LINUX
	#include "ofIcon.h"
	#include "ofImage.h"
	#define GLFW_EXPOSE_NATIVE_X11
	#ifndef TARGET_OPENGLES
		#define GLFW_EXPOSE_NATIVE_GLX
	#else
		#define GLFW_EXPOSE_NATIVE_EGL
	#endif
	#include <X11/XKBlib.h>
	#include <X11/Xatom.h>
	#include <X11/extensions/Xrandr.h>
	#include <xcb/xcb.h>
	#include <xcb/xcbext.h>


#elif defined(TARGET_OSX)
	#include <Cocoa/Cocoa.h>
	#define GLFW_EXPOSE_NATIVE_COCOA
	#define GLFW_EXPOSE_NATIVE_NSGL
#elif defined(TARGET_WIN32)
	#define GLFW_EXPOSE_NATIVE_WIN32
	#define GLFW_EXPOSE_NATIVE_WGL
#endif

#include "GLFW/glfw3native.h"

using std::numeric_limits;
using std::shared_ptr;
using std::vector;
using std::cout;
using std::endl;

//-------------------------------------------------------
ofAppGLFWWindow::ofAppGLFWWindow() : coreEvents(std::make_unique<ofCoreEvents>()){
	bEnableSetupScreen = true;
	buttonInUse = 0;
	buttonPressed = false;
	bWindowNeedsShowing = true;

	targetWindowMode = OF_WINDOW;

	ofAppPtr = nullptr;

	iconSet = false;
	windowP = nullptr;

	glfwSetErrorCallback(error_cb);
}

ofAppGLFWWindow::~ofAppGLFWWindow() {
	cout << "ofAppGLFWWindow::~ofAppGLFWWindow() " << settings.windowName << endl;
	close();
}

void ofAppGLFWWindow::close() {
	cout << "ofAppGLFWWindow::close! " << settings.windowName << endl;

	if (windowP) {
		glfwSetMouseButtonCallback( windowP, nullptr );
		glfwSetCursorPosCallback( windowP, nullptr );
		glfwSetCursorEnterCallback( windowP, nullptr );
		glfwSetKeyCallback( windowP, nullptr );
		glfwSetWindowSizeCallback( windowP, nullptr );
		glfwSetWindowPosCallback(windowP, nullptr);
		glfwSetFramebufferSizeCallback( windowP, nullptr);
		glfwSetWindowCloseCallback( windowP, nullptr );
		glfwSetScrollCallback( windowP, nullptr );
		glfwSetDropCallback( windowP, nullptr );
		glfwSetWindowRefreshCallback(windowP, nullptr);

		//hide the window before we destroy it stops a flicker on OS X on exit.
		glfwHideWindow(windowP);

		// We must ensure renderer is destroyed *before* glfw destroys the window in glfwDestroyWindow,
		// as `glfwDestroyWindow` at least on Windows has the effect of unloading OpenGL, making all
		// calls to OpenGL illegal.
		currentRenderer.reset();

		glfwDestroyWindow(windowP);
		windowP = nullptr;
		events().disable();
		bWindowNeedsShowing = true;
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::setup(const ofWindowSettings & _settings) {
//	cout << "yes recompile OK" << endl;
	if (windowP) {
		ofLogError() << "window already setup, probably you are mixing old and new style setup";
		ofLogError() << "call only ofCreateWindow(settings) or ofSetupOpenGL(...)";
		ofLogError() << "calling window->setup() after ofCreateWindow() is not necessary and won't do anything";
		return;
	}

	settings = _settings;
	
	if (!glfwInit()) {
		ofLogError("ofAppGLFWWindow") << "couldn't init GLFW";
		return;
	}


	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RED_BITS, settings.redBits);
	glfwWindowHint(GLFW_GREEN_BITS, settings.greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, settings.blueBits);
	glfwWindowHint(GLFW_ALPHA_BITS, settings.alphaBits);
	glfwWindowHint(GLFW_DEPTH_BITS, settings.depthBits);
	glfwWindowHint(GLFW_STENCIL_BITS, settings.stencilBits);
	glfwWindowHint(GLFW_STEREO, settings.stereo);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);
	// FIXME: Review this.
#ifndef TARGET_OSX
	glfwWindowHint(GLFW_AUX_BUFFERS, settings.doubleBuffering ? 1 : 0);
#else
	glfwWindowHint(GLFW_DOUBLEBUFFER, settings.doubleBuffering ? 1 : 0);
#endif
	glfwWindowHint(GLFW_SAMPLES, settings.numSamples);
	glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
	glfwWindowHint(GLFW_DECORATED, settings.decorated);

#ifdef TARGET_OPENGLES
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.glesVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	if (settings.glesVersion >= 2) {
		currentRenderer = std::make_shared<ofGLProgrammableRenderer>(this);
	} else {
		currentRenderer = std::make_shared<ofGLRenderer>(this);
	}
#else
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.glVersionMinor);
	if ((settings.glVersionMajor == 3 && settings.glVersionMinor >= 2) || settings.glVersionMajor >= 4) {
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	if (settings.glVersionMajor >= 3) {
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#if (GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR > 2) || (GLFW_VERSION_MAJOR > 3)
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, settings.transparent);
	#endif
		currentRenderer = std::make_shared<ofGLProgrammableRenderer>(this);
	} else {
		currentRenderer = std::make_shared<ofGLRenderer>(this);
	}
#endif

	GLFWwindow * sharedContext = nullptr;
	if (settings.shareContextWith) {
		sharedContext = (GLFWwindow *)settings.shareContextWith->getWindowContext();
	}

	allMonitors.update();
	//	ofLogNotice("ofAppGLFWWindow") << "WINDOW MODE IS " << screenMode;

	
	GLFWmonitor *monitor = nullptr;
	

	// FIXME: maybe use as a global variable for the window?
	bool hideWindow = false;
	
	int monitorIndex = 0;
	// Check to see if desired monitor is connected.
	if (allMonitors.rects.size() > settings.monitor) {
		monitorIndex = settings.monitor;
	} else {
		if (settings.showOnlyInSelectedMonitor) {
			hideWindow = true;
		}
		ofLogError("ofAppGLFWWindow") << "requested monitor is: " << settings.monitor << " monitor count is: " << allMonitors.rects.size();
	}
	
	if (settings.windowMode == OF_GAME_MODE)
	{
		windowRect = allMonitors.rects[monitorIndex];
		monitor = allMonitors.monitors[monitorIndex];
	}

//	cout << "will create window" << endl;
//	cout << "monitors size: " << allMonitors.rects.size() << endl;
	bool displayOK = false;
	if (settings.fullscreenDisplays.size()) {
		for (auto & d : settings.fullscreenDisplays) {
			cout << "GLFWWindow fullscreenDisplays " << d << " : " << allMonitors.rects.size() << endl;
			if (d < allMonitors.rects.size()) {
//				cout << "glfw ok opening " << endl;
				displayOK = true;
//				break;
			} else {
//				cout << "glfw not enough displays" << endl;
//				return nullptr;
			}
//			cout << d << endl;
		}
	} else {
		displayOK = true;
	}

//	cout << "glfw displayok " << displayOK << endl;
	windowP = glfwCreateWindow(settings.getWidth(), settings.getHeight(), settings.title.c_str(), monitor, sharedContext);

	if (displayOK) {
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	} else {
		// MARK: - WINDOW
//		cout << "glfw hint visible false" << endl;
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
//		return;
	}
	
	
	if (!windowP) {
		ofLogError("ofAppGLFWWindow") << "couldn't create GLFW window";
		return;
	}
	
	// saves window rectangle just created.
	windowRect = getWindowRect();

	
	if (settings.windowMode == OF_WINDOW || settings.windowMode == OF_FULLSCREEN) {
		if (settings.isPositionSet()) {
			windowRect.x = settings.getPosition().x;
			windowRect.y = settings.getPosition().y;
		}
		
		if (monitorIndex > 0) {
			windowRect.x += allMonitors.rects[monitorIndex].x;
			windowRect.y += allMonitors.rects[monitorIndex].y;
			// now it will trigger isPositionSet() as true
			settings.setPosition({ windowRect.x, windowRect.y });
		}
		
		if (settings.isPositionSet()) {
			setWindowRect(windowRect);
		} else {
			setWindowShape(windowRect.width, windowRect.height);
		}
	}
	



	// MARK: -
	if (settings.windowMode != OF_GAME_MODE) {

#ifdef TARGET_LINUX
//		if (!iconSet) {
//			ofPixels iconPixels;
//			// FIXME: we can have this define in ofIcon.h instead
//	#ifdef DEBUG
//			iconPixels.allocate(ofIconDebug.width, ofIconDebug.height, ofIconDebug.bytes_per_pixel);
//			GIMP_IMAGE_RUN_LENGTH_DECODE(iconPixels.getData(), ofIconDebug.rle_pixel_data, iconPixels.getWidth() * iconPixels.getHeight(), ofIconDebug.bytes_per_pixel);
//	#else
//			iconPixels.allocate(ofIcon.width, ofIcon.height, ofIcon.bytes_per_pixel);
//			GIMP_IMAGE_RUN_LENGTH_DECODE(iconPixels.getData(), ofIcon.rle_pixel_data, iconPixels.getWidth() * iconPixels.getHeight(), ofIcon.bytes_per_pixel);
//	#endif
//			setWindowIcon(iconPixels);
//		}
#endif

		//FIXME: check if it works with hint GLFW_AUTO_ICONIFY
		if (settings.iconified) {
			iconify(true);
		}

		//FIXME: check if it works with hint GLFW_MAXIMIZED
		if (settings.maximized) {
			glfwMaximizeWindow(windowP);
		}
	}

	//don't try and show a window if its been requsted to be hidden
	bWindowNeedsShowing = settings.visible;
	if (hideWindow) {
		bWindowNeedsShowing = false;
//		cout << "HIDE U" << endl;
	}

	glfwSetWindowUserPointer(windowP, this);
	glfwMakeContextCurrent(windowP);

//	windowMode = settings.windowMode;
	
#ifndef TARGET_OPENGLES
	static bool inited = false;
	if (!inited) {
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err) {
			/* Problem: glewInit failed, something is seriously wrong. */
			ofLogError("ofAppRunner") << "couldn't init GLEW: " << glewGetErrorString(err);
			return;
		}
		inited = true;
	}
#endif

	ofLogVerbose() << "GL Version: " << glGetString(GL_VERSION);

	if (currentRenderer->getType() == ofGLProgrammableRenderer::TYPE) {
#ifndef TARGET_OPENGLES
		static_cast<ofGLProgrammableRenderer *>(currentRenderer.get())->setup(settings.glVersionMajor, settings.glVersionMinor);
#else
		static_cast<ofGLProgrammableRenderer *>(currentRenderer.get())->setup(settings.glesVersion, 0);
#endif
	} else {
		static_cast<ofGLRenderer *>(currentRenderer.get())->setup();
	}

//	setVerticalSync(true);
	glfwSetMonitorCallback( monitor_cb );

	glfwSetMouseButtonCallback(windowP, mouse_cb);
	glfwSetCursorPosCallback(windowP, motion_cb);
	glfwSetCursorEnterCallback(windowP, entry_cb);
	glfwSetKeyCallback(windowP, keyboard_cb);
	glfwSetCharCallback(windowP, char_cb);
	glfwSetWindowSizeCallback(windowP, resize_cb);
	glfwSetWindowPosCallback(windowP,position_cb);
	glfwSetFramebufferSizeCallback(windowP, framebuffer_size_cb);
	glfwSetWindowCloseCallback(windowP, exit_cb);
	glfwSetScrollCallback(windowP, scroll_cb);
	glfwSetDropCallback(windowP, drop_cb);
	glfwSetWindowRefreshCallback(windowP, refresh_cb);

#ifdef TARGET_LINUX
	XSetLocaleModifiers("");
	xim = XOpenIM(getX11Display(), 0, 0, 0);
	if (!xim) {
		// fallback to internal input method
		XSetLocaleModifiers("@im=none");
		xim = XOpenIM(getX11Display(), 0, 0, 0);
	}
	xic = XCreateIC(xim,
		XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
		XNClientWindow, getX11Window(),
		XNFocusWindow, getX11Window(),
		NULL);
#endif

}

#ifdef TARGET_LINUX
    //------------------------------------------------------------
    void ofAppGLFWWindow::setWindowIcon(const of::filesystem::path & path) {
        ofPixels iconPixels;
        ofLoadImage(iconPixels, path);
        setWindowIcon(iconPixels);
    }
    
    //------------------------------------------------------------
    void ofAppGLFWWindow::setWindowIcon(const ofPixels & iconPixels) {
        iconSet = true;
        int length = 2 + iconPixels.getWidth() * iconPixels.getHeight();
        vector<unsigned long> buffer(length);
        buffer[0] = iconPixels.getWidth();
        buffer[1] = iconPixels.getHeight();
        for (size_t i = 0; i < iconPixels.getWidth() * iconPixels.getHeight(); i++) {
            buffer[i + 2] = iconPixels[i * 4 + 3] << 24;
            buffer[i + 2] += iconPixels[i * 4 + 0] << 16;
            buffer[i + 2] += iconPixels[i * 4 + 1] << 8;
            buffer[i + 2] += iconPixels[i * 4 + 2];
        }
        
        XChangeProperty(getX11Display(), getX11Window(), XInternAtom(getX11Display(), "_NET_WM_ICON", False), XA_CARDINAL, 32,
                        PropModeReplace, (const unsigned char *)buffer.data(), length);
        XFlush(getX11Display());
    }
#endif

//--------------------------------------------
ofCoreEvents & ofAppGLFWWindow::events() {
	return *coreEvents;
}

//--------------------------------------------
shared_ptr<ofBaseRenderer> & ofAppGLFWWindow::renderer() {
	return currentRenderer;
}

//--------------------------------------------
void ofAppGLFWWindow::update() {
	events().notifyUpdate();
	//show the window right before the first draw call.
	if (bWindowNeedsShowing && windowP) {
		// not working.
#ifdef TARGET_OSX
		NSWindow * cocoaWindow = glfwGetCocoaWindow(windowP);
//		[cocoaWindow setLevel:NSScreenSaverWindowLevel + 1];
		[cocoaWindow orderFrontRegardless];
#endif
		
		bWindowNeedsShowing = false;
		
		if (settings.windowMode == OF_FULLSCREEN) {
			// Meant to trigger fullscreen forced
			settings.windowMode = OF_WINDOWMODE_UNDEFINED;
			setFullscreen(true);
		} else {
			setFullscreen(false);
		}

		// FIXME: Test only
		if (settings.opacity != 1.0) {
			glfwSetWindowOpacity(windowP, settings.opacity);
		}

//		cout << "SHOW WINDOW! " << settings.windowName << endl;
		glfwShowWindow(windowP);
		
//		cout << "after show window rect " << getWindowRect() << endl;
	}
}

//--------------------------------------------
void ofAppGLFWWindow::pollEvents() {
	glfwPollEvents();
}


//--------------------------------------------
void ofAppGLFWWindow::beginDraw() {
	currentRenderer->startRender();
	if (bEnableSetupScreen) {
		currentRenderer->setupScreen();
	}
}

//--------------------------------------------
void ofAppGLFWWindow::endDraw() {
	if (settings.doubleBuffering) {
		glfwSwapBuffers(windowP);
		//		std::cout << "swap buffers " << ofGetFrameNum() << std::endl;
	} else {
		std::cout << "flush " << ofGetFrameNum()  << " : " << settings.windowName << std::endl;
		glFlush();
	}
	
	currentRenderer->finishRender();
}

//--------------------------------------------
void ofAppGLFWWindow::draw() {
	currentRenderer->startRender();
	if (bEnableSetupScreen) {
		currentRenderer->setupScreen();
	}

	events().notifyDraw();

	if (settings.doubleBuffering) {
		glfwSwapBuffers(windowP);
		//		std::cout << "swap buffers " << ofGetFrameNum() << std::endl;
	} else {
		glFlush();
	}
	
	currentRenderer->finishRender();
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
bool ofAppGLFWWindow::getWindowShouldClose() {
	return glfwWindowShouldClose(windowP);
}

//--------------------------------------------
void ofAppGLFWWindow::setWindowShouldClose() {
	glfwSetWindowShouldClose(windowP, 1);
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowTitle(const std::string & title) {
	settings.title = title;
	glfwSetWindowTitle(windowP, settings.title.c_str());
}

//------------------------------------------------------------
int ofAppGLFWWindow::getPixelScreenCoordScale() {
	// FIXME: cache?
	glm::vec2 contentScale;
	glfwGetWindowContentScale(windowP, &contentScale.x, &contentScale.y);
	return contentScale.x;
}

//------------------------------------------------------------
ofRectangle ofAppGLFWWindow::getWindowRect() {
	glm::ivec2 pos;
	glfwGetWindowPos( windowP, &pos.x, &pos.y );
	glm::ivec2 size;
	glfwGetWindowSize(windowP, &size.x, &size.y);
	return ofRectangle(pos.x, pos.y, size.x, size.y);
}

//------------------------------------------------------------
glm::ivec2 ofAppGLFWWindow::getWindowSize() {
	glm::ivec2 size;
	glfwGetWindowSize(windowP, &size.x, &size.y);
	return size;
}

//------------------------------------------------------------
glm::ivec2 ofAppGLFWWindow::getFramebufferSize() {
	// FIXME: cache size and handle in framebuffer_size_cb
	glm::ivec2 size;
	glfwGetFramebufferSize(windowP, &size.x, &size.y);
//	cout << "getFramebufferSize " << settings.windowName << " : " << size << " : " << ofGetFrameNum() << endl;
	return size;
}

//------------------------------------------------------------
glm::ivec2 ofAppGLFWWindow::getWindowPosition() {
	glm::ivec2 pos;
	glfwGetWindowPos( windowP, &pos.x, &pos.y );
	return pos;
}

//------------------------------------------------------------
glm::ivec2 ofAppGLFWWindow::getScreenSize() {
	// it will return the monitor/screen size where the windows sit.
	windowRect = getWindowRect();
	return allMonitors.getRectMonitorForScreenRect(windowRect).getSize();
}

//------------------------------------------------------------
int ofAppGLFWWindow::getWidth() {
	return windowRect.width;
}

//------------------------------------------------------------
int ofAppGLFWWindow::getHeight() {
	return windowRect.height;
}

//------------------------------------------------------------
GLFWwindow * ofAppGLFWWindow::getGLFWWindow() {
	return windowP;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowRect(const ofRectangle & rect) {
//	cout << settings.windowName << " setWindowRect " << rect << endl;
	
	glfwSetWindowMonitor(windowP, NULL, rect.x, rect.y, rect.width, rect.height, GLFW_DONT_CARE);
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowPosition(int x, int y) {
	glfwSetWindowPos(windowP, x, y);
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowShape(int w, int h) {
//	cout << "setWindowShape " << w << " : " <<  h << endl;
	glfwSetWindowSize(windowP, w, h);
}

//------------------------------------------------------------
void ofAppGLFWWindow::hideCursor() {
	if (settings.windowMode == OF_FULLSCREEN || settings.windowMode == OF_GAME_MODE) {
		glfwSetInputMode(windowP, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} else {
		glfwSetInputMode(windowP, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
};

//------------------------------------------------------------
void ofAppGLFWWindow::showCursor() {
	glfwSetInputMode(windowP, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
};

//------------------------------------------------------------
void ofAppGLFWWindow::enableSetupScreen() {
	bEnableSetupScreen = true;
};

//------------------------------------------------------------
void ofAppGLFWWindow::disableSetupScreen() {
	bEnableSetupScreen = false;
};

//------------------------------------------------------------
void ofAppGLFWWindow::setFSTarget(ofWindowMode targetWindowMode) {
//	cout << "setFSTarget " << targetWindowMode << endl;
	if (targetWindowMode == OF_FULLSCREEN) {
		// save window shape before going fullscreen
		windowRect = getWindowRect();
//		cout << "saving window rect " << windowRect << endl;

		if (settings.multiMonitorFullScreen) {
			windowRectFS = allMonitors.getRectForAllMonitors();
		} else {
			windowRectFS = allMonitors.getRectMonitorForScreenRect(windowRect);
		}
		
		// 
		if (settings.fullscreenDisplays.size()) {
			windowRectFS = allMonitors.getRectFromMonitors(settings.fullscreenDisplays);
		}
//		cout << "windowRectFS " << windowRectFS << " : " << settings.windowName << endl;
		setWindowRect(windowRectFS);
	}

	else if (targetWindowMode == OF_WINDOW) {
		setWindowRect(windowRect);
		setWindowTitle(settings.title);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::setFullscreen(bool fullscreen) {
//	cout << "setFullScreen " << fullscreen << " : " << settings.windowName << endl;
	if (fullscreen) {
		targetWindowMode = OF_FULLSCREEN;
	} else {
		targetWindowMode = OF_WINDOW;
	}

	//we only want to change window mode if the requested window is different to the current one.
	if (targetWindowMode == settings.windowMode) return;

#ifdef TARGET_OSX
	NSWindow * cocoaWindow = glfwGetCocoaWindow(windowP);

	if (targetWindowMode == OF_FULLSCREEN) {
		[NSApp setPresentationOptions:NSApplicationPresentationHideMenuBar | NSApplicationPresentationHideDock];
		[cocoaWindow setStyleMask:NSWindowStyleMaskBorderless];
		[cocoaWindow setHasShadow:NO];
	} else {
		[NSApp setPresentationOptions:NSApplicationPresentationDefault];
		// to recover correctly from a green button fullscreen
		if (([cocoaWindow styleMask] & NSWindowStyleMaskFullScreen) == NSWindowStyleMaskFullScreen) {
			[cocoaWindow toggleFullScreen:nil];
		}
		
		[cocoaWindow setStyleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable];
		[cocoaWindow setHasShadow:YES];
	}
	setFSTarget(targetWindowMode);

	//----------------------------------------------------
	//make sure the window is getting the mouse/key events
	[cocoaWindow makeFirstResponder:cocoaWindow.contentView];

#elif defined(TARGET_WIN32)

	HWND hwnd = glfwGetWin32Window(windowP);

	setFSTarget(targetWindowMode);

	if (targetWindowMode == OF_FULLSCREEN) {
		SetWindowLong(hwnd, GWL_EXSTYLE, 0);
		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		// TODO: Here discover windows dimensions
		// set window position, shape here.

//		SetWindowPos(hwnd, HWND_TOPMOST, xpos, ypos, fullscreenW, fullscreenH, SWP_SHOWWINDOW);

	} else if (targetWindowMode == OF_WINDOW) {

		DWORD EX_STYLE = WS_EX_OVERLAPPEDWINDOW;
		DWORD STYLE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_SIZEBOX;

		ChangeDisplaySettings(0, 0);
		SetWindowLong(hwnd, GWL_EXSTYLE, EX_STYLE);
		SetWindowLong(hwnd, GWL_STYLE, STYLE);
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		//not sure why this is - but if we don't do this the window shrinks by 4 pixels in x and y
		//should look for a better fix.
//		setWindowPosition(windowRect.x - 2, windowRect.y - 2);
//		setWindowShape(windowRect.width + 4, windowRect.height + 4);
	}

//	SetWindowPos(hwnd, HWND_TOPMOST, xpos, ypos, fullscreenW, fullscreenH, SWP_SHOWWINDOW);
	SetWindowPos(hwnd, HWND_TOPMOST, windowRectFS.x, windowRectFS.y, windowRectFS.width, windowRectFS.height, SWP_SHOWWINDOW);


#elif defined(TARGET_LINUX)
	setFSTarget(targetWindowMode);
#endif

	settings.windowMode = targetWindowMode;
}

//------------------------------------------------------------
void ofAppGLFWWindow::toggleFullscreen() {
	if (settings.windowMode == OF_GAME_MODE) return;

	if (settings.windowMode == OF_WINDOW) {
		setFullscreen(true);
	} else {
		setFullscreen(false);
	}
}

//------------------------------------------------------------
// FIXME: this can be up on base class. it is not GLFW exclusive
static void rotateMouseXY(ofOrientation orientation, int w, int h, double & x, double & y) {
	int savedY;
	switch (orientation) {
	case OF_ORIENTATION_180:
		x = w - x;
		y = h - y;
		break;

	case OF_ORIENTATION_90_RIGHT:
		savedY = y;
		y = x;
		x = w - savedY;
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
ofAppGLFWWindow * ofAppGLFWWindow::setCurrent(GLFWwindow * windowP) {
	auto instance { static_cast<ofAppGLFWWindow *>(glfwGetWindowUserPointer(windowP)) };
	if (ofCore.mainLoop) {
		ofCore.mainLoop->setCurrentWindow(instance);
	}
	instance->makeCurrent();
	return instance;
}

//------------------------------------------------------------
ofAppGLFWWindow * ofAppGLFWWindow::getWindow(GLFWwindow * windowP) {
	return static_cast<ofAppGLFWWindow *>(glfwGetWindowUserPointer(windowP));
	
//	auto instance = static_cast<ofAppGLFWWindow *>(glfwGetWindowUserPointer(windowP));
//	auto mainLoop = ofGetMainLoop();
//	if (mainLoop) {
//		mainLoop->setCurrentWindow(instance);
//	}
//	instance->makeCurrent();
//	return instance;
}

namespace {
int glfwtToOFModifiers(int mods) {
	int modifiers = 0;
	if (mods & GLFW_MOD_SHIFT) {
		modifiers |= OF_KEY_SHIFT;
	}
	if (mods & GLFW_MOD_ALT) {
		modifiers |= OF_KEY_ALT;
	}
	if (mods & GLFW_MOD_CONTROL) {
		modifiers |= OF_KEY_CONTROL;
	}
	if (mods & GLFW_MOD_SUPER) {
		modifiers |= OF_KEY_SUPER;
	}
	return modifiers;
}

unsigned long keycodeToUnicode(ofAppGLFWWindow * window, int scancode, int modifier) {
#ifdef TARGET_LINUX
	XkbStateRec xkb_state = {};
	XkbGetState(window->getX11Display(), XkbUseCoreKbd, &xkb_state);
	XEvent ev = { 0 };
	ev.xkey.keycode = scancode;
	ev.xkey.state = xkb_state.mods & ~ControlMask;
	ev.xkey.display = window->getX11Display();
	ev.xkey.type = KeyPress;
	KeySym keysym = NoSymbol;
	int status;
	char buffer[32] = { 0 };
	char * chars = buffer;
	auto count = Xutf8LookupString(window->getX11XIC(), &ev.xkey, chars, sizeof(buffer) - 1, &keysym, &status);
	if ((count > 0 && (status == XLookupChars || status == XLookupBoth)) || status == XLookupKeySym) {
		char ** c = &chars;
		unsigned int ch = 0, count = 0;
		static const unsigned int offsets[] = {
			0x00000000u, 0x00003080u, 0x000e2080u,
			0x03c82080u, 0xfa082080u, 0x82082080u
		};

		do {
			ch = (ch << 6) + (unsigned char)**c;
			(*c)++;
			count++;
		} while ((**c & 0xc0) == 0x80);

		if (count > 6) {
			return 0;
		} else {
			return ch - offsets[count - 1];
		}
	} else {
		return 0;
	}
#endif
#ifdef TARGET_WIN32
	static WCHAR buf[2];
	static BYTE keyboardState[256];
	GetKeyboardState(keyboardState);

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

	UINT fakeVirtualKey = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);

	int ret = ToUnicode(fakeVirtualKey, scancode, keyboardState, buf, 2, 0);

	if (ret == 1) {
		return buf[0];
	} else {
		return 0;
	}
#endif
#ifdef TARGET_OSX
	static UInt32 deadKeyState = 0;
	static UniChar characters[8];
	static UniCharCount characterCount = 0;

	typedef struct __TISInputSource * TISInputSourceRef;
	typedef TISInputSourceRef (*pFnGetInputSource)(void); // define function pointer that may return a input source ref, no arguments
	typedef void * (*pFnGetInputSourceProperty)(TISInputSourceRef, CFStringRef);
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
	static pFnGetInputSource getInputSource = (pFnGetInputSource)CFBundleGetFunctionPointerForName(tisBundle, CFSTR("TISCopyCurrentKeyboardLayoutInputSource"));
	static pFnGetKeyboardType getKeyboardType = (pFnGetKeyboardType)CFBundleGetFunctionPointerForName(tisBundle, CFSTR("LMGetKbdType"));
	static pFnGetInputSourceProperty getInputSourceProperty = (pFnGetInputSourceProperty)CFBundleGetFunctionPointerForName(tisBundle, CFSTR("TISGetInputSourceProperty"));

	static const TISInputSourceRef sourceRef = getInputSource(); // note that for the first time, this creates a copy on the heap, then we're re-using it.

	static const CFStringRef * kPropertyUnicodeKeyLayoutData = (CFStringRef *)CFBundleGetDataPointerForName(tisBundle, CFSTR("kTISPropertyUnicodeKeyLayoutData"));
	static const CFStringRef kTISPropertyUnicodeKeyLayoutData = *kPropertyUnicodeKeyLayoutData;
	static const CFDataRef UnicodeKeyLayoutData = (CFDataRef)getInputSourceProperty(sourceRef, kTISPropertyUnicodeKeyLayoutData);

	static const UCKeyboardLayout * pKeyboardLayout = (UCKeyboardLayout *)CFDataGetBytePtr(UnicodeKeyLayoutData);

	UInt32 mod_OSX = 0;
	{
		// We have to translate the GLFW modifier bitflags back to OS X,
		// so that SHIFT, CONTROL, etc can be taken into account when
		// calculating the unicode codepoint.

		// UCKeyTranslate expects the Carbon-era modifier mask values,
		// so use these instead of the NSEventModifierFlag enums
		if (modifier & GLFW_MOD_SHIFT)
			mod_OSX |= 512; // Carbon shiftKey value
		if (modifier & GLFW_MOD_CONTROL)
			mod_OSX |= 4096; // Carbon controlKey value
		if (modifier & GLFW_MOD_ALT)
			mod_OSX |= 2048; // Carbon optionKey value
		if (modifier & GLFW_MOD_SUPER)
			mod_OSX |= 256; // Carbon cmdKey

		// shift into 1 byte as per the Apple docs
		mod_OSX = (mod_OSX >> 8) & 0xFF;
	}

	// All this yak shaving was necessary to feed this diva of a function call:
	// https://developer.apple.com/library/mac/documentation/Carbon/Reference/Unicode_Utilities_Ref/index.html#//apple_ref/c/func/UCKeyTranslate

	if (noErr == UCKeyTranslate(pKeyboardLayout, scancode, kUCKeyActionDisplay, mod_OSX, getKeyboardType(), kUCKeyTranslateNoDeadKeysBit, &deadKeyState, sizeof(characters) / sizeof(characters[0]), &characterCount, characters)) {
		// if successful, first character contains codepoint
		return characters[0];
	} else {
		return 0;
	}

#endif
	return 0;
}
}

//------------------------------------------------------------
void ofAppGLFWWindow::mouse_cb(GLFWwindow * windowP_, int button, int state, int mods) {
	auto instance = setCurrent(windowP_);
//	auto instance = getWindow(windowP_);

#ifdef TARGET_OSX
	//we do this as unlike glut, glfw doesn't report right click for ctrl click or middle click for alt click
	if (instance->events().getKeyPressed(OF_KEY_CONTROL) && button == GLFW_MOUSE_BUTTON_LEFT) {
		button = GLFW_MOUSE_BUTTON_RIGHT;
	}
	if (instance->events().getKeyPressed(OF_KEY_ALT) && button == GLFW_MOUSE_BUTTON_LEFT) {
		button = GLFW_MOUSE_BUTTON_MIDDLE;
	}
#endif

	switch (button) {
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
	instance->buttonInUse = button;

	ofMouseEventArgs::Type action;
	if (state == GLFW_PRESS) {
		action = ofMouseEventArgs::Pressed;
		instance->buttonPressed = true;
	} else {
		action = ofMouseEventArgs::Released;
		instance->buttonPressed = false;
	}

	int modifiers = glfwtToOFModifiers(mods);

	ofMouseEventArgs args(action, instance->events().getMouseX(), instance->events().getMouseY(), button, modifiers);

	instance->events().notifyMouseEvent(args);
}

//------------------------------------------------------------
void ofAppGLFWWindow::motion_cb(GLFWwindow * windowP_, double x, double y) {
//	auto instance = getWindow(windowP_);
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	rotateMouseXY(instance->orientation, instance->getWidth(), instance->getHeight(), x, y);

	ofMouseEventArgs::Type action;
	if (!instance->buttonPressed) {
		action = ofMouseEventArgs::Moved;
	} else {
		action = ofMouseEventArgs::Dragged;
	}

	ofMouseEventArgs args(action,
		x,
		y,
		instance->buttonInUse,
		instance->events().getModifiers());
	instance->events().notifyMouseEvent(args);
}

//------------------------------------------------------------
void ofAppGLFWWindow::entry_cb(GLFWwindow * windowP_, int entered) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);
//	auto instance = getWindow(windowP_);

	ofMouseEventArgs::Type action;
	if (entered) {
		action = ofMouseEventArgs::Entered;
	} else {
		action = ofMouseEventArgs::Exited;
	}

	ofMouseEventArgs args(action,
		instance->events().getMouseX(),
		instance->events().getMouseY(),
		instance->buttonInUse,
		instance->events().getModifiers());
	instance->events().notifyMouseEvent(args);
}

//------------------------------------------------------------
void ofAppGLFWWindow::scroll_cb(GLFWwindow * windowP_, double x, double y) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);
//	auto instance = getWindow(windowP_);
	rotateMouseXY(instance->orientation, instance->getWidth(), instance->getHeight(), x, y);

	ofMouseEventArgs args(ofMouseEventArgs::Scrolled,
		instance->events().getMouseX(),
		instance->events().getMouseY(),
		instance->buttonInUse,
		instance->events().getModifiers());
	args.scrollX = x;
	args.scrollY = y;
	instance->events().notifyMouseEvent(args);
}

//------------------------------------------------------------
void ofAppGLFWWindow::drop_cb(GLFWwindow * windowP_, int numFiles, const char ** dropString) {
//	auto instance = getWindow(windowP_);
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	ofDragInfo drag;
	drag.position = { instance->events().getMouseX(), instance->events().getMouseY() };
	drag.files.resize(numFiles);
	for (int i = 0; i < (int)drag.files.size(); i++) {
		drag.files[i] = dropString[i];
	}
	instance->events().notifyDragEvent(drag);
}

//------------------------------------------------------------
void ofAppGLFWWindow::error_cb(int errorCode, const char * errorDescription) {
	ofLogError("ofAppGLFWWindow") << errorCode << ": " << errorDescription;
}

//------------------------------------------------------------
void ofAppGLFWWindow::keyboard_cb(GLFWwindow * windowP_, int keycode, int scancode, int action, int mods) {
	int key = 0;
	uint32_t codepoint = 0;
	ofAppGLFWWindow * instance = setCurrent(windowP_);
//	auto instance = getWindow(windowP_);
//	cout << "keyboard_cb " << instance->settings.windowName << endl;
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
		codepoint = '\n';
		break;
	case GLFW_KEY_KP_ENTER:
		key = OF_KEY_RETURN;
		codepoint = '\n';
		break;
	case GLFW_KEY_TAB:
		key = OF_KEY_TAB;
		codepoint = '\t';
		break;
	case GLFW_KEY_KP_0:
		key = codepoint = '0';
		break;
	case GLFW_KEY_KP_1:
		key = codepoint = '1';
		break;
	case GLFW_KEY_KP_2:
		key = codepoint = '2';
		break;
	case GLFW_KEY_KP_3:
		key = codepoint = '3';
		break;
	case GLFW_KEY_KP_4:
		key = codepoint = '4';
		break;
	case GLFW_KEY_KP_5:
		key = codepoint = '5';
		break;
	case GLFW_KEY_KP_6:
		key = codepoint = '6';
		break;
	case GLFW_KEY_KP_7:
		key = codepoint = '7';
		break;
	case GLFW_KEY_KP_8:
		key = codepoint = '8';
		break;
	case GLFW_KEY_KP_9:
		key = codepoint = '9';
		break;
	case GLFW_KEY_KP_DIVIDE:
		key = codepoint = '/';
		break;
	case GLFW_KEY_KP_MULTIPLY:
		key = codepoint = '*';
		break;
	case GLFW_KEY_KP_SUBTRACT:
		key = codepoint = '-';
		break;
	case GLFW_KEY_KP_ADD:
		key = codepoint = '+';
		break;
	case GLFW_KEY_KP_DECIMAL:
		key = codepoint = '.';
		break;
	case GLFW_KEY_KP_EQUAL:
		key = codepoint = '=';
		break;
	default:
		codepoint = keycodeToUnicode(instance, scancode, mods);
//		codepoint = scancode;
		key = codepoint;
		break;
	}

	int modifiers = glfwtToOFModifiers(mods);

	if (action == GLFW_PRESS) {
		ofKeyEventArgs keyE(ofKeyEventArgs::Pressed, key, keycode, scancode, codepoint, modifiers);
		instance->events().notifyKeyEvent(keyE);
	} else if (action == GLFW_REPEAT) {
		ofKeyEventArgs keyE(ofKeyEventArgs::Pressed, key, keycode, scancode, codepoint, modifiers);
		keyE.isRepeat = true;
		instance->events().notifyKeyEvent(keyE);
	} else if (action == GLFW_RELEASE) {
		ofKeyEventArgs keyE(ofKeyEventArgs::Released, key, keycode, scancode, codepoint, modifiers);
		instance->events().notifyKeyEvent(keyE);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::char_cb(GLFWwindow * windowP_, uint32_t key) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);
//	auto instance = getWindow(windowP_);
	instance->events().charEvent.notify(key);
}

//------------------------------------------------------------
void ofAppGLFWWindow::refresh_cb(GLFWwindow * windowP_) {
//	auto instance = getWindow(windowP_);
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	instance->draw();
//	thisWindow->draw();
}

//------------------------------------------------------------
void ofAppGLFWWindow::monitor_cb(GLFWmonitor * monitor, int event) {
	allMonitors.update();
//	cout << "monitor_cb!" << endl;
//	cout << "monitors rect size: " << allMonitors.rects.size() << endl;

	for (auto & w : ofGetMainLoop()->getWindows()) {
		if (w->settings.showOnlyInSelectedMonitor) {
//			cout << "monitor_cb windowName " << w->settings.windowName << endl;
			auto win = static_cast<ofAppGLFWWindow *>(w.get());
			if (allMonitors.rects.size() > w->settings.monitor) {
				if (w->settings.windowMode == OF_FULLSCREEN) {
					w->setWindowRect(allMonitors.rects[w->settings.monitor]);
				}
				// TODO: set window to monitor, if it is windowed.
				glfwShowWindow(win->getGLFWWindow());
			} else {
				glfwHideWindow(win->getGLFWWindow());
			}
		}
	}
//	ofGetMainLoop()->getWindows().size()
}

//------------------------------------------------------------
void ofAppGLFWWindow::position_cb(GLFWwindow* windowP_, int x, int y){
	ofAppGLFWWindow * instance = setCurrent(windowP_);
//	auto instance = getWindow(windowP_);
//	if (instance->settings.windowMode == OF_WINDOW) {
//		instance->windowRect.x = x;
//		instance->windowRect.y = y;
//	}
	instance->events().notifyWindowMoved(x, y);
}
//------------------------------------------------------------
void ofAppGLFWWindow::resize_cb(GLFWwindow * windowP_, int w, int h) {
//	auto instance = getWindow(windowP_);
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	instance->events().notifyWindowResized(w, h);
//	instance->nFramesSinceWindowResized = 0;

	
#if defined(TARGET_OSX)
	if (!instance->bWindowNeedsShowing) {
//		 FIXME - only after first update
		NSWindow * cocoaWindow = glfwGetCocoaWindow(windowP_);
		if (([cocoaWindow styleMask] & NSWindowStyleMaskFullScreen) == NSWindowStyleMaskFullScreen) {
			instance->settings.windowMode = OF_FULLSCREEN;
		} else {
			instance->settings.windowMode = OF_WINDOW;
		}
	}
#endif
}

//------------------------------------------------------------
void ofAppGLFWWindow::framebuffer_size_cb(GLFWwindow * windowP_, int w, int h) {
//	cout << "framebuffer_size_cb " << w << " : " << h << endl;
//	auto instance = getWindow(windowP_);
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	
	instance->currentRenderer->clear();
	instance->events().notifyFramebufferResized(w, h);
}

//--------------------------------------------
void ofAppGLFWWindow::exit_cb(GLFWwindow * windowP_) {
//	cout << "exit_cb " <<  endl;
//	auto instance = getWindow(windowP_);
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	instance->events().notifyExit();
	
	// TODO: handle window closing correctly here.
//	instance->close();
//	instance->setWindowShouldClose();
}

//------------------------------------------------------------
void ofAppGLFWWindow::setVerticalSync(bool bVerticalSync) {
	if (bVerticalSync) {
		glfwSwapInterval(1);
	} else {
		glfwSwapInterval(0);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::setClipboardString(const std::string & text) {
	glfwSetClipboardString(ofAppGLFWWindow::windowP, text.c_str());
}

//------------------------------------------------------------
std::string ofAppGLFWWindow::getClipboardString() {
	const char * clipboard = glfwGetClipboardString(ofAppGLFWWindow::windowP);

	if (clipboard) {
		return clipboard;
	} else {
		return "";
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::listVideoModes() {
	glfwInit();
	int numModes;
	const GLFWvidmode * vidModes = glfwGetVideoModes(nullptr, &numModes);
	for (int i = 0; i < numModes; i++) {
		ofLogNotice() << vidModes[i].width << " x " << vidModes[i].height
					  << vidModes[i].redBits + vidModes[i].greenBits + vidModes[i].blueBits << "bit";
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::listMonitors() {
	glfwInit();
	int count;
	const auto monitors = glfwGetMonitors(&count);
	for (int i = 0; i < count; i++) {
		auto monitor = monitors[i];
		int w, h, x, y;
		glfwGetMonitorPhysicalSize(monitor, &w, &h);
		glfwGetMonitorPos(monitor, &x, &y);
		ofLogNotice() << i << ": " << glfwGetMonitorName(monitor) << ", physical size: " << w << "x" << h << "mm at " << x << ", " << y;
	}
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowIconified() {
	return glfwGetWindowAttrib(windowP, GLFW_ICONIFIED);
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowActive() {
	//	return glfwGetWindowParam(GLFW_ACTIVE);
	return true;
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowResizeable() {
	return !glfwGetWindowAttrib(windowP, GLFW_RESIZABLE);
}

//------------------------------------------------------------
void ofAppGLFWWindow::iconify(bool bIconify) {
	if (bIconify)
		glfwIconifyWindow(windowP);
	else
		glfwRestoreWindow(windowP);
}

void ofAppGLFWWindow::makeCurrent() {
	glfwMakeContextCurrent(windowP);
}

#if defined(TARGET_LINUX)
    Display * ofAppGLFWWindow::getX11Display() {
        return glfwGetX11Display();
    }
    
    Window ofAppGLFWWindow::getX11Window() {
        return glfwGetX11Window(windowP);
    }
    
    XIC ofAppGLFWWindow::getX11XIC() {
        return xic;
    }
#endif
    
#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
    GLXContext ofAppGLFWWindow::getGLXContext() {
        return glfwGetGLXContext(windowP);
    }
#endif
    
#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
    EGLDisplay ofAppGLFWWindow::getEGLDisplay() {
        return glfwGetEGLDisplay();
    }
    
    EGLContext ofAppGLFWWindow::getEGLContext() {
        return glfwGetEGLContext(windowP);
    }
    
    EGLSurface ofAppGLFWWindow::getEGLSurface() {
        return glfwGetEGLSurface(windowP);
    }
#endif
    
#if defined(TARGET_OSX)
    void * ofAppGLFWWindow::getNSGLContext() {
        return (__bridge void *)glfwGetNSGLContext(windowP);
    }
    
    void * ofAppGLFWWindow::getCocoaWindow() {
        return (__bridge void *)glfwGetCocoaWindow(windowP);
    }
#endif
    
#if defined(TARGET_WIN32)
    HGLRC ofAppGLFWWindow::getWGLContext() {
        return glfwGetWGLContext(windowP);
    }
    
    HWND ofAppGLFWWindow::getWin32Window() {
        return glfwGetWin32Window(windowP);
    }
    
#endif
    
#endif


// DEPRECATED - REMOVE

void ofAppGLFWWindow::setNumSamples(int _samples) {
	settings.numSamples = _samples;
}

void ofAppGLFWWindow::setDoubleBuffering(bool doubleBuff) {
	settings.doubleBuffering = doubleBuff;
}

void ofAppGLFWWindow::setColorBits(int r, int g, int b) {
	settings.redBits = r;
	settings.greenBits = g;
	settings.blueBits = b;
}

void ofAppGLFWWindow::setAlphaBits(int a) {
	settings.alphaBits = a;
}

void ofAppGLFWWindow::setDepthBits(int depth) {
	settings.depthBits = depth;
}

void ofAppGLFWWindow::setStencilBits(int stencil) {
	settings.stencilBits = stencil;
}

void ofAppGLFWWindow::setMultiDisplayFullscreen(bool bMultiFullscreen) {
	settings.multiMonitorFullScreen = bMultiFullscreen;
}
