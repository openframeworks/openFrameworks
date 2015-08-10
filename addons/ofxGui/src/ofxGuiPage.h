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

	protected:
		virtual void sizeChangedCB();

};
