#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetCircleResolution(40);
	
	
	for (int i=1; i<TAIL_LENGTH; i++) {
		waveHistory[i] = ofVec3f(0, 0, 0);
	}
	for (int i=1; i<SAVED_HISTORY; i++) {
		horWaveHistory[i] = 0;
		vertWaveHistory[i] = 0;
	}
	
	center= ofPoint((ofGetWidth()-LEFT_MARGIN)*0.5f +LEFT_MARGIN, 
					(ofGetHeight()-TOP_MARGIN)*0.5f + TOP_MARGIN);
	
	bScaleMouse=false;
	
	scale=1;
	
	hWaveMult=(ofGetWidth()-LEFT_MARGIN)/float(SAVED_HISTORY);
	vWaveMult=(ofGetHeight()-TOP_MARGIN)/float(SAVED_HISTORY);
	
	selectedOscilator=-1;
	bSelectedOscHor = false;
	bSelectedOscVert = false;
	
	ofEnableSmoothing();
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::update(){

	for (int i=0; i<horizontalOscilators.size(); i++) {
		horizontalOscilators[i].update();
	}
	for (int i=0; i<verticalOscilators.size(); i++) {
		verticalOscilators[i].update();
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor(245), ofColor(200));
	//ofSetLineWidth(1);
	ofSetColor(80);
	ofDrawBitmapString("Click here to add horizontal oscilators.", LEFT_MARGIN +100, TOP_MARGIN-5 );
	
	ofDrawBitmapString("Click and drag\nover an\noscilator to\nmodify it's\nspeed and\namplitude", 15,25);
	ofDrawBitmapString("Click in this area and drag upwards/downwards to scale up/down.\nPress spacebar to delete all the oscilators.", LEFT_MARGIN + 200, ofGetHeight()-40);
	
	ofPushMatrix();
	ofTranslate(LEFT_MARGIN -5,  ofGetHeight() - 100, 0);
	ofRotate(-90, 0, 0, 1);
	ofDrawBitmapString("Click here to add vertical oscilators.",  0, 0 );
	ofPopMatrix();
	
	ofEnableSmoothing();
	
	
	ofSetColor(0, 0, 0, 150);
	ofLine(LEFT_MARGIN, 0, LEFT_MARGIN, ofGetHeight());
	ofLine(0, TOP_MARGIN, ofGetWidth(), TOP_MARGIN);
	
	ofSetColor(0, 0, 0, 80);
	ofLine(LEFT_MARGIN, center.y, ofGetWidth(), center.y);
	ofLine(center.x, TOP_MARGIN, center.x, ofGetHeight());
	
	//ofSetLineWidth(2);

	float horWave = 0;
	float vertWave = 0;
	
	for (int i=0; i<horizontalOscilators.size(); i++) {
		ofSetColor(255, 127+i, 0,150);
		drawSpinningDisc(horizontalOscilators[i]);
		horWave += horizontalOscilators[i].waveSin;
	}
	for (int i=0; i<verticalOscilators.size(); i++) {
		ofSetColor(0, 127+i, 255, 150);
		drawSpinningDisc(verticalOscilators[i]);
		vertWave += verticalOscilators[i].waveSin;
	}
	
	for (int i=1; i<TAIL_LENGTH; i++) {
		waveHistory[i-1] = waveHistory[i];
	}
	for (int i=1; i<SAVED_HISTORY; i++) {
		horWaveHistory[i-1] = horWaveHistory[i];
		vertWaveHistory[i-1]= vertWaveHistory[i];
	}
	horWaveHistory[SAVED_HISTORY-1] = horWave;
	vertWaveHistory[SAVED_HISTORY-1] = vertWave;
	waveHistory[TAIL_LENGTH-1] = ofVec3f(horWave, vertWave,0);
	
	
	ofMesh wave;
	wave.setMode(OF_PRIMITIVE_LINE_STRIP);
	
	for (int i=0; i<TAIL_LENGTH; i++) {
		wave.addColor(ofFloatColor(0.1f,0.1f,0.1f, 0.5f + 0.5f * i/float(TAIL_LENGTH) ));
		wave.addVertex(waveHistory[i]);
	}
	
	ofMesh hWave;
	hWave.setMode(OF_PRIMITIVE_LINE_STRIP);
	ofMesh vWave;
	vWave.setMode(OF_PRIMITIVE_LINE_STRIP);
	
	for (int i=0; i<SAVED_HISTORY; i++) {
		hWave.addColor(ofFloatColor(255, 240,10, 255));
		hWave.addVertex(ofVec3f(i*hWaveMult, horWaveHistory[i]*0.1f*scale, 0));
		vWave.addColor(ofFloatColor(255, 240,10, 255));
		vWave.addVertex(ofVec3f(vertWaveHistory[i]*0.1f*scale, i*vWaveMult, 0));
	}
	
	ofPushMatrix();
	ofTranslate(LEFT_MARGIN, TOP_MARGIN, 0);
	hWave.draw();
	vWave.draw();
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(center.x, center.y, 0);
	ofScale(scale, scale, 0);
	wave.draw();
	ofSetColor(0,10, 255),
	ofCircle(horWave, vertWave, 10);
			
	ofPopMatrix();
	
}


//--------------------------------------------------------------
void testApp::drawSpinningDisc(oscilator& osc){
	ofFill();
	ofCircle(osc.pos.x, osc.pos.y, osc.amplitude/4);
	ofSetColor(40);
	ofNoFill();
	ofCircle(osc.pos.x, osc.pos.y, osc.amplitude/4);
	ofLine(osc.pos.x, osc.pos.y, osc.pos.x +  osc.waveCos/4, osc.pos.y + osc.waveSin/4);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if (key == ' ') {
		horizontalOscilators.clear();
		verticalOscilators.clear();
	}
	
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if (selectedOscilator>-1) {
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
void testApp::mousePressed(int x, int y, int button){

	if (y< TOP_MARGIN && x>LEFT_MARGIN) {
		for (int i = 0; i < horizontalOscilators.size(); i++) {
			if(horizontalOscilators[i].checkOver(x, y)){
				selectedOscilator=i;
				bSelectedOscHor =true;
				bSelectedOscVert =false;
				cout << "horizontalOscilator "<< i <<" pressed" <<endl;
				break;
			}
		}
		if (!bSelectedOscHor) {
			horizontalOscilators.push_back(oscilator());
			horizontalOscilators.back().setup(x, y);
		}
	}else if(y>TOP_MARGIN && x<LEFT_MARGIN){
		for (int i = 0; i < verticalOscilators.size(); i++) {
			if(verticalOscilators[i].checkOver(x, y)){
				selectedOscilator=i;
				bSelectedOscVert =true;
				bSelectedOscHor =false;
				cout << "verticalOscilator "<< i <<" pressed" <<endl;
				break;
			}
		}
		if (!bSelectedOscVert) {
			verticalOscilators.push_back(oscilator());
			verticalOscilators.back().setup(x, y);
		}
	}else {
		bScaleMouse=true;
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
		bScaleMouse=false;
	selectedOscilator=-1;
	bSelectedOscHor=false;
	bSelectedOscVert=false;
}
