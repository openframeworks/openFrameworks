// =============================================================================
//
// Copyright (c) 2009-2016 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include <string>
#include <vector>
#include "ofx/DOM/Events.h"


namespace ofx {
namespace DOM {


/// \brief A class representing an EventTarget.
///
/// EventTargets know how to handle events. This class is usually inherited
/// using the curiously-recurring template pattern.
///
/// \tparam EventTargetType The type of the Tvent target.
template <class EventTargetType>
class EventTarget
{
public:
	/// \brief Create an EventTarget.
	EventTarget();

	/// \brief Destroy the EventTarget.
	virtual ~EventTarget();

	template <class EventType, typename ArgumentsType, class ListenerClass>
	void addEventListener(EventType& event,
						  void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&),
						  bool useCapture = false,
						  int priority = OF_EVENT_ORDER_AFTER_APP)
	{
		ofAddListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
	}

	template <class EventType, typename ArgumentsType, class ListenerClass>
	void addEventListener(EventType& event,
						  void (ListenerClass::*listenerMethod)(ArgumentsType&),
						  bool useCapture = false,
						  int priority = OF_EVENT_ORDER_AFTER_APP)
	{
		ofAddListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
	}

	template <class EventType, typename ArgumentsType, class ListenerClass>
	void addEventListener(EventType& event,
						  bool (ListenerClass::*listenerMethod)(const void*, ArgumentsType&),
						  bool useCapture = false,
						  int priority = OF_EVENT_ORDER_AFTER_APP)
	{
		ofAddListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
	}

	template <class EventType, typename ArgumentsType, class ListenerClass>
	void addEventListener(EventType& event,
						  bool (ListenerClass::*listenerMethod)(ArgumentsType&),
						  bool useCapture = false,
						  int priority = OF_EVENT_ORDER_AFTER_APP)
	{
		ofAddListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
	}

	template <class EventType, typename ArgumentsType, class ListenerClass>
	void removeEventListener(EventType& event,
							 void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&),
							 bool useCapture = false,
							 int priority = OF_EVENT_ORDER_AFTER_APP)
	{
		ofRemoveListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
	}

	template <class EventType, typename ArgumentsType, class ListenerClass>
	void removeEventListener(EventType& event,
							 void (ListenerClass::*listenerMethod)(ArgumentsType&),
							 bool useCapture = false,
							 int priority = OF_EVENT_ORDER_AFTER_APP)
	{
		ofRemoveListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
	}


	template <class EventType, typename ArgumentsType, class ListenerClass>
	void removeEventListener(EventType& event,
							 bool (ListenerClass::*listenerMethod)(const void*, ArgumentsType&),
							 bool useCapture = false,
							 int priority = OF_EVENT_ORDER_AFTER_APP)
	{
		ofRemoveListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
	}


	template <class EventType, typename ArgumentsType, class ListenerClass>
	void removeEventListener(EventType& event,
							 bool (ListenerClass::*listenerMethod)(ArgumentsType&),
							 bool useCapture = false,
							 int priority = OF_EVENT_ORDER_AFTER_APP)
	{
		ofRemoveListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
	}

	/// \brief Dispatch the given event.
	///
	/// This will return true if the default action for this event should be
	/// prevented.
	///
	/// \param event The Event to dispatch.
	/// \tparam EventType The Event type to dispatch.
	/// \returns true iff one of the responders called Event::preventDefault().
	template <class EventType>
	bool dispatchEvent(EventType& event);

	/// \brief Handle the given event.
	///
	/// This will return true if the default action for this event should be
	/// prevented.
	///
	/// \tparam EventArgsType The EventArgs type to dispatch.
	/// \param event The Event to dispatch.
	/// \returns true if the EventTarget was registered to handle the event.
	template <class EventArgsType>
	bool handleEvent(EventArgsType& e);

	/// \brief Determine if the EventTarget has listeners for an event.
	/// \param event The event name.
	/// \returns true if it has registered listeners for this event.
	bool hasListenersForEventType(const std::string& type) const;

	/// \brief Determine if the EventTarget is registered to receive the type of events.
	/// \param type The event type.
	/// \returns true if it is registered to receive the type of events.
	bool isEventTypeRegistered(const std::string& type) const;

	/// \brief Register a new event type by name.
	/// \param type The event type.
	/// \param event A pointer to the DOMEvent<> that will be called.
	void registerEventType(const std::string& type, BaseDOMEvent* event);

	/// \brief Unregister a new event type by name.
	/// \param type The event type.
	void unregisterEventType(const std::string& type);

	virtual void onSetup()
	{
	}

	virtual void onUpdate()
	{
	}

	virtual void onExit()
	{
	}

	DOMEvent<KeyboardUIEventArgs> keyDown;
	DOMEvent<KeyboardUIEventArgs> keyUp;

	DOMEvent<FocusEventArgs> blur;
	DOMEvent<FocusEventArgs> focusIn;
	DOMEvent<FocusEventArgs> focus;
	DOMEvent<FocusEventArgs> focusOut;

	ofEvent<ElementEventArgs> addedTo;
	ofEvent<ElementEventArgs> removedFrom;
	ofEvent<ElementOrderEventArgs> reordered;

	ofEvent<ElementEventArgs> siblingAdded;
	ofEvent<ElementEventArgs> siblingRemoved;
	ofEvent<ElementOrderEventArgs> siblingReordered;

	ofEvent<ElementEventArgs> childAdded;
	ofEvent<ElementEventArgs> childRemoved;
	ofEvent<ElementOrderEventArgs> childReordered;
	ofEvent<MoveEventArgs> childMoved;

	ofEvent<MoveEventArgs> move;
	ofEvent<ResizeEventArgs> resize;

	ofEvent<AttributeEventArgs> attributeSet;
	ofEvent<AttributeEventArgs> attributeCleared;

	ofEvent<EnablerEventArgs> enabled;
	ofEvent<EnablerEventArgs> locked;
	ofEvent<EnablerEventArgs> hidden;

protected:
	std::unordered_map<std::string, BaseDOMEvent*> _eventRegistry;

};


template <class EventTargetType>
EventTarget<EventTargetType>::EventTarget()
{
	// Register default events.
	// TODO: do we want to automatically register all of these events?
	// TODO: do we need the event target to have all of these events, or could
	// it be on more of an ad hoc basis ... could be a speed optimization not
	// to have each one.

	// theoretically not having any events registered woudl make isEventTypeRegistered much faster.
	_eventRegistry = {

		{ KeyboardUIEventArgs::KEY_DOWN, &keyDown },
		{ KeyboardUIEventArgs::KEY_UP, &keyUp }
	};
}


template <class EventTargetType>
EventTarget<EventTargetType>::~EventTarget()
{
}


template <class EventTargetType>
template <class EventType>
bool EventTarget<EventTargetType>::dispatchEvent(EventType& event)
{
	// Get the target (this Element).
	EventTargetType* target = dynamic_cast<EventTargetType*>(this);

	// Create the path from the target to the document.
	std::vector<EventTargetType*> targets;

	// The target will be at the beginning of the list.
	// The root document will be at the end of the list.
	do
	{
		targets.push_back(target);
		target = target->parent();
	}
	while (target);


	// Capture and Target phase (document -> target).

	// Begin with the document (at the end of the list).
	auto riter = targets.rbegin();

	// Cycle through the targets from the document to the event.target().
	while (riter != targets.rend())
	{
		event.setPhase(event.target() == *riter ? EventArgs::Phase::AT_TARGET : EventArgs::Phase::CAPTURING_PHASE);
		event.setCurrentTarget(*riter);

		// Here we handle event and assume that if the currentTarget
		// can't handle the event, it will return quickly with no errors.
		// This is potentially faster that asking the target to search its
		// registry and then asking it to search its registry _again_ to
		// actually handle the event.
		bool isRegisteredHandler = (*riter)->handleEvent(event);

		// If the event is cancelled, return appropriately.
		if (event.isCancelled())
		{
			return !event.isDefaultPrevented();
		}

		// TODO
		if (!isRegisteredHandler)
		{
			// Here we might remove any listeners from the list of targets that cannot handle ... ?
			// Does this prevent us from dynamically adding and removing listeners between the
			// bubble and capture phases though?
		}

		// Continue iterating.
		++riter;
	}

	// Bubble phase if needed (target -> document).
	if (targets.size() > 1 && event.bubbles())
	{
		// Begin with the _parent_ of the target element (we already dealt
		// with the target element during the capture / target phased).
		auto bubbleIter = targets.begin() + 1;

		while (bubbleIter != targets.end())
		{
			event.setPhase(EventArgs::Phase::BUBBLING_PHASE);

			event.setCurrentTarget(*bubbleIter);

			// Here we handle event and assume that if the currentTarget
			// can't handle the event, it will return quickly with no errors.
			// This is potentially faster that asking the target to search its
			// registry and then asking it to search its registry _again_ to
			// actually handle the event.
			(*bubbleIter)->handleEvent(event);

			if (event.isCancelled())
			{
				return !event.isDefaultPrevented();
			}

			++bubbleIter;
		}
	}

	return event.isDefaultPrevented();
}


template <class EventTargetType>
bool EventTarget<EventTargetType>::hasListenersForEventType(const std::string& type) const
{
	auto iter = _eventRegistry.find(type);

	if (iter != _eventRegistry.end())
	{
		return iter->second->hasListeners();
	}
	else
	{
		return false;
	}
}


template <class EventTargetType>
template <class EventArgsType>
bool EventTarget<EventTargetType>::handleEvent(EventArgsType& e)
{



	auto iter = _eventRegistry.find(e.type());

	if (iter != _eventRegistry.end())
	{
		DOMEvent<EventArgsType>* _event = dynamic_cast<DOMEvent<EventArgsType>*>(iter->second);

		if (_event)
		{
//            if (e.type() == "buttonpressed")
//            {
//                cout << "event: " << e.type() << " being handled by : " << (e.getCurrentTarget() ? e.getCurrentTarget()->getId() : "nullptr") << endl;
//                cout << " hasListeners -> " << _event->hasListeners() << std::endl;
//                cout << " e -> " << e.toString() << std::endl;
//            }
			_event->notify(e);
			return true;
		}
		else
		{
			throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "EventTarget::handleEvent: " + e.type() + " invalid listener registered.");
		}
	}
	else
	{
		return false;
	}
}


template <class EventTargetType>
bool EventTarget<EventTargetType>::isEventTypeRegistered(const std::string& type) const
{
	return _eventRegistry.find(type) != _eventRegistry.end();
}


template <class EventTargetType>
void EventTarget<EventTargetType>::registerEventType(const std::string& type,
													 BaseDOMEvent* event)
{
	_eventRegistry[type] = event;
}


template <class EventTargetType>
void EventTarget<EventTargetType>::unregisterEventType(const std::string& type)
{
	_eventRegistry.erase(_eventRegistry.find(type));
}


} } // namespace ofx::DOM
