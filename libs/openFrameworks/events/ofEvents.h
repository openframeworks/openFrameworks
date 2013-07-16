#pragma once

#include "ofConstants.h"
#include "ofPoint.h"
#include "ofEventUtils.h"

//-------------------------- mouse/key query
bool		ofGetMousePressed(int button=-1); //by default any button
bool		ofGetKeyPressed(int key=-1); //by default any key

int			ofGetMouseX();
int			ofGetMouseY();

int			ofGetPreviousMouseX();
int			ofGetPreviousMouseY();

void		ofSetEscapeQuitsApp(bool bQuitOnEsc);

void		exitApp(); 

//-----------------------------------------------
class ofDragInfo{
	public:
		vector <string> files;
		ofPoint position;
};


//-----------------------------------------------
// event arguments, this are used in oF to pass
// the data when notifying events

class ofEventArgs{};

class ofEntryEventArgs : public ofEventArgs {
public:
	int state;
};

class ofKeyEventArgs : public ofEventArgs {
  public:
	enum Type{
		Pressed,
		Released,
	} type;
	int key;
};

class ofMouseEventArgs : public ofEventArgs, public ofVec2f {
  public:
	enum Type{
		Pressed,
		Moved,
		Released,
		Dragged
	} type;
	int button;
};

class ofTouchEventArgs : public ofEventArgs, public ofVec2f {
  public:
	enum Type{
		down,
		up,
		move,
		doubleTap,
		cancel
	} type;

	int id;
	int time;
	int numTouches;
	float width, height;
	float angle;
	float minoraxis, majoraxis;
	float pressure;
	float xspeed, yspeed;
	float xaccel, yaccel;
};

class ofAudioEventArgs : public ofEventArgs {
  public:
	float* buffer;
	int bufferSize;
	int nChannels;
};

class ofResizeEventArgs : public ofEventArgs {
  public:
	int width;
	int height;
};

class ofMessage : public ofEventArgs{
	public:
		ofMessage( string msg ){
			message = msg;
		}
		string message;
};
		

class ofCoreEvents {
  public:
	ofEvent<ofEventArgs> 		setup;
	ofEvent<ofEventArgs> 		update;
	ofEvent<ofEventArgs> 		draw;
	ofEvent<ofEventArgs> 		exit;

	ofEvent<ofEntryEventArgs>	windowEntered;
	ofEvent<ofResizeEventArgs> 	windowResized;

	ofEvent<ofKeyEventArgs> 	keyPressed;
	ofEvent<ofKeyEventArgs> 	keyReleased;

	ofEvent<ofMouseEventArgs> 	mouseMoved;
	ofEvent<ofMouseEventArgs> 	mouseDragged;
	ofEvent<ofMouseEventArgs> 	mousePressed;
	ofEvent<ofMouseEventArgs> 	mouseReleased;

	ofEvent<ofAudioEventArgs> 	audioReceived;
	ofEvent<ofAudioEventArgs> 	audioRequested;

	ofEvent<ofTouchEventArgs>	touchDown;
	ofEvent<ofTouchEventArgs>	touchUp;
	ofEvent<ofTouchEventArgs>	touchMoved;
	ofEvent<ofTouchEventArgs>	touchDoubleTap;
	ofEvent<ofTouchEventArgs>	touchCancelled;

	ofEvent<ofMessage>			messageEvent;
	ofEvent<ofDragInfo>			fileDragEvent;

	void disable(){
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
		audioReceived.disable();
		audioRequested.disable();
		touchDown.disable();
		touchUp.disable();
		touchMoved.disable();
		touchDoubleTap.disable();
		touchCancelled.disable();
		messageEvent.disable();
		fileDragEvent.disable();
	}

	void enable(){
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
		audioReceived.enable();
		audioRequested.enable();
		touchDown.enable();
		touchUp.enable();
		touchMoved.enable();
		touchDoubleTap.enable();
		touchCancelled.enable();
		messageEvent.enable();
		fileDragEvent.enable();
	}
};

void ofSendMessage(ofMessage msg);
void ofSendMessage(string messageString);

ofCoreEvents & ofEvents();

template<class ListenerClass>
void ofRegisterMouseEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().mouseDragged,listener,&ListenerClass::mouseDragged,prio);
	ofAddListener(ofEvents().mouseMoved,listener,&ListenerClass::mouseMoved,prio);
	ofAddListener(ofEvents().mousePressed,listener,&ListenerClass::mousePressed,prio);
	ofAddListener(ofEvents().mouseReleased,listener,&ListenerClass::mouseReleased,prio);
}

template<class ListenerClass>
void ofRegisterKeyEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().keyPressed, listener, &ListenerClass::keyPressed,prio);
	ofAddListener(ofEvents().keyReleased, listener, &ListenerClass::keyReleased,prio);
}

template<class ListenerClass>
void ofRegisterTouchEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().touchDoubleTap, listener, &ListenerClass::touchDoubleTap,prio);
	ofAddListener(ofEvents().touchDown, listener, &ListenerClass::touchDown,prio);
	ofAddListener(ofEvents().touchMoved, listener, &ListenerClass::touchMoved,prio);
	ofAddListener(ofEvents().touchUp, listener, &ListenerClass::touchUp,prio);
	ofAddListener(ofEvents().touchCancelled, listener, &ListenerClass::touchCancelled,prio);
}

template<class ListenerClass>
void ofRegisterGetMessages(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().messageEvent, listener, &ListenerClass::gotMessage,prio);
}

template<class ListenerClass>
void ofRegisterDragEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().fileDragEvent, listener, &ListenerClass::dragEvent,prio);
}

template<class ListenerClass>
void ofUnregisterMouseEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().mouseDragged,listener,&ListenerClass::mouseDragged,prio);
	ofRemoveListener(ofEvents().mouseMoved,listener,&ListenerClass::mouseMoved,prio);
	ofRemoveListener(ofEvents().mousePressed,listener,&ListenerClass::mousePressed,prio);
	ofRemoveListener(ofEvents().mouseReleased,listener,&ListenerClass::mouseReleased,prio);
}

template<class ListenerClass>
void ofUnregisterKeyEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().keyPressed, listener, &ListenerClass::keyPressed,prio);
	ofRemoveListener(ofEvents().keyReleased, listener, &ListenerClass::keyReleased,prio);
}

template<class ListenerClass>
void ofUnregisterTouchEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().touchDoubleTap, listener, &ListenerClass::touchDoubleTap,prio);
	ofRemoveListener(ofEvents().touchDown, listener, &ListenerClass::touchDown,prio);
	ofRemoveListener(ofEvents().touchMoved, listener, &ListenerClass::touchMoved,prio);
	ofRemoveListener(ofEvents().touchUp, listener, &ListenerClass::touchUp,prio);
	ofRemoveListener(ofEvents().touchCancelled, listener, &ListenerClass::touchCancelled,prio);
}

template<class ListenerClass>
void ofUnregisterGetMessages(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().messageEvent, listener, &ListenerClass::gotMessage,prio);
}

template<class ListenerClass>
void ofUnregisterDragEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().fileDragEvent, listener, &ListenerClass::dragEvent,prio);
}

//  event notification only for internal OF use
void ofNotifySetup();
void ofNotifyUpdate();
void ofNotifyDraw();

void ofNotifyKeyPressed(int key);
void ofNotifyKeyReleased(int key);
void ofNotifyKeyEvent(const ofKeyEventArgs & keyEvent);

void ofNotifyMousePressed(int x, int y, int button);
void ofNotifyMouseReleased(int x, int y, int button);
void ofNotifyMouseDragged(int x, int y, int button);
void ofNotifyMouseMoved(int x, int y);
void ofNotifyMouseEvent(const ofMouseEventArgs & mouseEvent);

void ofNotifyExit();
void ofNotifyWindowResized(int width, int height);
void ofNotifyWindowEntry(int state);

void ofNotifyDragEvent(ofDragInfo info);
