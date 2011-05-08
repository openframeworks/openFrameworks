#include "testApp.h"

void testApp::setup() {	
	ofDirectory dir;
	
	dir.listDir("png8");
	imgPng8.resize(dir.size());
	for(int i = 0; i < dir.size(); i++) {
		imgPng8[i] = new ofImage();
		imgPng8[i]->loadImage(dir.getFile(i));
	}
	
	dir.listDir("png16");
	imgPng16.resize(dir.size());
	for(int i = 0; i < dir.size(); i++) {
		imgPng16[i] = new ofImage();
		imgPng16[i]->loadImage(dir.getFile(i));
	}
	
	dir.listDir("exrFloat");
	imgExrFloat.resize(dir.size());
	for(int i = 0; i < dir.size(); i++) {
		imgExrFloat[i] = new ofImage();
		imgExrFloat[i]->loadImage(dir.getFile(i));
	}
}

void testApp::update() {
}

void testApp::draw() {
	ofPushMatrix();
	for(int i = 0; i < imgPng8.size(); i++) {
		imgPng8[i]->draw(0, 0);
		ofTranslate(imgPng8[i]->getWidth());
	}
	ofPopMatrix();
	
	ofTranslate(0, 40);
	ofPushMatrix();
	for(int i = 0; i < imgPng16.size(); i++) {
		imgPng16[i]->draw(0, 0);
		ofTranslate(imgPng16[i]->getWidth());
	}
	ofPopMatrix();
	
	ofTranslate(0, 40);
	ofPushMatrix();
	for(int i = 0; i < imgExrFloat.size(); i++) {
		imgExrFloat[i]->draw(0, 0);
		ofTranslate(imgExrFloat[i]->getWidth());
	}
	ofPopMatrix();
}
