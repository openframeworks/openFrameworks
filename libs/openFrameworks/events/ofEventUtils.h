#pragma once

#include "ofConstants.h"

#include "Poco/PriorityEvent.h"
#include "Poco/PriorityDelegate.h"

#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------
// define ofEvent as a poco FIFOEvent
// to create your own events use:
// ofEvent<argType> myEvent

template <typename ArgumentsType>
class ofEvent: public Poco::PriorityEvent<ArgumentsType> {
public:

	ofEvent():Poco::PriorityEvent<ArgumentsType>(){

	}

	// allow copy of events, by copying everything except the mutex
	ofEvent(const ofEvent<ArgumentsType> & mom)
	:Poco::PriorityEvent<ArgumentsType>()
	{
		this->_enabled = mom._enabled;
	}

	ofEvent<ArgumentsType> & operator=(const ofEvent<ArgumentsType> & mom){
		if(&mom == this) return *this;
		this->_enabled = mom._enabled;
		return *this;
	}

};

class ofEventAttendedException: public Poco::Exception{

};

inline void ofEventMarkAttended(){
	throw ofEventAttendedException();
}

enum ofEventOrder{
	OF_EVENT_ORDER_BEFORE_APP=0,
	OF_EVENT_ORDER_APP=100,
	OF_EVENT_ORDER_AFTER_APP=200
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
void ofAddListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= Poco::priorityDelegate(listener, listenerMethod, prio);
    event += Poco::priorityDelegate(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofAddListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= Poco::priorityDelegate(listener, listenerMethod, prio);
    event += Poco::priorityDelegate(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofAddListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= Poco::priorityDelegate(listener, listenerMethod, prio);
    event += Poco::priorityDelegate(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofAddListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= Poco::priorityDelegate(listener, listenerMethod, prio);
    event += Poco::priorityDelegate(listener, listenerMethod, prio);
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
void ofRemoveListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= Poco::priorityDelegate(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofRemoveListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= Poco::priorityDelegate(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofRemoveListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= Poco::priorityDelegate(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofRemoveListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= Poco::priorityDelegate(listener, listenerMethod, prio);
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
	try{
		event.notify(sender,args);
	}catch(...){

	}
}

template <class EventType,typename ArgumentsType>
void ofNotifyEvent(EventType & event, ArgumentsType & args){
	try{
		event.notify(NULL,args);
	}catch(...){

	}
}

template <class EventType, typename ArgumentsType, typename SenderType>
void ofNotifyEvent(EventType & event, const ArgumentsType & args, SenderType * sender){
	try{
		event.notify(sender,args);
	}catch(...){

	}
}

template <class EventType,typename ArgumentsType>
void ofNotifyEvent(EventType & event, const ArgumentsType & args){
	try{
		event.notify(NULL,args);
	}catch(...){

	}
}

template <typename SenderType>
void ofNotifyEvent(ofEvent<void> & event, SenderType * sender){
	try{
		event.notify(sender);
	}catch(...){

	}
}

inline void ofNotifyEvent(ofEvent<void> & event){
	try{
		event.notify(NULL);
	}catch(...){

	}
}

