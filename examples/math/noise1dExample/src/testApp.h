#pragma once

#include "ofMain.h"
#include "ofxSimpleSlider.h"


class testApp : public ofBaseApp{

	public:
		void	setup();
		void	update();
		void	draw();
	
		void	setupSignedNoiseDemo();
		void	updateSignedNoiseDemo();
		void	renderSignedNoiseDemo();
		void	renderNoisyRobotArmDemo();
	
	
		//---------------------------------------
		void	keyPressed(int key);
		void	keyReleased(int key);
		void	mouseMoved(int x, int y );
		void	mouseDragged(int x, int y, int button);
		void	mousePressed(int x, int y, int button);
		void	mouseReleased(int x, int y, int button);
		void	windowResized(int w, int h);
		void	dragEvent(ofDragInfo dragInfo);
		void	gotMessage(ofMessage msg);
	
		
		void	renderRadialSignedNoiseDemo();
		void	renderLinearSignedNoiseDemo();
		float	*signedNoiseData;
		int		nSignedNoiseData; 
	
		float	radialNoiseCursor; 
		ofxSimpleSlider	radialNoiseStepSlider; 
		ofxSimpleSlider radialNoiseAmountSlider;
		float	radialNoiseDemoX; 
		float	radialNoiseDemoY;
		float	radialNoiseDemoR; // radius
	
		void	drawNoisyArmRect (float w, float h);
		
};
