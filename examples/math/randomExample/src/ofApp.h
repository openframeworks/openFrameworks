#pragma once

#include "ofMain.h"

struct Dist {
	std::vector<int> data_;
	std::array<size_t, 101> bins_;
	size_t max_ {0};
	
	std::string label_;
	
	Dist(std::string label): label_(label) {}
	
	auto clear() {
		data_.clear();
		bins_ = {0};
		max_ = 0;
	}
	
	auto push_back(size_t i) {
		data_.push_back(i);
	}
	
	auto compile() {
		for (auto & v: data_) {
			if (v < bins_.size()) {
				bins_[v] = bins_[v]+1;
				if (bins_[v] > max_) max_ = bins_[v];
			} else {
				ofLogNotice("ouf") << v;
			}
		}
	}
	
	auto draw(float x, float y, float w, float h) {
		ofPushStyle();
		ofPushMatrix();
		
		ofTranslate(x,y);
		ofSetColor(0,0,0);
		ofDrawRectangle(0,0,w,h);
		
		ofSetColor(255,255,255,255);
		
		ofPolyline line;
		int p = 0;
		auto fact = 100.0f/float(max_);
		line.addVertex(0,100-bins_[0]*fact);
		for (auto y: bins_) {
			line.lineTo(p++, 100-float(y)*fact);
		}
		line.draw();
		
		ofDrawBitmapStringHighlight(label_, w+5, h-5);
		
		ofPopStyle();
		ofPopMatrix();

	}
};

#include "ofxGui.h"

class ofApp : public ofBaseApp{

	ofxPanel panel_;
	ofParameter<float> uniform1_min_{"uniform1 min", 0, 0, 100};
	ofParameter<float> uniform1_max_{"uniform1 max", 100, 0, 100};
	ofParameter<float> normal1_mean_{"normal1 mean", 50, 0, 100};
	ofParameter<float> normal1_stddev_{"normal1 stddev", 10, 0, 50};
	ofParameter<float> gamma1_alpha_{"gamma1 alpha", 1, 0, 100};
	ofParameter<float> gamma1_beta_{"gamma1 beta", 12, 0, 50};
	ofParameter<float> poisson1_mean_{"poisson1 mean", 12, 0, 50};
	ofParameter<float> exponential1_lambda_{"exponential1 lambda", .1, 0.01, 0.5};

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
	
	Dist uniform1_ {"uniform"};
	Dist normal1_ {"normal"};
	Dist gamma1_ {"gamma"};
	Dist poisson1_ {"poisson"};
	Dist exponential1_ {"exponential"};
};
