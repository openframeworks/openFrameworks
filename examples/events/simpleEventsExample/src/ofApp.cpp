#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetCircleResolution(64);
	//---------- init gray circles----------

	for (int i = 0; i < TOTAL_GRAY_CIRCLES; i++) {
		float r = ofRandom(50) + 20;
		float x = ofRandom(ofGetWidth()-(r*2))+r;
		float y = ofRandom(ofGetHeight()-(r*2))+r;
		grayCircles[i].setup(r, x, y,ofColor::gray);
	}
	//---------- init my circle ---------
 	redCircle.setup(100, ofRandom(ofGetWidth()-200)+100, ofRandom(ofGetHeight()-200)+100, ofColor::red);

	//------------registering (adding) listeners ----------

	ofAddListener(redCircle.clickedInside, //the ofEvent that we want to listen to. In this case exclusively to the circleEvent of redCircle (red circle) object.
				  this, //pointer to the class that is going to be listening. it can be a pointer to any object. There's no need to declare the listeners within the class that's going to listen.
				  &ofApp::onMouseInCircle);//pointer to the method that's going to be called when a new event is broadcasted (callback method). The parameters of the event are passed to this method.
	ofAddListener(Circle::clickedInsideGlobal , this, &ofApp::onMouseInAnyCircle);//listening to this event will enable us to get events from any instance of the circle class as this event is static (shared by all instances of the same class).

	//Notice that when calling the static event we are using :: while when calling the instance event we use a . (dot).
  }
//--------------------------------------------------------------
void ofApp::update(){}
//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(bg);
	redCircle.draw();

	for (int i = 0; i < TOTAL_GRAY_CIRCLES; i++) {
		grayCircles[i].draw();
	}

	ofPushStyle();
	ofSetColor(0);


	ofDrawCircle(clickedPoint, 10);
	ofPopStyle();
	string msg = "by clicking on any circle the background is changed. Global event\n";
	msg += "by clicking over the red circle the  little black circle will be positioned at the click point.\n";
	ofDrawBitmapStringHighlight(msg, 30,30);
}
//--------------------------------------------------------------
void ofApp::onMouseInCircle(glm::vec2 & e){
	clickedPoint = e;
}
//--------------------------------------------------------------
void ofApp::onMouseInAnyCircle(glm::vec2 & e){
	 bg.set(ofRandom(255), ofRandom(255), ofRandom(255));
}
