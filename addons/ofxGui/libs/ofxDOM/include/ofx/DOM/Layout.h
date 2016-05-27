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


#include <vector>
#include "ofx/DOM/Types.h"


namespace ofx {
namespace DOM {


class Element;


/// \brief A base class for laying out Elements.
///
/// Generally this class and its subclasses should not be instantiated directly
/// but instead should be created using Element::createLayout<LayoutType>(...).
/// This pattern preserves the ownership of the Layout.
class Layout
{
public:
	/// \brief Create a Layout with a given Element parent.
	/// \param parent The parent Element.
	Layout(Element* parent);

	/// \brief Destroy the layout.
	virtual ~Layout();

	/// \returns a pointer to the parent Element or nullptr if none.
	Element* parent();

	/// \returns true iff this layout is currently being done.
	bool isDoingLayout() const;

	/// \brief Get all of the children for this element.
	std::vector<Element*> children();

	/// \brief Do
	virtual void doLayout() = 0;

protected:
	/// \brief The owning Widget class.
	Element* _parent = nullptr;

	/// \brief True if in doLayout(). Used to prevent recusive calls.
	bool _isDoingLayout = false;

	friend class Element;

};


class BoxLayout: public Layout
{
public:
	/// If the Orientation::DEFAULT is chosen, the default will be set to
	/// Orientation::HORIZONTAL.
	BoxLayout(Element* parent, Orientation orientation = Orientation::VERTICAL);

	virtual ~BoxLayout();

	virtual void doLayout() override;

	Orientation orientation() const;

protected:
	Orientation _orientation = Orientation::HORIZONTAL;

};


} } // ofx::DOM
