#pragma once

#include "ofxBaseGui.h"

class ofxValuePlotter : public ofxBaseGui {
	public:

		ofxValuePlotter();
		ofxValuePlotter(const ofJson & config);
		ofxValuePlotter(ofParameter<float> value, const ofJson & config = ofJson());
		ofxValuePlotter(string label, float minValue, float maxValue, int plotSize = 100, float width = defaultWidth, float height = defaultHeight);

		virtual ~ofxValuePlotter();

		void setup();

		void setDecimalPlace(int place);

		virtual ofAbstractParameter & getParameter() override;

	protected:

		virtual void _setConfig(const ofJson & config) override;
		virtual void render() override;
		virtual void generateDraw() override;

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
