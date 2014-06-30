#include "ofEvents.h"
#include "ofAppRunner.h"
#include "ofBaseApp.h"
#include "ofUtils.h"
#include "ofGraphics.h"
#include "ofAppBaseWindow.h"
#include <set>

static const double MICROS_TO_SEC = .000001;
static const double MICROS_TO_MILLIS = .001;

static unsigned long long   timeThen = 0, oneSec = 0;
static float    			targetRate = 0;
static float				fps = 60;
static unsigned long long	microsForFrame = 0;
static unsigned long long	lastFrameTime = 0;
static bool      			bFrameRateSet = 0;
static int      			nFramesForFPS = 0;
static int      			nFrameCount	  = 0;
static unsigned long long   prevMicrosForFPS = 0;

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
		microsForFrame	= 1000000.0 / (double)targetRate;
	}
}

//--------------------------------------
float ofGetFrameRate(){
	return fps;
}

//--------------------------------------
float ofGetTargetFrameRate(){
	return targetRate;
}

//--------------------------------------
double ofGetLastFrameTime(){
	return lastFrameTime*MICROS_TO_SEC;
}

//--------------------------------------
int ofGetFrameNum(){
	return nFrameCount;
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
	// calculate sleep time to adjust to target fps
	unsigned long long timeNow = ofGetElapsedTimeMicros();
	if (nFrameCount != 0 && bFrameRateSet == true){
		unsigned long long diffMicros = timeNow - prevMicrosForFPS;
		prevMicrosForFPS = timeNow;
		if(diffMicros < microsForFrame){
			unsigned long long waitMicros = microsForFrame - diffMicros;
            // Theoretical value to compensate for the extra time that it might sleep
			prevMicrosForFPS += waitMicros;
			#ifdef TARGET_WIN32
				Sleep(waitMicros*MICROS_TO_MILLIS);
			#else
				usleep(waitMicros);
			#endif
		}
	}else{
		prevMicrosForFPS = timeNow;
	}

	// calculate fps
	timeNow = ofGetElapsedTimeMicros();

	if(nFrameCount==0){
		timeThen = timeNow;
		if(bFrameRateSet)	fps = targetRate;
	}else{
		unsigned long long oneSecDiff = timeNow-oneSec;

		if( oneSecDiff  >= 1000000 ){
			fps = nFramesForFPS/(oneSecDiff*MICROS_TO_SEC);
			oneSec  = timeNow;
			nFramesForFPS = 0;
		}else{
			double deltaTime = ((double)oneSecDiff)*MICROS_TO_SEC;
			if( deltaTime > 0.0 ){
				fps = fps*0.99 + (nFramesForFPS/deltaTime)*0.01;
			}
		}
		nFramesForFPS++;


		lastFrameTime 	= timeNow-timeThen;
		timeThen    	= timeNow;
	}

	// update renderer, application and notify update event
	ofGetCurrentRenderer()->update();

	ofNotifyEvent( ofEvents().update, voidEventArgs );
}

//------------------------------------------
void ofNotifyDraw(){
	if(ofGetCurrentRenderer()){
		ofNotifyEvent( ofEvents().draw, voidEventArgs );
	}

	nFrameCount++;
}

//------------------------------------------
void ofNotifyKeyPressed(int key, int keycode, int scancode, int codepoint){
	static ofKeyEventArgs keyEventArgs;
	// FIXME: modifiers are being reported twice, for generic and for left/right
	// add operators to the arguments class so it can be checked for both
    if(key == OF_KEY_RIGHT_CONTROL || key == OF_KEY_LEFT_CONTROL){
        pressedKeys.insert(OF_KEY_CONTROL);
    	keyEventArgs.key = OF_KEY_CONTROL;
        ofNotifyEvent( ofEvents().keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_RIGHT_SHIFT || key == OF_KEY_LEFT_SHIFT){
        pressedKeys.insert(OF_KEY_SHIFT);
    	keyEventArgs.key = OF_KEY_SHIFT;
        ofNotifyEvent( ofEvents().keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_ALT || key == OF_KEY_RIGHT_ALT){
        pressedKeys.insert(OF_KEY_ALT);
    	keyEventArgs.key = OF_KEY_ALT;
        ofNotifyEvent( ofEvents().keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_SUPER || key == OF_KEY_RIGHT_SUPER){
        pressedKeys.insert(OF_KEY_SUPER);
    	keyEventArgs.key = OF_KEY_SUPER;
        ofNotifyEvent( ofEvents().keyPressed, keyEventArgs );
    }
            
	pressedKeys.insert(key);

	keyEventArgs.key = key;
	keyEventArgs.keycode = keycode;
	keyEventArgs.scancode = scancode;
	keyEventArgs.codepoint = codepoint;
	ofNotifyEvent( ofEvents().keyPressed, keyEventArgs );
	
	
	if (key == OF_KEY_ESC && bEscQuits == true){				// "escape"
		ofGetWindowPtr()->windowShouldClose();
    }
	
	
}

//------------------------------------------
void ofNotifyKeyReleased(int key, int keycode, int scancode, int codepoint){
	static ofKeyEventArgs keyEventArgs;

	// FIXME: modifiers are being reported twice, for generic and for left/right
	// add operators to the arguments class so it can be checked for both
    if(key == OF_KEY_RIGHT_CONTROL || key == OF_KEY_LEFT_CONTROL){
        pressedKeys.erase(OF_KEY_CONTROL);
    	keyEventArgs.key = OF_KEY_CONTROL;
    	ofNotifyEvent( ofEvents().keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_RIGHT_SHIFT || key == OF_KEY_LEFT_SHIFT){
        pressedKeys.erase(OF_KEY_SHIFT);
    	keyEventArgs.key = OF_KEY_SHIFT;
    	ofNotifyEvent( ofEvents().keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_ALT || key == OF_KEY_RIGHT_ALT){
        pressedKeys.erase(OF_KEY_ALT);
    	keyEventArgs.key = OF_KEY_ALT;
    	ofNotifyEvent( ofEvents().keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_SUPER || key == OF_KEY_RIGHT_SUPER){
        pressedKeys.erase(OF_KEY_SUPER);
    	keyEventArgs.key = OF_KEY_SUPER;
    	ofNotifyEvent( ofEvents().keyReleased, keyEventArgs );
    }
    
	pressedKeys.erase(key);
	
	keyEventArgs.key = key;
	keyEventArgs.keycode = keycode;
	keyEventArgs.scancode = scancode;
	keyEventArgs.codepoint = codepoint;
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
		
	}
}

//------------------------------------------
void ofNotifyMousePressed(int x, int y, int button){
	static ofMouseEventArgs mouseEventArgs;
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


	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mouseEventArgs.button = button;
    mouseEventArgs.type = ofMouseEventArgs::Pressed;
	ofNotifyEvent( ofEvents().mousePressed, mouseEventArgs );
}

//------------------------------------------
void ofNotifyMouseReleased(int x, int y, int button){
	static ofMouseEventArgs mouseEventArgs;

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

	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mouseEventArgs.button = button;
    mouseEventArgs.type = ofMouseEventArgs::Released;
	ofNotifyEvent( ofEvents().mouseReleased, mouseEventArgs );
}

//------------------------------------------
void ofNotifyMouseDragged(int x, int y, int button){
	static ofMouseEventArgs mouseEventArgs;

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

	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mouseEventArgs.button = button;
    mouseEventArgs.type = ofMouseEventArgs::Dragged;
	ofNotifyEvent( ofEvents().mouseDragged, mouseEventArgs );
}

//------------------------------------------
void ofNotifyMouseMoved(int x, int y){
	static ofMouseEventArgs mouseEventArgs;
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

	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
    mouseEventArgs.type = ofMouseEventArgs::Moved;
	ofNotifyEvent( ofEvents().mouseMoved, mouseEventArgs );
}

//------------------------------------------
void ofNotifyExit(){
	ofNotifyEvent( ofEvents().exit, voidEventArgs );
}

//------------------------------------------
void ofNotifyWindowResized(int width, int height){
	static ofResizeEventArgs resizeEventArgs;
	
	resizeEventArgs.width	= width;
	resizeEventArgs.height	= height;
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
	
	static ofEntryEventArgs entryArgs;
	entryArgs.state = state;
	ofNotifyEvent(ofEvents().windowEntered, entryArgs);
	
}
