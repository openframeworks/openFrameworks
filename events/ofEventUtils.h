#include "ofConstants.h"

#ifdef OF_USING_POCO

#include "Poco/FIFOEvent.h"
#include "Poco/Delegate.h"

using Poco::Delegate;


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
//     ofAddListenerMethod(addon.newIntEvent, this, &Class::method)

template <class EventType,typename ArgumentsType, class ListenerClass>
static void ofAddListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&)){
    event += delegate(listener, listenerMethod);
}


//----------------------------------------------------
// unregister any method of any class to an event.
// the method must provide one the following
// signatures:
//     void method(ArgumentsType & args)
//     void method(const void * sender, ArgumentsType &args)
// ie:
//     ofAddListenerMethod(addon.newIntEvent, this, &Class::method)

template <class EventType,typename ArgumentsType, class ListenerClass>
static void ofRemoveListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&)){
    event -= delegate(listener, listenerMethod);
}

//----------------------------------------------------
// notifies an event so all the registered listeners
// get called
// ie:
//	ofEvents::notify(addon.newIntEvent, intArgument, this)
//
// or in case there's no sender:
//	ofEvents::notify(addon.newIntEvent, intArgument)

template <class EventType,typename ArgumentsType, typename SenderType>
static void ofNotifyEvent(EventType & event, ArgumentsType & args, SenderType * sender){
	event.notify(sender,args);
}

template <class EventType,typename ArgumentsType>
static void ofNotifyEvent(EventType & event, ArgumentsType & args){
	event.notify(NULL,args);
}


#endif
