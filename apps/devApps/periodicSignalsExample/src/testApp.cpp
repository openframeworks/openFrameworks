#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	trail.resize(8);
	y.resize(8);

	x = 0;
	prevX = 0;
	y[0] = 60*2;
	radius = 3;
	rightMargin = 190;

	gui.setup();
	gui.add(yFactor.setup("yFactor (px)",20,0,30));
	gui.add(speed.setup("x speed (px/s)",30,1,200));
	gui.add(freq.setup("frequency (hz)",1,0,10));
	gui.add(showPos.setup("time/pos",true));

	speed.addListener(this,&testApp::speedChanged);


	ofSetVerticalSync(true);
	//ofBackground(0);
	ofEnableAlphaBlending();
	ofSetColor(20);
	//ofSetLineWidth(1.5);
}

//--------------------------------------------------------------
void testApp::update(){
	t = ofGetElapsedTimef();
	if(trail[0].size()==0){
		initTime = t;
	}
	t = t - initTime;

	x = int(t*speed)%(ofGetWidth()-rightMargin);
	y[1] = 60*3+ofNoise(t*freq)*yFactor;
	y[2] = 60*4+ofSignedNoise(t*freq)*yFactor;
	y[3] = 60*5+ofRandom(-1,1)*yFactor; // random can't be dependent on time or frequency
	y[4] = 60*6+sin(t*freq*TWO_PI)*yFactor;
	y[5] = 60*7+(sin(t*freq*TWO_PI)+1)*.5*yFactor;
	y[6] = 60*8+fmod(t*freq,1)*yFactor;
	y[7] = 60*9+(fmod(t*freq,1)*2-1)*yFactor;

	for(int i=0;i<(int)trail.size();i++){
		if(x<prevX){
			trail[i].clear();
		}else{
			trail[i].addVertex(ofPoint(x,y[i]));
		}
	}

	prevX = x;

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor(240),ofColor(230),OF_GRADIENT_BAR);

	ofSetColor(170);
	for(int i=0;i<(ofGetWidth()-rightMargin)/speed;i++){
		ofLine(i*speed,0,i*speed,ofGetHeight());
		if(showPos)
			ofDrawBitmapString(ofToString(i)+"s",i*speed,ofGetHeight()-20);
		else
			ofDrawBitmapString(ofToString(i*speed,0)+"p",i*speed,ofGetHeight()-20);
	}

	ofSetColor(20);
	ofLine(ofGetWidth()-rightMargin,0,ofGetWidth()-rightMargin,ofGetHeight());

	for(int i=0;i<(int)trail.size();i++){
		ofEnableSmoothing();
		ofCircle(x,y[i],radius);
		trail[i].draw();


		ofDisableSmoothing();
		ofLine(0,60*(i+2),ofGetWidth(),60*(i+2));

		float rectY = 60*(i+2);
		ofRect(ofGetWidth()-rightMargin+10,rectY,10,y[i]-rectY);
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

	ofSetColor(255);
	gui.draw();
}

//--------------------------------------------------------------
void testApp::speedChanged(int & speed){
	for(int i=0;i<trail.size();i++){
		trail[i].clear();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
