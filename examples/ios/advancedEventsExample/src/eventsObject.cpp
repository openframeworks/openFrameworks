#include "eventsObject.h"

//--------------------------------------------------------------
eventsObject::eventsObject(){
    counter = 0;
}

//--------------------------------------------------------------
void eventsObject::enable(){
    ofAddListener(ofEvents().update, this, &eventsObject::update);
}

//--------------------------------------------------------------
void eventsObject::disable(){
    ofRemoveListener(ofEvents().update, this, &eventsObject::update);
}

//--------------------------------------------------------------
void eventsObject::update(ofEventArgs & args){
    counter += 0.3;
    int intCounter = (int)counter;
    ofNotifyEvent(newFloatEvent, counter, this);
    ofNotifyEvent(newIntEvent, intCounter, this);
}

