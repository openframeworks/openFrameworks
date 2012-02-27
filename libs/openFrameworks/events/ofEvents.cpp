#include "ofEvents.h"
#include "ofAppRunner.h"
#include "ofBaseApp.h"
#include "ofUtils.h"
#include "ofGraphics.h"
#include <set>

// core events instance & arguments
#ifdef OF_USING_POCO
	ofCoreEvents ofEvents;
	ofEventArgs voidEventArgs;
#endif


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
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.setup, voidEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyUpdate(){
	ofGetCurrentRenderer()->update();

	ofBaseApp * ofAppPtr = ofGetAppPtr();
	
	if(ofAppPtr){
		ofAppPtr->update();
	}
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.update, voidEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyDraw(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	
	if(ofAppPtr){
		ofAppPtr->draw();
	}
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.draw, voidEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyKeyPressed(int key){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofKeyEventArgs keyEventArgs;

	pressedKeys.insert(key);

	if(ofAppPtr){
		ofAppPtr->keyPressed(key);
	}
	
	#ifdef OF_USING_POCO
		keyEventArgs.key = key;
		ofNotifyEvent( ofEvents.keyPressed, keyEventArgs );
	#endif
	
	
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
	
	#ifdef OF_USING_POCO
		keyEventArgs.key = key;
		ofNotifyEvent( ofEvents.keyReleased, keyEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyMousePressed(int x, int y, int button){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofMouseEventArgs mouseEventArgs;
	
	pressedMouseButtons.insert(button);

	if(ofAppPtr){
		ofAppPtr->mousePressed(x,y,button);
		ofAppPtr->mouseX = x;
		ofAppPtr->mouseY = y;
	}

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		mouseEventArgs.button = button;
		ofNotifyEvent( ofEvents.mousePressed, mouseEventArgs );
	#endif
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

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		mouseEventArgs.button = button;
		ofNotifyEvent( ofEvents.mouseReleased, mouseEventArgs );
	#endif
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

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		mouseEventArgs.button = button;
		ofNotifyEvent( ofEvents.mouseDragged, mouseEventArgs );
	#endif
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

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		ofNotifyEvent( ofEvents.mouseMoved, mouseEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyExit(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->exit();
	}
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.exit, voidEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyWindowResized(int width, int height){
	static ofResizeEventArgs resizeEventArgs;

	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->windowResized(width, height);
	}
	
	#ifdef OF_USING_POCO
		resizeEventArgs.width	= width;
		resizeEventArgs.height	= height;
		ofNotifyEvent( ofEvents.windowResized, resizeEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyDragEvent(ofDragInfo info){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->dragEvent(info);
	}
	
	#ifdef OF_USING_POCO
		ofNotifyEvent(ofEvents.fileDragEvent, info);
	#endif
}

//------------------------------------------
void ofSendMessage(ofMessage msg){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->gotMessage(msg);
	}
	
	#ifdef OF_USING_POCO
		ofNotifyEvent(ofEvents.messageEvent, msg);
	#endif
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
	
#ifdef OF_USING_POCO
	entryArgs.state = state;
	ofNotifyEvent(ofEvents.windowEntered, entryArgs);
#endif
	
}
