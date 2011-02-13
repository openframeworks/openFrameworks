#ifndef _OF_EVENTS
#error "ofEventUtils shouldn't be included directly, include ofEvents.h or ofMain.h"
#endif

#include "ofConstants.h"

#ifdef OF_USING_POCO

#include "Poco/FIFOEvent.h"
#include "Poco/Delegate.h"



//-----------------------------------------
// define ofEvent as a poco FIFOEvent
// to create your own events use:
// ofEvent<argType> myEvent

template <typename ArgumentsType>
class ofEvent: public Poco::FIFOEvent<ArgumentsType> {};



//----------------------------------------------------
// register any method of any class to an event.
// the method must provide one of the following
// signatures:
//     void method(ArgumentsType & args)
//     void method(const void * sender, ArgumentsType &args)
// ie:
//     ofAddListener(addon.newIntEvent, this, &Class::method)
template <class EventType,typename ArgumentsType, class ListenerClass>
static void ofAddListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&)){
    event += Poco::delegate(listener, listenerMethod);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
static void ofAddListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&)){
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
static void ofRemoveListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&)){
    event -= Poco::delegate(listener, listenerMethod);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
static void ofRemoveListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&)){
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
static void ofNotifyEvent(EventType & event, ArgumentsType & args, SenderType * sender){
	event.notify(sender,args);
}

template <class EventType,typename ArgumentsType>
static void ofNotifyEvent(EventType & event, ArgumentsType & args){
	event.notify(NULL,args);
}


#endif
