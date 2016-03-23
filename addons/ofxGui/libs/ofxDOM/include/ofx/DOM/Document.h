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


#include "ofx/DOM/Element.h"


namespace ofx {
namespace DOM {


/// \brief The Document represents the root DOM Element in an DOM tree.
class Document: public Element
{
public:
	/// \brief Create a default Document.
	///
	/// The Document will have the default id of "document" and will fill
	/// the entire screen.
	Document();

	/// \brief Destroy the Document.
	virtual ~Document();

	/// \brief Callback for the setup event.
	/// \param e the event data.
	void setup(ofEventArgs& e);

	/// \brief Callback for the update event.
	/// \param e the event data.
	void update(ofEventArgs& e);

	/// \brief Callback for the draw event.
	/// \param e the event data.
	void draw(ofEventArgs& e);

	/// \brief Callback for the exit event.
	/// \param e the event data.
	void exit(ofEventArgs& e);

	/// \brief Callback for the window resized event.
	/// \param e the event data.
	void windowResized(ofResizeEventArgs& e);

	/// \brief Callback for file drag events.
	/// \param e the event data.
	/// \returns true iff the event was handled.
	bool fileDragEvent(ofDragInfo& e);

	/// \brief Callback for key events events.
	/// \param e the event data.
	/// \returns true iff the event was handled.
	bool onKeyEvent(ofKeyEventArgs& e);

	/// \brief Determine if the Document size should always match the screen size.
	/// \param true if if the Document size should always match the screen size.
	void setAutoFillScreen(bool autoFillScreen);

	/// \returns true if the Document size will always match the screen size.
	bool getAutoFillScreen() const;

protected:

	/// \brief True if the Document size should always match the screen size.
	bool _autoFillScreen = true;

};


} } // namespace ofx::DOM
