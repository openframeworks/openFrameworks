#include "testApp.h"

void loadImages(string directory, vector<ofImage*>& images) {
	ofDirectory dir;
	dir.listDir(directory);
	images.resize(dir.size());
	for(int i = 0; i < dir.size(); i++) {
		images[i] = new ofImage();
		images[i]->loadImage(dir.getFile(i));
	}
}

void drawImages(vector<ofImage*>& images) {
	ofPushMatrix();
	for(int i = 0; i < images.size(); i++) {
		images[i]->draw(0, 0);
		ofTranslate(images[i]->getWidth());
	}
	ofPopMatrix();
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
	drawImages(jpg8);
	ofTranslate(0, 40);
	drawImages(png8);
	ofTranslate(0, 40);
	drawImages(png16);
	ofTranslate(0, 40);
	drawImages(exrFloat);
}
