/*
 * ofAppEmscriptenWindow.cpp
 *
 *  Created on: May 8, 2014
 *      Author: arturo
 */

#include "ofxAppEmscriptenWindow.h"
#include "ofLog.h"
#include "ofEvents.h"
#include "ofGLProgrammableRenderer.h"

ofxAppEmscriptenWindow * ofxAppEmscriptenWindow::instance = NULL;

// from http://cantuna.googlecode.com/svn-history/r16/trunk/src/screen.cpp
#define CASE_STR(x,y) case x: str = y; break

static const char* eglErrorString(EGLint err) {
    string str;
    switch (err) {
        CASE_STR(EGL_SUCCESS, "no error");
        CASE_STR(EGL_NOT_INITIALIZED, "EGL not, or could not be, initialized");
        CASE_STR(EGL_BAD_ACCESS, "access violation");
        CASE_STR(EGL_BAD_ALLOC, "could not allocate resources");
        CASE_STR(EGL_BAD_ATTRIBUTE, "invalid attribute");
        CASE_STR(EGL_BAD_CONTEXT, "invalid context specified");
        CASE_STR(EGL_BAD_CONFIG, "invald frame buffer configuration specified");
        CASE_STR(EGL_BAD_CURRENT_SURFACE, "current window, pbuffer or pixmap surface is no longer valid");
        CASE_STR(EGL_BAD_DISPLAY, "invalid display specified");
        CASE_STR(EGL_BAD_SURFACE, "invalid surface specified");
        CASE_STR(EGL_BAD_MATCH, "bad argument match");
        CASE_STR(EGL_BAD_PARAMETER, "invalid paramater");
        CASE_STR(EGL_BAD_NATIVE_PIXMAP, "invalid NativePixmap");
        CASE_STR(EGL_BAD_NATIVE_WINDOW, "invalid NativeWindow");
        CASE_STR(EGL_CONTEXT_LOST, "APM event caused context loss");
        default: str = "unknown error " + ofToString(err); break;
    }
    return str.c_str();
}

ofxAppEmscriptenWindow::ofxAppEmscriptenWindow()
:display(NULL)
,context(NULL)
,surface(NULL)
,bEnableSetupScreen(true){
	instance = this;

}

ofxAppEmscriptenWindow::~ofxAppEmscriptenWindow() {
	// TODO Auto-generated destructor stub
}

void ofxAppEmscriptenWindow::setup(const ofGLESWindowSettings & settings){
	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
	EGLint attribList[] =
	   {
		   EGL_RED_SIZE, 8,
		   EGL_GREEN_SIZE, 8,
		   EGL_BLUE_SIZE, 8,
		   EGL_ALPHA_SIZE, 8,
		   EGL_DEPTH_SIZE, 8,
		   EGL_STENCIL_SIZE, EGL_DONT_CARE,
		   EGL_SAMPLE_BUFFERS, EGL_DONT_CARE,
		   EGL_NONE
	   };

	// Get Display
	display = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
	if ( display == EGL_NO_DISPLAY ){
		ofLogError() << "coudln't get display";
		return;
	}

	// Initialize EGL
	if ( !eglInitialize(display, &majorVersion, &minorVersion) ){
		ofLogError() << "couldn't initialize display";
		return;
	}

	// Get configs
	if ( !eglGetConfigs(display, NULL, 0, &numConfigs) ){
		ofLogError() << "couldn't get configs";
		return;
	}

	// Choose config
	if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) ){
		ofLogError() << "couldn't choose display";
		return;
	}

	// Create a surface
	surface = eglCreateWindowSurface(display, config, NULL, NULL);
	if ( surface == EGL_NO_SURFACE ){
		ofLogError() << "couldn't create surface";
		return;
	}

	// Create a GL context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
	if ( context == EGL_NO_CONTEXT )
	{
	  return;
	}

	// Make the context current
	if ( !eglMakeCurrent(display, surface, surface, context) ){
		ofLogError() << "couldn't make current display";
		return;
	}

	setWindowShape(settings.width,settings.height);

	_renderer = make_shared<ofGLProgrammableRenderer>(this);
	((ofGLProgrammableRenderer*)_renderer.get())->setup(2,0);

    emscripten_set_keydown_callback(0,this,1,&keydown_cb);
    emscripten_set_keyup_callback(0,this,1,&keyup_cb);
    emscripten_set_mousedown_callback(0,this,1,&mousedown_cb);
    emscripten_set_mouseup_callback(0,this,1,&mouseup_cb);
    emscripten_set_mousemove_callback(0,this,1,&mousemoved_cb);
}

void ofxAppEmscriptenWindow::loop(){

	instance->events().notifySetup();


	// Emulate loop via callbacks
	emscripten_set_main_loop( display_cb, -1, 1 );
}

void ofxAppEmscriptenWindow::update(){
	events().notifyUpdate();
}

void ofxAppEmscriptenWindow::draw(){
	///////////////////////////////////////////////////////////////////////////////////////
	// set viewport, clear the screen
	renderer()->startRender();
	if( bEnableSetupScreen ) renderer()->setupScreen();

	events().notifyDraw();

	renderer()->finishRender();


	EGLBoolean success = eglSwapBuffers( display, surface );
	if( !success ) {
		EGLint error = eglGetError();
		ofLogNotice("of::emscripten::EGLPage") << "display(): eglSwapBuffers failed: " << eglErrorString(error);
	}
}

void ofxAppEmscriptenWindow::display_cb(){
	if(instance){
		instance->update();
		instance->draw();
	}
}

int ofxAppEmscriptenWindow::keydown_cb(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData){
	int key = keyEvent->key[0];
	if(key==0){
		key = keyEvent->which + 32;
	}
	instance->events().notifyKeyPressed(key);
	return 0;
}

int ofxAppEmscriptenWindow::keyup_cb(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData){
	int key = keyEvent->key[0];
	if(key==0){
		key = keyEvent->which + 32;
	}
	instance->events().notifyKeyReleased(key);
	return 0;
}

int ofxAppEmscriptenWindow::mousedown_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	instance->events().notifyMousePressed(ofGetMouseX(),ofGetMouseY(),mouseEvent->button);
	return 0;
}

int ofxAppEmscriptenWindow::mouseup_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	instance->events().notifyMouseReleased(ofGetMouseX(),ofGetMouseY(),mouseEvent->button);
	return 0;

}

int ofxAppEmscriptenWindow::mousemoved_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	if(ofGetMousePressed()){
		instance->events().notifyMouseDragged(mouseEvent->canvasX,mouseEvent->canvasY,0);
	}else{
		instance->events().notifyMouseMoved(mouseEvent->canvasX,mouseEvent->canvasY);
	}
	return 0;

}

void ofxAppEmscriptenWindow::hideCursor(){
	emscripten_hide_mouse();
}


void ofxAppEmscriptenWindow::showCursor(){

}

void ofxAppEmscriptenWindow::setWindowPosition(int x, int y){

}

void ofxAppEmscriptenWindow::setWindowShape(int w, int h){
	emscripten_set_canvas_size(w,h);
}



ofPoint	ofxAppEmscriptenWindow::getWindowPosition(){
	return ofVec2f(0,0);
}


ofPoint	ofxAppEmscriptenWindow::getWindowSize(){
	int width;
	int height;
	int isFullscreen;
	emscripten_get_canvas_size( &width, &height, &isFullscreen );
	return ofVec2f(width,height);
}

ofPoint	ofxAppEmscriptenWindow::getScreenSize(){
	return getWindowSize();
}

void ofxAppEmscriptenWindow::setOrientation(ofOrientation orientation){

}

ofOrientation ofxAppEmscriptenWindow::getOrientation(){
	return OF_ORIENTATION_DEFAULT;
}

bool ofxAppEmscriptenWindow::doesHWOrientation(){
	return false;
}

//this is used by ofGetWidth and now determines the window width based on orientation
int	ofxAppEmscriptenWindow::getWidth(){
	return getWindowSize().x;
}

int	ofxAppEmscriptenWindow::getHeight(){
	return getWindowSize().y;
}

void ofxAppEmscriptenWindow::setWindowTitle(string title){

}

ofWindowMode ofxAppEmscriptenWindow::getWindowMode(){
	return OF_WINDOW;
}

void ofxAppEmscriptenWindow::setFullscreen(bool fullscreen){
	if(fullscreen){
		emscripten_request_fullscreen(0,1);
	}else{
		emscripten_exit_fullscreen();
	}
}

void ofxAppEmscriptenWindow::toggleFullscreen(){
	EmscriptenFullscreenChangeEvent fullscreenStatus;
	emscripten_get_fullscreen_status(&fullscreenStatus);
	if(fullscreenStatus.isFullscreen){
		setFullscreen(false);
	}else if(fullscreenStatus.fullscreenEnabled){
		setFullscreen(true);
	}
}

void ofxAppEmscriptenWindow::enableSetupScreen(){
	bEnableSetupScreen = true;
}


void ofxAppEmscriptenWindow::disableSetupScreen(){
	bEnableSetupScreen = false;
}

void ofxAppEmscriptenWindow::setVerticalSync(bool enabled){
	eglSwapInterval(display, enabled ? 1 : 0);
}

EGLDisplay ofxAppEmscriptenWindow::getEGLDisplay(){
	return display;
}


EGLContext ofxAppEmscriptenWindow::getEGLContext(){
	return context;
}

EGLSurface ofxAppEmscriptenWindow::getEGLSurface(){
	return surface;
}

ofCoreEvents & ofxAppEmscriptenWindow::events(){
	return _events;
}

shared_ptr<ofBaseRenderer> & ofxAppEmscriptenWindow::renderer(){
	return _renderer;
}

