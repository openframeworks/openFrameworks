#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	
	ofDirectory dir;
	dir.allowExt("png");
	dir.open(".");
	dir.listDir();
	for(int i = 0; i < dir.size(); i++) {
		ofImage cur;
		cur.loadImage(dir.getName(i));
		cur.setImageType(OF_IMAGE_GRAYSCALE);
		input.push_back(cur);
		output.push_back(cur);
		canny.push_back(cur);
	}
	
	gui.setup();
	gui.addPanel("Settings");
	gui.addToggle("doFDoG", true);
	gui.addSlider("halfw", 4, 1, 8, true);
	gui.addSlider("smoothPasses", 2, 1, 4, true);
	gui.addSlider("sigma1", 0.68, 0.01, 2.0, false);
	gui.addSlider("sigma2", 6.0, 0.01, 10.0, false);
	gui.addSlider("tau", 0.974, 0.8, 1.0, false);
	gui.addSlider("black", -8, -255, 255, true);
	gui.addToggle("doThresh", true);
	gui.addSlider("thresh", 150, 0, 255, false);
	gui.addToggle("doThin", true);
	gui.addToggle("doCanny", true);
	gui.addSlider("cannyParam1", 400, 0, 1024, true);
	gui.addSlider("cannyParam2", 600, 0, 1024, true);
	gui.loadSettings("settings.xml");
}

void testApp::update(){
	bool doFDoG = gui.getValueB("doFDoG");
	bool doCanny = gui.getValueB("doCanny");
	bool doThin = gui.getValueB("doThin");
	bool doThresh = gui.getValueB("doThresh");
	int black = gui.getValueI("black");
	float sigma1 = gui.getValueF("sigma1");
	float sigma2 = gui.getValueF("sigma2");
	float tau = gui.getValueF("tau");
	float thresh = gui.getValueF("thresh");
	int halfw = gui.getValueI("halfw");
	int smoothPasses = gui.getValueI("smoothPasses");
	int cannyParam1 = gui.getValueI("cannyParam1");
	int cannyParam2 = gui.getValueI("cannyParam2");
	
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

void testApp::draw(){
	ofBackground(255);
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
