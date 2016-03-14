#pragma once

#include "ofxBaseGui.h"

class ofxGuiSpacer : public ofxBaseGui {
	public:

		ofxGuiSpacer(const ofJson & config = ofJson());

		virtual ~ofxGuiSpacer(){
		}

		void setup(const ofJson & config = ofJson());

		virtual ofAbstractParameter & getParameter();

	protected:
		virtual bool setValue(float mx, float my){
			return false;
		}

		// not needed, but has be be there
		ofParameter <bool> parameter;

};
