#include "ofApp.h"
#include "ofRandom.hpp"

// https://rovdownloads.com/blog/quick-overview-of-probability-distributions/


void ofApp::setup(){

	ofSetWindowShape(1920,1000);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	panel_.setup("Random Generators");
	panel_.add(size_);
	panel_.add(size_calc_);
	panel_.setPosition(400,0);
	size_t i = 0; // to spread hue
	float chunk = 255.0/(dists_.size() + of_dists_.size());
	for (auto & dist: dists_) {
		panel_.add(dist->parameters_);
		dist->color_.setHsb(chunk * i++, 192, 64);
		panel_.getGroup(dist->parameters_.getName()).setHeaderBackgroundColor(dist->color_);
	}
	
	for (auto & dist: of_dists_) {
		panel_.add(dist->parameters_);
		dist->color_.setHsb(chunk * i++, 192, 64);
		panel_.getGroup(dist->parameters_.getName()).setHeaderBackgroundColor(dist->color_);
	}
	
	std::vector<int> v {1,2,3,4,5,6,7,8};
	ofRandomize(v);
	for (auto & i: v) ofLogNotice("1") << i;

	ofShuffle(v);
	for (auto & i: v) ofLogNotice("1") << i;

	of::random::shuffle(v);
	for (auto & i: v) ofLogNotice("1") << i;

}

void ofApp::update(){
	
	// section seed
	// NON-DETERMINISTIC  vs  DETERMINISTIC
	// shuffle
	// barbo
	
	
	// DISTRIBUTIONS
	auto num_samples = pow(size_.get(),3);
	size_calc_ = ofToString(num_samples);
	for (auto & group: {dists_, of_dists_}) {
		for (const auto & d: group) {
			d->clear();
			float start = ofGetElapsedTimef();
			for (std::size_t i=0; i < num_samples; i++) d->gen();
			d->cost_ = ofGetElapsedTimef() - start;
			d->compile();
		}
	}
	
	// VEC2 THINGS
	// truc vec2d avec params animés
}

void ofApp::draw(){
	ofClear(20);
	panel_.draw();

	ofPushMatrix();
	{
		ofTranslate(panel_.getPosition());
		ofPushMatrix();
		{
			ofTranslate(220,15);
			ofDrawBitmapString("C++ fundamental numerics pseudorandom generators",0,0);
			ofTranslate(0,15);
			for (const auto & dist: dists_) {
				dist->draw(0,0,100,100);
				ofTranslate(0,110);
			}
		}
		ofPopMatrix();
		
		ofPushMatrix();
		{
			ofTranslate(800,15);
			ofDrawBitmapString("OF-centric generator wrappers",0,0);
			ofTranslate(0,15);
			for (const auto & dist: of_dists_) {
				dist->draw(0,0,100,100);
				ofTranslate(0,110);
			}
		}
		ofPopMatrix();
	}
	ofPopMatrix();
}

// MARK: ofApp boilerplate
/*
//--------------------------------------------------------------
void ofApp::exit(){

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
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

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
*/
