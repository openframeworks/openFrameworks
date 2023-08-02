#include "ofApp.h"
#include "ofRandom.hpp"

// https://rovdownloads.com/blog/quick-overview-of-probability-distributions/


void ofApp::setup(){
    
    ofColor(255,255,255);
    ofRectangle(10,10,200,200);
    
	ofSetWindowShape(1920,1000);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
	panel_.setup("Random Generators");
	panel_.add(size_);
    panel_.add(size_calc_);
    panel_.add(seed_);
    panel_.add(reinit_);
	panel_.setPosition(400,10);
    
	size_t i = 0; // to spread hue
	auto chunk = 255.0/(dists_.size() + of_dists_.size());
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
    
    seed_.addListener(this, &ofApp::seed);
    reinit_.addListener(this, &ofApp::reinit);
    reinit(); // not required, but keeps the demo together

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
}

void ofApp::draw(){
	ofClear(20);
    
    if (of::random::Engine::instance()->is_deterministic()) {
        ofDrawBitmapStringHighlight("engine is deterministic (seeded)", 20, 20, ofColor::black, ofColor::green);
    } else {
        ofDrawBitmapStringHighlight("engine is non-deterministic", 20, 20, ofColor::black, ofColor::white);
    }

    ofDrawBitmapStringHighlight(dna_string_, 20, 40);
    ofDrawBitmapStringHighlight(shuffle_string_, 20, 60);

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
