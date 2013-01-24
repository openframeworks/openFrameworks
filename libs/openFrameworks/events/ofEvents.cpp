#include "ofEvents.h"
#include "ofAppRunner.h"
#include "ofBaseApp.h"
#include "ofUtils.h"
#include "ofGraphics.h"
#include <set>

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

void exitApp(){
	ofLog(OF_LOG_VERBOSE,"OF app is being terminated!");
	OF_EXIT_APP(0);
}



//------------------------------------------
void ofNotifySetup(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	
	if(ofAppPtr){
		ofAppPtr->setup();
	}
	ofNotifyEvent( ofEvents().setup, voidEventArgs );
}

//------------------------------------------
void ofNotifyUpdate(){
	ofGetCurrentRenderer()->update();

	ofBaseApp * ofAppPtr = ofGetAppPtr();
	
	if(ofAppPtr){
		ofAppPtr->update();
	}
	ofNotifyEvent( ofEvents().update, voidEventArgs );
}

//------------------------------------------
void ofNotifyDraw(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	
	if(ofAppPtr){
		ofAppPtr->draw();
	}
	ofNotifyEvent( ofEvents().draw, voidEventArgs );
}

//------------------------------------------
void ofNotifyKeyPressed(int key){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofKeyEventArgs keyEventArgs;

	pressedKeys.insert(key);

	if(ofAppPtr){
		ofAppPtr->keyPressed(key);
	}
	
	keyEventArgs.key = key;
	ofNotifyEvent( ofEvents().keyPressed, keyEventArgs );
	
	
	if (key == OF_KEY_ESC && bEscQuits == true){				// "escape"
		exitApp();
	}
	
	
}

//------------------------------------------
void ofNotifyKeyReleased(int key){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofKeyEventArgs keyEventArgs;

	pressedKeys.erase(key);

	if(ofAppPtr){
		ofAppPtr->keyReleased(key);
	}
	
	keyEventArgs.key = key;
	ofNotifyEvent( ofEvents().keyReleased, keyEventArgs );
}

//------------------------------------------
void ofNotifyMousePressed(int x, int y, int button){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
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

	if(ofAppPtr){
		ofAppPtr->mousePressed(x,y,button);
		ofAppPtr->mouseX = x;
		ofAppPtr->mouseY = y;
	}

	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mouseEventArgs.button = button;
	ofNotifyEvent( ofEvents().mousePressed, mouseEventArgs );
}

//------------------------------------------
void ofNotifyMouseReleased(int x, int y, int button){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
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

	if(ofAppPtr){
		ofAppPtr->mouseReleased(x,y,button);
		ofAppPtr->mouseReleased();
		ofAppPtr->mouseX = x;
		ofAppPtr->mouseY = y;
	}

	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mouseEventArgs.button = button;
	ofNotifyEvent( ofEvents().mouseReleased, mouseEventArgs );
}

//------------------------------------------
void ofNotifyMouseDragged(int x, int y, int button){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
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
	
	if(ofAppPtr){
		ofAppPtr->mouseDragged(x,y,button);
		ofAppPtr->mouseX = x;
		ofAppPtr->mouseY = y;
	}

	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mouseEventArgs.button = button;
	ofNotifyEvent( ofEvents().mouseDragged, mouseEventArgs );
}

//------------------------------------------
void ofNotifyMouseMoved(int x, int y){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
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
	
	if(ofAppPtr){
		ofAppPtr->mouseMoved(x,y);
		ofAppPtr->mouseX = x;
		ofAppPtr->mouseY = y;
	}

	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	ofNotifyEvent( ofEvents().mouseMoved, mouseEventArgs );
}

//------------------------------------------
void ofNotifyExit(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->exit();
	}
	ofNotifyEvent( ofEvents().exit, voidEventArgs );
}

//------------------------------------------
void ofNotifyWindowResized(int width, int height){
	static ofResizeEventArgs resizeEventArgs;

	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->windowResized(width, height);
	}
	
	resizeEventArgs.width	= width;
	resizeEventArgs.height	= height;
	ofNotifyEvent( ofEvents().windowResized, resizeEventArgs );
}

//------------------------------------------
void ofNotifyDragEvent(ofDragInfo info){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->dragEvent(info);
	}
	
	ofNotifyEvent(ofEvents().fileDragEvent, info);
}

//------------------------------------------
void ofSendMessage(ofMessage msg){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->gotMessage(msg);
	}
	
	ofNotifyEvent(ofEvents().messageEvent, msg);
}

//------------------------------------------
void ofSendMessage(string messageString){
	ofMessage msg(messageString);
	ofSendMessage(msg);
}

void ofNotifyWindowEntry( int state ) {
	
	static ofEntryEventArgs entryArgs;

	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->windowEntry(state);
	}
	
	entryArgs.state = state;
	ofNotifyEvent(ofEvents().windowEntered, entryArgs);
	
}
