#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	camera.initGrabber(320, 240);
	
	panel.setup(250, 800);
	
	panel.addPanel("Optical Flow");
	
	panel.addSlider("pyrScale", .5, 0, 1);
	panel.addSlider("levels", 4, 1, 8, true);
	panel.addSlider("winsize", 8, 4, 64, true);
	panel.addSlider("iterations", 2, 1, 8, true);
	panel.addSlider("polyN", 7, 5, 10, true);
	panel.addSlider("polySigma", 1.5, 1.1, 2);
	panel.addToggle("OPTFLOW_FARNEBACK_GAUSSIAN", false);
	
	panel.addToggle("useFarneback", true);
	panel.addSlider("winSize", 32, 4, 64, true);
	panel.addSlider("maxLevel", 3, 0, 8, true);
	
	panel.addSlider("maxFeatures", 200, 1, 1000);
	panel.addSlider("qualityLevel", 0.01, 0.001, .02);
	panel.addSlider("minDistance", 4, 1, 16);

	curFlow = &farneback;
}

void testApp::update(){
	camera.update();
	
	if(camera.isFrameNew()) {
		
		
		if(panel.getValueB("useFarneback")) {
			curFlow = &farneback;
			farneback.setPyramidScale( panel.getValueF("pyrScale") );
			farneback.setNumLevels( panel.getValueF("levels") );
			farneback.setWindowSize( panel.getValueF("winsize") );
			farneback.setNumIterations( panel.getValueF("iterations") );
			farneback.setPolyN( panel.getValueF("polyN") );
			farneback.setPolySigma( panel.getValueF("polySigma") );
			farneback.setUseGaussian(panel.getValueB("OPTFLOW_FARNEBACK_GAUSSIAN"));
			
		} else {
			curFlow = &pyrLk;
			pyrLk.setMaxFeatures( panel.getValueI("maxFeatures") );
			pyrLk.setQualityLevel( panel.getValueF("qualityLevel") );
			pyrLk.setMinDistance( panel.getValueF("minDistance") );			 	
			pyrLk.setWindowSize( panel.getValueI("winSize") );
			pyrLk.setMaxLevel( panel.getValueI("maxLevel") );
		}
		
		//check it out that that you can use Flow polymorphically
		curFlow->calcOpticalFlow(camera);
	}
}

void testApp::draw(){
	ofBackground(0);
		
	ofSetColor(255);
	camera.draw(400,100,640,480);
	curFlow->draw(400,100,640,480);
	
}