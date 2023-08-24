#include "ofApp.h"
#include "ofRandomEngine.h"
#include "ofRandomDistributions.h"

void ofApp::setup(){
    
    ofSetWindowShape(1920,1000);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetWindowTitle("Random Explorer");
    
    panel_.setup("Global state");
    panel_.add(size_);
    panel_.add(seed_);
	panel_.add(reinit_);
    
//	panel_.add(ok_color_);
//	panel_.add(saturation_);
//	panel_.add(value_);
//	panel_.add(offset_);
	
	for (auto & [name, group]: dists_) {
		group->panel_.setup();
		//        group->panel_.disableHeader();
		for (auto & dist: group->dists_)  {
			group->panel_.add(dist->parameters_);
			group->panel_.getGroup(dist->parameters_.getName()).getButton("click for reference").setTextColor(ofColor{60,60,120});
		}
	}
	
	colorize();
    
    auto y = panel_.getPosition().y + panel_.getHeight() + 120;

    dists_["core"]->panel_.setPosition(10,y);
    dists_["special"]->panel_.setPosition(10+col_w_,y);
    dists_["of"]->panel_.setPosition(10+col_w_*2,y);
    dists_["old"]->panel_.setPosition(10+col_w_*2,y+((square_+gap_)*(dists_["of"]->dists_.size()+1)));

    seed_.addListener(this, &ofApp::seed);
	reinit_.addListener(this, &ofApp::reinit);

	ok_color_.addListener(this, &ofApp::colorize_bool);
	saturation_.addListener(this, &ofApp::colorize_float);
	value_.addListener(this, &ofApp::colorize_float);
	offset_.addListener(this, &ofApp::colorize_float);
	
    reinit(); // not required from the random perspective, but keeps the demo together
    
}

void ofApp::update(){
    
    // DISTRIBUTIONS
    auto num_samples = pow(size_.get(),3);
	size_string_ = "samples per distribution per frame: "s+ofToString(num_samples)+" total random calls per frame: "+ofToString(num_samples*20)+" @ "+ofToString(ofGetFrameRate())+"fps";
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
    
    auto x =  220;
	ofDrawBitmapStringHighlight(size_string_, x, 40);
    ofDrawBitmapStringHighlight(dna_string_, x, 80);
	ofDrawBitmapStringHighlight(shuffle_string_, x, 100);
	ofDrawBitmapStringHighlight("other random functions (like std::shuffle) can feed from the same thread-safe engine, ensuring coherence", x, 140);

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
