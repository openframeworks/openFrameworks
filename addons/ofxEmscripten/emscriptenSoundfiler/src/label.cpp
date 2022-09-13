#include "label.h"

label::label(){

}

label::~label() {

}

void label::setup(float sendXpos, float sendYpos, float sendWidth,  float sendHeight, std::string sendSymbol){
	xpos = sendXpos;
	ypos = sendYpos;
	width = sendWidth;
	height = sendHeight;
	symbol = sendSymbol;
}

void label::update(){

}

void label::draw(){
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(xpos, ypos);
	ofSetColor(255);
	ofDrawRectangle(0, 0, width, height);
	ofSetColor(0);
	ofDrawRectangle(2, 2, width - 4, height- 4);
	ofSetColor(255);
	ofDrawBitmapString(symbol, 5, 14);
	ofPopMatrix();
	ofPopStyle();
}
