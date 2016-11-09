#include "ofEvents.h"
#include "ofAppRunner.h"


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
	return fps.getLastFrameSecs();
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
	// FIXME: modifiers are being reported twice, for generic and for left/right
	// add operators to the arguments class so it can be checked for both
	bool attended = false;
    if(key == OF_KEY_RIGHT_CONTROL || key == OF_KEY_LEFT_CONTROL){
        pressedKeys.insert(OF_KEY_CONTROL);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_CONTROL);
		attended = ofNotifyEvent( keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_RIGHT_SHIFT || key == OF_KEY_LEFT_SHIFT){
        pressedKeys.insert(OF_KEY_SHIFT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_SHIFT);
		attended = ofNotifyEvent( keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_ALT || key == OF_KEY_RIGHT_ALT){
        pressedKeys.insert(OF_KEY_ALT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_ALT);
		attended = ofNotifyEvent( keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_SUPER || key == OF_KEY_RIGHT_SUPER){
        pressedKeys.insert(OF_KEY_SUPER);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_SUPER);
		attended = ofNotifyEvent( keyPressed, keyEventArgs );
    }

	pressedKeys.insert(key);
	if(!attended){
		ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,key,keycode,scancode,codepoint);
		return ofNotifyEvent( keyPressed, keyEventArgs );
	}else{
		return attended;
	}
}

//------------------------------------------
bool ofCoreEvents::notifyKeyReleased(int key, int keycode, int scancode, uint32_t codepoint){
	// FIXME: modifiers are being reported twice, for generic and for left/right
	// add operators to the arguments class so it can be checked for both
	bool attended = false;
    if(key == OF_KEY_RIGHT_CONTROL || key == OF_KEY_LEFT_CONTROL){
        pressedKeys.erase(OF_KEY_CONTROL);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_CONTROL);
		attended = ofNotifyEvent( keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_RIGHT_SHIFT || key == OF_KEY_LEFT_SHIFT){
        pressedKeys.erase(OF_KEY_SHIFT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_SHIFT);
		attended = ofNotifyEvent( keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_ALT || key == OF_KEY_RIGHT_ALT){
        pressedKeys.erase(OF_KEY_ALT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_ALT);
		attended = ofNotifyEvent( keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_SUPER || key == OF_KEY_RIGHT_SUPER){
        pressedKeys.erase(OF_KEY_SUPER);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_SUPER);
		attended = ofNotifyEvent( keyReleased, keyEventArgs );
    }
    
	pressedKeys.erase(key);
	if(!attended){
		ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,key,keycode,scancode,codepoint);
		return ofNotifyEvent( keyReleased, keyEventArgs );
	}else{
		return attended;
	}
}


//------------------------------------------
bool ofCoreEvents::notifyKeyEvent(const ofKeyEventArgs & keyEvent){
	switch(keyEvent.type){
		case ofKeyEventArgs::Pressed:
			return notifyKeyPressed(keyEvent.key, keyEvent.keycode, keyEvent.scancode, keyEvent.codepoint);
		case ofKeyEventArgs::Released:
			return notifyKeyReleased(keyEvent.key, keyEvent.keycode, keyEvent.scancode, keyEvent.codepoint);
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
	ofNotifyEvent( touchDown, touchArgs );
}

//------------------------------------------
void ofCoreEvents::notifyTouchUp(int x, int y, int touchID){
	ofTouchEventArgs touchArgs;
	touchArgs.id = touchID;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.type = ofTouchEventArgs::up;
	ofNotifyEvent( touchUp, touchArgs );
}

//------------------------------------------
void ofCoreEvents::notifyTouchMoved(int x, int y, int touchID){
	ofTouchEventArgs touchArgs;
	touchArgs.id = touchID;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.type = ofTouchEventArgs::move;
	ofNotifyEvent( touchMoved, touchArgs );
}

//------------------------------------------
void ofCoreEvents::notifyTouchCancelled(int x, int y, int touchID){
	ofTouchEventArgs touchArgs;
	touchArgs.id = touchID;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.type = ofTouchEventArgs::cancel;
	ofNotifyEvent( touchCancelled, touchArgs );
}


//------------------------------------------
void ofCoreEvents::notifyTouchDoubleTap(int x, int y, int touchID){
	ofTouchEventArgs touchArgs;
	touchArgs.id = touchID;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.type = ofTouchEventArgs::doubleTap;
	ofNotifyEvent( touchDoubleTap, touchArgs );
}


//------------------------------------------
void ofCoreEvents::notifyTouchEvent(const ofTouchEventArgs & touchEvent){
	switch(touchEvent.type){
		case ofTouchEventArgs::move:
			notifyTouchMoved(touchEvent.x,touchEvent.y,touchEvent.id);
			break;
		case ofTouchEventArgs::down:
			notifyTouchDown(touchEvent.x,touchEvent.y,touchEvent.id);
			break;
		case ofTouchEventArgs::up:
			notifyTouchUp(touchEvent.x,touchEvent.y,touchEvent.id);
			break;
		case ofTouchEventArgs::cancel:
			notifyTouchCancelled(touchEvent.x,touchEvent.y,touchEvent.id);
			break;
		case ofTouchEventArgs::doubleTap:
			notifyTouchDoubleTap(touchEvent.x,touchEvent.y,touchEvent.id);
			break;
	}
}


//------------------------------------------
bool  ofCoreEvents::notifyMouseEvent(const ofMouseEventArgs & mouseEvent){
	switch(mouseEvent.type){
		case ofMouseEventArgs::Moved:
			return notifyMouseMoved(mouseEvent.x,mouseEvent.y);
		case ofMouseEventArgs::Dragged:
			return notifyMouseDragged(mouseEvent.x,mouseEvent.y,mouseEvent.button);
		case ofMouseEventArgs::Pressed:
			return notifyMousePressed(mouseEvent.x,mouseEvent.y,mouseEvent.button);
		case ofMouseEventArgs::Released:
			return notifyMouseReleased(mouseEvent.x,mouseEvent.y,mouseEvent.button);
		case ofMouseEventArgs::Scrolled:
			return notifyMouseScrolled(mouseEvent.x,mouseEvent.y,mouseEvent.scrollX,mouseEvent.scrollY);
		case ofMouseEventArgs::Entered:
			return notifyMouseEntered(mouseEvent.x,mouseEvent.y);
		case ofMouseEventArgs::Exited:
			return notifyMouseExited(mouseEvent.x,mouseEvent.y);
	}
	return false;
}

//------------------------------------------
bool ofCoreEvents::notifyMousePressed(int x, int y, int button){
    if( bPreMouseNotSet ){
		previousMouseX	= x;
		previousMouseY	= y;
		bPreMouseNotSet	= false;
	}else{
		previousMouseX = currentMouseX;
		previousMouseY = currentMouseY;
	}
    
	currentMouseX = x;
	currentMouseY = y;
	pressedMouseButtons.insert(button);


	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Pressed,x,y,button);
	return ofNotifyEvent( mousePressed, mouseEventArgs );
}

//------------------------------------------
bool ofCoreEvents::notifyMouseReleased(int x, int y, int button){
	if( bPreMouseNotSet ){
		previousMouseX	= x;
		previousMouseY	= y;
		bPreMouseNotSet	= false;
	}else{
		previousMouseX = currentMouseX;
		previousMouseY = currentMouseY;
	}

	currentMouseX = x;
	currentMouseY = y;
	pressedMouseButtons.erase(button);

	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Released,x,y,button);
	return ofNotifyEvent( mouseReleased, mouseEventArgs );
}

//------------------------------------------
bool ofCoreEvents::notifyMouseDragged(int x, int y, int button){
	if( bPreMouseNotSet ){
		previousMouseX	= x;
		previousMouseY	= y;
		bPreMouseNotSet	= false;
	}else{
		previousMouseX = currentMouseX;
		previousMouseY = currentMouseY;
	}

	currentMouseX = x;
	currentMouseY = y;

	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Dragged,x,y,button);
	return ofNotifyEvent( mouseDragged, mouseEventArgs );
}

//------------------------------------------
bool ofCoreEvents::notifyMouseMoved(int x, int y){
	if( bPreMouseNotSet ){
		previousMouseX	= x;
		previousMouseY	= y;
		bPreMouseNotSet	= false;
	}else{
		previousMouseX = currentMouseX;
		previousMouseY = currentMouseY;
	}

	currentMouseX = x;
	currentMouseY = y;

	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Moved,x,y,0);
	return ofNotifyEvent( mouseMoved, mouseEventArgs );
}

//------------------------------------------
bool ofCoreEvents::notifyMouseScrolled(int x, int y, float scrollX, float scrollY){
	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Scrolled,x,y);
	mouseEventArgs.scrollX = scrollX;
	mouseEventArgs.scrollY = scrollY;
	return ofNotifyEvent( mouseScrolled, mouseEventArgs );
}

//------------------------------------------
bool ofCoreEvents::notifyMouseEntered(int x, int y){
	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Entered,x,y);
	return ofNotifyEvent( mouseEntered, mouseEventArgs );
}

//------------------------------------------
bool ofCoreEvents::notifyMouseExited(int x, int y){
	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Exited,x,y);
	return ofNotifyEvent( mouseExited, mouseEventArgs );
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
bool ofCoreEvents::notifyDragEvent(ofDragInfo info){
	return ofNotifyEvent(fileDragEvent, info);
}

//------------------------------------------
bool ofSendMessage(ofMessage msg){
	return ofNotifyEvent(ofEvents().messageEvent, msg);
}

//------------------------------------------
bool ofSendMessage(string messageString){
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
