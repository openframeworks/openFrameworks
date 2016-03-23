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
#include <type_traits>
#include <utility>
#include <ctime>
#include "ofEvents.h"
#include "ofx/DOM/Exceptions.h"
#include "ofx/DOM/Types.h"


namespace ofx {
namespace DOM {


class Element;


/// \brief The base type describing a named Element Event.
///
/// \sa http://www.w3.org/TR/DOM-Level-3-Events/
class EventArgs
{
public:
	/// \brief Create EventArgs with a type.
	/// \param type The event type string (case-insensitive).
	/// \param source The source of the event.
	/// \param target The target element.
	/// \param bubbles True iff the argument bubbles after AT_TARGET phase.
	/// \param cancelable True iff the event can be cancelled by a listener.
	/// \param timestamp The timestamp of the event.
	EventArgs(const std::string& type,
			  Element* source,
			  Element* target,
			  bool bubbles,
			  bool cancelable,
			  uint64_t timestamp);

	/// \brief Create EventArgs with a type.
	/// \param type The event type string (case-insensitive).
	/// \param source The source of the event.
	/// \param target The target element.
	/// \param bubbles True iff the argument bubbles after AT_TARGET phase.
	/// \param cancelable True iff the event can be cancelled by a listener.
	/// \param timestamp The timestamp of the event.
	EventArgs(const std::string& type,
			  Element* source,
			  Element* target,
			  Element* relatedTarget,
			  bool bubbles,
			  bool cancelable,
			  uint64_t timestamp);

	/// \brief Destroy the EventArgs.
	virtual ~EventArgs();

	/// \brief Get the event type.
	/// \returns the event type string.
	const std::string& type() const;

	enum class Phase
	{
		/// \brief Events not currently dispatched are in this phase.
		NONE = 0,
		/// \brief When an event is dispatched to an object that participates in a tree it will be in this phase before it reaches its target attribute value.
		CAPTURING_PHASE = 1,
		/// \brief When an event is dispatched it will be in this phase on its target attribute value.
		AT_TARGET = 2,
		/// \brief When an event is dispatched to an object that participates in a tree it will be in this phase after it reaches its target attribute value.
		BUBBLING_PHASE = 3
	};


	/// \brief Stop the propagation of the event.
	///
	/// The stopPropagation method is used prevent further propagation of an
	/// event during event flow. If this method is called by any EventListener
	/// the event will cease propagating through the tree. The event will
	/// complete dispatch to all listeners on the current EventTarget before
	/// event flow stops. This method may be used during any stage of event
	/// flow.
	void stopPropagation();

	// \brief Stop the propagation of the event, including all other events at the target.
	//
	// If successful this event throws ofEventAttendedException(). This exception
	// should be handled gracefully by the ofEvent dispatching this event.
	//
	// The same effect can be achieved by returning "true" from any event
	// callback with a boolean return type.
	//
	// \throws ofEventAttendedException if the event is cancelable.
	void stopImmediatePropagation();

	/// \brief Prevent any default actions associated with the event.
	///
	/// For some events, there are default actions.  Calling preventDefault will
	/// tell the event dispatcher that it should not call the default action
	/// after the event has been dispatched.  This is different from
	/// stopPropagation.  Calling stopPropagation does not prevent the default
	/// action from taking place.  To stop propagation and prevent the default,
	/// both methods must be called.
	void preventDefault();

	/// \returns true iff the event was cancelled.
	bool isCancelled() const;

	/// \returns true iff the default activity was prevented.
	bool isDefaultPrevented() const;

	/// \brief Set the Phase of the event.
	/// \param phase The phase to set.
	void setPhase(Phase phase);

	/// \returns the Phase of the event.
	Phase getPhase() const;

	/// \brief Determine if the event has a bubbling phase.
	/// \returns true iff the event should bubble.
	bool bubbles() const;

	/// \returns true iff the Event can be cancelled.
	bool isCancelable() const;

	/// \returns the source Element.
	Element* source();

	/// \returns the target Element.
	Element* target();

	/// \returns the related target Element.
	Element* relatedTarget();

	/// \returns a pointer to the current target Element.
	Element* getCurrentTarget();

	/// \returns a pointer to the current target Element.
	const Element* getCurrentTarget() const;

	/// \brief Set the current target Element.
	/// \param target The current target Element.
	void setCurrentTarget(Element* target);

	/// \returns the timestamp (in milliseconds relative to the epoch).
	uint64_t timestamp() const;

	/// \brief A utility method to print get the Event as a std::string.
	/// \returns The Event as a std::string.
	/// \note Not for serialization.
	std::string toString() const;

protected:
	/// \brief The name of the event (case-insensitive).
	std::string _type;

	/// \brief The source of the event.
	Element* _source = nullptr;

	/// \brief The event's target.
	Element* _target = nullptr;

	/// \brief The event's related target.
	Element* _relatedTarget = nullptr;

	/// \brief Used to indicate whether or not an event is a bubbling event.
	/// If the event can bubble the value is true, else the value is false.
	bool _bubbles = true;

	/// \brief Used to indicate whether propgation was stopped.  The currentTarget
	/// will indicate which target stopped propagation.
	bool _cancelable = true;

	/// \brief Used to indicated
	bool _defaultPrevented = false;

	/// \brief Used to indicate the EventTarget whose EventListeners are currently being processed.
	/// This is particularly useful during capturing and bubbling.
	Element* _currentTaget  = nullptr;

	/// \brief Used to indicate which phase of event flow is currently being evaluated.
	Phase _phase = Phase::NONE;

	/// \brief Used to indicate if an event is canceled.
	bool _canceled = false;

	/// \brief Used to specify the time (in milliseconds relative to the epoch)
	/// at which the event was created.
	///
	/// Due to the fact that some systems may not provide this information the
	/// value of timeStamp may be not available for all events. When not
	/// available, a value of 0 will be returned. Examples of epoch time are
	/// the time of the system start or 0:0:0 UTC 1st January 1970.
	uint64_t _timestamp = 0;

	/// \brief The Document class has access to Event data.
	friend class Document;
};


/// can we handle this named ui event at this coordinate?
class UIEventArgs: public EventArgs
{
public:
	using EventArgs::EventArgs;

	virtual ~UIEventArgs()
	{
	}

};


class KeyboardUIEventArgs: public UIEventArgs
{
public:
	KeyboardUIEventArgs(const ofKeyEventArgs& args,
						Element* source,
						Element* target);

	virtual ~KeyboardUIEventArgs();

	const ofKeyEventArgs& key() const;

	static const std::string KEY_DOWN;
	static const std::string KEY_UP;

protected:
	ofKeyEventArgs _key;

};


/// \sa http://www.w3.org/TR/DOM-Level-3-Events/#event-type-focus
class FocusEventArgs: public EventArgs
{
public:
	FocusEventArgs(const std::string& type,
				   Element* source,
				   Element* target,
				   Element* relatedTarget = nullptr);

	virtual ~FocusEventArgs();

	static const std::string FOCUS_IN;
	static const std::string FOCUS;
	static const std::string FOCUS_OUT;
	static const std::string BLUR;

};


class DragDropEventArgs: public EventArgs
{
public:

};


class AbstractDOMEvent
{
public:
	virtual ~AbstractDOMEvent()
	{
	}
};



class BaseDOMEvent
{
public:
	virtual ~BaseDOMEvent()
	{
	}

	//virtual std::string type() const = 0;

	virtual bool hasBubblePhaseListeners() const = 0;
	virtual bool hasCapturePhaseListeners() const = 0;

	bool hasListeners() const
	{
		return hasBubblePhaseListeners() || hasCapturePhaseListeners();
	}

};


/// \brief DOM Events follow the DOM capture, target, bubble propagation scheme.
///
///
template <typename EventArgsType>
class DOMEvent: public BaseDOMEvent
{
public:
	virtual ~DOMEvent()
	{
	}

	bool hasBubblePhaseListeners() const override
	{
		return _bubbleEvent.size() > 0;
	}

	bool hasCapturePhaseListeners() const override
	{
		return _captureEvent.size() > 0;
	}

	ofEvent<EventArgsType>& event(bool useCapture = false)
	{
		return useCapture ? _captureEvent : _bubbleEvent;
	}

	void notify(EventArgsType& e)
	{
		switch (e.getPhase())
		{
			case EventArgs::Phase::NONE:
				throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "DOMEvent::notify");
			case EventArgs::Phase::CAPTURING_PHASE:
				_captureEvent.notify(e.source(), e);
				return;
			case EventArgs::Phase::AT_TARGET:
				_captureEvent.notify(e.source(), e);
				_bubbleEvent.notify(e.source(), e);
				return;
			case EventArgs::Phase::BUBBLING_PHASE:
				_bubbleEvent.notify(e.source(), e);
				return;
		}
	}

private:
	ofEvent<EventArgsType> _bubbleEvent;
	ofEvent<EventArgsType> _captureEvent;

};


class MoveEventArgs
{
public:
	MoveEventArgs(const Position& position, Element* element);
	virtual ~MoveEventArgs();

	const Position& position() const;
	Element* element();

protected:
	Position _position;
	Element* _element;

};

class ResizeEventArgs
{
public:
	ResizeEventArgs(const Shape& shape);
	virtual ~ResizeEventArgs();

	const Shape& shape() const;

protected:
	Shape _shape;

};


class AttributeEventArgs
{
public:
	AttributeEventArgs(const std::string& key, const Any& value = Any());

	virtual ~AttributeEventArgs();

	const std::string& key() const;

	const Any& value() const;

protected:
	std::string _key;
	Any _value;

};


class EnablerEventArgs
{
public:
	EnablerEventArgs(bool value);
	virtual ~EnablerEventArgs();

	bool value() const;

protected:
	bool _value;

};


class ElementEventArgs
{
public:
	ElementEventArgs(Element* element);

	virtual ~ElementEventArgs();

	Element* element();

protected:
	Element* _element;

};


class ElementOrderEventArgs: public ElementEventArgs
{
public:
	ElementOrderEventArgs(Element* element,
						  std::size_t oldIndex,
						  std::size_t newIndex);

	virtual ~ElementOrderEventArgs();

	std::size_t newIndex() const;

	std::size_t oldIndex() const;

	bool wasMovedForward() const;

	bool wasMovedBackward() const;

	bool isAtFront() const;

	bool isAtBack() const;

protected:
	std::size_t _oldIndex;
	std::size_t _newIndex;

};


} } // namespace ofx::DOM
