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

//------------------------------------------------------------
ofxAppEmscriptenWindow::ofxAppEmscriptenWindow(){
	instance = this;
}

//------------------------------------------------------------
ofxAppEmscriptenWindow::~ofxAppEmscriptenWindow() {
	if(context != 0){
		emscripten_webgl_destroy_context(context);	
	}
}

//------------------------------------------------------------
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

    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&keydown_cb);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&keyup_cb);
    
    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&mousedown_cb);
    emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&mouseup_cb);
    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&mousemoved_cb);
    emscripten_set_mouseenter_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&mouseenter_cb);
    emscripten_set_mouseleave_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&mouseleave_cb);

    emscripten_set_touchstart_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&touch_cb);
    emscripten_set_touchend_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&touch_cb);
    emscripten_set_touchmove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&touch_cb);
    emscripten_set_touchcancel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&touch_cb);
	
    emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,this,1,&mousescrolled_cb);
	
	// the following locks up the window for some reason.....
	//emscripten_set_resize_callback(const char *target, void *userData, EM_BOOL useCapture, em_ui_callback_func callback)
//	emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, 1, &emscripten_window_resized_callback );
	
	mStartWindowMode = settings.windowMode;
	
	if( settings.windowMode == OF_FULLSCREEN || settings.windowMode == OF_GAME_MODE ) {
		mStartWindowMode = OF_GAME_MODE;
		mTargetWindowMode = OF_GAME_MODE;
	}
	
	mCurrentWindowMode = OF_WINDOW;
	
	nFramesSinceWindowResized = 0;
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::loop(){

	instance->events().notifySetup();

	// Emulate loop via callbacks
	emscripten_set_main_loop(display_cb, -1, 1);
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::update(){
	if (bSetMainLoopTiming) {
		// choose the render speed with the second argument. 1 is for every screen refresh, 2 for every second, 3 for every third. and so on...
		emscripten_set_main_loop_timing(1, 2);
		// disable default touch behaviour 
		preventDefaultBrowserTouchMoveBehavior();
		
		bSetMainLoopTiming = false;
	}
	events().notifyUpdate();
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::draw(){
	
	if(nFramesSinceWindowResized == 1 ) {
		if( mCurrentWindowMode != mTargetWindowMode ) {
			ofLogVerbose("ofxAppEmscriptenWindow :: draw : mCurrentWindowMode != mTargetWindowMode: ") << " current: " << mCurrentWindowMode << " target: " << mTargetWindowMode << " | " << ofGetFrameNum();
			if( mTargetWindowMode == OF_GAME_MODE ) {
				// soft full screen //
				EmscriptenFullscreenStrategy strategy;
				strategy.scaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF;
				strategy.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
				strategy.canvasResizedCallback = emscripten_game_window_resized_callback;
				strategy.canvasResizedCallbackUserData = this;   // pointer to user data
				if(emscripten_enter_soft_fullscreen("canvas", &strategy) == EMSCRIPTEN_RESULT_SUCCESS ) {
					mCurrentWindowMode = mTargetWindowMode;
				} else {
					ofLogError("ofxAppEmscriptenWindow :: draw : unable to enter soft full screen");
					mCurrentWindowMode = OF_WINDOW;
					mTargetWindowMode = OF_WINDOW;
				}
				
			} else {
				mCurrentWindowMode = mTargetWindowMode;
			}
		}
		
		if( mCurrentWindowMode == OF_WINDOW || mCurrentWindowMode == OF_FULLSCREEN ) {
			mCachedWidth = -1;
			mCachedHeight = -1;
		}
		
		// since the emscripten resize call back seems to lock up the main thread, just check here if we need
		// to send a resize event, it will be 1 frame behind
		if( ofGetFrameNum() != mLastResizeFrameNum ) {
			events().notifyWindowResized( getWidth(), getHeight() );
			mLastResizeFrameNum = ofGetFrameNum();
		}
	}
	
	// set viewport, clear the screen
	renderer()->startRender();
	if( bEnableSetupScreen ) renderer()->setupScreen();

	events().notifyDraw();

	renderer()->finishRender();
	
	nFramesSinceWindowResized++;
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::display_cb(){
	if(instance){
		instance->update();
		instance->draw();
	}
}

//------------------------------------------------------------
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

//------------------------------------------------------------
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

//------------------------------------------------------------
int ofxAppEmscriptenWindow::mousedown_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	float mouseX = mouseEvent->targetX - EM_ASM_INT(return canvas.getBoundingClientRect().left);
	float mouseY = mouseEvent->targetY - EM_ASM_INT(return canvas.getBoundingClientRect().top);
	int canvasWidth, canvasHeight;
	emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);
        double cssWidth, cssHeight;
	emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);
	if(mouseX * canvasWidth / cssWidth >= 0 && mouseX * canvasWidth / cssWidth < canvasWidth && mouseY * canvasHeight / cssHeight >= 0 && mouseY * canvasHeight / cssHeight < canvasHeight){
		instance->events().notifyMousePressed(mouseX * canvasWidth / cssWidth, mouseY * canvasHeight / cssHeight, mouseEvent->button);
	}
	return 0;
}

//------------------------------------------------------------
int ofxAppEmscriptenWindow::mouseup_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	float mouseX = mouseEvent->targetX - EM_ASM_INT(return canvas.getBoundingClientRect().left);
	float mouseY = mouseEvent->targetY - EM_ASM_INT(return canvas.getBoundingClientRect().top);
	int canvasWidth, canvasHeight;
	emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);
        double cssWidth, cssHeight;
	emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);
	if(ofGetMousePressed()){
		instance->events().notifyMouseReleased(mouseX * canvasWidth / cssWidth, mouseY * canvasHeight / cssHeight, mouseEvent->button);
	}
	return 0;
}

//------------------------------------------------------------
int ofxAppEmscriptenWindow::mousemoved_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	float mouseX = mouseEvent->targetX - EM_ASM_INT(return canvas.getBoundingClientRect().left);
	float mouseY = mouseEvent->targetY - EM_ASM_INT(return canvas.getBoundingClientRect().top);
	int canvasWidth, canvasHeight;
	emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);
        double cssWidth, cssHeight;
	emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);
	if(ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)){
		instance->events().notifyMouseDragged(mouseX * canvasWidth / cssWidth, mouseY * canvasHeight / cssHeight, 0);
	}else if(ofGetMousePressed(OF_MOUSE_BUTTON_MIDDLE)){
		instance->events().notifyMouseDragged(mouseX * canvasWidth / cssWidth, mouseY * canvasHeight / cssHeight, 1);
	}else if(ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)){
		instance->events().notifyMouseDragged(mouseX * canvasWidth / cssWidth, mouseY * canvasHeight / cssHeight, 2);
	}else if(mouseX * canvasWidth / cssWidth >= 0 && mouseX * canvasWidth / cssWidth < canvasWidth && mouseY * canvasHeight / cssHeight >= 0 && mouseY * canvasHeight / cssHeight < canvasHeight){
		instance->events().notifyMouseMoved(mouseX * canvasWidth / cssWidth, mouseY * canvasHeight / cssHeight);
	}
	return 0;
}

//------------------------------------------------------------
int ofxAppEmscriptenWindow::mousescrolled_cb(int eventType, const EmscriptenWheelEvent *wheelEvent, void *userData){
	instance->events().notifyMouseScrolled(ofGetMouseX(), ofGetMouseY(), wheelEvent->deltaX / 100, wheelEvent->deltaY / 100);
	return 0;
}

//------------------------------------------------------------
int ofxAppEmscriptenWindow::mouseenter_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	float mouseX = mouseEvent->targetX - EM_ASM_INT(return canvas.getBoundingClientRect().left);
	float mouseY = mouseEvent->targetY - EM_ASM_INT(return canvas.getBoundingClientRect().top);
	int canvasWidth, canvasHeight;
	emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);
        double cssWidth, cssHeight;
	emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);
	instance->events().notifyMouseEntered(mouseX * canvasWidth / cssWidth, mouseY * canvasHeight / cssHeight);
	return 0;
}

//------------------------------------------------------------
int ofxAppEmscriptenWindow::mouseleave_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData){
	float mouseX = mouseEvent->targetX - EM_ASM_INT(return canvas.getBoundingClientRect().left);
	float mouseY = mouseEvent->targetY - EM_ASM_INT(return canvas.getBoundingClientRect().top);
	int canvasWidth, canvasHeight;
	emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);
        double cssWidth, cssHeight;
	emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);
	instance->events().notifyMouseExited(mouseX * canvasWidth / cssWidth, mouseY * canvasHeight / cssHeight);
	return 0;
}

//------------------------------------------------------------
int ofxAppEmscriptenWindow::touch_cb(int eventType, const EmscriptenTouchEvent* e, void* userData) {
	float boundingX = EM_ASM_INT(return canvas.getBoundingClientRect().left);
	float boundingY = EM_ASM_INT(return canvas.getBoundingClientRect().top);
	int canvasWidth, canvasHeight;
	emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);
	double cssWidth, cssHeight;
	emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);
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
		touchArgs.id = e->touches[i].identifier;//i;
		touchArgs.x =  (e->touches[i].targetX - boundingX) * canvasWidth / cssWidth;
		touchArgs.y =  (e->touches[i].targetY - boundingY) * canvasHeight / cssHeight;
		instance->events().notifyTouchEvent(touchArgs);
		if(i == 0 && instance->mBTouchSimulatesMouse) {
			ofMouseEventArgs::Type action = ofMouseEventArgs::Pressed;
			if(touchArgsType == ofTouchEventArgs::up) {
				action = ofMouseEventArgs::Released;
			} else if( touchArgsType == ofTouchEventArgs::move) {
				action = ofMouseEventArgs::Dragged;
			} else if(touchArgsType == ofTouchEventArgs::cancel) {
				action = ofMouseEventArgs::Exited;
			}
			ofMouseEventArgs args(action, touchArgs.x, touchArgs.y, OF_MOUSE_BUTTON_LEFT, 0);
			instance->events().notifyMouseEvent(args);
		}
	}
    return 0;
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::hideCursor(){
	emscripten_hide_mouse();
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::setWindowShape(int w, int h){
    emscripten_set_canvas_size(w,h);
//	emscripten_set_canvas_element_size("#canvas", w, h);
}

//------------------------------------------------------------
glm::vec2 ofxAppEmscriptenWindow::getWindowPosition(){
	return glm::vec2(0,0);
}

//------------------------------------------------------------
glm::vec2 ofxAppEmscriptenWindow::getWindowSize(){
	if(mCurrentWindowMode == OF_GAME_MODE) {
		double twidth, theight;
		emscripten_get_element_css_size("canvas", &twidth, &theight);
		return glm::vec2((int)twidth,(int)theight);
	}
	int width;
	int height;
	emscripten_get_canvas_element_size("canvas", &width, &height);
	return glm::vec2(width,height);
}

//------------------------------------------------------------
glm::vec2 ofxAppEmscriptenWindow::getScreenSize(){
	return getWindowSize();
}

//------------------------------------------------------------
ofOrientation ofxAppEmscriptenWindow::getOrientation(){
	return OF_ORIENTATION_DEFAULT;
}

//------------------------------------------------------------
bool ofxAppEmscriptenWindow::doesHWOrientation(){
	return false;
}

//this is used by ofGetWidth and now determines the window width based on orientation
//------------------------------------------------------------
int	ofxAppEmscriptenWindow::getWidth(){
	return getWindowSize().x;
}

//------------------------------------------------------------
int	ofxAppEmscriptenWindow::getHeight(){
	return getWindowSize().y;
}

//------------------------------------------------------------
ofWindowMode ofxAppEmscriptenWindow::getWindowMode(){
	return mCurrentWindowMode;
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::setFullscreen(bool fullscreen){
	if(fullscreen){
		if(mFullscreenScaleStrategy > -1 || mStartWindowMode == OF_GAME_MODE || mStartWindowMode == OF_FULLSCREEN ) {
			// we want the canvas to resize to fill the screen //
			EmscriptenFullscreenStrategy strategy;
			// TODO: this should probably be EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH
			strategy.scaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF;
			strategy.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
			if( mFullscreenScaleStrategy > -1 ) {
				strategy.scaleMode = mFullscreenScaleStrategy;
			}
			//strategy.canvasResizedCallback = emscripten_window_resized_callback;
			//strategy.canvasResizedCallbackUserData = this;   // pointer to user data
			EM_BOOL result = emscripten_request_fullscreen_strategy("#canvas", 1, &strategy );
			
			if(result == EMSCRIPTEN_RESULT_SUCCESS || result == EMSCRIPTEN_RESULT_DEFERRED) {
				mTargetWindowMode = OF_FULLSCREEN;
			} else {
				ofLogError("ofxAppEmscriptenWindow::setFullscreen error entering full screen");
			}
		} else {
			if(emscripten_request_fullscreen("#canvas", 1) == EMSCRIPTEN_RESULT_SUCCESS) {
				mTargetWindowMode = OF_FULLSCREEN;
			} else {
				ofLogError("ofxAppEmscriptenWindow::setFullscreen error entering full screen");
			}
		}
	}else{
		if(emscripten_exit_fullscreen() != EMSCRIPTEN_RESULT_SUCCESS) {
			ofLogError("ofxAppEmscriptenWindow::setFullscreen error exiting full screen");
		}
		if( mStartWindowMode == OF_GAME_MODE || mStartWindowMode == OF_FULLSCREEN ) {
			mTargetWindowMode = OF_GAME_MODE;
		} else {
			mTargetWindowMode = OF_WINDOW;
		}
	}
	nFramesSinceWindowResized=0;
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::toggleFullscreen(){
	EmscriptenFullscreenChangeEvent fullscreenStatus;
	emscripten_get_fullscreen_status(&fullscreenStatus);
	if(fullscreenStatus.isFullscreen){
		setFullscreen(false);
	}else if(fullscreenStatus.fullscreenEnabled){
		setFullscreen(true);
	}
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::enableSetupScreen(){
	bEnableSetupScreen = true;
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::disableSetupScreen(){
	bEnableSetupScreen = false;
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::setTouchSimulatesMouse(bool abEnable) {
	mBTouchSimulatesMouse = abEnable;
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::preventDefaultBrowserTouchMoveBehavior() {
	EM_ASM(
		   function preventBehavior(e) {
			   e.preventDefault();
		   };
		   document.addEventListener("touchmove", preventBehavior, {passive: false});
	);
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::setFullscreenScaleStrategy(int aStrategy) {
	mFullscreenScaleStrategy = aStrategy;
}

//------------------------------------------------------------
ofCoreEvents & ofxAppEmscriptenWindow::events(){
	return _events;
}

//------------------------------------------------------------
shared_ptr<ofBaseRenderer> & ofxAppEmscriptenWindow::renderer(){
	return _renderer;
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::makeCurrent(){
	if(context != 0){
		emscripten_webgl_make_context_current(context);	
	}
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::startRender(){
	renderer()->startRender();
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::finishRender(){
	renderer()->finishRender();
}

//------------------------------------------------------------
EM_BOOL ofxAppEmscriptenWindow::emscripten_game_window_resized_callback(int eventType, const void *reserved, void *userData){
	ofLogVerbose("ofxAppEmscriptenWindow :: emscripten_game_window_resized_callback: ") << " | " << ofGetFrameNum();
	if(instance->mLastResizeFrameNum != ofGetFrameNum() ) {
		double twidth, theight;
		emscripten_get_element_css_size("canvas", &twidth, &theight);
		int iwidth = twidth;
		int iheight = theight;
		
		emscripten_set_canvas_element_size("#canvas", int(twidth), int(theight));
		ofLogVerbose("ofxAppEmscriptenWindow :: trying to set the canvas size to: ") << twidth << " x " << theight << " num frames: " << instance->nFramesSinceWindowResized;
		
		if( iwidth != instance->mCachedWidth || iheight != instance->mCachedHeight ) {
			instance->mCachedWidth = iwidth;
			instance->mCachedHeight = iheight;
			
			instance->mLastResizeFrameNum = ofGetFrameNum();
			instance->nFramesSinceWindowResized = 0;
//			instance->events().notifyWindowResized( iwidth, iheight );
		}
	}
	return true;
}
