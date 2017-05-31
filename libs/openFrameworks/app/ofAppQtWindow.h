#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"

#include <QApplication>
#include "QtGLWidget.h"

class ofBaseApp;
class QtGLWidget;

#ifdef TARGET_OPENGLES
class ofQtGLWindowSettings : public ofGLESWindowSettings{
#else
class ofQtGLWindowSettings : public ofGLWindowSettings{
#endif

public:
	ofQtGLWindowSettings() {}

#ifdef TARGET_OPENGLES
	ofGLFWWindowSettings(const ofGLESWindowSettings & settings)
		:ofGLESWindowSettings(settings) {}
#else
	ofQtGLWindowSettings(const ofGLWindowSettings & settings)
		: ofGLWindowSettings(settings) {}
#endif

	int numSamples = 4;
	bool doubleBuffering = true;
	int redBits = 8;
	int greenBits = 8;
	int blueBits = 8;
	int alphaBits = 8;
	int depthBits = 24;
	int stencilBits = 0;
	bool stereo = false;
	bool visible = true;
	bool iconified = false;
	bool decorated = true;
	bool resizable = true;
	int monitor = 0;
	bool multiMonitorFullScreen = false;
	shared_ptr<ofAppBaseWindow> shareContextWith;
};

class ofAppQtWindow : public ofAppBaseGLWindow{

public:

	ofAppQtWindow(QApplication * qtApp = 0);
	~ofAppQtWindow();

	// Can't be copied, use shared_ptr
	ofAppQtWindow(ofAppQtWindow & w) = delete;
	ofAppQtWindow & operator=(ofAppQtWindow & w) = delete;

	static void loop() {};
	static bool doesLoop() { return false; }
	static bool allowsMultiWindow() { return true; }
	static bool needsPolling() { return true; }
	static void pollEvents() { glfwPollEvents(); }

	using ofAppBaseWindow::setup;
#ifdef TARGET_OPENGLES
	void setup(const ofGLESWindowSettings & settings);
#else
	void setup(const ofGLWindowSettings & settings);
#endif
	void setup(const ofQtGLWindowSettings & settings);
	void update();
	void draw();

//	bool getWindowShouldClose();
//	void setWindowShouldClose();

	void close();

	void hideCursor();
	void showCursor();

//	int getHeight();
//	int getWidth();

	ofCoreEvents & events();
	shared_ptr<ofBaseRenderer> & renderer();

//	QtWindow* getQtWindow();
//	void * getWindowContext() { return getQtWindow(); }
	ofGLWindowSettings getSettings() { return settings; }

	glm::vec2 	getWindowPosition();
	glm::vec2	getWindowSize();
	glm::vec2	getScreenSize();

	void setWindowTitle(string title);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);

//	void			setOrientation(ofOrientation orientation);
//	ofOrientation	getOrientation();
	ofWindowMode	getWindowMode();

//	void		setFullscreen(bool fullscreen);
//	void		toggleFullscreen();

	void		enableSetupScreen();
	void		disableSetupScreen();

//	void		setVerticalSync(bool bSync);

//	void        setClipboardString(const string& text);
//	string      getClipboardString();

//	int         getPixelScreenCoordScale();

	void 		makeCurrent();
	void		swapBuffers();
	void		startRender();
	void		finishRender();

//	static void listVideoModes();
//	static void listMonitors();
//	bool isWindowIconified();
//	bool isWindowActive();
//	bool isWindowResizeable();
//	void iconify(bool bIconify);

	///----- from GLUT example
//	void setAppPtr(shared_ptr<ofApp> appPtr);
	static void exitApp();

//	int			getFrameNum();
//	float		getFrameRate();
//	void		setFrameRate(float targetRate);


public:
	ofCoreEvents coreEvents;
	shared_ptr<ofBaseRenderer> currentRenderer;
	ofQtGLWindowSettings settings;

	ofWindowMode	windowMode;
	ofRectangle		windowRect;
	ofOrientation	orientation;

	bool			bEnableSetupScreen;
	int				windowW, windowH;		// physical pixels width
	int				currentW, currentH;		// scaled pixels width

	int				buttonInUse;
	bool			buttonPressed;

	int 			nFramesSinceWindowResized;
	bool			bWindowNeedsShowing;
	bool			iconSet;
	int				pixelScreenCoordScale;

	// Of App Ptr
	shared_ptr<ofBaseApp>		ofAppPtr;
	// Qt Stuff
	QApplication *				qtAppPtr;
	QtGLWidget*					windowPtr;

	void setStatusMessage(string s);


};