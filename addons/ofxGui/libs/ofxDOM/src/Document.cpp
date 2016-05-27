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


#include "ofx/DOM/Document.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"


namespace ofx {
namespace DOM {


Document::Document():
	Element("document", 0, 0, 1024, 768)
{
	ofAddListener(ofEvents().setup, this, &Document::setup);
	ofAddListener(ofEvents().update, this, &Document::update);
	ofAddListener(ofEvents().draw, this, &Document::draw, OF_EVENT_ORDER_AFTER_APP);
	ofAddListener(ofEvents().exit, this, &Document::exit);
	ofAddListener(ofEvents().windowResized, this, &Document::windowResized, std::numeric_limits<int>::lowest());

	ofAddListener(ofEvents().fileDragEvent, this, &Document::fileDragEvent, std::numeric_limits<int>::lowest());

	ofAddListener(ofEvents().keyPressed, this, &Document::onKeyEvent, std::numeric_limits<int>::lowest());
	ofAddListener(ofEvents().keyReleased, this, &Document::onKeyEvent, std::numeric_limits<int>::lowest());
}


Document::~Document()
{
	ofRemoveListener(ofEvents().setup, this, &Document::setup);
	ofRemoveListener(ofEvents().update, this, &Document::update);
	ofRemoveListener(ofEvents().draw, this, &Document::draw, OF_EVENT_ORDER_AFTER_APP);
	ofRemoveListener(ofEvents().exit, this, &Document::exit);
	ofRemoveListener(ofEvents().windowResized, this, &Document::windowResized, std::numeric_limits<int>::lowest());

	ofRemoveListener(ofEvents().fileDragEvent, this, &Document::fileDragEvent, std::numeric_limits<int>::lowest());

	ofRemoveListener(ofEvents().keyPressed, this, &Document::onKeyEvent, std::numeric_limits<int>::lowest());
	ofRemoveListener(ofEvents().keyReleased, this, &Document::onKeyEvent, std::numeric_limits<int>::lowest());
}


void Document::setup(ofEventArgs& e)
{
	if (_autoFillScreen)
	{
		setSize(ofGetWidth(), ofGetHeight());
	}

	Element::_setup(e);
}


void Document::update(ofEventArgs& e)
{
	Element::_update(e);
}


void Document::draw(ofEventArgs& e)
{
	Element::_draw(e);
}


void Document::exit(ofEventArgs& e)
{
	Element::_exit(e);
}


void Document::windowResized(ofResizeEventArgs& e)
{
	if (_autoFillScreen)
	{
		setSize(e.width, e.height);
	}
}


bool Document::fileDragEvent(ofDragInfo& e)
{
	return false;
}


bool Document::onKeyEvent(ofKeyEventArgs& e)
{
	return false;
}


void Document::setAutoFillScreen(bool autoFillScreen)
{
	_autoFillScreen = autoFillScreen;

	if (_autoFillScreen)
	{
		setSize(ofGetWidth(), ofGetHeight());
	}
}


bool Document::getAutoFillScreen() const
{
	return _autoFillScreen;
}


} } // namespace ofx::DOM
