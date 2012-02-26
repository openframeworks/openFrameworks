#pragma once

#include "ofMain.h"

#define TOP_MARGIN 150
#define LEFT_MARGIN 150

#define SAVED_HISTORY 800
#define TAIL_LENGTH 800


class  oscilator {
public:	
	
	float freq;
	float waveSin;
	float waveCos;
	float amplitude;
	float phase;
	double counter;
	
	ofVec2f pos;
	
	oscilator(){}
	~oscilator(){}
	void setup(int x, int y){
	
		pos.set(x, y);
		freq = ofRandom(0.00001f, 0.01f);
		
		counter = 0;
		if (y<TOP_MARGIN) {
			phase = ofMap(x-LEFT_MARGIN, LEFT_MARGIN, ofGetWidth(), -PI, PI, false);
			
			amplitude = ofMap(y, 0, TOP_MARGIN, 0.1f, 300, false);
			
		}else {
			phase = ofMap(y-TOP_MARGIN, TOP_MARGIN, ofGetHeight(), -PI, PI, false);
			amplitude = ofMap(x, 0, LEFT_MARGIN, 0.1f, 300, false);
		}
	}
	
	void update(){
		counter += freq;
		waveSin = sin(counter + phase) * amplitude;
		waveCos = cos(counter + phase) * amplitude;
	}
	bool checkOver(int x, int y){
		if (pos.distance(ofVec2f(x,y)) <amplitude*0.25) {
			return true;
		}else{
			return false;
		}
	}
};



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
		
	
	void drawSpinningDisc(oscilator& osc);
		

	
	vector<oscilator>horizontalOscilators;
	vector<oscilator>verticalOscilators;
	
	ofVec3f waveHistory [TAIL_LENGTH];
	
	float horWaveHistory [SAVED_HISTORY];
	float vertWaveHistory [SAVED_HISTORY];

	ofPoint center;
	float scale;
	bool bScaleMouse;
	
	float hWaveMult;
	float vWaveMult;
	
	int selectedOscilator;
	bool bSelectedOscHor;
	bool bSelectedOscVert;
};
