/*
 * ofAppAndroidWindow.h
 *
 *  Created on: 24/05/2010
 *      Author: arturo
 */

#pragma once

#include "ofAppBaseWindow.h"
#include "ofEvents.h"
#include "ofConstants.h"
#include "ofTypes.h"

class ofxAndroidWindowSettings : public ofGLESWindowSettings
{
public:
	ofxAndroidWindowSettings()
	: preserveContextOnPause( true ){}
	
	ofxAndroidWindowSettings( const ofGLESWindowSettings & settings )
	: ofGLESWindowSettings( settings ), preserveContextOnPause( true ) {
		const ofxAndroidWindowSettings * androidSettings = dynamic_cast<const ofxAndroidWindowSettings*>(&settings);
        if(androidSettings){
			preserveContextOnPause = androidSettings->preserveContextOnPause;
		}
	}
	
	/**
		This corresponds to GLSurfaceView.setPreserveEGLContextOnPause.
		If this is false, all opengl resources (textures, shaders, ...) will be destroyed when the App is minimized (Activity::onStop).
		I would suggest to have this set to 'true' when releasing the App, but try to run it with this set to 'false'
			from time to time to make sure that opengl resources reloading works properly.
		Use events ofxAndroidEventsClass::unloadGL and ofxAndroidEventsClass::loadGL to track when the opengl context is lost.
	*/
	void setPreserveContextOnPause(bool preserve){
		preserveContextOnPause = preserve;
	}
	
	bool preserveContextOnPause;
};

class ofAppAndroidWindow: public ofAppBaseGLESWindow {
public:
	ofAppAndroidWindow();
	virtual ~ofAppAndroidWindow();

	static bool doesLoop(){ return true; }
	static void loop(){ }
	static bool needsPolling(){ return false; }
	static void pollEvents(){}
	static bool allowsMultiWindow(){ return false; }
	static bool isSurfaceDestroyed();

    using ofAppBaseWindow::setup;
	void setup(const ofGLESWindowSettings & settings);
	void setup(const ofxAndroidWindowSettings & settings);
	void update();
	void draw();

	void hideCursor() {}
	void showCursor() {}

	void	setWindowPosition(int x, int y) {}
	void	setWindowShape(int w, int h) {}

	glm::vec2	getWindowPosition() {return glm::vec2(); }
	glm::vec2	getWindowSize();
	glm::vec2	getScreenSize(){return getWindowSize(); }

	int		getWidth();
	int		getHeight();

	bool	doesHWOrientation(){return true;}

	void	setWindowTitle(std::string title){}

	ofWindowMode	getWindowMode() {return OF_WINDOW;}

	void	setFullscreen(bool fullscreen);
	void	toggleFullscreen();

	void	enableSetupScreen();
	void	disableSetupScreen();

	void			setOrientation(ofOrientation orientation);
	ofOrientation	getOrientation();

	ofCoreEvents & events();
	std::shared_ptr<ofBaseRenderer> & renderer();

	void	setThreadedEvents(bool threadedEvents);
	void 	setAccumulateTouchEvents(bool accumEvents);

	int 	getGlesVersion();

private:
	ofCoreEvents coreEvents;
	std::shared_ptr<ofBaseRenderer> currentRenderer;
	int glesVersion;
};
