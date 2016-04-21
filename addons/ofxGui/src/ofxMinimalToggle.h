#pragma once

#include "ofParameter.h"
#include "ofxToggle.h"

class ofxMinimalToggle : public ofxToggle {

	public:
		ofxMinimalToggle();
		virtual ~ofxMinimalToggle();
		ofxMinimalToggle(ofParameter<bool> &val, const Config & config = Config());

		ofxMinimalToggle & setup(ofParameter <bool> &val, const Config & config = Config());
		ofxMinimalToggle & setup(ofParameter <bool> &val, float width = defaultWidth, float height = defaultHeight);
		ofxMinimalToggle & setup(const std::string & toggleName, bool val, float width = defaultWidth, float height = defaultHeight);

	protected:
		virtual void render();
		void generateDraw();

		float getTextWidth(std::string text, float height);

		ofPath border;
};

