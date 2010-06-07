#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"

#ifdef TARGET_WIN32
	#define GLUT_BUILDING_LIB
	#include "glut.h"
#endif
#ifdef TARGET_OSX
	#include <GLUT/glut.h>
#endif
#ifdef TARGET_LINUX
	#include <GL/glut.h>
#endif

class ofPoint;
class ofBaseApp;

class ofAppGlutWindow : public ofAppBaseWindow {

public:

	ofAppGlutWindow();
	~ofAppGlutWindow(){}

	void setupOpenGL(int w, int h, int screenMode);
	void initializeWindow();
	void runAppViaInfiniteLoop(ofBaseApp * appPtr);
	
	//note if you fail to set a compatible string the app will not launch
	void setGlutDisplayString(string str);

	void hideCursor();
	void showCursor();
	
	void setFullscreen(bool fullScreen);
	void toggleFullscreen();
	
	static void exitApp();

	void setWindowTitle(string title);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);

	ofPoint		getWindowPosition();
	ofPoint		getWindowSize();
	ofPoint		getScreenSize();

	int			getWindowMode();

	int			getFrameNum();
	float		getFrameRate();
	double		getLastFrameTime();
	void		setFrameRate(float targetRate);

	void		enableSetupScreen();
	void		disableSetupScreen();

	static void display(void);
	static void mouse_cb(int button, int state, int x, int y);
	static void motion_cb(int x, int y);
	static void passive_motion_cb(int x, int y);
	static void idle_cb(void);
	static void keyboard_cb(unsigned char key, int x, int y);
	static void keyboard_up_cb(unsigned char key, int x, int y);
	static void special_key_cb(int key, int x, int y) ;
	static void special_key_up_cb(int key, int x, int y) ;
	static void resize_cb(int w, int h);


	string displayString;
	 
};

