#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(ofMouseEventArgs & args);
		void mouseDragged(ofMouseEventArgs & args);
		void mousePressed(ofMouseEventArgs & args);
		void mouseReleased(ofMouseEventArgs & args);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:

		ofxGui gui;

		ofxPanel *panel1, *panel3, *sliders;
		ofxGuiGroup *rotary, *labels, *buttons, *toggles;
		ofxGuiGroup* group3, *group1;
		ofParameter<string> active_name;
		ofParameter<int> active_index;
		ofParameter<float> floatfield_param;
		ofParameter<string> textfield_param;

		ofParameter <string> label_param;
		ofParameter <bool> toggle_param;
		ofParameter <float> slider_param;
		ofParameter <bool> toggle1_param, toggle2_param, toggle3_param, toggle4_param;
		ofParameter <float> slider1_param, slider2_param, slider3_param, slider4_param;
		vector <ofParameter <bool> > matrix_params;
		ofParameterGroup g;

		ofParameterGroup colorParameters;
		ofParameter<bool> color0;
		ofParameter<bool> color2;
		ofParameter<bool> color3;
		ofParameter<bool> color1;
		ofxGuiGroup *color_toggles;

		ofColor color;

		void toggleGroupHeader(bool & val);
		void setHeaderColors(int& index);
		void updateMatrixIndex(int& index);

};

