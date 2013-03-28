#pragma once

#include "ofConstants.h"

//#ifdef TARGET_LINUX
//#include <GL/glfw.h>   // For GLFW, OpenGL and GLU
//#elif defined(TARGET_OSX)
//#elif defined(TARGET_WIN32)
//#include "GL/glfw.h"
//#endif
//

//#define _GLFW_COCOA
//#define GLFW_INCLUDE_GL3  /* don't drag in legacy GL headers. */
//#define GLFW_NO_GLU       /* don't drag in the old GLU lib - unless you must. */
//#define _GLFW_USE_MENUBAR

#include "glfw_config.h"
#include "glfw3.h"

#include "ofAppBaseWindow.h"
#include "ofEvents.h"

//class ofVec3f;
class ofBaseApp;

class ofAppGLFWWindow : public ofAppBaseWindow {

	static GLFWwindow* windowP;
	
public:

	ofAppGLFWWindow();
	~ofAppGLFWWindow(){}

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
	static void 	motion_cb(GLFWwindow* windowP_, int x, int y);
	static void 	keyboard_cb(GLFWwindow* windowP_, int key, int state);
	static void 	resize_cb(GLFWwindow* windowP_, int w, int h);
	static int 		exit_cb(GLFWwindow* windowP_);
	static void		scroll_cb(GLFWwindow* windowP_, double x, double y);
	
	void			idle_cb();
	

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
	bool			bNewScreenMode;

	bool			bEnableSetupScreen;

	float			frameRate;

	int				requestedWidth;
	int				requestedHeight;

	int 			nonFullScreenW;
	int 			nonFullScreenH;
	int 			nonFullScreenX;
	int 			nonFullScreenY;

	ofOrientation orientation;

};


//#endif
