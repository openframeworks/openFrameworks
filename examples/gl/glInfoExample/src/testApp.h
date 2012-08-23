#pragma once

#include "ofMain.h"



typedef struct {
    
    string version;
    string vendor;
    string renderer;
    
   
    bool bPointSpritesSupported;
    bool bVboSupported;
    bool bShadersSupported;
    int maxTextureSize;
    int maxDimensions[2];
    int maxLights;
    
    
} commonOpenGlInfo;


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    
        commonOpenGlInfo info;
    
    
};
