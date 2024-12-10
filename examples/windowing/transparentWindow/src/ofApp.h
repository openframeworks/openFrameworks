#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
	
	public:
		
		void setup() override;
		void update() override;
		void draw() override;
		
		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
		
		ofTrueTypeFont	fontRenderer;
		
		bool	bFullscreen;
		bool	bAllowPassThrough = true;
		bool	bForcePassThrough = false;
		bool	bDrawGuides = true;
		bool	bLogVerbose = false;
		
		float	ballPositionX;
		float	ballPositionY;
		float	ballVelocityX;
		float	ballVelocityY;
	
		const float margin = 20;
		const float lineSpacing = 30;
		float lineHeight;
	
		float centerX;
		float centerY;
		float baseY;
	
		ofColor bgColor;
		ofColor textColor;
		ofColor primaryBtnColor;
		ofColor secondaryBtnColor;
		ofColor highlightColor;
	
};

