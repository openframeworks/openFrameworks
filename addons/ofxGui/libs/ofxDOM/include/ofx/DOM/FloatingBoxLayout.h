#pragma once

#include "ofx/DOM/Layout.h"
#include "ofParameter.h"
#include "Element.h"

namespace ofx {
namespace DOM {

class FloatingBoxLayout: public BoxLayout
{
public:
	/// If the Orientation::DEFAULT is chosen, the default will be set to
	/// Orientation::HORIZONTAL.
	FloatingBoxLayout(Element* parent, Orientation orientation = Orientation::VERTICAL);

	virtual ~FloatingBoxLayout();

	virtual void doLayout() override;

protected:

	void setNextLeftFloatPosition(ofPoint p, Element* e);
	void setNextRightFloatPosition(ofPoint p, Element* e);
	ofPoint nextLeftFloatPositionLoop(Element* e, Element *s, const ofPoint& res);
	ofPoint nextRightFloatPositionLoop(Element* e, Element *s, const ofPoint& res);

};

}}
