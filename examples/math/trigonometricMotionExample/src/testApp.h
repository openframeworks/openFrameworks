#pragma once

#include "ofMain.h"


//The following defines are called preprocessor directives. 
//This are just to make the programmers life easier.
//This are actually not compiled, instead whenever you use a defined directive it gets replaced by it's associated value in the source code BEFORE it compiles.
//So this don't use memory space and if you are using this directives a lot it's easier and faster to change it's value.
#define TOP_MARGIN 150
#define LEFT_MARGIN 150

#define WAVEFORM_HISTORY 800
#define TAIL_LENGTH 400



//this is our oscilator class. 
//This defines how to save, update and draw each oscilators properties and values.
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
	void setup(int x, int y){//this function initializes all the properties of the oscilator. It must be called just after creating a new oscilator.
	
		pos.set(x, y);//sets the position of the oscilator using the values passed to the function. In this case is the position where the mouse was clicked.
		freq = ofRandom(0.0001f, 0.1f);// the frequency get's initialized with a random value.
		
		counter = 0; 
		if (y<TOP_MARGIN) {//this if is to determine if the oscilator is either an horizontal or vertical oscilator.
			phase = ofMap(x-LEFT_MARGIN, LEFT_MARGIN, ofGetWidth(), -PI, PI, false);//the phase is defined by to position of the locator.Here it's value gets remaped to the correct range.
			
			amplitude = ofMap(y, 0, TOP_MARGIN, 0.1f, 300, false);//Amplitude is defined by the position, and it's value gets remaped to the correct range.
			
		}else {
			phase = ofMap(y-TOP_MARGIN, TOP_MARGIN, ofGetHeight(), -PI, PI, false);
			amplitude = ofMap(x, 0, LEFT_MARGIN, 0.1f, 300, false);
		}
	}
	
	void update(){ 
		counter += freq;//the counter is incremented by the value of freq each time the oscilators update methods is called.
		waveSin = sin(counter + phase) * amplitude;//Here is where we calulate the sine value for the oscilator. 
												   //Then is multiplied by amplitude, because the sin() function returns a value between -1 and 1
		waveCos = cos(counter + phase) * amplitude;
	}
	
	void draw(){//here we draw the oscilator
		ofFill();
		ofCircle(pos.x, pos.y, amplitude/4);
		ofSetColor(40);
		ofNoFill();
		ofCircle(pos.x, pos.y, amplitude/4);
		ofLine(pos.x, pos.y, pos.x +  waveCos/4, pos.y + waveSin/4);
	}
	bool checkOver(int x, int y){//this is to check if the mouse is over the oscilator.
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
	void setPressedOscilator(int index, bool isHorizontal);
	
	vector<oscilator>horizontalOscilators; //In this vector object we're going to save our horizontal oscilators.
	vector<oscilator>verticalOscilators;   //In this vector object we're going to save our vertical oscilators.
	
	ofVec3f waveHistory [TAIL_LENGTH]; // This array object is to save the previous positions of the composite wave we're creating, so we can draw it's path later.
	
	float horWaveHistory [WAVEFORM_HISTORY];//This and the following array are for saving the wave form history.
	float vertWaveHistory [WAVEFORM_HISTORY];

	ofPoint center; //This is to store the center location for the composite wave .
	float scale; // the amount by which the waveforms get scaled.
	bool bScaleMouse;//just to know that you're using the mouse to change the scale.
	
	float hWaveMult;//this are to scale or multiply the vertical and horizontal waveforms.
	float vWaveMult;
	
	int selectedOscilator; //this stores the clicked oscilator position within the vector when it's being pressed.
	bool bSelectedOscHor; //when you click over an oscilator this boolean indicates whether it was one of the horizontal oscilators or not.                                                                                                          
	bool bSelectedOscVert;//when you click over an oscilator this boolean indicates whether it was one of the vertical oscilators or not.
};
