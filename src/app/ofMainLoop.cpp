/*
 * ofMainLoop.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: arturo
 */

#include <ofMainLoop.h>
#include "ofWindowSettings.h"
#include "ofConstants.h"

//========================================================================
// default windowing
#ifdef TARGET_NODISPLAY
	#include "ofAppNoWindow.h"
#elif defined(TARGET_OF_IOS)
	#include "ofAppiOSWindow.h"
#elif defined(TARGET_ANDROID)
	#include "ofAppAndroidWindow.h"
	#include "ofxAndroidUtils.h"
	#include "ofxAndroidApp.h"
#elif defined(TARGET_RASPBERRY_PI)
	#include "ofAppEGLWindow.h"
#elif defined(TARGET_EMSCRIPTEN)
	#include "ofxAppEmscriptenWindow.h"
#else
	#include "ofAppGLFWWindow.h"
#endif


ofMainLoop::ofMainLoop()
:bShouldClose(false)
,status(0)
,allowMultiWindow(true)
,escapeQuits(true){

}

ofMainLoop::~ofMainLoop() {
	exit();
}

shared_ptr<ofAppBaseWindow> ofMainLoop::createWindow(const ofWindowSettings & settings){
#ifdef TARGET_NODISPLAY
	shared_ptr<ofAppNoWindow> window = shared_ptr<ofAppNoWindow>(new ofAppNoWindow());
#else
	#if defined(TARGET_OF_IOS)
	shared_ptr<ofAppiOSWindow> window = shared_ptr<ofAppiOSWindow>(new ofAppiOSWindow());
	#elif defined(TARGET_ANDROID)
	shared_ptr<ofAppAndroidWindow> window = shared_ptr<ofAppAndroidWindow>(new ofAppAndroidWindow());
	#elif defined(TARGET_RASPBERRY_PI)
	shared_ptr<ofAppEGLWindow> window = shared_ptr<ofAppEGLWindow>(new ofAppEGLWindow());
	#elif defined(TARGET_EMSCRIPTEN)
	shared_ptr<ofxAppEmscriptenWindow> window = shared_ptr<ofxAppEmscriptenWindow>(new ofxAppEmscriptenWindow);
	#elif defined(TARGET_OPENGLES)
	shared_ptr<ofAppGLFWWindow> window = shared_ptr<ofAppGLFWWindow>(new ofAppGLFWWindow());
	#else
	shared_ptr<ofAppGLFWWindow> window = shared_ptr<ofAppGLFWWindow>(new ofAppGLFWWindow());
	#endif
#endif
	addWindow(window);
	window->setup(settings);
	return window;
}

void ofMainLoop::run(shared_ptr<ofAppBaseWindow> window, shared_ptr<ofBaseApp> app){
	windowsApps[window] = app;
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
		}
#endif
	}
	currentWindow = window;
	window->makeCurrent();
	if(!windowLoop){
		window->events().notifySetup();
	}
}

void ofMainLoop::run(shared_ptr<ofBaseApp> app){
	if(!windowsApps.empty()){
		run(windowsApps.begin()->first,app);
	}
}

int ofMainLoop::loop(){
	if(!windowLoop){
		while(!bShouldClose && !windowsApps.empty()){
			loopOnce();
			pollEvents();
		}
		exit();
	}else{
		windowLoop();
	}
	return status;
}

void ofMainLoop::loopOnce(){
	for(auto i = windowsApps.begin(); !windowsApps.empty() && i != windowsApps.end();){
		if(i->first->getWindowShouldClose()){
			i->first->close();
			windowsApps.erase(i++); ///< i now points at the window after the one which was just erased
		}else{
			currentWindow = i->first;
			i->first->makeCurrent();
			i->first->update();
			i->first->draw();
			i++; ///< continue to next window
		}
	}
}

void ofMainLoop::pollEvents(){
	if(windowPollEvents){
		windowPollEvents();
	}
}

void ofMainLoop::exit(){
	for(auto i: windowsApps){
		shared_ptr<ofAppBaseWindow> window = i.first;
		shared_ptr<ofBaseApp> app = i.second;
		
		if(window == nullptr) {
			continue;
		}
		if(app == nullptr) {
			continue;
		}
		
		ofRemoveListener(window->events().setup,app.get(),&ofBaseApp::setup,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().update,app.get(),&ofBaseApp::update,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().draw,app.get(),&ofBaseApp::draw,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().exit,app.get(),&ofBaseApp::exit,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().keyPressed,app.get(),&ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().keyReleased,app.get(),&ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseMoved,app.get(),&ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseDragged,app.get(),&ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mousePressed,app.get(),&ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseReleased,app.get(),&ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseScrolled,app.get(),&ofBaseApp::mouseScrolled,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseEntered,app.get(),&ofBaseApp::mouseEntered,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseExited,app.get(),&ofBaseApp::mouseExited,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().windowResized,app.get(),&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().messageEvent,app.get(),&ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().fileDragEvent,app.get(),&ofBaseApp::dragged,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().touchCancelled,app.get(),&ofBaseApp::touchCancelled,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().touchDoubleTap,app.get(),&ofBaseApp::touchDoubleTap,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().touchDown,app.get(),&ofBaseApp::touchDown,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().touchMoved,app.get(),&ofBaseApp::touchMoved,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().touchUp,app.get(),&ofBaseApp::touchUp,OF_EVENT_ORDER_APP);
#ifdef TARGET_ANDROID
		auto androidApp = dynamic_cast<ofxAndroidApp*>(app.get());
		if(androidApp){
			ofRemoveListener(ofxAndroidEvents().okPressed,androidApp,&ofxAndroidApp::okPressed,OF_EVENT_ORDER_APP);
			ofRemoveListener(ofxAndroidEvents().cancelPressed,androidApp,&ofxAndroidApp::cancelPressed,OF_EVENT_ORDER_APP);
			ofRemoveListener(ofxAndroidEvents().backPressed,androidApp,&ofxAndroidApp::backPressed,OF_EVENT_ORDER_APP);
			ofRemoveListener(ofxAndroidEvents().networkConnected,androidApp,&ofxAndroidApp::networkConnectedEvent,OF_EVENT_ORDER_APP);
			ofRemoveListener(ofxAndroidEvents().pause,androidApp,&ofxAndroidApp::pause,OF_EVENT_ORDER_APP);
			ofRemoveListener(ofxAndroidEvents().resume,androidApp,&ofxAndroidApp::resume,OF_EVENT_ORDER_APP);
			ofRemoveListener(ofxAndroidEvents().unloadGL,androidApp,&ofxAndroidApp::unloadGL,OF_EVENT_ORDER_APP);
			ofRemoveListener(ofxAndroidEvents().reloadGL,androidApp,&ofxAndroidApp::reloadGL,OF_EVENT_ORDER_APP);
			ofRemoveListener(ofxAndroidEvents().swipe,androidApp,&ofxAndroidApp::swipe,OF_EVENT_ORDER_APP);
		}
#endif
	}
	
	exitEvent.notify(this);
	windowsApps.clear();
}

shared_ptr<ofAppBaseWindow> ofMainLoop::getCurrentWindow(){
	return currentWindow;
}

void ofMainLoop::setCurrentWindow(shared_ptr<ofAppBaseWindow> window){
	currentWindow = window;
}

void ofMainLoop::setCurrentWindow(ofAppBaseWindow * window){
	if(currentWindow.get() == window){
		return;
	}
	for(auto i: windowsApps){
		if(i.first.get() == window){
			currentWindow = i.first;
			break;
		}
	}
}

shared_ptr<ofBaseApp> ofMainLoop::getCurrentApp(){
	return windowsApps[currentWindow];
}

ofCoreEvents & ofMainLoop::events(){
	return currentWindow->events();
}

void ofMainLoop::shouldClose(int _status){
	for(auto i: windowsApps){
		i.first->setWindowShouldClose();
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
