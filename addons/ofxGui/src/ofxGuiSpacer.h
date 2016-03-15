#pragma once

#include "ofxBaseGui.h"

class ofxGuiSpacer : public ofxBaseGui {
	public:

		ofxGuiSpacer():ofxBaseGui(){
			setup();
		}

		ofxGuiSpacer(const ofJson & config):ofxGuiSpacer(){
			_setConfig(config);
		}

		virtual ~ofxGuiSpacer(){
		}

		void setup(){
			backgroundColor = ofColor(0, 0, 0, 0);
			borderWidth = 0;

		}

		virtual ofAbstractParameter & getParameter(){
			return parameter;
		}

	protected:
		virtual bool setValue(float mx, float my){
			return false;
		}

		// not needed, but has be be there
		ofParameter <bool> parameter;

};
