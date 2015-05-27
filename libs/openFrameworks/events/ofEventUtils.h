#pragma once

#include "ofConstants.h"

#include "Poco/PriorityEvent.h"
#include "Poco/PriorityDelegate.h"
#include "ofDelegate.h"

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

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofAddListener(EventType & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= ofDelegate<ListenerClass,ArgumentsType,true>(listener, listenerMethod, prio);
    event += ofDelegate<ListenerClass,ArgumentsType,true>(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofAddListener(EventType & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= ofDelegate<ListenerClass,ArgumentsType,false>(listener, listenerMethod, prio);
    event += ofDelegate<ListenerClass,ArgumentsType,false>(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofAddListener(ofEvent<void> & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= ofDelegate<ListenerClass,void,true>(listener, listenerMethod, prio);
    event += ofDelegate<ListenerClass,void,true>(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofAddListener(ofEvent<void> & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= ofDelegate<ListenerClass,void,false>(listener, listenerMethod, prio);
    event += ofDelegate<ListenerClass,void,false>(listener, listenerMethod, prio);
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

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofRemoveListener(EventType & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= ofDelegate<ListenerClass,ArgumentsType,true>(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofRemoveListener(EventType & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= ofDelegate<ListenerClass,ArgumentsType,false>(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofRemoveListener(ofEvent<void> & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= ofDelegate<ListenerClass,void,true>(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofRemoveListener(ofEvent<void> & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event -= ofDelegate<ListenerClass,void,false>(listener, listenerMethod, prio);
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
	}catch(ofEventAttendedException & e){

	}
}

template <class EventType,typename ArgumentsType>
void ofNotifyEvent(EventType & event, ArgumentsType & args){
	try{
		event.notify(NULL,args);
	}catch(ofEventAttendedException & e){

	}
}

template <class EventType, typename ArgumentsType, typename SenderType>
void ofNotifyEvent(EventType & event, const ArgumentsType & args, SenderType * sender){
	try{
		event.notify(sender,args);
	}catch(ofEventAttendedException & e){

	}
}

template <class EventType,typename ArgumentsType>
void ofNotifyEvent(EventType & event, const ArgumentsType & args){
	try{
		event.notify(NULL,args);
	}catch(ofEventAttendedException & e){

	}
}

template <typename SenderType>
void ofNotifyEvent(ofEvent<void> & event, SenderType * sender){
	try{
		event.notify(sender);
	}catch(ofEventAttendedException & e){

	}
}

inline void ofNotifyEvent(ofEvent<void> & event){
	try{
		event.notify(NULL);
	}catch(ofEventAttendedException & e){

	}
}

