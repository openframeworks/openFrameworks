
#include "ofEventModel.h"


Poco::FIFOEvent<Poco::EventArgs> ofSetupEventManager;
Poco::FIFOEvent<Poco::EventArgs> ofUpdateEventManager;
Poco::FIFOEvent<Poco::EventArgs> ofDrawEventManager;
Poco::FIFOEvent<Poco::EventArgs> ofExitEventManager;

Poco::FIFOEvent<ofKeyEvent> ofKeyPressedEventManager;
Poco::FIFOEvent<ofKeyEvent> ofKeyReleasedEventManager;

Poco::FIFOEvent<ofMouseEvent> ofMouseMovedEventManager;
Poco::FIFOEvent<ofMouseEvent> ofMouseDraggedEventManager;
Poco::FIFOEvent<ofMouseEvent> ofMousePressedEventManager;
Poco::FIFOEvent<ofMouseEvent> ofMouseReleasedEventManager;


