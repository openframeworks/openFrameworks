#include "ofApp.h"

void ofApp::setup(){
    
    ofSetWindowTitle("Random Example");
    
    panel_.setup();
    panel_.add(seed_);
    panel_.add(reinit_);
    seed_.addListener(this, &ofApp::seed);
    reinit_.addListener(this, &ofApp::reinit);
    
    // get things shuffled
    perform_shuffle();
}

void ofApp::draw(){
    panel_.draw();
    ofDrawBitmapStringHighlight(shuffled_string_, 230, 80);
    if (of::random::engine()->is_deterministic()) {
        ofDrawBitmapStringHighlight("engine is deterministic (seeded)", 230, 40, ofColor::black, ofColor::green);
    } else {
        ofDrawBitmapStringHighlight("engine is non-deterministic", 230, 40, ofColor::black, ofColor::white);
    }
}
