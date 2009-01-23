#pragma once

#include "ofConstants.h"

#ifdef OF_WINDOWING_GLUT

#ifdef TARGET_OSX
	#include <GLUT/glut.h>
#endif

#ifdef TARGET_LINUX
	#include <GL/glut.h>
#endif

#ifdef TARGET_WINDOWS
	#define GLUT_BUILDING_LIB
	#include "glut.h"
#endif

#include "ofAppBaseWindow.h"
#include "ofEvents.h"

class ofPoint;
class ofBaseApp;

class ofAppGlutWindow : public ofAppBaseWindow {

public:

	ofAppGlutWindow();
	~ofAppGlutWindow(){}

	void setupOpenGL(int w, int h, int screenMode);
	void initializeWindow();
	void runAppViaInfiniteLoop(ofBaseApp * appPtr);

	void hideCursor();
	void showCursor();

	void setWindowTitle(string title);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);

	ofPoint		getWindowPosition();
	ofPoint		getWindowSize();
	ofPoint		getScreenSize();

	int			getWindowMode();
	float		getFrameRate();

	void		setFrameRate(float targetRate);

	int			getFrameNum();
	void		setFullscreen(bool fullscreen);
	void		toggleFullscreen();

	void		enableSetupScreen();
	void		disableSetupScreen();

	void		exit();

	void		setVerticalSync(bool bSync){}; // no specific glut set sync

private:
	// callbacks
	static void	display(void);
	static void mouse_cb(int button, int state, int x, int y);
	static void motion_cb(int x, int y);
	static void passive_motion_cb(int x, int y);
	static void idle_cb(void);
	static void timer_cb(int millis);
	static void keyboard_cb(unsigned char key, int x, int y);
	static void keyboard_up_cb(unsigned char key, int x, int y);
	static void special_key_cb(int key, int x, int y) ;
	static void special_key_up_cb(int key, int x, int y) ;
	static void resize_cb(int w, int h);


};

#endif //OF_WINDOWING_GLUT

