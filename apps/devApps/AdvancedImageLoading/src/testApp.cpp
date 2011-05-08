#include "testApp.h"

template <class T>
void loadImages(string directory, vector<ofImage_<T>*>& images) {
	ofDirectory dir;
	dir.listDir(directory);
	dir.sort();
	images.resize(dir.size());
	for(int i = 0; i < dir.size(); i++) {
		images[i] = new ofImage_<T>();
		images[i]->loadImage(dir.getFile(i));
	}
}

template <class T>
void drawImages(string name, vector<ofImage_<T>*>& images) {
	float offset = 0;
	for(int i = 0; i < images.size(); i++) {
		images[i]->draw(offset, 0);
		if(offset + 100 > ofGetWidth()) {
			offset = 0;
			ofTranslate(0, 32);
		}
		offset += images[i]->getWidth();
	}
	ofDrawBitmapString(name, offset + 10, 20);
}

void testApp::setup() {	
	loadImages("jpg8", jpg8);
	loadImages("png8", png8);
	loadImages("png16", png16);
	loadImages("exrFloat", exrFloat);
}

void testApp::update() {
}

void testApp::draw() {
	ofBackground(85);
	ofSetColor(170);
	for(int i = -ofGetHeight(); i < ofGetWidth(); i += 3) {
		ofLine(i, 0, i + ofGetWidth(), ofGetWidth());
	}

	ofSetColor(255);
	drawImages("jpg8", jpg8);
	ofTranslate(0, 40);
	drawImages("png8", png8);
	ofTranslate(0, 40);
	drawImages("png16", png16);
	ofTranslate(0, 40);
	drawImages("exrFloat", exrFloat);
}
