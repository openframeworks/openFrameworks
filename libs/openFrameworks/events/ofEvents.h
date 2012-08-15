#pragma once

#include "ofConstants.h"
#include "ofPoint.h"

class ofWindow;

//-------------------------- mouse/key query
bool		ofGetMousePressed(int button=-1); //by default any button
bool		ofGetKeyPressed(int key=-1); //by default any key

int			ofGetMouseX();
int			ofGetMouseY();

int			ofGetPreviousMouseX();
int			ofGetPreviousMouseY();

void		ofSetMouseValues(int currentX, int currentY, int prevX, int prevY);


void		ofSetEscapeQuitsApp(bool bQuitOnEsc);

void		exitApp(); 

//-----------------------------------------------
class ofDragInfo{
	public:
		vector <string> files;
		ofPoint position;
		ofWindow* window;
};

//-----------------------------------------------

#ifdef OF_USING_POCO
	#define _OF_EVENTS
	#ifndef OF_EVENTS_ADDON
		#include "ofEventUtils.h"

		//-----------------------------------------------
		// event arguments, this are used in oF to pass
		// the data when notifying events

		class ofEventArgs{};

		class ofWindowEventArgs: public ofEventArgs{
		public:
			ofWindow* window;
		};

		class ofEntryEventArgs : public ofEventArgs {
		public:
			int state;
		};

		class ofKeyEventArgs : public ofWindowEventArgs {
		  public:
			int key;
		};

		class ofMouseEventArgs : public ofWindowEventArgs {
		  public:
			int x;
			int y;
			int button;
		};

		class ofScrollEventArgs: public ofWindowEventArgs{
		public:
			float deltaX;
			float deltaY;
		};

		class ofTouchEventArgs : public ofEventArgs {
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
			float x, y;
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

		class ofResizeEventArgs : public ofWindowEventArgs {
		  public:
			int width;
			int height;
		};

		class ofMoveEventArgs : public ofWindowEventArgs {
		public:
			int x;
			int y;
		};
		
		class ofMessage : public ofEventArgs{
			public:
				ofMessage( string msg ){
					message = msg;
				}
				string message;
		};
		
	#else
		#include "ofxEventUtils.h"
	#endif

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
		ofEvent<ofScrollEventArgs> 	scrolled;

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

	class ofWindowEvents
	{
	public:
		ofEvent<ofWindowEventArgs> 	setup;
		ofEvent<ofWindowEventArgs> 	update;
		ofEvent<ofWindowEventArgs> 	draw;
		ofEvent<ofResizeEventArgs> 	windowResized;
		ofEvent<ofMoveEventArgs> 	windowMoved;
		ofEvent<ofWindowEventArgs> 	windowClosed;
		
		ofEvent<ofKeyEventArgs> 	keyPressed;
		ofEvent<ofKeyEventArgs> 	keyReleased;
		
		ofEvent<ofMouseEventArgs> 	mouseMoved;
		ofEvent<ofMouseEventArgs> 	mouseDragged;
		ofEvent<ofMouseEventArgs> 	mousePressed;
		ofEvent<ofMouseEventArgs> 	mouseReleased;
		ofEvent<ofScrollEventArgs> 	scrolled;
		
		ofEvent<ofTouchEventArgs>	touchDown;
		ofEvent<ofTouchEventArgs>	touchUp;
		ofEvent<ofTouchEventArgs>	touchMoved;
		ofEvent<ofTouchEventArgs>	touchDoubleTap;
		ofEvent<ofTouchEventArgs>	touchCancelled;
		
		ofEvent<ofMessage>			messageEvent;
		ofEvent<ofDragInfo>			fileDragEvent;
	};

#endif

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
void ofNotifyScrolled(float dx, float dy);

void ofNotifyExit();
void ofNotifyWindowResized(int width, int height);
void ofNotifyWindowEntry(int state);

void ofNotifyDragEvent(ofDragInfo info);
