#pragma once

#include "ofxBaseGui.h"

class ofxValuePlotter : public ofxBaseGui {
	public:
		struct Config : public ofxBaseGui::Config {
			Config(){
			}
			Config(const typename ofxBaseGui::Config & c) : ofxBaseGui::Config(c){
			}
			int decimalPlace = 3;
			float minValue = 0;
			float maxValue = 0;
			int plotSize = 100;
		};

		ofxValuePlotter();
		ofxValuePlotter(ofParameter<float> value, const Config & config);
		virtual ~ofxValuePlotter();

		ofxValuePlotter & setup(const Config & config);
		ofxValuePlotter & setup(string label = "", float minValue = Config().minValue, float maxValue = Config().maxValue, int plotSize = Config().plotSize, float width = defaultWidth, float height = defaultHeight);

		void setDecimalPlace(int place);

		// Abstract methods we must implement, but have no need for!
		virtual bool mouseMoved(ofMouseEventArgs & args){
			return false;
		}
		virtual bool mousePressed(ofMouseEventArgs & args){
			return false;
		}
		virtual bool mouseDragged(ofMouseEventArgs & args){
			return false;
		}
		virtual bool mouseReleased(ofMouseEventArgs & args){
			return false;
		}
		virtual bool mouseScrolled(ofMouseEventArgs & args){
			return false;
		}

		virtual ofAbstractParameter & getParameter();

	protected:

		bool setValue(float mx, float my, bool bCheckBounds){
			return false;
		}
		void render();
		void generateDraw();
		ofPath bg;
		ofVboMesh textMesh;
		vector <float> buffer;
		int plotSize;
		ofPath plot;
		float minVal, maxVal;
		bool autoscale;
		int decimalPlace;
		ofParameter <string> label;
		ofParameter<float> value;

		void valueChanged(float & value);

};
