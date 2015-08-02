#pragma once
#include "ofxPanel.h"

class ofxGuiPage : public ofxPanel {

	public:

		ofxGuiPage();
		virtual ~ofxGuiPage(){
		}

		void add(ofxBaseGui * element);

		void clear();

		virtual bool mouseDragged(ofMouseEventArgs & args);

		virtual void setWidthElements(float w);
		virtual void scaleWidthElements(float factor);

		virtual void setSize(float w, float h, bool callBack = true);
		virtual void setShape(ofRectangle r, bool callBack = true);
		virtual void setShape(float x, float y, float w, float h, bool callBack = true);

	protected:
		virtual void sizeChangedCB();

};
