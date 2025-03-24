#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofShader shader;
		
		ofImage brushImage;
		
		ofFbo maskFbo;
		ofMesh drawMesh;
		
		vector<ofTexture> textures;
		
		bool bUseTextures = true;
		bool bBrushDown=false;
		bool bEraser = false;
		int brushSize = 80;
		int brushAlpha = 10;
		
		float eTimeCounter = 0.0;
		
		ofPolyline oLine;
		ofPolyline fLine;
};
