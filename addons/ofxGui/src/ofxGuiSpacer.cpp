#include "ofxGuiSpacer.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiSpacer::ofxGuiSpacer(const Config & config) :
	ofxBaseGui(config){
}

ofxGuiSpacer & ofxGuiSpacer::setup(float size, float x, float y){
	return setup("", size, x, y);
}

ofxGuiSpacer & ofxGuiSpacer::setup(string name, float size, float x, float y){
	this->setName(name);
	this->setPosition(x, y);
	spacing_size = size;
	return *this;
}

void ofxGuiSpacer::generateDraw(){
	bg.clear();
	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(b.x, b.y, b.width, b.height);
}

void ofxGuiSpacer::render(){
	bg.draw();
}

void ofxGuiSpacer::sizeChangedCB(){
	setNeedsRedraw();
}

ofAbstractParameter & ofxGuiSpacer::getParameter(){
	return parameter;
}
