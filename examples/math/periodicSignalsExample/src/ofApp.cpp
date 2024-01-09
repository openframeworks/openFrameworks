#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	trail.resize(8);
	y.resize(8);

	x = 0;
	prevX = 0;
	y[0] = 60*2;
	radius = 3;
	rightMargin = 190;
	
	showPos = false;
	
	yFactor.setup(130, 20, 100, 20,0,30,20,false, false);
	yFactor.setLabelString("y amnt");
	
	speed.setup(130, 45, 100, 20,1,200,30,false, false);
	speed.setLabelString("x speed (px/s)");
	
	freq.setup(130, 70, 100, 20,0,10,1,false, false);
	freq.setLabelString("frequency (hz)");
	
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	ofSetColor(20);
	
	preSpeed = speed.getValue();
}

//--------------------------------------------------------------
void ofApp::update(){

	//if the speed has changed we need to clear the lines.
	// we can't test for float equality or non-equality 
	// (speed.getValue() != preSpeed)
	// so we look at the change and if the change is greater then a certain
	// amount we clear. 
	
	if( fabs(speed.getValue() - preSpeed) > 0.00001f ){
		speedChanged();
	}
	preSpeed = speed.getValue();

	t = ofGetElapsedTimef();
	if(trail[0].size()==0){
		initTime = t;
	}
	t = t - initTime;

	x = int(t*speed.getValue())%(ofGetWidth()-rightMargin);
	y[1] = 60*3+ofNoise(t*freq.getValue())*yFactor.getValue();
	y[2] = 60*4+ofSignedNoise(t*freq.getValue())*yFactor.getValue();
	
	float test = 60*4+ofSignedNoise(t*freq.getValue())*yFactor.getValue();
	
	cout<< test << endl;
	
	y[3] = 60*5+ofRandom(-1,1)*yFactor.getValue(); // random can't be dependent on time or frequency
	y[4] = 60*6+sin(t*freq.getValue()*TWO_PI)*yFactor.getValue();
	y[5] = 60*7+(sin(t*freq.getValue()*TWO_PI)+1)*.5*yFactor.getValue();
	y[6] = 60*8+fmod(t*freq.getValue(),1)*yFactor.getValue();
	y[7] = 60*9+(fmod(t*freq.getValue(),1)*2-1)*yFactor.getValue();

	for(int i=0;i<(int)trail.size();i++){
		if(x<prevX){
			trail[i].clear();
		}else{
			trail[i].addVertex(x,y[i]);
		}
	}

	prevX = x;

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackgroundGradient(ofColor(240),ofColor(210),OF_GRADIENT_BAR);

	ofSetColor(170);
	for(int i=0;i<(ofGetWidth()-rightMargin)/speed.getValue();i++){
		ofDrawLine(i*speed.getValue(),120,i*speed.getValue(),ofGetHeight());
		if(showPos)
			ofDrawBitmapString(ofToString(i)+"s",i*speed.getValue(),ofGetHeight()-20);
		else
			ofDrawBitmapString(ofToString(i*speed.getValue(),0)+"p",i*speed.getValue(),ofGetHeight()-20);
	}

	ofSetColor(20);
	ofDrawLine(ofGetWidth()-rightMargin,0,ofGetWidth()-rightMargin,ofGetHeight());

	for(int i=0;i<(int)trail.size();i++){
		ofEnableSmoothing();
		ofDrawCircle(x,y[i],radius);
		trail[i].draw();


		ofDisableSmoothing();
		ofDrawLine(0,60*(i+2),ofGetWidth(),60*(i+2));

		float rectY = 60*(i+2);
		ofDrawRectangle(ofGetWidth()-rightMargin+10,rectY,10,y[i]-rectY);
	}

	ofDrawBitmapString("constant",ofGetWidth()-160,60*2-4);
	ofDrawBitmapString("ofNoise(t)",ofGetWidth()-160,60*3-4);
	ofDrawBitmapString("ofSignedNoise(t)",ofGetWidth()-160,60*4-4);
	ofDrawBitmapString("ofRandom(-1,1)",ofGetWidth()-160,60*5-4);
	ofDrawBitmapString("sin(TWO_PI*t)",ofGetWidth()-160,60*6-4);
	ofDrawBitmapString("(sin(TWO_PI*t)+1)/2",ofGetWidth()-160,60*7-4);
	ofDrawBitmapString("modulo(t,1)",ofGetWidth()-160,60*8-4);
	ofDrawBitmapString("modulo(t,1)*2-1",ofGetWidth()-160,60*9-4);

	ofDrawBitmapString("fps:" + ofToString(ofGetFrameRate(),0),ofGetWidth()-80,20);
	ofDrawBitmapString("t:" + ofToString(t,2),ofGetWidth()-80,35);
	ofDrawBitmapString("x:" + ofToString(x,2),ofGetWidth()-80,50);

}

//--------------------------------------------------------------
void ofApp::speedChanged(){
	for(unsigned int i=0;i<trail.size();i++){
		trail[i].clear();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
