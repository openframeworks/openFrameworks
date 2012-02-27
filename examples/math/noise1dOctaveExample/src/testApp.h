#pragma once

#include "ofMain.h"
#include "ofxSimpleSlider.h"

#define NOISE_DATA_STRIP_LENGTH 256

struct NoiseDataStrip {
	float data[NOISE_DATA_STRIP_LENGTH];
	float x;
	float y;
	float width; 
	float height; 
	float noiseStep; 
};

class testApp : public ofBaseApp{

	public:
		void	setup();
		void	update();
		void	draw();
	
		void	setupMultibandNoiseDemo();
		void	renderMultibandNoiseDemo();
		void 	updateMultibandNoiseDemo();

	
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
	
		float	originX; 
		float	originY;
	
		void	render1DNoiseStrip (float x, float y, float width, float height, float dt, float *data);
		float	summedNoiseData[NOISE_DATA_STRIP_LENGTH];
		int		nNoiseStrips;	
		vector<ofxSimpleSlider> sliderGroup;
		vector<NoiseDataStrip> noiseDataStripGroup;

		
};
