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


#include "ofx/DOM/Events.h"
#include "ofx/DOM/Element.h"


namespace ofx {
namespace DOM {


EventArgs::EventArgs(const std::string& type,
					 Element* source,
					 Element* target,
					 bool bubbles,
					 bool cancelable,
					 uint64_t timestamp):
	EventArgs(type, source, target, nullptr, bubbles, cancelable, timestamp)
{
}


EventArgs::EventArgs(const std::string& type,
					 Element* source,
					 Element* target,
					 Element* relatedTarget,
					 bool bubbles,
					 bool cancelable,
					 uint64_t timestamp):
	_type(type),
	_source(source),
	_target(target),
	_relatedTarget(relatedTarget),
	_bubbles(bubbles),
	_cancelable(cancelable),
	_timestamp(timestamp)
{
}


EventArgs::~EventArgs()
{
}


const std::string& EventArgs::type() const
{
	return _type;
}


void EventArgs::stopPropagation()
{
	if (_cancelable)
	{
		_canceled = true;
	}
}


void EventArgs::stopImmediatePropagation()
{
	if (_cancelable)
	{
		_canceled = true;
		throw ofEventAttendedException();
	}
}


void EventArgs::preventDefault()
{
	_defaultPrevented = true;
}


bool EventArgs::isCancelled() const
{
	return _canceled;
}


bool EventArgs::isDefaultPrevented() const
{
	return _defaultPrevented;
}


void EventArgs::setPhase(Phase phase)
{
	_phase = phase;
}


EventArgs::Phase EventArgs::getPhase() const
{
	return _phase;
}


bool EventArgs::bubbles() const
{
	return _bubbles;
}


bool EventArgs::isCancelable() const
{
	return _cancelable;
}


Element* EventArgs::source()
{
	return _source;
}


Element* EventArgs::target()
{
	return _target;
}


Element* EventArgs::relatedTarget()
{
	return _relatedTarget;
}


Element* EventArgs::getCurrentTarget()
{
	return _currentTaget;
}


const Element* EventArgs::getCurrentTarget() const
{
	return _currentTaget;
}


void EventArgs::setCurrentTarget(Element* target)
{
	_currentTaget = target;
}


uint64_t EventArgs::timestamp() const
{
	return _timestamp;
}


std::string EventArgs::toString() const
{
	std::stringstream ss;

	std::string phaseString = "";

	switch (_phase)
	{
		case Phase::NONE:
			phaseString = "NONE";
			break;
		case Phase::CAPTURING_PHASE:
			phaseString = "CAPTURING_PHASE";
			break;
		case Phase::AT_TARGET:
			phaseString = "AT_TARGET";
			break;
		case Phase::BUBBLING_PHASE:
			phaseString = "BUBBLING_PHASE";
			break;
	}


	ss << "Event Type: " << _type << std::endl;
	ss << "     Phase: " << phaseString << std::endl;
	ss << "    Source: " << (_source != nullptr ? _source->getId() : "nullptr") << std::endl;
	ss << "    Target: " << (_target != nullptr ? _target->getId() : "nullptr") << std::endl;
	ss << "Rel-Target: " << (_relatedTarget != nullptr ? _relatedTarget->getId() : "nullptr") << std::endl;
	ss << "Cur-Target: " << (_currentTaget != nullptr ? _currentTaget->getId() : "nullptr") << std::endl;
	ss << " Bubs/Canc: " << _bubbles << "/" << _cancelable << std::endl;
	ss << " DP/Canc'd: " << _defaultPrevented << "/" << _canceled << std::endl;
	ss << " timestamp: " << _timestamp << std::endl;

	return ss.str();
}


const std::string KeyboardUIEventArgs::KEY_DOWN = "keydown";
const std::string KeyboardUIEventArgs::KEY_UP = "keyup";


KeyboardUIEventArgs::KeyboardUIEventArgs(const ofKeyEventArgs& args,
										 Element* source,
										 Element* target):
	UIEventArgs(args.type == ofKeyEventArgs::Pressed ? KEY_DOWN : KEY_UP,
				source,
				target,
				true,
				true,
				ofGetElapsedTimeMillis())
{
}


KeyboardUIEventArgs::~KeyboardUIEventArgs()
{
}


const ofKeyEventArgs& KeyboardUIEventArgs::key() const
{
	return _key;
}


const std::string FocusEventArgs::FOCUS_IN = "focusin";
const std::string FocusEventArgs::FOCUS = "focus";
const std::string FocusEventArgs::FOCUS_OUT = "focusout";
const std::string FocusEventArgs::BLUR = "blur";


FocusEventArgs::FocusEventArgs(const std::string& type,
							   Element* source,
							   Element* target,
							   Element* relatedTarget):
	EventArgs(type,
			  source,
			  target,
			  (type != FOCUS), // In the spec.
			  false,
			  ofGetElapsedTimeMillis())
{
	// TODO: better Event constructor.
	_relatedTarget = relatedTarget;
}


FocusEventArgs::~FocusEventArgs()
{
}


MoveEventArgs::MoveEventArgs(const Position& position, Element *element):
	_element(element),
	_position(position)
{
}


MoveEventArgs::~MoveEventArgs()
{
}


const Position& MoveEventArgs::position() const
{
	return _position;
}


Element* MoveEventArgs::element()
{
	return _element;
}


ResizeEventArgs::ResizeEventArgs(const Shape& shape):
	_shape(shape)
{
}


ResizeEventArgs::~ResizeEventArgs()
{
}


const Shape& ResizeEventArgs::shape() const
{
	return _shape;
}


AttributeEventArgs::AttributeEventArgs(const std::string& key,
									   const Any& value):
	_key(key),
	_value(value)
{
}


AttributeEventArgs::~AttributeEventArgs()
{
}


const std::string& AttributeEventArgs::key() const
{
	return _key;
}


const Any& AttributeEventArgs::value() const
{
	return _value;
}


EnablerEventArgs::EnablerEventArgs(bool value):
	_value(value)
{
}


EnablerEventArgs::~EnablerEventArgs()
{
}


bool EnablerEventArgs::value() const
{
	return _value;
}


ElementEventArgs::ElementEventArgs(Element* element):
	_element(element)
{
}


ElementEventArgs::~ElementEventArgs()
{
}


Element* ElementEventArgs::element()
{
	return _element;
}


ElementOrderEventArgs::ElementOrderEventArgs(Element* element,
											 std::size_t newIndex,
											 std::size_t oldIndex):
	ElementEventArgs(element),
	_newIndex(newIndex),
	_oldIndex(oldIndex)
{
}


ElementOrderEventArgs::~ElementOrderEventArgs()
{
}


std::size_t ElementOrderEventArgs::newIndex() const
{
	return _newIndex;
}


std::size_t ElementOrderEventArgs::oldIndex() const
{
	return _oldIndex;
}


bool ElementOrderEventArgs::wasMovedForward() const
{
	return _oldIndex > _newIndex;
}


bool ElementOrderEventArgs::wasMovedBackward() const
{
	return _newIndex > _oldIndex;
}


bool ElementOrderEventArgs::isAtFront() const
{
	return _newIndex == 0;
}


bool ElementOrderEventArgs::isAtBack() const
{
	return _element->numSiblings() == _newIndex;
}


} } // namespace ofx::DOM
