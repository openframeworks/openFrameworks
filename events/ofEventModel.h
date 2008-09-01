#ifndef OF_EVENT_MODEL
#define OF_EVENT_MODEL

#include "Poco/FIFOEvent.h"
#include "Poco/EventArgs.h"


class ofKeyEvent {
  public:
    int key;
    
    ofKeyEvent( int _key=0 ) {
        key = _key;
    }
};


class ofMouseEvent {
  public:
    int x; 
    int y; 
    int button;
   
    ofMouseEvent( int _x=0, int _y=0, int _button=0 ) {
        x = _x; y = _y; button = _button;
    }
};


extern Poco::FIFOEvent<Poco::EventArgs> ofSetupEventManager;
extern Poco::FIFOEvent<Poco::EventArgs> ofUpdateEventManager;
extern Poco::FIFOEvent<Poco::EventArgs> ofDrawEventManager;
extern Poco::FIFOEvent<Poco::EventArgs> ofExitEventManager;

extern Poco::FIFOEvent<ofKeyEvent> ofKeyPressedEventManager;
extern Poco::FIFOEvent<ofKeyEvent> ofKeyReleasedEventManager;

extern Poco::FIFOEvent<ofMouseEvent> ofMouseMovedEventManager;
extern Poco::FIFOEvent<ofMouseEvent> ofMouseDraggedEventManager;
extern Poco::FIFOEvent<ofMouseEvent> ofMousePressedEventManager;
extern Poco::FIFOEvent<ofMouseEvent> ofMouseReleasedEventManager;


#endif
