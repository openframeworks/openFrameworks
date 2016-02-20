#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    ofBackground(255);
	ofDirectory dir;
	dir.allowExt("png");
	dir.open(".");
	dir.listDir();
	for(int i = 0; i < dir.size(); i++) {
		ofImage cur;
		cur.load(dir.getName(i));
		cur.setImageType(OF_IMAGE_GRAYSCALE);
		input.push_back(cur);
		output.push_back(cur);
		canny.push_back(cur);
	}
	
	gui.setup();
    gui.add(doFDoG.set("doFDoG", true));
    gui.add(halfw.set("halfw", 4, 1, 8));
    gui.add(smoothPasses.set("smoothPasses", 2, 1, 4));
    gui.add(sigma1.set("sigma1", 0.68, 0.01, 2.0));
    gui.add(sigma2.set("sigma2", 6.0, 0.01, 10.0));
    gui.add(tau.set("tau", 0.974, 0.8, 1.0));
    gui.add(black.set("black", -8, -255, 255));
    gui.add(doThresh.set("doThresh", true));
    gui.add(thresh.set("thresh", 150, 0, 255));
    gui.add(doThin.set("doThin", true));
    gui.add(doCanny.set("doCanny", true));
    gui.add(cannyParam1.set("cannyParam1", 400, 0, 1024));
    gui.add(cannyParam2.set("cannyParam2", 600, 0, 1024));
}

void ofApp::update(){
	for(int i = 0; i < input.size(); i++) {
		if(doFDoG) {
			CLD(input[i], output[i], halfw, smoothPasses, sigma1, sigma2, tau, black);
			invert(output[i]);
			if(doThresh) {
				threshold(output[i], thresh);
			}
			if(doThin) {
				thin(output[i]);
			}
			output[i].update();
			if(doCanny) {
				Canny(input[i], canny[i], cannyParam1 * 2, cannyParam2 * 2, 5);
				canny[i].update();
			}
		}
	}
}

void ofApp::draw(){
    gui.draw();
    
	ofTranslate(300, 0);
	for(int i = 0; i < input.size(); i++) {
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		input[i].draw(i * 256, 0);
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		output[i].draw(i * 256, 0);
		
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		input[i].draw(i * 256, 256);
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		canny[i].draw(i * 256, 256);
	}
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofDrawBitmapStringHighlight("Coherent line drawing", 10, 20);
	ofDrawBitmapStringHighlight("Canny edge detection", 10, 256 + 20);
}
