#include "ofEvents.h"
#include "ofAppRunner.h"
#include "ofAppBaseWindow.h"
#include "ofLog.h"

static ofEventArgs voidEventArgs;


//--------------------------------------
void ofSetFrameRate(int targetRate){
	auto window = ofGetMainLoop()->getCurrentWindow();
	if(window){
		window->events().setFrameRate(targetRate);
	}else{
		ofLogWarning("ofEvents") << "Trying to set framerate before mainloop is ready";
	}
}

//--------------------------------------
float ofGetFrameRate(){
	auto window = ofGetMainLoop()->getCurrentWindow();
	if(window){
		return window->events().getFrameRate();
	}else{
		return 0.f;
	}
}

//--------------------------------------
float ofGetTargetFrameRate(){
	auto window = ofGetMainLoop()->getCurrentWindow();
	if(window){
		return window->events().getTargetFrameRate();
	}else{
		return 0.f;
	}
}

//--------------------------------------
double ofGetLastFrameTime(){
	auto window = ofGetMainLoop()->getCurrentWindow();
	if(window){
		return window->events().getLastFrameTime();
	}else{
		return 0.f;
	}
}

//--------------------------------------
uint64_t ofGetFrameNum(){
	auto window = ofGetMainLoop()->getCurrentWindow();
	if(window){
		return window->events().getFrameNum();
	}else{
		return 0;
	}
}

//--------------------------------------
bool ofGetMousePressed(int button){ //by default any button
	auto window = ofGetMainLoop()->getCurrentWindow();
	if(window){
		return window->events().getMousePressed(button);
	}else{
		return false;
	}
}

//--------------------------------------
bool ofGetKeyPressed(int key){
	auto window = ofGetMainLoop()->getCurrentWindow();
	if(window){
		return window->events().getKeyPressed(key);
	}else{
		return false;
	}
}

//--------------------------------------
int ofGetMouseX(){
	auto window = ofGetMainLoop()->getCurrentWindow();
	if(window){
		return window->events().getMouseX();
	}else{
		return 0;
	}
}

//--------------------------------------
int ofGetMouseY(){
	auto window = ofGetMainLoop()->getCurrentWindow();
	if(window){
		return window->events().getMouseY();
	}else{
		return 0;
	}
}

//--------------------------------------
int ofGetPreviousMouseX(){
	auto window = ofGetMainLoop()->getCurrentWindow();
	if(window){
		return window->events().getPreviousMouseX();
	}else{
		return 0;
	}
}

//--------------------------------------
int ofGetPreviousMouseY(){
	auto window = ofGetMainLoop()->getCurrentWindow();
	if(window){
		return window->events().getPreviousMouseY();
	}else{
		return 0;
	}
}

ofCoreEvents::ofCoreEvents()
:targetRate(0)
,bFrameRateSet(false)
,fps(60)
,currentMouseX(0)
,currentMouseY(0)
,previousMouseX(0)
,previousMouseY(0)
,bPreMouseNotSet(false){

}

//------------------------------------------
void ofCoreEvents::disable(){
	setup.disable();
	draw.disable();
	update.disable();
	exit.disable();
	keyPressed.disable();
	keyReleased.disable();
	mouseDragged.disable();
	mouseReleased.disable();
	mousePressed.disable();
	mouseMoved.disable();
	mouseScrolled.disable();
	mouseEntered.disable();
	mouseExited.disable();
	touchDown.disable();
	touchUp.disable();
	touchMoved.disable();
	touchDoubleTap.disable();
	touchCancelled.disable();
	messageEvent.disable();
	fileDragEvent.disable();
}

//------------------------------------------
void ofCoreEvents::enable(){
	setup.enable();
	draw.enable();
	update.enable();
	exit.enable();
	keyPressed.enable();
	keyReleased.enable();
	mouseDragged.enable();
	mouseReleased.enable();
	mousePressed.enable();
	mouseMoved.enable();
	mouseScrolled.enable();
	mouseEntered.enable();
	mouseExited.enable();
	touchDown.enable();
	touchUp.enable();
	touchMoved.enable();
	touchDoubleTap.enable();
	touchCancelled.enable();
	messageEvent.enable();
	fileDragEvent.enable();
}

void ofCoreEvents::setTimeModeSystem(){
	timeMode = System;
}

void ofCoreEvents::setTimeModeFixedRate(uint64_t nanosecsPerFrame){
	timeMode = FixedRate;
	fixedRateTimeNanos = std::chrono::nanoseconds(nanosecsPerFrame);
}

void ofCoreEvents::setTimeModeFiltered(float alpha){
	timeMode = Filtered;
	fps.setFilterAlpha(alpha);
}

//--------------------------------------
void ofCoreEvents::setFrameRate(int _targetRate){
	// given this FPS, what is the amount of millis per frame
	// that should elapse?

	// --- > f / s

	if (_targetRate <= 0){
		bFrameRateSet = false;
	}else{
		bFrameRateSet	= true;
		targetRate		= _targetRate;
		uint64_t nanosPerFrame = 1000000000.0 / (double)targetRate;
		timer.setPeriodicEvent(nanosPerFrame);
	}
}

//--------------------------------------
float ofCoreEvents::getFrameRate() const{
	return fps.getFps();
}

//--------------------------------------
float ofCoreEvents::getTargetFrameRate() const{
	return targetRate;
}

//--------------------------------------
double ofCoreEvents::getLastFrameTime() const{
	switch(timeMode){
		case Filtered:
			return fps.getLastFrameFilteredSecs();
		case FixedRate:
			return std::chrono::duration<double>(fixedRateTimeNanos).count();
		case System:
		default:
			return fps.getLastFrameSecs();
	}

}

//--------------------------------------
uint64_t ofCoreEvents::getFrameNum() const{
	return fps.getNumFrames();
}

//--------------------------------------
bool ofCoreEvents::getMousePressed(int button) const{ //by default any button
	if(button==-1) return pressedMouseButtons.size();
	return pressedMouseButtons.find(button)!=pressedMouseButtons.end();
}

//--------------------------------------
bool ofCoreEvents::getKeyPressed(int key) const{
	if(key==-1) return pressedKeys.size();
	return pressedKeys.find(key)!=pressedKeys.end();
}

//--------------------------------------
int ofCoreEvents::getMouseX() const{
	return currentMouseX;
}

//--------------------------------------
int ofCoreEvents::getMouseY() const{
	return currentMouseY;
}

//--------------------------------------
int ofCoreEvents::getPreviousMouseX() const{
	return previousMouseX;
}

//--------------------------------------
int ofCoreEvents::getPreviousMouseY() const{
	return previousMouseY;
}

//--------------------------------------
int ofCoreEvents::getModifiers() const{
	return modifiers;
}

//------------------------------------------
bool ofCoreEvents::notifySetup(){
	return ofNotifyEvent( setup, voidEventArgs );
}

#include "ofGraphics.h"
//------------------------------------------
bool ofCoreEvents::notifyUpdate(){
	return ofNotifyEvent( update, voidEventArgs );
}

//------------------------------------------
bool ofCoreEvents::notifyDraw(){
	auto attended = ofNotifyEvent( draw, voidEventArgs );

	if (bFrameRateSet){
		timer.waitNext();
	}
	
	if(fps.getNumFrames()==0){
		if(bFrameRateSet) fps = ofFpsCounter(targetRate);
	}else{
		/*if(ofIsVerticalSyncEnabled()){
			float rate = ofGetRefreshRate();
			int intervals = round(lastFrameTime*rate/1000000.);//+vsyncedIntervalsRemainder;
			//vsyncedIntervalsRemainder = lastFrameTime*rate/1000000.+vsyncedIntervalsRemainder - intervals;
			lastFrameTime = intervals*1000000/rate;
		}*/
	}
	fps.newFrame();
	return attended;
}

//------------------------------------------
bool ofCoreEvents::notifyKeyPressed(int key, int keycode, int scancode, uint32_t codepoint){
	ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,key,keycode,scancode,codepoint,0);
	return notifyKeyEvent(keyEventArgs);
}

//------------------------------------------
bool ofCoreEvents::notifyKeyReleased(int key, int keycode, int scancode, uint32_t codepoint){
	ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,key,keycode,scancode,codepoint,0);
	return notifyKeyEvent(keyEventArgs);
}


//------------------------------------------
bool ofCoreEvents::notifyKeyEvent(ofKeyEventArgs & e){
	bool attended = false;
	modifiers = e.modifiers;
	switch(e.type){
		case ofKeyEventArgs::Pressed:
			// FIXME: modifiers are being reported twice, for generic and for left/right
			// add operators to the arguments class so it can be checked for both
			if(e.key == OF_KEY_RIGHT_CONTROL || e.key == OF_KEY_LEFT_CONTROL){
				pressedKeys.insert(OF_KEY_CONTROL);
				ofKeyEventArgs keyEventArgs = e;
				keyEventArgs.key = OF_KEY_CONTROL;
				attended = ofNotifyEvent( keyPressed, keyEventArgs );
			}
			else if(e.key == OF_KEY_RIGHT_SHIFT || e.key == OF_KEY_LEFT_SHIFT){
				pressedKeys.insert(OF_KEY_SHIFT);
				ofKeyEventArgs keyEventArgs = e;
				keyEventArgs.key = OF_KEY_SHIFT;
				attended = ofNotifyEvent( keyPressed, keyEventArgs );
			}
			else if(e.key == OF_KEY_LEFT_ALT || e.key == OF_KEY_RIGHT_ALT){
				pressedKeys.insert(OF_KEY_ALT);
				ofKeyEventArgs keyEventArgs = e;
				keyEventArgs.key = OF_KEY_ALT;
				attended = ofNotifyEvent( keyPressed, keyEventArgs );
			}
			else if(e.key == OF_KEY_LEFT_SUPER || e.key == OF_KEY_RIGHT_SUPER){
				pressedKeys.insert(OF_KEY_SUPER);
				ofKeyEventArgs keyEventArgs = e;
				keyEventArgs.key = OF_KEY_SUPER;
				attended = ofNotifyEvent( keyPressed, keyEventArgs );
			}

			pressedKeys.insert(e.key);
			if(!attended){
				return ofNotifyEvent( keyPressed, e );
			}else{
				return attended;
			}
		case ofKeyEventArgs::Released:
			// FIXME: modifiers are being reported twice, for generic and for left/right
			// add operators to the arguments class so it can be checked for both
			if(e.key == OF_KEY_RIGHT_CONTROL || e.key == OF_KEY_LEFT_CONTROL){
				pressedKeys.erase(OF_KEY_CONTROL);
				ofKeyEventArgs keyEventArgs = e;
				keyEventArgs.key = OF_KEY_CONTROL;
				attended = ofNotifyEvent( keyReleased, keyEventArgs );
			}
			else if(e.key == OF_KEY_RIGHT_SHIFT || e.key == OF_KEY_LEFT_SHIFT){
				pressedKeys.erase(OF_KEY_SHIFT);
				ofKeyEventArgs keyEventArgs = e;
				keyEventArgs.key = OF_KEY_SHIFT;
				attended = ofNotifyEvent( keyReleased, keyEventArgs );
			}
			else if(e.key == OF_KEY_LEFT_ALT || e.key == OF_KEY_RIGHT_ALT){
				pressedKeys.erase(OF_KEY_ALT);
				ofKeyEventArgs keyEventArgs = e;
				keyEventArgs.key = OF_KEY_ALT;
				attended = ofNotifyEvent( keyReleased, keyEventArgs );
			}
			else if(e.key == OF_KEY_LEFT_SUPER || e.key == OF_KEY_RIGHT_SUPER){
				pressedKeys.erase(OF_KEY_SUPER);
				ofKeyEventArgs keyEventArgs = e;
				keyEventArgs.key = OF_KEY_SUPER;
				attended = ofNotifyEvent( keyReleased, keyEventArgs );
			}

			pressedKeys.erase(e.key);
			if(!attended){
				return ofNotifyEvent( keyReleased, e );
			}else{
				return attended;
			}
	}
	return false;
}

//------------------------------------------
void ofCoreEvents::notifyTouchDown(int x, int y, int touchID){
	ofTouchEventArgs touchArgs;
	touchArgs.id = touchID;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.type = ofTouchEventArgs::down;
	notifyTouchEvent(touchArgs);
}

//------------------------------------------
void ofCoreEvents::notifyTouchUp(int x, int y, int touchID){
	ofTouchEventArgs touchArgs;
	touchArgs.id = touchID;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.type = ofTouchEventArgs::up;
	notifyTouchEvent(touchArgs);
}

//------------------------------------------
void ofCoreEvents::notifyTouchMoved(int x, int y, int touchID){
	ofTouchEventArgs touchArgs;
	touchArgs.id = touchID;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.type = ofTouchEventArgs::move;
	notifyTouchEvent(touchArgs);
}

//------------------------------------------
void ofCoreEvents::notifyTouchCancelled(int x, int y, int touchID){
	ofTouchEventArgs touchArgs;
	touchArgs.id = touchID;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.type = ofTouchEventArgs::cancel;
	notifyTouchEvent(touchArgs);
}


//------------------------------------------
void ofCoreEvents::notifyTouchDoubleTap(int x, int y, int touchID){
	ofTouchEventArgs touchArgs;
	touchArgs.id = touchID;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.type = ofTouchEventArgs::doubleTap;
	notifyTouchEvent(touchArgs);
}


//------------------------------------------
void ofCoreEvents::notifyTouchEvent(ofTouchEventArgs & touchArgs){
	switch(touchArgs.type){
		case ofTouchEventArgs::move:
			ofNotifyEvent( touchMoved, touchArgs );
			break;
		case ofTouchEventArgs::down:
			ofNotifyEvent( touchDown, touchArgs );
			break;
		case ofTouchEventArgs::up:
			ofNotifyEvent( touchUp, touchArgs );
			break;
		case ofTouchEventArgs::cancel:
			ofNotifyEvent( touchCancelled, touchArgs );
			break;
		case ofTouchEventArgs::doubleTap:
			ofNotifyEvent( touchDoubleTap, touchArgs );
			break;
	}
}


//------------------------------------------
bool  ofCoreEvents::notifyMouseEvent(ofMouseEventArgs & e){
	modifiers = e.modifiers;
	switch(e.type){
		case ofMouseEventArgs::Moved:
			if( bPreMouseNotSet ){
				previousMouseX	= e.x;
				previousMouseY	= e.y;
				bPreMouseNotSet	= false;
			}else{
				previousMouseX = currentMouseX;
				previousMouseY = currentMouseY;
			}

			currentMouseX = e.x;
			currentMouseY = e.y;

			return ofNotifyEvent( mouseMoved, e );
		case ofMouseEventArgs::Dragged:
			if( bPreMouseNotSet ){
				previousMouseX	= e.x;
				previousMouseY	= e.y;
				bPreMouseNotSet	= false;
			}else{
				previousMouseX = currentMouseX;
				previousMouseY = currentMouseY;
			}

			currentMouseX = e.x;
			currentMouseY = e.y;

			return ofNotifyEvent( mouseDragged, e );
		case ofMouseEventArgs::Pressed:{
			if( bPreMouseNotSet ){
				previousMouseX	= e.x;
				previousMouseY	= e.y;
				bPreMouseNotSet	= false;
			}else{
				previousMouseX = currentMouseX;
				previousMouseY = currentMouseY;
			}

			currentMouseX = e.x;
			currentMouseY = e.y;
			pressedMouseButtons.insert(e.button);


			return ofNotifyEvent( mousePressed, e );
		}
		case ofMouseEventArgs::Released:
			if( bPreMouseNotSet ){
				previousMouseX	= e.x;
				previousMouseY	= e.y;
				bPreMouseNotSet	= false;
			}else{
				previousMouseX = currentMouseX;
				previousMouseY = currentMouseY;
			}

			currentMouseX = e.x;
			currentMouseY = e.y;
			pressedMouseButtons.erase(e.button);

			return ofNotifyEvent( mouseReleased, e );
		case ofMouseEventArgs::Scrolled:
			return ofNotifyEvent( mouseScrolled, e );
		case ofMouseEventArgs::Entered:
			return ofNotifyEvent( mouseEntered, e );
		case ofMouseEventArgs::Exited:
			return ofNotifyEvent( mouseExited, e );
	}
	return false;
}

//------------------------------------------
bool ofCoreEvents::notifyMousePressed(int x, int y, int button){
	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Pressed,x,y,button);
	return notifyMouseEvent(mouseEventArgs);
}

//------------------------------------------
bool ofCoreEvents::notifyMouseReleased(int x, int y, int button){
	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Released,x,y,button);
	return notifyMouseEvent(mouseEventArgs);
}

//------------------------------------------
bool ofCoreEvents::notifyMouseDragged(int x, int y, int button){
	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Dragged,x,y,button);
	return notifyMouseEvent(mouseEventArgs);
}

//------------------------------------------
bool ofCoreEvents::notifyMouseMoved(int x, int y){
	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Moved,x,y,0);
	return notifyMouseEvent(mouseEventArgs);
}

//------------------------------------------
bool ofCoreEvents::notifyMouseScrolled(int x, int y, float scrollX, float scrollY){
	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Scrolled,x,y);
	mouseEventArgs.scrollX = scrollX;
	mouseEventArgs.scrollY = scrollY;
	return notifyMouseEvent(mouseEventArgs);
}

//------------------------------------------
bool ofCoreEvents::notifyMouseEntered(int x, int y){
	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Entered,x,y);
	return notifyMouseEvent(mouseEventArgs);
}

//------------------------------------------
bool ofCoreEvents::notifyMouseExited(int x, int y){
	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Exited,x,y);
	return notifyMouseEvent(mouseEventArgs);
}

//------------------------------------------
bool ofCoreEvents::notifyExit(){
	return ofNotifyEvent( exit, voidEventArgs );
}

//------------------------------------------
bool ofCoreEvents::notifyWindowResized(int width, int height){
	ofResizeEventArgs resizeEventArgs(width,height);
	return ofNotifyEvent( windowResized, resizeEventArgs );
}

//------------------------------------------
bool ofCoreEvents::notifyWindowMoved(int x, int y){
	ofWindowPosEventArgs windowPosEventArgs(x,y);
	return ofNotifyEvent( windowMoved, windowPosEventArgs );
}

//------------------------------------------
bool ofCoreEvents::notifyDragEvent(ofDragInfo info){
	return ofNotifyEvent(fileDragEvent, info);
}

//------------------------------------------
bool ofSendMessage(ofMessage msg){
	return ofNotifyEvent(ofEvents().messageEvent, msg);
}

//------------------------------------------
bool ofSendMessage(std::string messageString){
	ofMessage msg(messageString);
	return ofSendMessage(msg);
}

//------------------------------------------
namespace of{
	namespace priv{
		std::atomic<uint_fast64_t> StdFunctionId::nextId;

		AbstractEventToken::~AbstractEventToken(){}

		BaseFunctionId::~BaseFunctionId(){}

		StdFunctionId::~StdFunctionId(){}
	}
}
