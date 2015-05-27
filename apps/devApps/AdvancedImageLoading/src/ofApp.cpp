#include "ofApp.h"

template <class T>
void loadImages(string directory, vector<ofImage_<T>*>& images) {
	ofDirectory dir;
	dir.listDir(directory);
	dir.sort();
	images.resize(dir.size());
	for(int i = 0; i < (int)dir.size(); i++) {
		images[i] = new ofImage_<T>();
		images[i]->loadImage(dir.getFile(i));
	}
}

template <class T>
void resaveImages(string directory) {
	ofDirectory dir;
	dir.listDir(directory);
	dir.sort();
	for(int i = 0; i < (int)dir.size(); i++) {
		T img;
		ofLogVerbose() << "resaving " << dir.getPath(i);
		img.loadImage(dir.getFile(i));
		img.saveImage("resaved/" + dir.getPath(i));
	}
}

template <class T>
void drawImages(string name, vector<ofImage_<T>*>& images) {
	float offset = 0;
	for(int i = 0; i < (int)images.size(); i++) {
		images[i]->draw(offset, 0);
		offset += images[i]->getWidth();
		if(offset > ofGetWidth()) {
			offset = 0;
			ofTranslate(0, 32);
		}
	}
	ofDrawBitmapString(name, offset + 10, 20);
}

void ofApp::setup() {	
	ofSetFrameRate(12);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	loadImages("gif8", gif8);
	resaveImages<ofImage>("gif8");
	
	loadImages("png8", png8);
	resaveImages<ofImage>("png8");
	
	loadImages("jpg8", jpg8);
	resaveImages<ofImage>("jpg8");

	loadImages("png16", png16);
	resaveImages<ofShortImage>("png16");
	
	loadImages("exrFloat", exrFloat);
	resaveImages<ofFloatImage>("exrFloat");
	img8 = *exrFloat[0];
	img16 = *exrFloat[0];
	imgf = *exrFloat[0];
}

void ofApp::update() {
}

void ofApp::draw() {
	ofBackground(85);
	ofSetColor(170);
	for(int i = -ofGetHeight(); i < ofGetWidth(); i += 3) {
		ofLine(i, 0, i + ofGetHeight(), ofGetHeight());
	}

	ofSetColor(255);
	drawImages("gif8", gif8);
	ofTranslate(0, 40);
	drawImages("png8", png8);
	ofTranslate(0, 40);
	drawImages("jpg8", jpg8);
	ofTranslate(0, 40);
	drawImages("png16", png16);
	ofTranslate(0, 40);
	drawImages("exrFloat", exrFloat);
	ofTranslate(exrFloat[0]->getWidth()+10, 0);
	img8.draw(0, 0, 128, 128);
	img16.draw(0, 128, 128, 128);
	imgf.draw(0, 256, 128, 128);
}
