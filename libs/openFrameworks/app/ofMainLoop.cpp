/*
 * ofMainLoop.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: arturo
 */

#include "ofMainLoop.h"
#include "ofBaseApp.h"
// TEST WITHOUT
#include "ofConstants.h"
#include "ofAppBaseWindow.h"

//========================================================================
// default windowing
#ifdef TARGET_NODISPLAY
	#include "ofAppNoWindow.h"
	typedef ofAppNoWindow ofWindow;

#elif defined(TARGET_OF_IOS)
	#include "ofAppiOSWindow.h"
	typedef ofAppiOSWindow ofWindow;

#elif defined(TARGET_ANDROID)
	#include "ofAppAndroidWindow.h"
	#include "ofxAndroidUtils.h"
	#include "ofxAndroidApp.h"
	typedef ofAppAndroidWindow ofWindow;

// Raspi removed to use GLFW
//#elif defined(TARGET_RASPBERRY_PI)
//	#include "ofAppEGLWindow.h"
#elif defined(TARGET_EMSCRIPTEN)
	#include "ofxAppEmscriptenWindow.h"
	typedef ofxAppEmscriptenWindow ofWindow;
#else
	#include "ofAppGLFWWindow.h"
	typedef ofAppGLFWWindow ofWindow;
#endif

ofMainLoop::ofMainLoop() : bShouldClose(false), status(0), allowMultiWindow(true), escapeQuits(true) {
}

ofMainLoop::~ofMainLoop() {
	exit();
}

std::shared_ptr<ofAppBaseWindow> ofMainLoop::createWindow(const ofWindowSettings & settings){
	std::shared_ptr<ofWindow> window { std::make_shared<ofWindow>() };
	addWindow(window);
	window->setup(settings);
	return window;
}

void ofMainLoop::run(
			const std::shared_ptr<ofAppBaseWindow> & window,
			const std::shared_ptr<ofBaseApp> & app) {
	mainApp = app;
//	windowsApps[window] = app;
	if(app){
		ofAddListener(window->events().setup,app.get(),&ofBaseApp::setup,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().update,app.get(),&ofBaseApp::update,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().draw,app.get(),&ofBaseApp::draw,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().exit,app.get(),&ofBaseApp::exit,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().keyPressed,app.get(),&ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().keyReleased,app.get(),&ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseMoved,app.get(),&ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseDragged,app.get(),&ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mousePressed,app.get(),&ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseReleased,app.get(),&ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseScrolled,app.get(),&ofBaseApp::mouseScrolled,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseEntered,app.get(),&ofBaseApp::mouseEntered,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseExited,app.get(),&ofBaseApp::mouseExited,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().windowResized,app.get(),&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().messageEvent,app.get(),&ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().fileDragEvent,app.get(),&ofBaseApp::dragged,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().touchCancelled,app.get(),&ofBaseApp::touchCancelled,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().touchDoubleTap,app.get(),&ofBaseApp::touchDoubleTap,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().touchDown,app.get(),&ofBaseApp::touchDown,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().touchMoved,app.get(),&ofBaseApp::touchMoved,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().touchUp,app.get(),&ofBaseApp::touchUp,OF_EVENT_ORDER_APP);
#ifdef TARGET_ANDROID
		auto androidApp = dynamic_cast<ofxAndroidApp*>(app.get());
		if(androidApp){
			ofAddListener(ofxAndroidEvents().okPressed,androidApp,&ofxAndroidApp::okPressed,OF_EVENT_ORDER_APP);
			ofAddListener(ofxAndroidEvents().cancelPressed,androidApp,&ofxAndroidApp::cancelPressed,OF_EVENT_ORDER_APP);
			ofAddListener(ofxAndroidEvents().backPressed,androidApp,&ofxAndroidApp::backPressed,OF_EVENT_ORDER_APP);
			ofAddListener(ofxAndroidEvents().networkConnected,androidApp,&ofxAndroidApp::networkConnectedEvent,OF_EVENT_ORDER_APP);
			ofAddListener(ofxAndroidEvents().pause,androidApp,&ofxAndroidApp::pause,OF_EVENT_ORDER_APP);
			ofAddListener(ofxAndroidEvents().resume,androidApp,&ofxAndroidApp::resume,OF_EVENT_ORDER_APP);
			ofAddListener(ofxAndroidEvents().unloadGL,androidApp,&ofxAndroidApp::unloadGL,OF_EVENT_ORDER_APP);
			ofAddListener(ofxAndroidEvents().reloadGL,androidApp,&ofxAndroidApp::reloadGL,OF_EVENT_ORDER_APP);
			ofAddListener(ofxAndroidEvents().swipe,androidApp,&ofxAndroidApp::swipe,OF_EVENT_ORDER_APP);
			ofAddListener(ofxAndroidEvents().deviceOrientationChanged,androidApp,&ofxAndroidApp::deviceOrientationChangedEvent,OF_EVENT_ORDER_APP);
		}
#endif
	}
	currentWindow = window;
	
	// FIXME: remove?
	window->makeCurrent();
	if(!windowLoop){
		window->events().notifySetup();
	}
}

void ofMainLoop::run(const std::shared_ptr<ofBaseApp> & app){
	if(!windows.empty()){
		run(windows[0], app);
	}
}

int ofMainLoop::loop(){
	if(!windowLoop){
		while(!bShouldClose && !windows.empty()){
			loopOnce();
			pollEvents();
		}
	}else{
		windowLoop();
	}
	return status;
}

void ofMainLoop::loopOnce(){
	if(bShouldClose) return;

	auto i = windows.begin();
	for ( ; i != windows.end(); ) {
		if (i->get()->getWindowShouldClose()) {
			i = windows.erase(i);
		} 
		else {
			currentWindow = (*i);
			i->get()->makeCurrent();
			i->get()->update();
			i->get()->draw();
		++i;
		}
	}
	
	loopEvent.notify(this);
}

void ofMainLoop::pollEvents(){
	if(windowPollEvents){
		windowPollEvents();
	}
}

void ofMainLoop::exit(){
	exitEvent.notify(this);

	for (auto window : windows) {
		if(window == nullptr) {
			continue;
		}

		ofEventArgs args;
		ofNotifyEvent(window->events().exit, args, this);

//		ofRemoveListener(window->events().setup,app.get(),&ofBaseApp::setup,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().update,app.get(),&ofBaseApp::update,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().draw,app.get(),&ofBaseApp::draw,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().exit,app.get(),&ofBaseApp::exit,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().keyPressed,app.get(),&ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().keyReleased,app.get(),&ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().mouseMoved,app.get(),&ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().mouseDragged,app.get(),&ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().mousePressed,app.get(),&ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().mouseReleased,app.get(),&ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().mouseScrolled,app.get(),&ofBaseApp::mouseScrolled,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().mouseEntered,app.get(),&ofBaseApp::mouseEntered,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().mouseExited,app.get(),&ofBaseApp::mouseExited,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().windowResized,app.get(),&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().messageEvent,app.get(),&ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().fileDragEvent,app.get(),&ofBaseApp::dragged,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().touchCancelled,app.get(),&ofBaseApp::touchCancelled,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().touchDoubleTap,app.get(),&ofBaseApp::touchDoubleTap,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().touchDown,app.get(),&ofBaseApp::touchDown,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().touchMoved,app.get(),&ofBaseApp::touchMoved,OF_EVENT_ORDER_APP);
//		ofRemoveListener(window->events().touchUp,app.get(),&ofBaseApp::touchUp,OF_EVENT_ORDER_APP);
#ifdef TARGET_ANDROID
		
		// FIXME: how will this work with Android?
//		auto androidApp = dynamic_cast<ofxAndroidApp*>(app.get());
//		if(androidApp){
//			ofRemoveListener(ofxAndroidEvents().okPressed,androidApp,&ofxAndroidApp::okPressed,OF_EVENT_ORDER_APP);
//			ofRemoveListener(ofxAndroidEvents().cancelPressed,androidApp,&ofxAndroidApp::cancelPressed,OF_EVENT_ORDER_APP);
//			ofRemoveListener(ofxAndroidEvents().backPressed,androidApp,&ofxAndroidApp::backPressed,OF_EVENT_ORDER_APP);
//			ofRemoveListener(ofxAndroidEvents().networkConnected,androidApp,&ofxAndroidApp::networkConnectedEvent,OF_EVENT_ORDER_APP);
//			ofRemoveListener(ofxAndroidEvents().pause,androidApp,&ofxAndroidApp::pause,OF_EVENT_ORDER_APP);
//			ofRemoveListener(ofxAndroidEvents().resume,androidApp,&ofxAndroidApp::resume,OF_EVENT_ORDER_APP);
//			ofRemoveListener(ofxAndroidEvents().unloadGL,androidApp,&ofxAndroidApp::unloadGL,OF_EVENT_ORDER_APP);
//			ofRemoveListener(ofxAndroidEvents().reloadGL,androidApp,&ofxAndroidApp::reloadGL,OF_EVENT_ORDER_APP);
//			ofRemoveListener(ofxAndroidEvents().swipe,androidApp,&ofxAndroidApp::swipe,OF_EVENT_ORDER_APP);
//			ofRemoveListener(ofxAndroidEvents().deviceOrientationChanged,androidApp,&ofxAndroidApp::deviceOrientationChangedEvent,OF_EVENT_ORDER_APP);
//		}
#endif
	}


	// reset applications then windows
	// so events are present until the
	// end of the application
//	for(auto & window_app: windowsApps){
//		window_app.second.reset();
//	}
//	windowsApps.clear();
	
	mainApp.reset();
	windows.clear();
}

std::shared_ptr<ofAppBaseWindow> ofMainLoop::getCurrentWindow(){
	return currentWindow.lock();
}

void ofMainLoop::setCurrentWindow(const std::shared_ptr<ofAppBaseWindow> & window){
	currentWindow = window;
}

void ofMainLoop::setCurrentWindow(ofAppBaseWindow * window){
	if(currentWindow.lock().get() == window){
		return;
	}
	
	for (const auto & w : windows) {
		if (w.get() == window) {
			currentWindow = w;
			break;
		}
	}
}

std::shared_ptr<ofBaseApp> ofMainLoop::getCurrentApp(){
//	return windowsApps[currentWindow.lock()];
	return mainApp;
}

void ofMainLoop::shouldClose(int _status){
//	for(auto i: windowsApps){
//		i.first->setWindowShouldClose();
//	}
	for(auto w : windows){
		w->setWindowShouldClose();
	}
	bShouldClose = true;
	status = _status;
}

void ofMainLoop::setEscapeQuitsLoop(bool quits){
	escapeQuits = quits;
}

void ofMainLoop::keyPressed(ofKeyEventArgs & key){
	if (key.key == OF_KEY_ESC && escapeQuits == true){				// "escape"
		shouldClose(0);
	}
}

int thisWindow = 0;
void ofMainLoop::ofBeginWindow(int n) {
	if (n < windows.size()) {
		thisWindow = n;
		currentWindow = windows[thisWindow];
		windows[thisWindow]->makeCurrent();
		windows[thisWindow]->update();
//		windows[n].draw();
		windows[thisWindow]->beginDraw();
		
	}
//		currentWindow.beginDraw();
}

void ofMainLoop::ofEndWindow() {
	windows[thisWindow]->endDraw();
	thisWindow = 0;
	currentWindow = windows[thisWindow];
	windows[thisWindow]->makeCurrent();
}
