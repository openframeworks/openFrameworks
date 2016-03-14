#pragma once

#include "ofxBaseGui.h"

class ofxValuePlotter : public ofxBaseGui {
	public:

		ofxValuePlotter(const ofJson & config = ofJson());
		ofxValuePlotter(ofParameter<float> value, const ofJson & config = ofJson());
		ofxValuePlotter(string label, float minValue, float maxValue, int plotSize = 100, float width = defaultWidth, float height = defaultHeight);

		virtual ~ofxValuePlotter();

		void setup(const ofJson &config = ofJson());

		void setDecimalPlace(int place);

		virtual ofAbstractParameter & getParameter();

	protected:

		virtual void processConfig(const ofJson & config);

		bool setValue(float mx, float my){
			return false;
		}
		void render();
		void generateDraw();
		ofVboMesh textMesh;
		vector <float> buffer;
		ofPath plot;
		bool autoscale;
		ofParameter <string> label;
		ofParameter<float> value;

		ofParameter<int> decimalPlace;
		ofParameter<float> minVal;
		ofParameter<float> maxVal;
		ofParameter<int> plotSize;

		void valueChanged(float & value);

};
