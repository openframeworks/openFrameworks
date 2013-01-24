#pragma once

#include "ofConstants.h"

#include "Poco/FIFOEvent.h"
#include "Poco/Delegate.h"

#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------
// define ofEvent as a poco FIFOEvent
// to create your own events use:
// ofEvent<argType> myEvent

template <typename ArgumentsType>
class ofEvent: public Poco::FIFOEvent<ArgumentsType> {
public:

	ofEvent():Poco::FIFOEvent<ArgumentsType>(){

	}

	// allow copy of events, by copying everything except the mutex
	ofEvent(const ofEvent<ArgumentsType> & mom)
	:Poco::FIFOEvent<ArgumentsType>()
	{
		this->_enabled = mom._enabled;
	}

	ofEvent<ArgumentsType> & operator=(const ofEvent<ArgumentsType> & mom){
		if(&mom == this) return *this;
		this->_enabled = mom._enabled;
		return *this;
	}

};


//----------------------------------------------------
// register any method of any class to an event.
// the method must provide one of the following
// signatures:
//     void method(ArgumentsType & args)
//     void method(const void * sender, ArgumentsType &args)
// ie:
//     ofAddListener(addon.newIntEvent, this, &Class::method)

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofAddListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&)){
    event -= Poco::delegate(listener, listenerMethod);
    event += Poco::delegate(listener, listenerMethod);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofAddListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&)){
    event -= Poco::delegate(listener, listenerMethod);
    event += Poco::delegate(listener, listenerMethod);
}

template <class ListenerClass>
void ofAddListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*)){
    event -= Poco::delegate(listener, listenerMethod);
    event += Poco::delegate(listener, listenerMethod);
}

template <class ListenerClass>
void ofAddListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)()){
    event -= Poco::delegate(listener, listenerMethod);
    event += Poco::delegate(listener, listenerMethod);
}


//----------------------------------------------------
// unregister any method of any class to an event.
// the method must provide one the following
// signatures:
//     void method(ArgumentsType & args)
//     void method(const void * sender, ArgumentsType &args)
// ie:
//     ofAddListener(addon.newIntEvent, this, &Class::method)

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofRemoveListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&)){
    event -= Poco::delegate(listener, listenerMethod);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofRemoveListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&)){
    event -= Poco::delegate(listener, listenerMethod);
}

template <class ListenerClass>
void ofRemoveListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*)){
    event -= Poco::delegate(listener, listenerMethod);
}

template <class ListenerClass>
void ofRemoveListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)()){
    event -= Poco::delegate(listener, listenerMethod);
}

//----------------------------------------------------
// notifies an event so all the registered listeners
// get called
// ie:
//	ofNotifyEvent(addon.newIntEvent, intArgument, this)
//
// or in case there's no sender:
//	ofNotifyEvent(addon.newIntEvent, intArgument)

template <class EventType,typename ArgumentsType, typename SenderType>
void ofNotifyEvent(EventType & event, ArgumentsType & args, SenderType * sender){
	event.notify(sender,args);
}

template <class EventType,typename ArgumentsType>
void ofNotifyEvent(EventType & event, ArgumentsType & args){
	event.notify(NULL,args);
}

template <class EventType, typename ArgumentsType, typename SenderType>
void ofNotifyEvent(EventType & event, const ArgumentsType & args, SenderType * sender){
	event.notify(sender,args);
}

template <class EventType,typename ArgumentsType>
void ofNotifyEvent(EventType & event, const ArgumentsType & args){
	event.notify(NULL,args);
}

template <typename SenderType>
void ofNotifyEvent(ofEvent<void> & event, SenderType * sender){
	event.notify(sender);
}

inline void ofNotifyEvent(ofEvent<void> & event){
	event.notify(NULL);
}

