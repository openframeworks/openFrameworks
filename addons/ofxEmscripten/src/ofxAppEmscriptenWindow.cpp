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

using namespace std;

ofxAppEmscriptenWindow * ofxAppEmscriptenWindow::instance = NULL;


ofxAppEmscriptenWindow::ofxAppEmscriptenWindow(){
	instance = this;
}

ofxAppEmscriptenWindow::~ofxAppEmscriptenWindow() {
	if(context != 0){
		emscripten_webgl_destroy_context(context);	
	}
}

void ofxAppEmscriptenWindow::setup(const ofGLESWindowSettings & settings){
    setWindowShape(settings.getWidth(),settings.getHeight());
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);

/// when setting explicitSwapControl to 0 it is emscripten that is in charge of swapping on each render call.
    attrs.explicitSwapControl = 0;
    attrs.depth = 1;
    attrs.stencil = 1;
    attrs.antialias = 1;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    attrs.alpha = 0;

    context = emscripten_webgl_create_context("#canvas", &attrs);
    assert(context);
	  
    makeCurrent();

    _renderer = make_shared<ofGLProgrammableRenderer>(this);
    ((ofGLProgrammableRenderer*)_renderer.get())->setup(2,0);

    emscripten_set_keydown_callback("#canvas",this,1,&keydown_cb);
    emscripten_set_keyup_callback("#canvas",this,1,&keyup_cb);
    
    emscripten_set_mousedown_callback("#canvas",this,1,&mousedown_cb);
    emscripten_set_mouseup_callback("#canvas",this,1,&mouseup_cb);
    emscripten_set_mousemove_callback("#canvas",this,1,&mousemoved_cb);
    emscripten_set_mouseenter_callback("#canvas",this,1,&mouseenter_cb);
    emscripten_set_mouseleave_callback("#canvas",this,1,&mouseleave_cb);

    emscripten_set_touchstart_callback("#canvas",this,1,&touch_cb);
    emscripten_set_touchend_callback("#canvas",this,1,&touch_cb);
    emscripten_set_touchmove_callback("#canvas",this,1,&touch_cb);
    emscripten_set_touchcancel_callback("#canvas",this,1,&touch_cb);

    emscripten_set_focus_callback("#canvas",this,1,&focus_cb);    
    emscripten_set_blur_callback("#canvas",this,1,&blur_cb);
    emscripten_set_focusin_callback("#canvas",this,1,&focusin_cb);
    emscripten_set_focusout_callback("#canvas",this,1,&focusout_cb);
}

void ofxAppEmscriptenWindow::loop(){
	instance->events().notifySetup();

	// Emulate loop via callbacks
	emscripten_set_main_loop( display_cb, -1, 1);
}

void ofxAppEmscriptenWindow::update(){
        if (bSetMainLoopTiming) {
		// choose the render speed with the second argument. 1 is for every screen refresh, 2 for every second, 3 for every third. and so on...
		emscripten_set_main_loop_timing(1, 2);
		bSetMainLoopTiming = false;
	}
	events().notifyUpdate();
}

void ofxAppEmscriptenWindow::draw(){
	// set viewport, clear the screen
	renderer()->startRender();
	if( bEnableSetupScreen ) renderer()->setupScreen();

	events().notifyDraw();

	renderer()->finishRender();
}

void ofxAppEmscriptenWindow::display_cb(){
	if(instance){
		instance->update();
		instance->draw();
	}
}

int ofxAppEmscriptenWindow::keydown_cb(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData){
	int key = keyEvent->key[0];
	std::string id = keyEvent->key;
	if(key == 0){
		key = keyEvent->which + 32;
	}else if(id == "Enter"){
		key = OF_KEY_RETURN;
	}else if(id == "Escape"){
		key = OF_KEY_ESC;
	}else if(id == "Tab"){
		key = OF_KEY_TAB;
	}else if(id == "Backspace"){
		key = OF_KEY_BACKSPACE;
	}else if(id == "Delete"){
		key = OF_KEY_DEL;
	}else if(id == "Shift"){
		key = OF_KEY_SHIFT;
	}else if(id == "Control"){
		key = OF_KEY_CONTROL;
	}else if(id == "Meta"){
		key = OF_KEY_SUPER;
	}else if(id == "Alt"){
		key = OF_KEY_LEFT_ALT;
	}else if(id == "AltGraph"){
		key = OF_KEY_RIGHT_ALT;
	}else if(id == "F1"){
		key = OF_KEY_F1;
	}else if(id == "F2"){
		key = OF_KEY_F2;
	}else if(id == "F3"){
		key = OF_KEY_F3;
	}else if(id == "F4"){
		key = OF_KEY_F4;
	}else if(id == "F5"){
		key = OF_KEY_F5;
	}else if(id == "F6"){
		key = OF_KEY_F6;
	}else if(id == "F7"){
		key = OF_KEY_F7;
	}else if(id == "F8"){
		key = OF_KEY_F8;
	}else if(id == "F9"){
		key = OF_KEY_F9;
	}else if(id == "F10"){
		key = OF_KEY_F10;
	}else if(id == "F11"){
		key = OF_KEY_F11;
	}else if(id == "F12"){
		key = OF_KEY_F12;
	}else if(id == "ArrowLeft"){
		key = OF_KEY_LEFT;
	}else if(id == "ArrowUp"){
		key = OF_KEY_UP;
	}else if(id == "ArrowRight"){
		key = OF_KEY_RIGHT;
	}else if(id == "ArrowDown"){
		key = OF_KEY_DOWN;
	}else if(id == "PageUp"){
		key = OF_KEY_PAGE_UP;
	}else if(id == "PageDown"){
		key = OF_KEY_PAGE_DOWN;
	}else if(id == "Home"){
		key = OF_KEY_HOME;
	}else if(id == "End"){
		key = OF_KEY_END;
	}else if(id == "Insert"){
		key = OF_KEY_INSERT;
	}else if(id == "ß"){
		key = 223;
	}else if(id == "§"){
		key = 167;
	}else if(id == "ä"){
		key = 228;
	}else if(id == "Ä"){
		key = 196;
	}else if(id == "ö"){
		key = 246;
	}else if(id == "Ö"){
		key = 214;
	}else if(id == "ü"){
		key = 252;
	}else if(id == "Ü"){
		key = 220;
	}else if(id == "Dead" || id == "NumLock" || id == "ScrollLock" || id == "CapsLock" || id == "Pause" || key < 0){
		return 0;
	}
	instance->events().notifyKeyPressed(key);
	return 0;
}

int ofxAppEmscriptenWindow::keyup_cb(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData){
	int key = keyEvent->key[0];
	std::string id = keyEvent->key;
	if(key == 0){
		key = keyEvent->which + 32;
	}else if(id == "Enter"){
		key = OF_KEY_RETURN;
	}else if(id == "Escape"){
		key = OF_KEY_ESC;
	}else if(id == "Tab"){
		key = OF_KEY_TAB;
	}else if(id == "Backspace"){
		key = OF_KEY_BACKSPACE;
	}else if(id == "Delete"){
		key = OF_KEY_DEL;
	}else if(id == "Shift"){
		key = OF_KEY_SHIFT;
	}else if(id == "Control"){
		key = OF_KEY_CONTROL;
	}else if(id == "Meta"){
		key = OF_KEY_SUPER;
	}else if(id == "Alt"){
		key = OF_KEY_LEFT_ALT;
	}else if(id == "AltGraph"){
		key = OF_KEY_RIGHT_ALT;
	}else if(id == "F1"){
		key = OF_KEY_F1;
	}else if(id == "F2"){
		key = OF_KEY_F2;
	}else if(id == "F3"){
		key = OF_KEY_F3;
	}else if(id == "F4"){
		key = OF_KEY_F4;
	}else if(id == "F5"){
		key = OF_KEY_F5;
	}else if(id == "F6"){
		key = OF_KEY_F6;
	}else if(id == "F7"){
		key = OF_KEY_F7;
	}else if(id == "F8"){
		key = OF_KEY_F8;
	}else if(id == "F9"){
		key = OF_KEY_F9;
	}else if(id == "F10"){
		key = OF_KEY_F10;
	}else if(id == "F11"){
		key = OF_KEY_F11;
	}else if(id == "F12"){
		key = OF_KEY_F12;
	}else if(id == "ArrowLeft"){
		key = OF_KEY_LEFT;
	}else if(id == "ArrowUp"){
		key = OF_KEY_UP;
	}else if(id == "ArrowRight"){
		key = OF_KEY_RIGHT;
	}else if(id == "ArrowDown"){
		key = OF_KEY_DOWN;
	}else if(id == "PageUp"){
		key = OF_KEY_PAGE_UP;
	}else if(id == "PageDown"){
		key = OF_KEY_PAGE_DOWN;
	}else if(id == "Home"){
		key = OF_KEY_HOME;
	}else if(id == "End"){
		key = OF_KEY_END;
	}else if(id == "Insert"){
		key = OF_KEY_INSERT;
	}else if(id == "ß"){
		key = 223;
	}else if(id == "§"){
		key = 167;
	}else if(id == "ä"){
		key = 228;
	}else if(id == "Ä"){
		key = 196;
	}else if(id == "ö"){
		key = 246;
	}else if(id == "Ö"){
		key = 214;
	}else if(id == "ü"){
		key = 252;
	}else if(id == "Ü"){
		key = 220;
	}else if(id == "Dead" || id == "NumLock" || id == "ScrollLock" || id == "CapsLock" || id == "Pause" || key < 0){
		return 0;
	}
	instance->events().notifyKeyReleased(key);
	return 0;
}

int ofxAppEmscriptenWindow::mousedown_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	int canvas_width, canvas_height;
	emscripten_get_canvas_element_size("#canvas", &canvas_width, &canvas_height);
        double css_width, css_height;
	emscripten_get_element_css_size("#canvas", &css_width, &css_height);
	instance->events().notifyMousePressed(mouseEvent->targetX * (canvas_width / css_width), mouseEvent->targetY * (canvas_height / css_height), mouseEvent->button);
	return 0;
}

int ofxAppEmscriptenWindow::mouseup_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	int canvas_width, canvas_height;
	emscripten_get_canvas_element_size("#canvas", &canvas_width, &canvas_height);
        double css_width, css_height;
	emscripten_get_element_css_size("#canvas", &css_width, &css_height);
	instance->events().notifyMouseReleased(mouseEvent->targetX * (canvas_width / css_width), mouseEvent->targetY * (canvas_height / css_height), mouseEvent->button);
	return 0;
}

int ofxAppEmscriptenWindow::mousemoved_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	int canvas_width, canvas_height;
	emscripten_get_canvas_element_size("#canvas", &canvas_width, &canvas_height);
        double css_width, css_height;
	emscripten_get_element_css_size("#canvas", &css_width, &css_height);
	if(ofGetMousePressed()){
		instance->events().notifyMouseDragged(mouseEvent->targetX * (canvas_width / css_width), mouseEvent->targetY * (canvas_height / css_height),0);
	}else{
		instance->events().notifyMouseMoved(mouseEvent->targetX * (canvas_width / css_width), mouseEvent->targetY * (canvas_height / css_height));
	}
	return 0;
}

int ofxAppEmscriptenWindow::mouseenter_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	int canvas_width, canvas_height;
	emscripten_get_canvas_element_size("#canvas", &canvas_width, &canvas_height);
        double css_width, css_height;
	emscripten_get_element_css_size("#canvas", &css_width, &css_height);
	instance->events().notifyMouseEntered(mouseEvent->targetX * (canvas_width / css_width), mouseEvent->targetY * (canvas_height / css_height));
	if(mouseEvent->buttons == 0){
		instance->events().notifyMouseReleased(mouseEvent->targetX * (canvas_width / css_width), mouseEvent->targetY * (canvas_height / css_height), mouseEvent->button);
	}	
	return 0;
}

int ofxAppEmscriptenWindow::mouseleave_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	int canvas_width, canvas_height;
	emscripten_get_canvas_element_size("#canvas", &canvas_width, &canvas_height);
        double css_width, css_height;
	emscripten_get_element_css_size("#canvas", &css_width, &css_height);
	instance->events().notifyMouseExited(mouseEvent->targetX * (canvas_width / css_width), mouseEvent->targetY * (canvas_height / css_height));
	return 0;
}

int ofxAppEmscriptenWindow::touch_cb(int eventType, const EmscriptenTouchEvent* e, void* userData) {
	int canvas_width, canvas_height;
	emscripten_get_canvas_element_size("#canvas", &canvas_width, &canvas_height);
        double css_width, css_height;
	emscripten_get_element_css_size("#canvas", &css_width, &css_height);
        ofTouchEventArgs::Type touchArgsType;
        switch (eventType) {
                    case EMSCRIPTEN_EVENT_TOUCHSTART:
                        touchArgsType = ofTouchEventArgs::down;
                        break;
                    case EMSCRIPTEN_EVENT_TOUCHEND:
                        touchArgsType = ofTouchEventArgs::up;
                        break;
                    case EMSCRIPTEN_EVENT_TOUCHMOVE:
                        touchArgsType = ofTouchEventArgs::move;
                        break;
                    case EMSCRIPTEN_EVENT_TOUCHCANCEL:
                        touchArgsType = ofTouchEventArgs::cancel;
                        break;
                    default:
                        return 1;
            }
        int numTouches = e->numTouches;
        for (int i = 0; i < numTouches; i++) {
                ofTouchEventArgs touchArgs;
                touchArgs.type = touchArgsType;
                touchArgs.id = i;
                touchArgs.x =  std::ceil(e->touches[i].targetX * (canvas_width / css_width));
                touchArgs.y =  std::ceil(e->touches[i].targetY* (canvas_height / css_height));
                instance->events().notifyTouchEvent(touchArgs);
           }
    return 0;
}

int ofxAppEmscriptenWindow::focus_cb(int eventType, const EmscriptenFocusEvent *focusEvent, void *userData){
	return 0;
}

int ofxAppEmscriptenWindow::blur_cb(int eventType, const EmscriptenFocusEvent *focusEvent, void *userData){
	return 0;
}

int ofxAppEmscriptenWindow::focusin_cb(int eventType, const EmscriptenFocusEvent *focusEvent, void *userData){
	return 0;
}

int ofxAppEmscriptenWindow::focusout_cb(int eventType, const EmscriptenFocusEvent *focusEvent, void *userData){
	return 0;
}

void ofxAppEmscriptenWindow::hideCursor(){
	emscripten_hide_mouse();
}

void ofxAppEmscriptenWindow::setWindowShape(int w, int h){
    emscripten_set_canvas_size(w,h);
}

glm::vec2 ofxAppEmscriptenWindow::getWindowPosition(){
	return glm::vec2(0,0);
}

glm::vec2 ofxAppEmscriptenWindow::getWindowSize(){
	int width;
	int height;
	emscripten_get_canvas_element_size("#canvas", &width, &height);
	return glm::vec2(width,height);
}

glm::vec2 ofxAppEmscriptenWindow::getScreenSize(){
	return getWindowSize();
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

ofWindowMode ofxAppEmscriptenWindow::getWindowMode(){
	return OF_WINDOW;
}

void ofxAppEmscriptenWindow::setFullscreen(bool fullscreen){
	if(fullscreen){
		emscripten_request_fullscreen("#canvas",1);
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

ofCoreEvents & ofxAppEmscriptenWindow::events(){
	return _events;
}

shared_ptr<ofBaseRenderer> & ofxAppEmscriptenWindow::renderer(){
	return _renderer;
}

void ofxAppEmscriptenWindow::makeCurrent(){
	if(context != 0){
		emscripten_webgl_make_context_current(context);	
	}
}

void ofxAppEmscriptenWindow::startRender(){
	renderer()->startRender();
}

void ofxAppEmscriptenWindow::finishRender(){
	renderer()->finishRender();
}
