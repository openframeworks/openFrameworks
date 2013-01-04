#pragma once

#include "ofConstants.h"
#include "ofPoint.h"
#include "ofEventUtils.h"

//-------------------------- mouse/key query
bool ofGetMousePressed(int button=-1); // by default any button
bool ofGetKeyPressed(int key=-1);      // by default any key

int  ofGetMouseX();
int  ofGetMouseY();

int  ofGetPreviousMouseX();
int  ofGetPreviousMouseY();

void ofSetEscapeQuitsApp(bool bQuitOnEsc);

void exitApp(); 

//-----------------------------------------------
class ofDragInfo{
  public:
    ofDragInfo() {}
    virtual ~ofDragInfo() {}
    vector <string> files;
    ofPoint position;
};

//-----------------------------------------------
// event arguments, this are used in oF to pass
// the data when notifying events

class ofEventArgs{};

class ofEntryEventArgs : public ofEventArgs {
  public:
    ofEntryEventArgs() : state(0) {}
    virtual ~ofEntryEventArgs() {}
    
	int state;
};

class ofKeyEventArgs : public ofEventArgs {
  public:
    ofKeyEventArgs() : key(0), deviceId(0) {}
    virtual ~ofKeyEventArgs() {}

	int key;
    unsigned int deviceId;
};

class ofMouseEventArgs : public ofEventArgs {
  public:
    ofMouseEventArgs() : x(0), y(0), button(0), numClicks(0), deviceId(0) {}
	virtual ~ofMouseEventArgs() {}
    int x;
	int y;
	int button;
    int numClicks;
    unsigned int deviceId;    
};

class ofTouchEventArgs : public ofEventArgs {
  public:
    ofTouchEventArgs() {
        type = stationary;
        id = 0;
        time = 0;
        frame = 0;
        x = y = 0;
        numTouches = 0;
        numTaps = 0;
        width = height = 0;
        angle = 0;
        minoraxis = majoraxis = 0;
        pressure = 0;
        xspeed = yspeed = 0;
        xaccel = yaccel = 0;
        deviceId = 0;
    }
    virtual ~ofTouchEventArgs() {};
    
	enum Type{
		down,
		up,
		move,
		doubleTap,
		cancel,
		stationary
	} type;
    
	int id;
	unsigned long long time;
    unsigned long long frame;
    
	float x, y;
	int numTouches;
    int numTaps;
	float width, height;
	float angle;
	float minoraxis, majoraxis;
	float pressure;
	float xspeed, yspeed;
	float xaccel, yaccel;
    
    unsigned int deviceId;
    
};

class ofAudioEventArgs : public ofEventArgs {
  public:
    ofAudioEventArgs() : buffer(NULL), bufferSize(0), nChannels(0) {}
    virtual ~ofAudioEventArgs() {}
	float* buffer;
	int bufferSize;
	int nChannels;
};

class ofResizeEventArgs : public ofEventArgs {
  public:
    ofResizeEventArgs() : width(0), height(0) {}
    virtual ~ofResizeEventArgs() {}
	int width;
	int height;
};

class ofMessage : public ofEventArgs{
  public:
    ofMessage( string msg ) : message(msg) {}
    virtual ~ofMessage() {}
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
void ofRegisterMouseEvents(ListenerClass * listener){
	ofAddListener(ofEvents().mouseDragged,listener,&ListenerClass::mouseDragged);
	ofAddListener(ofEvents().mouseMoved,listener,&ListenerClass::mouseMoved);
	ofAddListener(ofEvents().mousePressed,listener,&ListenerClass::mousePressed);
	ofAddListener(ofEvents().mouseReleased,listener,&ListenerClass::mouseReleased);
}

template<class ListenerClass>
void ofRegisterKeyEvents(ListenerClass * listener){
	ofAddListener(ofEvents().keyPressed, listener, &ListenerClass::keyPressed);
	ofAddListener(ofEvents().keyReleased, listener, &ListenerClass::keyReleased);
}

template<class ListenerClass>
void ofRegisterTouchEvents(ListenerClass * listener){
	ofAddListener(ofEvents().touchDoubleTap, listener, &ListenerClass::touchDoubleTap);
	ofAddListener(ofEvents().touchDown, listener, &ListenerClass::touchDown);
	ofAddListener(ofEvents().touchMoved, listener, &ListenerClass::touchMoved);
	ofAddListener(ofEvents().touchUp, listener, &ListenerClass::touchUp);
	ofAddListener(ofEvents().touchCancelled, listener, &ListenerClass::touchCancelled);
}

template<class ListenerClass>
void ofRegisterGetMessages(ListenerClass * listener){
	ofAddListener(ofEvents().messageEvent, listener, &ListenerClass::gotMessage);
}

template<class ListenerClass>
void ofRegisterDragEvents(ListenerClass * listener){
	ofAddListener(ofEvents().fileDragEvent, listener, &ListenerClass::dragEvent);
}

template<class ListenerClass>
void ofUnregisterMouseEvents(ListenerClass * listener){
	ofRemoveListener(ofEvents().mouseDragged,listener,&ListenerClass::mouseDragged);
	ofRemoveListener(ofEvents().mouseMoved,listener,&ListenerClass::mouseMoved);
	ofRemoveListener(ofEvents().mousePressed,listener,&ListenerClass::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased,listener,&ListenerClass::mouseReleased);
}

template<class ListenerClass>
void ofUnregisterKeyEvents(ListenerClass * listener){
	ofRemoveListener(ofEvents().keyPressed, listener, &ListenerClass::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, listener, &ListenerClass::keyReleased);
}

template<class ListenerClass>
void ofUnregisterTouchEvents(ListenerClass * listener){
	ofRemoveListener(ofEvents().touchDoubleTap, listener, &ListenerClass::touchDoubleTap);
	ofRemoveListener(ofEvents().touchDown, listener, &ListenerClass::touchDown);
	ofRemoveListener(ofEvents().touchMoved, listener, &ListenerClass::touchMoved);
	ofRemoveListener(ofEvents().touchUp, listener, &ListenerClass::touchUp);
	ofRemoveListener(ofEvents().touchCancelled, listener, &ListenerClass::touchCancelled);
}

template<class ListenerClass>
void ofUnregisterGetMessages(ListenerClass * listener){
	ofRemoveListener(ofEvents().messageEvent, listener, &ListenerClass::gotMessage);
}

template<class ListenerClass>
void ofUnregisterDragEvents(ListenerClass * listener){
	ofRemoveListener(ofEvents().fileDragEvent, listener, &ListenerClass::dragEvent);
}

//  event notification only for internal OF use
void ofNotifySetup();
void ofNotifyUpdate();
void ofNotifyDraw();

void ofNotifyKeyPressed(int key);
void ofNotifyKeyReleased(int key);

void ofNotifyMousePressed(int x, int y, int button);
void ofNotifyMouseReleased(int x, int y, int button);
void ofNotifyMouseDragged(int x, int y, int button);
void ofNotifyMouseMoved(int x, int y);

void ofNotifyExit();
void ofNotifyWindowResized(int width, int height);
void ofNotifyWindowEntry(int state);

void ofNotifyDragEvent(ofDragInfo info);
