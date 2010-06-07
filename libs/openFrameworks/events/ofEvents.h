#ifndef _OF_EVENTS
#define _OF_EVENTS

#include "ofConstants.h"
#include "ofUtils.h"

#ifdef OF_USING_POCO

	#ifndef OF_EVENTS_ADDON
		#include "ofEventUtils.h"

		//-----------------------------------------------
		// event arguments, this are used in oF to pass
		// the data when notifying events

		class ofEventArgs{};

		class ofKeyEventArgs : public ofEventArgs {
		  public:
			int key;
		};

		class ofMouseEventArgs : public ofEventArgs {
		  public:
			int x;
			int y;
			int button;
		};

		class ofTouchEventArgs : public ofEventArgs {
		  public:
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

		class ofResizeEventArgs : public ofEventArgs {
		  public:
			int width;
			int height;
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
		}
	};



	extern ofCoreEvents ofEvents;

	template<class ListenerClass>
	void ofRegisterMouseEvents(ListenerClass * listener){
		ofAddListener(ofEvents.mouseDragged,listener,&ListenerClass::mouseDragged);
		ofAddListener(ofEvents.mouseMoved,listener,&ListenerClass::mouseMoved);
		ofAddListener(ofEvents.mousePressed,listener,&ListenerClass::mousePressed);
		ofAddListener(ofEvents.mouseReleased,listener,&ListenerClass::mouseReleased);
	}

	template<class ListenerClass>
	void ofRegisterKeyEvents(ListenerClass * listener){
		ofAddListener(ofEvents.keyPressed, listener, &ListenerClass::keyPressed);
		ofAddListener(ofEvents.keyReleased, listener, &ListenerClass::keyReleased);
	}

	template<class ListenerClass>
	void ofRegisterTouchEvents(ListenerClass * listener){
		ofAddListener(ofEvents.touchDoubleTap, listener, &ListenerClass::touchDoubleTap);
		ofAddListener(ofEvents.touchDown, listener, &ListenerClass::touchDown);
		ofAddListener(ofEvents.touchMoved, listener, &ListenerClass::touchMoved);
		ofAddListener(ofEvents.touchUp, listener, &ListenerClass::touchUp);
	}

	template<class ListenerClass>
	void ofUnregisterMouseEvents(ListenerClass * listener){
		ofRemoveListener(ofEvents.mouseDragged,listener,&ListenerClass::mouseDragged);
		ofRemoveListener(ofEvents.mouseMoved,listener,&ListenerClass::mouseMoved);
		ofRemoveListener(ofEvents.mousePressed,listener,&ListenerClass::mousePressed);
		ofRemoveListener(ofEvents.mouseReleased,listener,&ListenerClass::mouseReleased);
	}

	template<class ListenerClass>
	void ofUnregisterKeyEvents(ListenerClass * listener){
		ofRemoveListener(ofEvents.keyPressed, listener, &ListenerClass::keyPressed);
		ofRemoveListener(ofEvents.keyReleased, listener, &ListenerClass::keyReleased);
	}

	template<class ListenerClass>
	void ofUnregisterTouchEvents(ListenerClass * listener){
		ofRemoveListener(ofEvents.touchDoubleTap, listener, &ListenerClass::touchDoubleTap);
		ofRemoveListener(ofEvents.touchDown, listener, &ListenerClass::touchDown);
		ofRemoveListener(ofEvents.touchMoved, listener, &ListenerClass::touchMoved);
		ofRemoveListener(ofEvents.touchUp, listener, &ListenerClass::touchUp);
	}

	#endif

#endif

