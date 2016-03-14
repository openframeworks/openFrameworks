#pragma once

#include "ofParameter.h"
#include "ofxToggle.h"

class ofxMinimalToggle : public ofxToggle {

	public:
		ofxMinimalToggle(const std::string & toggleName="", const ofJson & config = ofJson());
		ofxMinimalToggle(ofParameter<bool> &val, const ofJson & config = ofJson());
		ofxMinimalToggle(ofParameter <bool> &val, float width, float height = defaultHeight);
		ofxMinimalToggle(const std::string & toggleName, bool val, float width = defaultWidth, float height = defaultHeight);

		virtual ~ofxMinimalToggle();

		void setup(const ofJson &config = ofJson());

	protected:
		virtual void render() override;
		void generateDraw() override;

		float getTextWidth(std::string text, float height);

};

