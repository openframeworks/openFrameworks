#pragma once

#include "ofConstants.h"

#define GLFW_INCLUDE_NONE

#include "GL/glfw3.h"

#include "ofAppBaseWindow.h"
#include "ofEvents.h"

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

	static void exitApp();

	void hideCursor();
	void showCursor();

	int getHeight();
	int getWidth();

	ofVec3f		getWindowSize();
	ofVec3f		getScreenSize();
	ofVec3f 	getWindowPosition();

	void setWindowTitle(string title);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);

	void			setOrientation(ofOrientation orientation);
	ofOrientation	getOrientation();

	int			getWindowMode();
	float		getFrameRate();
	void		setFrameRate(float targetRate);
	double		getLastFrameTime();


	int			getFrameNum();

	void		setFullscreen(bool fullscreen);
	void		toggleFullscreen();

	void		enableSetupScreen();
	void		disableSetupScreen();

	void		setVerticalSync(bool bSync);


	//GLFW specifics
	void		setFSAASamples(int samples);
	void		listVideoModes();
	bool		isWindowIconified();
	bool		isWindowActive();
	bool		isWindowResizeable();
	void		iconify(bool bIconify);

private:
	// callbacks
	void			display(void);
	void 			idle(void);

	static void 	mouse_cb(GLFWwindow* windowP_, int button, int state);
	static void 	motion_cb(GLFWwindow* windowP_, double x, double y);
	static void 	keyboard_cb(GLFWwindow* windowP_, int key, int state);
	static void 	resize_cb(GLFWwindow* windowP_, int w, int h);
	static void 	exit_cb(GLFWwindow* windowP_);
	static void		scroll_cb(GLFWwindow* windowP_, double x, double y);


	//utils
	void			changeMode();

	int				samples;

	int				nFrameCount;

	int 			millisForFrame;
	int 			prevMillis;
	int 			diffMillis;
	bool 			bFrameRateSet;
	double			lastFrameTime;
	double			fps;

	int				windowMode;

	bool			bEnableSetupScreen;

	float			frameRate;

	int				requestedWidth;
	int				requestedHeight;

	int 			nonFullScreenW;
	int 			nonFullScreenH;
	int 			nonFullScreenX;
	int 			nonFullScreenY;

	ofOrientation orientation;

	int glVersionMinor, glVersionMajor;

};


//#endif
