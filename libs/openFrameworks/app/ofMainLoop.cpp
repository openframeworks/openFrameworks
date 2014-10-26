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
#elif defined(TARGET_RASPBERRY_PI)
	#include "ofAppEGLWindow.h"
#elif defined(TARGET_EMSCRIPTEN)
	#include "ofxAppEmscriptenWindow.h"
#else
	#include "ofAppGLFWWindow.h"
#endif


ofMainLoop::ofMainLoop()
:bShouldClose(false)
,status(0){

}

ofMainLoop::~ofMainLoop() {
}

shared_ptr<ofAppBaseWindow> ofMainLoop::createWindow(const ofWindowSettings & settings){
	shared_ptr<ofAppBaseWindow> window;
#ifdef TARGET_NODISPLAY
	window = shared_ptr<ofAppBaseWindow>(new ofAppNoWindow());
#else
	#if defined(TARGET_OF_IOS)
	window = shared_ptr<ofAppBaseGLESWindow>(new ofAppiOSWindow());
	#elif defined(TARGET_ANDROID)
	window = shared_ptr<ofAppBaseGLESWindow>(new ofAppAndroidWindow());
	#elif defined(TARGET_RASPBERRY_PI)
	window = shared_ptr<ofAppBaseGLESWindow>(new ofAppEGLWindow());
	#elif defined(TARGET_EMSCRIPTEN)
	window = shared_ptr<ofAppBaseGLESWindow>(new ofxAppEmscriptenWindow);
	#elif defined(TARGET_OPENGLES)
	window = shared_ptr<ofAppBaseGLESWindow>(new ofAppGLFWWindow());
	#else
	window = shared_ptr<ofAppBaseGLWindow>(new ofAppGLFWWindow());
	#endif
#endif
	currentWindow = window;
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
		ofAddListener(window->events().windowEntered,app.get(),&ofBaseApp::windowEntry,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().windowResized,app.get(),&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().messageEvent,app.get(),&ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().fileDragEvent,app.get(),&ofBaseApp::dragged,OF_EVENT_ORDER_APP);
	}
	currentWindow = window;
    window->events().notifySetup();
}

int ofMainLoop::loop(){
	while(!bShouldClose && !windowsApps.empty()){
		for(auto i = windowsApps.begin();i!=windowsApps.end();i++){
			if(i->first->windowShouldClose()){
				cout << "destroiying " << i->first.get() << endl;
				i->first->close();
				windowsApps.erase(i);
			}else{
				currentWindow = i->first;
				i->first->update();
				i->first->draw();
			}
		}
	}
	return status;
}

void ofMainLoop::loopOnce(){

}

shared_ptr<ofAppBaseWindow> ofMainLoop::getCurrentWindow(){
	return currentWindow;
}

shared_ptr<ofBaseApp> ofMainLoop::getCurrentApp(){
	return windowsApps[currentWindow];
}

ofCoreEvents & ofMainLoop::events(){
	return currentWindow->events();
}

void ofMainLoop::shouldClose(int _status){
	for(auto i = windowsApps.begin();i!=windowsApps.end();i++){
		i->first->windowShouldClose();
	}
	bShouldClose = true;
	status = _status;
}
