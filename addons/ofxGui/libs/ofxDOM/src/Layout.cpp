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


#include "ofx/DOM/Layout.h"
#include "ofx/DOM/Element.h"
#include <iostream>


namespace ofx {
namespace DOM {


Layout::Layout(Element* parent): _parent(parent)
{
}


Layout::~Layout()
{
}


Element* Layout::parent()
{
	return _parent;
}


bool Layout::isDoingLayout() const
{
	return _isDoingLayout;
}


std::vector<Element*> Layout::children()
{
	if (_parent)
	{
		return _parent->children();
	}
	else
	{
		std::vector<Element*> _children;
		return _children;
	}
}


BoxLayout::BoxLayout(Element* parent, Orientation orientation):
	Layout(parent),
	_orientation((orientation == Orientation::DEFAULT) ? Orientation::HORIZONTAL : orientation)
{
}


BoxLayout::~BoxLayout()
{
}


void BoxLayout::doLayout()
{
	if (_parent && !_isDoingLayout)
	{
		// Prevent recursive calls to doLayout.
		_isDoingLayout = true;

		float totalWidth = 0;
		float totalHeight = 0;
		float currentX = 0;
		float currentY = 0;

		for (Element* element : children())
		{
			if (element)
			{
				element->setPosition(currentX, currentY);

				if (_orientation == Orientation::HORIZONTAL)
				{
					totalHeight = std::max(totalHeight, element->getHeight());
					currentX += element->getWidth();
					totalWidth = currentX;
				}
				else
				{
					totalWidth = std::max(totalWidth, element->getWidth());
					currentY += element->getHeight();
					totalHeight = currentY;
				}
			}
		}

		totalWidth = std::max(totalWidth, _parent->getShape().getWidth());
		totalHeight = std::max(totalHeight, _parent->getShape().getHeight());
		_parent->setSize(totalWidth, totalHeight);

		_isDoingLayout = false;
	}
}


Orientation BoxLayout::orientation() const
{
	return _orientation;
}


} } // namespace ofx::DOM
