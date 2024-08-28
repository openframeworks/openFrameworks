#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		ofxPanel gui;
		ofParameterGroup paramGroupTransformFeedback;
		ofParameter<float> paramMaxLife;
		ofParameter<float> paramNoiseStrength;
		ofParameter<float> paramUpwardForce;
		ofParameter<float> paramNoiseTimeOffsetMultiplier;
	
		ofParameterGroup paramGroupRender;
		ofParameter<float> paramPointSize;
		ofParameter<ofFloatColor> paramRenderStartColor1;
		ofParameter<ofFloatColor> paramRenderStartColor2;
		ofParameter<ofFloatColor> paramRenderEndColor;

		vector< shared_ptr<ofVbo> > vbos;
		vector<ofShader::TransformFeedbackBaseBinding> transformFeedbackVertexBindings;
		vector<ofShader::TransformFeedbackBaseBinding> transformFeedbackNormalBindings;
		ofEasyCam cam;
		ofShader shaderTransformFeedback;
		ofShader shaderRender;
		int bufferIndex = 0;
		int numVertices = 1381;
		ofTexture dotTexture;
};
