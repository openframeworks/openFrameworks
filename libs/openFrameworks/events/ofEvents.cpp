#include "ofEvents.h"
#include "ofAppRunner.h"
#include "ofBaseApp.h"
#include "ofUtils.h"
#include "ofGraphics.h"
#include "ofAppBaseWindow.h"
#include <set>
#include "ofTimer.h"
#include "ofFpsCounter.h"

static float    			targetRate = 0;
static bool      			bFrameRateSet = 0;
static ofTimer 				timer;
static ofFpsCounter			fps(60);

// core events instance & arguments
ofCoreEvents & ofEvents(){
	static ofCoreEvents * events = new ofCoreEvents;
	return *events;
}

ofEventArgs voidEventArgs;


static int	currentMouseX=0, currentMouseY=0;
static int	previousMouseX=0, previousMouseY=0;
static bool		bPreMouseNotSet;
static set<int> pressedMouseButtons;
static set<int> pressedKeys;

static bool bEscQuits = true;


//--------------------------------------
void ofSetFrameRate(int _targetRate){
	// given this FPS, what is the amount of millis per frame
	// that should elapse?

	// --- > f / s

	if (_targetRate == 0){
		bFrameRateSet = false;
	}else{
		bFrameRateSet	= true;
		targetRate		= _targetRate;
		unsigned long long nanosPerFrame = 1000000000.0 / (double)targetRate;
		timer.setPeriodicEvent(nanosPerFrame);
	}
}

//--------------------------------------
float ofGetFrameRate(){
	return fps.getFps();
}

//--------------------------------------
float ofGetTargetFrameRate(){
	return targetRate;
}

//--------------------------------------
double ofGetLastFrameTime(){
	return fps.getLastFrameSecs();
}

//--------------------------------------
int ofGetFrameNum(){
	return fps.getNumFrames();
}

//--------------------------------------
bool ofGetMousePressed(int button){ //by default any button
	if(button==-1) return pressedMouseButtons.size();
	return pressedMouseButtons.find(button)!=pressedMouseButtons.end();
}

//--------------------------------------
bool ofGetKeyPressed(int key){
	if(key==-1) return pressedKeys.size();
	return pressedKeys.find(key)!=pressedKeys.end();
}

//--------------------------------------
int ofGetMouseX(){
	return currentMouseX;
}

//--------------------------------------
int ofGetMouseY(){
	return currentMouseY;
}

//--------------------------------------
int ofGetPreviousMouseX(){
	return previousMouseX;
}

//--------------------------------------
int ofGetPreviousMouseY(){
	return previousMouseY;
}

//--------------------------------------
void ofSetEscapeQuitsApp(bool bQuitOnEsc){
	bEscQuits = bQuitOnEsc;
}

//------------------------------------------
void ofNotifySetup(){
	ofNotifyEvent( ofEvents().setup, voidEventArgs );
}

//------------------------------------------
void ofNotifyUpdate(){
	
	// update renderer, application and notify update event
	ofGetCurrentRenderer()->update();

	ofNotifyEvent( ofEvents().update, voidEventArgs );
}

//------------------------------------------
void ofNotifyDraw(){
	if(ofGetCurrentRenderer()){
		ofNotifyEvent( ofEvents().draw, voidEventArgs );
	}

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
	
}

//------------------------------------------
void ofNotifyKeyPressed(int key, int keycode, int scancode, int codepoint){
	// FIXME: modifiers are being reported twice, for generic and for left/right
	// add operators to the arguments class so it can be checked for both
    if(key == OF_KEY_RIGHT_CONTROL || key == OF_KEY_LEFT_CONTROL){
        pressedKeys.insert(OF_KEY_CONTROL);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_CONTROL);
        ofNotifyEvent( ofEvents().keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_RIGHT_SHIFT || key == OF_KEY_LEFT_SHIFT){
        pressedKeys.insert(OF_KEY_SHIFT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_SHIFT);
        ofNotifyEvent( ofEvents().keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_ALT || key == OF_KEY_RIGHT_ALT){
        pressedKeys.insert(OF_KEY_ALT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_ALT);
        ofNotifyEvent( ofEvents().keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_SUPER || key == OF_KEY_RIGHT_SUPER){
        pressedKeys.insert(OF_KEY_SUPER);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_SUPER);
        ofNotifyEvent( ofEvents().keyPressed, keyEventArgs );
    }
            
	pressedKeys.insert(key);
    ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,key,keycode,scancode,codepoint);
	ofNotifyEvent( ofEvents().keyPressed, keyEventArgs );
	
	
	if (key == OF_KEY_ESC && bEscQuits == true){				// "escape"
		ofGetWindowPtr()->windowShouldClose();
    }
	
	
}

//------------------------------------------
void ofNotifyKeyReleased(int key, int keycode, int scancode, int codepoint){
	// FIXME: modifiers are being reported twice, for generic and for left/right
	// add operators to the arguments class so it can be checked for both
    if(key == OF_KEY_RIGHT_CONTROL || key == OF_KEY_LEFT_CONTROL){
        pressedKeys.erase(OF_KEY_CONTROL);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_CONTROL);
    	ofNotifyEvent( ofEvents().keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_RIGHT_SHIFT || key == OF_KEY_LEFT_SHIFT){
        pressedKeys.erase(OF_KEY_SHIFT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_SHIFT);
    	ofNotifyEvent( ofEvents().keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_ALT || key == OF_KEY_RIGHT_ALT){
        pressedKeys.erase(OF_KEY_ALT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_ALT);
    	ofNotifyEvent( ofEvents().keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_SUPER || key == OF_KEY_RIGHT_SUPER){
        pressedKeys.erase(OF_KEY_SUPER);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_SUPER);
    	ofNotifyEvent( ofEvents().keyReleased, keyEventArgs );
    }
    
	pressedKeys.erase(key);
    ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,key,keycode,scancode,codepoint);
	ofNotifyEvent( ofEvents().keyReleased, keyEventArgs );
}


//------------------------------------------
void ofNotifyKeyEvent(const ofKeyEventArgs & keyEvent){
	switch(keyEvent.type){
		case ofKeyEventArgs::Pressed:
			ofNotifyKeyPressed(keyEvent.key);
			break;
		case ofKeyEventArgs::Released:
			ofNotifyKeyReleased(keyEvent.key);
			break;
		
	}
}

//------------------------------------------
void ofNotifyMouseEvent(const ofMouseEventArgs & mouseEvent){
	switch(mouseEvent.type){
		case ofMouseEventArgs::Moved:
			ofNotifyMouseMoved(mouseEvent.x,mouseEvent.y);
			break;
		case ofMouseEventArgs::Dragged:
			ofNotifyMouseDragged(mouseEvent.x,mouseEvent.y,mouseEvent.button);
			break;
		case ofMouseEventArgs::Pressed:
			ofNotifyMousePressed(mouseEvent.x,mouseEvent.y,mouseEvent.button);
			break;
		case ofMouseEventArgs::Released:
			ofNotifyMouseReleased(mouseEvent.x,mouseEvent.y,mouseEvent.button);
			break;
		case ofMouseEventArgs::Scrolled:
			ofNotifyMouseScrolled(mouseEvent.x,mouseEvent.y);
			break;
	}
}

//------------------------------------------
void ofNotifyMousePressed(int x, int y, int button){
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
	ofNotifyEvent( ofEvents().mousePressed, mouseEventArgs );
}

//------------------------------------------
void ofNotifyMouseReleased(int x, int y, int button){
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
	ofNotifyEvent( ofEvents().mouseReleased, mouseEventArgs );
}

//------------------------------------------
void ofNotifyMouseDragged(int x, int y, int button){
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
	ofNotifyEvent( ofEvents().mouseDragged, mouseEventArgs );
}

//------------------------------------------
void ofNotifyMouseMoved(int x, int y){
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
	ofNotifyEvent( ofEvents().mouseMoved, mouseEventArgs );
}

//------------------------------------------
void ofNotifyMouseScrolled(float x, float y){
	ofMouseEventArgs mouseEventArgs(ofMouseEventArgs::Scrolled,x,y);

	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mouseEventArgs.type = ofMouseEventArgs::Scrolled;
	ofNotifyEvent( ofEvents().mouseScrolled, mouseEventArgs );
}

//------------------------------------------
void ofNotifyExit(){
	ofNotifyEvent( ofEvents().exit, voidEventArgs );
}

//------------------------------------------
void ofNotifyWindowResized(int width, int height){
	ofResizeEventArgs resizeEventArgs(width,height);
	ofNotifyEvent( ofEvents().windowResized, resizeEventArgs );
}

//------------------------------------------
void ofNotifyDragEvent(ofDragInfo info){
	ofNotifyEvent(ofEvents().fileDragEvent, info);
}

//------------------------------------------
void ofSendMessage(ofMessage msg){
	ofNotifyEvent(ofEvents().messageEvent, msg);
}

//------------------------------------------
void ofSendMessage(string messageString){
	ofMessage msg(messageString);
	ofSendMessage(msg);
}

void ofNotifyWindowEntry( int state ) {
	static ofEntryEventArgs entryArgs(state);
	ofNotifyEvent(ofEvents().windowEntered, entryArgs);
	
}
