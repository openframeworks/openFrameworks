#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	ofSetLogLevel(OF_LOG_VERBOSE);


	/*
	 *  create tab container
	 */
	tabbed_pages = gui.addTabs("", ofJson({{"width", "70%"}}));
	tabbed_pages->setPosition(40, 70);

	/*
	 *  create pages and add them to tab layout
	 */
	page1 = tabbed_pages->addPanel("page1", ofJson({{"show-header", false}}));
	page2 = tabbed_pages->addPanel("page2", ofJson({{"show-header", false}}));
	page3 = tabbed_pages->addPanel("page3", ofJson({{"show-header", false}}));


	/*
	 *  fill page 1
	 */
	vector<ofJson> choices;
	choices.push_back({{"float", "left"}, {"background-color", "#2da1e3"}});
	choices.push_back({{"float", "right"}, {"background-color", "#0ff"}});
	choices.push_back({{"float", "none"}, {"background-color", "rgba(255,0,0,0.7)"}});

	for(int i = 0; i < 10; i++){

		ofRandomize(choices);

		ofxLabel* label = page1->add<ofxLabel>(ofToString(i,2), choices[0]);
		label->setPercentalWidth(false);
		label->setSize((int)ofRandom(100,200), (int)ofRandom(60, 80));
		label->setMargin(1);
	}

	/*
	 *  raise the chance to create left or right floating elements
	 */
	choices.push_back({{"float", "left"}, {"background-color", "#2da1e3"}});
	choices.push_back({{"float", "right"}, {"background-color", "#0ff"}});

	/*
	 *  fill page 2
	 */
	for(int i = 0; i < 20; i++){

		ofRandomize(choices);

		ofxLabel* label = page2->add<ofxLabel>(ofToString(i,2), choices[0]);
		label->setPercentalWidth(false);
		label->setSize((int)ofRandom(70,50), (int)ofRandom(30, 50));
		label->setMargin(1);
	}

	/*
	 *  fill page 3
	 */
	for(int i = 0; i < 30; i++){

		ofRandomize(choices);

		ofxLabel* label = page3->add<ofxLabel>(ofToString(i,2), choices[0]);
		label->setPercentalWidth(false);
		label->setSize((int)ofRandom(130,150), (int)ofRandom(30, 50));
		label->setMargin(1);
	}


}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackgroundGradient(ofColor::white, ofColor::gray);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key){
	 case 'f': {
		 ofToggleFullscreen();
		 break;
	 }

	 default:
		 break;
	}

}

//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs & args){
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs & args){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs & args){
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
