#include "ofApp.h"

void ofApp::setup(){
        
    panel_.setup("Engine Seeding");
    panel_.add(seed_);
    panel_.add(reconstruct_);
    
    seed_.addListener(this, &ofApp::seed);
    reconstruct_.addListener(this, &ofApp::reconstruct);
    
    reconstruct();
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
