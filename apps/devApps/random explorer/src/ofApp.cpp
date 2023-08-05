#include "ofApp.h"
#include "ofRandom.hpp"

void ofApp::setup(){
    
    ofSetWindowShape(1920,1000);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetWindowTitle("Random Explorer");
    
    panel_.setup("Global state");
    panel_.add(size_);
    panel_.add(size_calc_);
    panel_.add(seed_);
    panel_.add(reinit_);
        
    size_t sum = 0;
    for (auto& group : {dists_["core"], dists_["special"], dists_["of"]}) sum+=group->dists_.size();
    auto chunk = 255.0/sum;
    
    size_t i = 0; // to spread the hue; order below matters
    for (auto & group: {dists_["core"], dists_["special"], dists_["of"], dists_["old"]}) {
                
        group->panel_.setup();
//        group->panel_.disableHeader();
        for (auto & dist: group->dists_) {
            group->panel_.add(dist->parameters_);
            if (i>=sum) {
                dist->color_ = {48,48,48,255}; // sad
            } else {
                dist->color_.setHsb(chunk * i++, 192, 64);
            }
            group->panel_.getGroup(dist->parameters_.getName()).setHeaderBackgroundColor(dist->color_);
        }
    }
    
    auto y = panel_.getPosition().y + panel_.getHeight() + 50;

    // because ofxpanel does not track ofTranslate()
    dists_["core"]->panel_.setPosition(10,y);
    dists_["special"]->panel_.setPosition(10+col_w_,y);
    dists_["of"]->panel_.setPosition(10+col_w_*2,y);
    dists_["old"]->panel_.setPosition(10+col_w_*2,y+((square_+gap_)*(dists_["of"]->dists_.size()+1)));

    seed_.addListener(this, &ofApp::seed);
    reinit_.addListener(this, &ofApp::reinit);
    reinit(); // not required from the random perspective, but keeps the demo together
    
}

void ofApp::update(){
    
    // DISTRIBUTIONS
    auto num_samples = pow(size_.get(),3);
    size_calc_ = ofToString(num_samples);
    for (auto & [name, group]: dists_) {
        for (const auto & d: group->dists_) {
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
    
    auto x =  220;
    ofDrawBitmapStringHighlight(dna_string_, x, 60);
    ofDrawBitmapStringHighlight(shuffle_string_, x, 80);
    
    if (of::random::Engine::instance()->is_deterministic()) {
        ofDrawBitmapStringHighlight("engine is deterministic (seeded)", x, 20, ofColor::black, ofColor::green);
    } else {
        ofDrawBitmapStringHighlight("engine is non-deterministic", x, 20, ofColor::black, ofColor::white);
    }
    panel_.draw();
    dists_["core"]->draw("C++ fundamental distributions", square_, gap_);
    dists_["special"]->draw("more specialized distributions", square_, gap_);
    dists_["of"]->draw("OF/art-centric wrappers/utils", square_, gap_);
    dists_["old"]->draw("Previous implementation (reference)", square_, gap_);
    
}
