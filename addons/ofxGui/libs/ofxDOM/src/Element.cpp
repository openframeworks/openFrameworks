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


#include "ofx/DOM/Element.h"
#include "ofx/DOM/Document.h"
#include "ofGraphics.h"
#include <algorithm>


namespace ofx {
namespace DOM {


Element::Element(float x, float y, float width, float height):
	Element("", x, y, width, height)
{
}


Element::Element(const std::string& id,
				 float x,
				 float y,
				 float width,
				 float height):
	_id(id),
	_shape(x, y, width, height),
	needsRedraw(true),
	_percentalWidthAmount(1),
	_usePercentalWidth(false)
{
	ofAddListener(this->move, this, &Element::_onMoved);
	ofAddListener(this->resize, this, &Element::_onResized);
}


Element::~Element()
{
}


std::unique_ptr<Element> Element::removeChild(Element* element)
{
	auto iter = findChild(element);

	if (iter != _children.end())
	{
		// Move the child out of the children array.
		std::unique_ptr<Element> detachedChild = std::move(*iter);

		// Disown the detached child
		_children.erase(iter);

		// Set the parent to nullptr.
		detachedChild->_parent = nullptr;

		// Invalidate all cached child shape.
		invalidateChildShape();

		// Alert the node that its parent was set.
		ElementEventArgs removedFromEvent(this);
		ofNotifyEvent(detachedChild->removedFrom, removedFromEvent, this);

		ElementEventArgs childRemovedEvent(detachedChild.get());
		ofNotifyEvent(childRemoved, childRemovedEvent, this);

		/// Alert the node's siblings that it no longer has a sibling.
		for (auto& child : _children)
		{
			if (detachedChild.get() != child.get())
			{
				ElementEventArgs e(detachedChild.get());
				ofNotifyEvent(child->siblingRemoved, e, this);
			}
		}

		// Detatch child listeners.
		ofRemoveListener(detachedChild.get()->move, this, &Element::_onChildMoved);
		ofRemoveListener(detachedChild.get()->resize, this, &Element::_onChildResized);

		// Return the detached child.
		// If the return value is ignored, it will be deleted.

		return detachedChild;
	}
	else
	{
		// Return nullptr because we couldn't find anything.
		return nullptr;
	}
}

void Element::clear()
{
	while(children().size() > 0) {
		if(!removeChild(children().at(0))){
			ofLogError("Element::clear") << "Could not remove child";
			return;
		}
	}
}

void Element::moveToFront()
{
	if (_parent)
	{
		_parent->moveChildToFront(this);
	}
}


void Element::moveForward()
{
	if (_parent)
	{
		_parent->moveChildForward(this);
	}
}


void Element::moveToBack()
{
	if (_parent)
	{
		_parent->moveChildToBack(this);
	}
}


void Element::moveBackward()
{
	if (_parent)
	{
		_parent->moveChildBackward(this);
	}
}


void Element::moveChildToIndex(Element* element, std::size_t index)
{
	auto iter = findChild(element);

	if (iter != _children.end())
	{
		std::size_t oldIndex = iter - _children.begin();
		std::size_t newIndex = std::min(index, _children.size() - 1);

		auto detachedChild = std::move(*iter);

		_children.erase(iter);

		_children.insert(_children.begin() + newIndex, std::move(detachedChild));

		ElementOrderEventArgs e(element, oldIndex, newIndex);
		ofNotifyEvent(reordered, e, element);
		ofNotifyEvent(childReordered, e, this);
	}
	else
	{
		throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::moveChildToFront: Element does not exist.");
	}
}


void Element::moveChildToFront(Element* element)
{
	auto iter = findChild(element);

	if (iter != _children.end())
	{
		std::size_t oldIndex = iter - _children.begin();
		std::size_t newIndex = 0;

		auto detachedChild = std::move(*iter);
		_children.erase(iter);
		_children.insert(_children.begin(), std::move(detachedChild));

		ElementOrderEventArgs e(element, oldIndex, newIndex);
		ofNotifyEvent(reordered, e, element);
		ofNotifyEvent(childReordered, e, this);
	}
	else
	{
		throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::moveChildToFront: Element does not exist.");
	}
}


void Element::moveChildForward(Element* element)
{
	auto iter = findChild(element);

	if (iter != _children.end())
	{
		// Make sure it's not already in the front.
		if (iter != _children.begin())
		{
			std::size_t oldIndex = iter - _children.begin();
			std::size_t newIndex = oldIndex - 1;

			std::iter_swap(iter, iter - 1);

			ElementOrderEventArgs e(element, oldIndex, newIndex);
			ofNotifyEvent(reordered, e, element);
			ofNotifyEvent(childReordered, e, this);
		}
	}
	else
	{
		throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::moveChildForward: Element does not exist.");
	}
}


void Element::moveChildToBack(Element* element)
{
	auto iter = findChild(element);

	if (iter != _children.end())
	{
		// Make sure it's not already in the back.
		if (iter != _children.end() - 1)
		{
			std::size_t oldIndex = iter - _children.begin();
			std::size_t newIndex = _children.size() - 1;

			auto detachedChild = std::move(*iter);
			_children.erase(iter);
			_children.push_back(std::move(detachedChild));

			ElementOrderEventArgs e(element, oldIndex, newIndex);
			ofNotifyEvent(reordered, e, element);
			ofNotifyEvent(childReordered, e, this);
		}
	}
	else
	{
		throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::moveChildToBack: Element does not exist.");
	}
}


void Element::moveChildBackward(Element* element)
{
	auto iter = findChild(element);

	if (iter != _children.end())
	{
		if (iter != _children.end() - 1)
		{
			std::size_t oldIndex = iter - _children.begin();
			std::size_t newIndex = _children.size() + 1;

			std::iter_swap(iter, iter + 1);

			ElementOrderEventArgs e(element, oldIndex, newIndex);
			ofNotifyEvent(reordered, e, element);
			ofNotifyEvent(childReordered, e, this);
		}
	}
}


bool Element::isChild(Element* element) const
{
	return element
		&& element->_parent == this;
}


bool Element::isSibling(Element* element) const
{
	return element
		&& element->_parent
		&& element->_parent == _parent;
}


std::size_t Element::numSiblings() const
{
	// Don't count self.
	return _parent ? (_parent->numChildren() - 1) : 0;
}


std::vector<Element*> Element::siblings()
{
	std::vector<Element*> results;

	if (_parent)
	{
		results.reserve(_parent->_children.size());

		for (auto& child : _parent->_children)
		{
			Element* sibling = child.get();

			if (sibling)
			{
				if (this != sibling)
				{
					results.push_back(sibling);
				}
			}
			else
			{
				throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::siblings(): Child element is nullptr.");
			}
		}
	}

	return results;
}


bool Element::isParent(Element* element) const
{
	return element
		&& element == this->_parent;
}


std::size_t Element::numChildren() const
{
	return _children.size();
}


std::vector<Element*> Element::children()
{
	std::vector<Element*> results;

	results.reserve(_children.size());

	for (auto& child : _children)
	{
		Element* pChild = child.get();

		if (pChild)
		{
			results.push_back(pChild);
		}
		else
		{
			throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::children(): Child element is nullptr.");
		}
	}

	return results;
}


bool Element::hasParent() const
{
	return _parent;
}


Element* Element::findFirstChildById(const std::string& id)
{
	auto iter = std::find_if(_children.begin(),
							 _children.end(),
							 [&](const std::unique_ptr<Element>& child) {
								 return child->getId() == id;
							 });

	return (iter != _children.end()) ? iter->get() : nullptr;
}


std::vector<Element*> Element::findChildrenById(const std::string& id)
{
	std::vector<Element*> matches;

	matches.reserve(_children.size());

	for (auto& child : _children)
	{
		Element* pChild = child.get();

		if (pChild)
		{
			if (child->getId() == id)
			{
				matches.push_back(child.get());
			}
		}
		else
		{
			throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::findChildrenById(): Child element is nullptr.");
		}
	}

	return matches;
}


std::vector<std::unique_ptr<Element>>::iterator Element::findChild(Element* element)
{
	return std::find_if(_children.begin(),
						_children.end(),
						[&](const std::unique_ptr<Element>& child) {
							return element == child.get();
						});
}


Element* Element::parent()
{
	return _parent;
}


const Element* Element::parent() const
{
	return _parent;
}


Document* Element::document()
{
	if (_parent)
	{
		// If a parent exists, return it recursively.
		return _parent->document();
	}
	else
	{
		// Return self if a Document, otherwise, will return nullptr.
		return dynamic_cast<Document*>(this);
	}
}


const Document* Element::document() const
{
	if (_parent)
	{
		// If a parent exists, return it recursively.
		return _parent->document();
	}
	else
	{
		// Return self if a Document, otherwise, will return nullptr.
		return dynamic_cast<const Document*>(this);
	}
}

std::unique_ptr<Layout> Element::removeLayout(){
	// TODO
}

Layout* Element::layout(){
	return _layout.get();
}

bool Element::hitTest(const Position& parentPosition) const
{
	return getShape().inside(parentPosition);
}


bool Element::childHitTest(const Position& localPosition) const
{
	return getChildShape().inside(localPosition);
}


Position Element::localToScreen(const Position& localPosition) const
{
	return localPosition + getScreenPosition();
}


Shape Element::localToScreen(const Shape& localShape) const
{
	return Shape(localShape.getPosition() + getScreenPosition(),
				 localShape.getWidth(), localShape.getHeight());
}


Position Element::screenToLocal(const Position& screenPosition) const
{
	return screenPosition - getScreenPosition();
}


Position Element::parentToScreen(const Position& parentPosition) const
{
	if (_parent)
	{
		return parentPosition + _parent->getScreenPosition();
	}
	else
	{
		return parentPosition;
	}
}


Position Element::screenToParent(const Position& screenPosition) const
{
	if (_parent)
	{
		return screenPosition - _parent->getScreenPosition();
	}
	else
	{
		return screenPosition;
	}
}


void Element::setPosition(float x, float y)
{
	if(_shape.getPosition() != ofPoint(x,y))
	{
		_shape.setPosition(x, y);
		MoveEventArgs e(getPosition(), this);
		ofNotifyEvent(move, e, this);
	}
}


void Element::setPosition(const Position& position)
{
	setPosition(position.x, position.y);
}


Position Element::getPosition() const
{
	return _shape.getPosition();
}


float Element::getX() const
{
	return _shape.getX();
}


float Element::getY() const
{
	return _shape.getY();
}


Position Element::getScreenPosition() const
{
	if (_parent)
	{
		return getPosition() + _parent->getScreenPosition();
	}
	else
	{
		return getPosition();
	}
}


float Element::getScreenX() const
{
	return getScreenPosition().x;
}


float Element::getScreenY() const
{
	return getScreenPosition().y;
}


void Element::setSize(float width, float height)
{
	if(_shape.getWidth()!= width || _shape.getHeight() != height)
	{
		_shape.setWidth(width);
		_shape.setHeight(height);
		_shape.standardize();
		ResizeEventArgs e(_shape);
		ofNotifyEvent(resize, e, this);
	}
}


Size Element::getSize() const
{
	return Size(_shape.width, _shape.height);
}


///\todo use ofCompareFloat
void Element::setWidth(float width)
{
	if(width != getWidth()){
		_shape.setWidth(width);
		_shape.standardize();
		ResizeEventArgs e(_shape);
		ofNotifyEvent(resize, e, this);
	}
}


float Element::getWidth() const
{
	return _shape.getWidth();
}

float Element::getPercentalWidth() const
{
	return _percentalWidthAmount;
}

bool Element::usesPercentalWidth() const
{
	return _usePercentalWidth;
}

void Element::setPercentalWidth(bool usePercentalWidth, float percentalWidthAmount)
{
	_usePercentalWidth = usePercentalWidth;
	_percentalWidthAmount = percentalWidthAmount;
	if(_usePercentalWidth && _percentalWidthAmount <= 1 && parent()){
		setWidth(parent()->getWidth()*_percentalWidthAmount);
	}
}


///\todo use ofCompareFloat
void Element::setHeight(float height)
{
	if(height != getHeight()){
		_shape.setHeight(height);
		_shape.standardize();
		ResizeEventArgs e(_shape);
		ofNotifyEvent(resize, e, this);
	}
}


float Element::getHeight() const
{
	return _shape.getHeight();
}


Shape Element::getShape() const
{
	return _shape;
}


void Element::setShape(const Shape& shape)
{
	setPosition(shape.x, shape.y);
	setSize(shape.width, shape.height);
}

void Element::setShape(float x, float y, float width, float height)
{
	setShape(ofRectangle(x, y, width, height));
}

Shape Element::getChildShape() const
{
	if (_childShapeInvalid)
	{
		_childShape = Shape(); // Clear.

		auto iter = _children.begin();

		while (iter != _children.end())
		{
			const Element* child = iter->get();

			if (child)
			{
				if (iter == _children.begin())
				{
					_childShape = child->getTotalShape();
				}
				else
				{
					_childShape.growToInclude(child->getTotalShape());
				}
			}
			else
			{
				throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::getChildGeometry(): Child element is nullptr.");
			}

			++iter;
		}

		_childShapeInvalid = false;
	}

	return _childShape;
}


Shape Element::getTotalShape() const
{
	Shape totalGeometry(_shape);

	if (!_children.empty())
	{
		totalGeometry.growToInclude(getChildShape() + getPosition());
	}

	return totalGeometry;
}


std::string Element::getId() const
{
	return _id;
}


void Element::setId(const std::string& id)
{
	_id = id;
}


bool Element::hasAttribute(const std::string& key) const
{
	return _attributes.find(key) != _attributes.end();
}


void Element::setAttribute(const std::string& key, const Any& value)
{
	_attributes[key] = value;

	AttributeEventArgs e(key, value);
	ofNotifyEvent(attributeSet, e, this);
}


void Element::clearAttribute(const std::string& key)
{
	_attributes.erase(key);
	AttributeEventArgs e(key);
	ofNotifyEvent(attributeCleared, e, this);
}



void Element::_setup(ofEventArgs& e)
{
	for (auto& child : _children)
	{
		child->_setup(e);
	}

	onSetup();
}


void Element::_update(ofEventArgs& e)
{
	if (_enabled && !_hidden)
	{
		for (auto& child : _children)
		{
			child->_update(e);
		}

		onUpdate();
	}
}


void Element::_draw(ofEventArgs& e)
{
	if (_enabled && !_hidden)
	{
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(_shape.getPosition());

		// Draw parent behind children.
		if(needsRedraw){
			generateDraw();
			needsRedraw = false;
		}
		render();

		// Now draw in reverse order.
		auto iter = _children.rbegin();

		while (iter != _children.rend())
		{
			(*iter)->_draw(e);
			++iter;
		}

		ofPopMatrix();
		ofPopStyle();
	}
}

void Element::setNeedsRedraw()
{
	needsRedraw = true;
}

void Element::_exit(ofEventArgs& e)
{
	for (auto& child : _children)
	{
		child->_exit(e);
	}

	onExit();
}


Element* Element::recursiveHitTest(const Position& parentPosition)
{
	if (_enabled && !_hidden)
	{
		Position childLocal = parentPosition - this->getPosition();

		if (!_children.empty() && childHitTest(childLocal))
		{
			for (auto& child : _children)
			{
				Element* target = child->recursiveHitTest(childLocal);

				if (target)
				{
					return target;
				}
			}
		}

		// If there is no child target, is this a viable target?
		if (hitTest(parentPosition))
		{
			return this;
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}


bool Element::isEnabled() const
{
	return _enabled;
}


void Element::setEnabled(bool enabled_)
{
	_enabled = enabled_;
	EnablerEventArgs e(_enabled);
	ofNotifyEvent(enabled, e, this);
}


bool Element::isHidden() const
{
	if(parent()){
		return parent()->isHidden() || _hidden;
	}
	return _hidden;
}


void Element::setHidden(bool hidden_)
{
	_hidden = hidden_;
	invalidateChildShape();
	EnablerEventArgs e(_hidden);
	ofNotifyEvent(hidden, e, this);
}


bool Element::isLocked() const
{
	return _locked;
}


void Element::setLocked(bool locked_)
{
	_locked = locked_;
	EnablerEventArgs e(_locked);
	ofNotifyEvent(locked, e, this);
}


void Element::invalidateChildShape() const
{
	_childShapeInvalid = true;

	if (_parent)
	{
		_parent->invalidateChildShape();
	}

	if (_layout)
	{
		_layout->doLayout();
	}
}


void Element::_onMoved(MoveEventArgs&)
{
	invalidateChildShape();
	setNeedsRedraw();
}


void Element::_onResized(ResizeEventArgs&)
{
	// TODO performance optimization: check if mouse is dragged and update only on release
	for(auto& e : children()){
		if(e->usesPercentalWidth() && e->getPercentalWidth() <= 1){
			e->setWidth(getWidth()*e->getPercentalWidth());
		}
	}

	invalidateChildShape();
	setNeedsRedraw();
}


void Element::_onChildMoved(MoveEventArgs &args)
{
	invalidateChildShape();
	ofNotifyEvent(childMoved, args, this);
}


void Element::_onChildResized(ResizeEventArgs&)
{
	invalidateChildShape();
}


} } // namespace ofx::DOM
