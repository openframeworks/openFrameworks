#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    
    ofxPanel panel_;
    std::string shuffled_string_;
    ofParameter<unsigned long> seed_{"seed", 0, 0, 1000};
    ofParameter<void> reconstruct_{"re-construct engine"};

    void setup() override;
    void draw() override;
    
    auto perform_shuffle() {
        std::vector<uint8_t> values = { 1, 2, 3, 4, 5, 6, 7, 8 }; // initial order
        ofShuffle(values);
        shuffled_string_ = "shuffled values:";
        for (const auto v: values) {
            shuffled_string_ += " " + ofToUpper(ofToHex(v));
        }
    }
    
    auto seed (unsigned long & seed) {
        of::random::seed(seed);
        perform_shuffle();
        panel_.getControl("seed")->setTextColor(ofColor::green);
    }
    
    void reconstruct() {
        seed_ = 0;
        of::random::Engine::destruct();
        of::random::Engine::construct(); // from scratch
        perform_shuffle();
        panel_.getControl("seed")->setTextColor(ofColor::gray);
    }
    
    //  MARK: - ofBaseApp interface boilerplate:
    //  void update() override;
    //	void keyPressed(int key) override;
    //	void keyReleased(int key) override;
    //	void mouseMoved(int x, int y ) override;
    //	void mouseDragged(int x, int y, int button) override;
    //	void mousePressed(int x, int y, int button) override;
    //	void mouseReleased(int x, int y, int button) override;
    //	void mouseEntered(int x, int y) override;
    //	void mouseExited(int x, int y) override;
    //	void windowResized(int w, int h) override;
    //	void dragEvent(ofDragInfo dragInfo) override;
    //	void gotMessage(ofMessage msg) override;
    
};
