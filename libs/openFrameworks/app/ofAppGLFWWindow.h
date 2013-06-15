#pragma once

#include "ofConstants.h"

#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"

#include "ofAppBaseWindow.h"
#include "ofEvents.h"
#include "ofPixels.h"

//class ofVec3f;
class ofBaseApp;

class ofAppGLFWWindow : public ofAppBaseWindow {

	static GLFWwindow* windowP;

public:

	ofAppGLFWWindow();
	~ofAppGLFWWindow(){}

	void setOpenGLVersion(int major, int minor);
	void setupOpenGL(int w, int h, int screenMode);
	void initializeWindow();
	void runAppViaInfiniteLoop(ofBaseApp * appPtr);


	void hideCursor();
	void showCursor();

	int getHeight();
	int getWidth();

	ofVec3f		getWindowSize();
	ofVec3f		getScreenSize();
	ofVec3f 	getWindowPosition();
#ifdef TARGET_LINUX
	void setWindowIcon(const string & path);
	void setWindowIcon(const ofPixels & iconPixels);
#endif

	void setWindowTitle(string title);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);

	void			setOrientation(ofOrientation orientation);
	ofOrientation	getOrientation();

	int			getWindowMode();

	void		setFullscreen(bool fullscreen);
	void		toggleFullscreen();

	void		enableSetupScreen();
	void		disableSetupScreen();

	void		setVerticalSync(bool bSync);


	//GLFW specifics
	void 		setNumSamples(int samples);
	void 		setDoubleBuffering(bool doubleBuff);
	void 		setColorBits(int r, int g, int b);
	void		setAlphaBits(int a);
	void		setDepthBits(int depth);
	void		setStencilBits(int stencil);
	void		listVideoModes();
	bool		isWindowIconified();
	bool		isWindowActive();
	bool		isWindowResizeable();
	void		iconify(bool bIconify);

private:
	// callbacks
	void			display(void);

	static void 	mouse_cb(GLFWwindow* windowP_, int button, int state, int mods);
	static void 	motion_cb(GLFWwindow* windowP_, double x, double y);
	static void 	keyboard_cb(GLFWwindow* windowP_, int key, int scancode, int action, int mods);
	static void 	resize_cb(GLFWwindow* windowP_, int w, int h);
	static void 	exit_cb(GLFWwindow* windowP_);
	static void		scroll_cb(GLFWwindow* windowP_, double x, double y);
	static void 	exitApp();


	//utils
	int				samples;
	int				rBits,gBits,bBits,aBits,depthBits,stencilBits;

	int				windowMode;

	bool			bEnableSetupScreen;

	int				requestedWidth;
	int				requestedHeight;

	int 			nonFullScreenW;
	int 			nonFullScreenH;
	int 			nonFullScreenX;
	int 			nonFullScreenY;

	int				buttonInUse;
	bool			buttonPressed;

	int				windowW;
	int				windowH;

	int 			nFramesSinceWindowResized;
	bool			bDoubleBuffered;

	static ofAppGLFWWindow	* instance;
	static ofBaseApp *	ofAppPtr;

	ofOrientation orientation;

	int glVersionMinor, glVersionMajor;

	bool iconSet;

    #ifdef TARGET_WIN32
    LONG lExStyle, lStyle;
    #endif // TARGET_WIN32
};


//#endif
