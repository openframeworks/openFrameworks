#include "ofxBaseGui.h"

const ofColor
ofxBaseGui::headerBackgroundColor(64),
ofxBaseGui::backgroundColor(0),
ofxBaseGui::textColor(255),
ofxBaseGui::fillColor(128);

const int ofxBaseGui::textPadding = 4;
const int ofxBaseGui::defaultWidth = 200;
const int ofxBaseGui::defaultHeight = 16;

ofxBaseGui::ofxBaseGui(){
	bGuiActive = false;
	currentFrame = 0;
}

ofxBaseGui::~ofxBaseGui(){
	ofUnregisterMouseEvents(this);
}

void ofxBaseGui::saveToFile(string filename) {
	ofxXmlSettings xml;
	xml.loadFile(filename);
	saveToXml(xml);
	xml.saveFile(filename);
}

void ofxBaseGui::loadFromFile(string filename) {
	ofxXmlSettings xml;
	xml.loadFile(filename);
	loadFromXml(xml);
}

string ofxBaseGui::getName(){
	return name;
}

void ofxBaseGui::setName(string _name){
	name = _name;
}

void ofxBaseGui::setPosition(ofPoint p){
	setPosition(p.x,p.y);
}

void ofxBaseGui::setPosition(float x, float y){
	b.x = x;
	b.y = y;
}

void ofxBaseGui::setSize(float w, float h){
	b.width = w;
	b.height = h;
}

void ofxBaseGui::setShape(ofRectangle r){
	b = r;
}

void ofxBaseGui::setShape(float x, float y, float w, float h){
	b.set(x,y,w,h);
}

ofPoint ofxBaseGui::getPosition(){
	return ofPoint(b.x,b.y);
}

ofRectangle ofxBaseGui::getShape(){
	return b;
}

float ofxBaseGui::getWidth(){
	return b.width;
}

float ofxBaseGui::getHeight(){
	return b.height;
}
