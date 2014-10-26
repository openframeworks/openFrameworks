#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofEvents.h"
#include "ofTypes.h"
#include "ofPixels.h"

//class ofPoint;
class ofBaseApp;
class ofBaseRenderer;

class ofAppGlutWindow : public ofAppBaseGLWindow {

public:

	ofAppGlutWindow();
	~ofAppGlutWindow(){}

	static bool doesLoop(){ return true; }
	static bool allowsMultiWindow(){ return false; }
	static void loop();
	static bool needsPolling(){ return false; }
	static void pollEvents(){  }

	using ofAppBaseWindow::setup;
	void setup(const ofGLWindowSettings & settings);
	void update();
	void draw();
	void run(ofBaseApp * appPtr);
	void close();
	
	void setDoubleBuffering(bool _bDoubleBuffered); 
	
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
	
	ofWindowMode	getWindowMode();

	void		enableSetupScreen();
	void		disableSetupScreen();

	void		setVerticalSync(bool enabled);

	ofCoreEvents & events();
	shared_ptr<ofBaseRenderer> & renderer();

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
	
	ofCoreEvents coreEvents;
	shared_ptr<ofBaseRenderer> currentRenderer;
	int windowId;
};

