#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofEvents.h"
#include "ofTypes.h"

#ifdef TARGET_LINUX
#include "ofPixels.h"
#endif

//class ofPoint;
class ofBaseApp;

class ofAppGlutWindow : public ofAppBaseWindow {

public:

	ofAppGlutWindow();
	~ofAppGlutWindow(){}

	void setupOpenGL(int w, int h, int screenMode);
	
	void setDoubleBuffering(bool _bDoubleBuffered); 
	
	void initializeWindow();
	void runAppViaInfiniteLoop(ofBaseApp * appPtr);
	
	//note if you fail to set a compatible string the app will not launch
	void setGlutDisplayString(string str);

	void hideCursor();
	void showCursor();
	
	void setFullscreen(bool fullScreen);
	void toggleFullscreen();

	void setWindowTitle(string title);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);

	ofPoint		getWindowPosition();
	ofPoint		getWindowSize();
	ofPoint		getScreenSize();
	
	void			setOrientation(ofOrientation orientation);
	ofOrientation	getOrientation();
	
	int			getWidth();
	int			getHeight();	
	
	int			getWindowMode();

	void		enableSetupScreen();
	void		disableSetupScreen();

	void		setVerticalSync(bool enabled);

private:
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
	static void entry_cb(int state);
	static void dragEvent(char ** fileNames, int howManyFiles, int dragX, int dragY);
	string displayString;

	bool iconSet;
#ifdef TARGET_LINUX
	void setWindowIcon(const string & path);
	void setWindowIcon(const ofPixels & iconPixels);
#endif
	
		 
};

