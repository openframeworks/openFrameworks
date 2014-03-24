#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	cam.initGrabber(320, 240);
    shader.load("shader");
    scaleFactor = 1. / 10; // could dynamically calculate this from flow3
    needToReset = false;
    
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	stepSize = 1;
	ySteps = 1 + (cam.getHeight() / stepSize);
	xSteps = 1 + (cam.getWidth() / stepSize);
	for(int y = 0; y < ySteps; y++) {
		for(int x = 0; x < xSteps; x++) {
            ofVec2f pos(x, y);
            pos *= stepSize;
			mesh.addVertex(pos);
			mesh.addTexCoord(pos);
		}
	}
	for(int y = 0; y + 1 < ySteps; y++) {
		for(int x = 0; x + 1 < xSteps; x++) {
			int nw = y * xSteps + x;
			int ne = nw + 1;
			int sw = nw + xSteps;
			int se = sw + 1;
			mesh.addIndex(nw);
			mesh.addIndex(ne);
			mesh.addIndex(se);
			mesh.addIndex(nw);
			mesh.addIndex(se);
			mesh.addIndex(sw);
		}
	}
}

void duplicateFirstChannel(Mat& twoChannel, Mat& threeChannel) {
    vector<Mat> each;
    split(twoChannel, each);
    each.push_back(each[0]);
    merge(each, threeChannel);
}

void testApp::update() {
    strength = ofMap(mouseX, 0, ofGetWidth(), -4, 4);
    learningRate = ofMap(mouseY, 0, ofGetHeight(), 0, 1, true);
    blurAmount = 7;
    
	cam.update();
	if(cam.isFrameNew()) {
		flow.setWindowSize(8);
		flow.calcOpticalFlow(cam);
        duplicateFirstChannel(flow.getFlow(), flow3);
        flow3 *= scaleFactor;
        flow3 += cv::Scalar_<float>(.5, .5, 0);
        blur(flow3, blurAmount);
        int w = flow3.cols, h = flow3.rows;
        
        if(needToReset || accumulator.size() != flow3.size()) {
			needToReset = false;
			copy(flow3, accumulator);
		}
		cv::accumulateWeighted(flow3, accumulator, learningRate);
        
        flowTexture.loadData((float*) accumulator.ptr(), w, h, GL_RGB);
	}
}

void testApp::draw() {
    ofSetupScreenOrtho(ofGetWidth(), ofGetHeight(), -10 / scaleFactor, +10 / scaleFactor);
    ofEnableDepthTest();
    float scale = ofGetWidth() / cam.getWidth();
    ofScale(scale, scale);
    if(flowTexture.isAllocated()) {
        ofBackground(0);
        ofSetColor(255);
        shader.begin();
        shader.setUniformTexture("source", cam.getTextureReference(), 1);
        shader.setUniformTexture("flow", flowTexture, 0);
        shader.setUniform1f("strength", strength);
        shader.setUniform1f("scaleFactor", scaleFactor);
        mesh.drawFaces();
//        mesh.drawWireframe();
        shader.end();
        ofSetColor(255, 128);
//        drawMat(flow3, 0, 0);
//        drawMat(accumulator, 0, 0);
    }
}