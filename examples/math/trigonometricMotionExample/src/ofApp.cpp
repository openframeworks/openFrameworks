#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetCircleResolution(40);
	
	//our history arrays are initialized with a value of zero for all of it's elements.
	for (int i=1; i<TAIL_LENGTH; i++) {
		waveHistory[i] = ofVec3f(0, 0, 0);
	}
	for (int i=1; i<WAVEFORM_HISTORY; i++) {
		horWaveHistory[i] = 0;
		vertWaveHistory[i] = 0;
	}
	
	//our center point is defined here.
	center= ofPoint((ofGetWidth()-LEFT_MARGIN)*0.5f +LEFT_MARGIN, 
									(ofGetHeight()-TOP_MARGIN)*0.5f + TOP_MARGIN);
	
	bScaleMouse=false;
	
	scale=1;
	
	//this are the multipliers for scaling the horizontal and vertical waveforms so this fit into the screen.
	hWaveMult=(ofGetWidth()-LEFT_MARGIN)/float(WAVEFORM_HISTORY);
	vWaveMult=(ofGetHeight()-TOP_MARGIN)/float(WAVEFORM_HISTORY);
	
	selectedOscilator=-1;
	bSelectedOscHor = false;
	bSelectedOscVert = false;
	
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	for (unsigned int i=0; i<horizontalOscilators.size(); i++) {
		horizontalOscilators[i].update();
	}
	for (unsigned int i=0; i<verticalOscilators.size(); i++) {
		verticalOscilators[i].update();
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor(245), ofColor(200));
	//ofSetLineWidth(1);
	
	//The following is just to print the instructions to the screen.
	ofSetColor(80);
	ofDrawBitmapString("Click here to add horizontal oscillators.", LEFT_MARGIN +100, TOP_MARGIN-5 );
	
	ofDrawBitmapString("Click and drag\nover an\noscillator to\nmodify it's\nspeed and\namplitude", 15,25);
	ofDrawBitmapString("Click in this area and drag upwards/downwards to scale up/down.\nPress spacebar to delete all the oscillators.", LEFT_MARGIN + 200, ofGetHeight()-40);
	
	//All this bunch is to print the vertical text
	ofPushMatrix();
	ofTranslate(LEFT_MARGIN -5,  ofGetHeight() - 100, 0);
	ofRotate(-90, 0, 0, 1);
	ofDrawBitmapString("Click here to add vertical oscillators.",  0, 0 );
	ofPopMatrix();
	
	
	
	
	ofEnableSmoothing();
	
	//This are just the reference lines draw in the screen.
	ofSetColor(0, 0, 0, 150);
	ofDrawLine(LEFT_MARGIN, 0, LEFT_MARGIN, ofGetHeight());
	ofDrawLine(0, TOP_MARGIN, ofGetWidth(), TOP_MARGIN);
	
	ofSetColor(0, 0, 0, 80);
	ofDrawLine(LEFT_MARGIN, center.y, ofGetWidth(), center.y);
	ofDrawLine(center.x, TOP_MARGIN, center.x, ofGetHeight());
	
	//ofSetLineWidth(2);
	
	
	float horWave = 0;
	float vertWave = 0;
	
	//here we go through all the horizontal oscillators
	for (unsigned int i=0; i<horizontalOscilators.size(); i++) {
		ofSetColor(255, 127+i, 0,150);
		horizontalOscilators[i].draw(); //we draw each oscillator
		horWave += horizontalOscilators[i].waveSin;
		//THIS IS IMPORTANT. Here we are adding together all the current sine values of each oscillator.
		// This is what is creates all the crazy motion that we get.
	}
	//the same as above but for vertical oscillators
	for (unsigned int i=0; i<verticalOscilators.size(); i++) {
		ofSetColor(0, 127+i, 255, 150);
		verticalOscilators[i].draw();
		vertWave += verticalOscilators[i].waveSin;
	}
	//here we move all the elements of the array one position forward so to make space for a new value.
	for (int i=1; i<TAIL_LENGTH; i++) {
		waveHistory[i-1] = waveHistory[i];
	} 
	for (int i=1; i<WAVEFORM_HISTORY; i++) {
		horWaveHistory[i-1] = horWaveHistory[i];
		vertWaveHistory[i-1]= vertWaveHistory[i];
	}
	// here we save into our history
	horWaveHistory[WAVEFORM_HISTORY-1] = horWave;
	vertWaveHistory[WAVEFORM_HISTORY-1] = vertWave;
	waveHistory[TAIL_LENGTH-1] = ofVec3f(horWave, vertWave,0);
	
	
	
	ofMesh wave; // declaring a new ofMesh object with which we're drawing the motion path created by summing the vertical and horizontal oscillators
	wave.setMode(OF_PRIMITIVE_LINE_STRIP);
	for (int i=0; i<TAIL_LENGTH; i++) {
		wave.addColor(ofFloatColor(0.1f,0.1f,0.1f, 0.5f + 0.5f * i/float(TAIL_LENGTH) ));
		wave.addVertex(waveHistory[i]);
	}
	
	//all the following is to create and populate the horizontal and vertical waveforms.
	ofMesh hWave;
	hWave.setMode(OF_PRIMITIVE_LINE_STRIP);
	ofMesh vWave;
	vWave.setMode(OF_PRIMITIVE_LINE_STRIP);
	for (int i=0; i<WAVEFORM_HISTORY; i++) {
		hWave.addColor(ofFloatColor(255, 240,10, 255));
		hWave.addVertex(ofVec3f(i*hWaveMult, horWaveHistory[i]*0.1f*scale, 0));
		vWave.addColor(ofFloatColor(255, 240,10, 255));
		vWave.addVertex(ofVec3f(vertWaveHistory[i]*0.1f*scale, i*vWaveMult, 0));
	}
	
	//draw the vertical and horizontal wave.
	ofPushMatrix();
	ofTranslate(LEFT_MARGIN, TOP_MARGIN, 0);
	hWave.draw();
	vWave.draw();
	ofPopMatrix();
	
	//draw the composite wave.
	ofPushMatrix();
	ofTranslate(center.x, center.y, 0);
	ofScale(scale, scale, 0);
	wave.draw();
	ofSetColor(0,10, 255),
	ofDrawCircle(horWave, vertWave, 10);
	ofPopMatrix();
	
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == ' ') {//when the spacebar is pressed the vectors that contain all the oscillators are cleared.
		horizontalOscilators.clear();
		verticalOscilators.clear();
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (selectedOscilator>-1) {//when an oscillator is clicked and dragged it's speed (freq) and amplitude are changed.
		//the following lines do so.
		if(bSelectedOscHor==true){
			horizontalOscilators[selectedOscilator].freq += 0.1f * (ofGetPreviousMouseX() - ofGetMouseX())/ float(ofGetWidth());
			horizontalOscilators[selectedOscilator].amplitude += ofGetMouseY() - ofGetPreviousMouseY();	
		}else if (bSelectedOscVert==true) {
			verticalOscilators[selectedOscilator].freq += 0.1f * (ofGetPreviousMouseY() - ofGetMouseY())/ float(ofGetHeight());
			verticalOscilators[selectedOscilator].amplitude += ofGetMouseX() - ofGetPreviousMouseX(); 
		}
	}else if (bScaleMouse) {
		scale += float(ofGetMouseY()-ofGetPreviousMouseY())/ofGetHeight();
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
	if (y< TOP_MARGIN && x>LEFT_MARGIN) {
		for (unsigned int i = 0; i < horizontalOscilators.size(); i++) {//this goes through the horizontal oscillators checking if anyone has been clicked over.
			if(horizontalOscilators[i].checkOver(x, y)){
				setPressedOscilator(i, true);
				break;
			}
		}
		if (!bSelectedOscHor) {//in case that no oscillator was clicked then create a new one at the position of the mouse.
			horizontalOscilators.push_back(oscillator());
			horizontalOscilators.back().setup(x, y);
		}
	}else if(y>TOP_MARGIN && x<LEFT_MARGIN){
		for (unsigned int i = 0; i < verticalOscilators.size(); i++) {
			if(verticalOscilators[i].checkOver(x, y)){
				setPressedOscilator(i, false);	
				break;
			}
		}
		if (!bSelectedOscVert) {
			verticalOscilators.push_back(oscillator());
			verticalOscilators.back().setup(x, y);
		}
	}else {
		bScaleMouse=true;
	}
}
//--------------------------------------------------------------
void ofApp::setPressedOscilator(int index, bool isHorizontal){
	selectedOscilator=index;
	bSelectedOscHor =isHorizontal;
	bSelectedOscVert =!isHorizontal;
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bScaleMouse=false;
	selectedOscilator=-1;
	bSelectedOscHor=false;
	bSelectedOscVert=false;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
}
