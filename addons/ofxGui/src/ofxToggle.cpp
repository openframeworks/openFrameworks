#include "ofxToggle.h"

ofxToggle::ofxToggle(string toggleName, ofxParameter<bool> _bVal, float width, float height){
	setup(toggleName,_bVal,defaultWidth,height);
}

ofxToggle * ofxToggle::setup(string toggleName, ofxParameter<bool> _bVal, float width, float height){
	name = toggleName;
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	currentFrame = 0;
	bGuiActive = false;
	value = _bVal;
	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	ofRegisterMouseEvents(this);
	return this;
}


void ofxToggle::mouseMoved(ofMouseEventArgs & args){
}

void ofxToggle::mousePressed(ofMouseEventArgs & args){
	setValue(args.x, args.y, true);
}

void ofxToggle::mouseDragged(ofMouseEventArgs & args){
}

void ofxToggle::mouseReleased(ofMouseEventArgs & args){
	bGuiActive = false;
}

void ofxToggle::saveToXml(ofxXmlSettings& xml) {
	string xmlName = name;
	ofStringReplace(xmlName," ","_");
	xml.setValue(xmlName, value);
}

void ofxToggle::loadFromXml(ofxXmlSettings& xml) {
	string xmlName = name;
	ofStringReplace(xmlName," ","_");
	value = xml.getValue(xmlName, value);
}

void ofxToggle::draw(){
	currentFrame = ofGetFrameNum();

	ofPushStyle();
	ofPushMatrix();

	ofSetColor(backgroundColor);
	ofRect(b);

	ofTranslate(b.x, b.y);

	ofFill();
	ofSetColor(fillColor);
	ofRect(checkboxRect);

	if( value ){
		ofSetColor(textColor);
		ofLine(checkboxRect.x, checkboxRect.y, checkboxRect.x + checkboxRect.width, checkboxRect.y + checkboxRect.height);
		ofLine(checkboxRect.x, checkboxRect.y+ checkboxRect.height, checkboxRect.x + checkboxRect.width, checkboxRect.y);
	} else {
		ofSetColor(backgroundColor);
		ofRect(checkboxRect.x + 1, checkboxRect.y + 1, checkboxRect.width - 2, checkboxRect.height - 2);
	}

	ofSetColor(textColor);
	ofTranslate(0, b.height / 2 + 4);
	ofDrawBitmapString(name, textPadding + checkboxRect.width, 0);
	//string valStr = value ? "true" : "false";
	//ofDrawBitmapString(valStr, b.width - textPadding - valStr.length() * 8, 0);

	ofPopMatrix();
	ofPopStyle();
}

bool ofxToggle::operator=(bool v){
	value = v;
	return v;
}

ofxToggle::operator bool & (){
	return value;
}

void ofxToggle::setValue(float mx, float my, bool bCheck){

	if( ofGetFrameNum() - currentFrame > 1 ){
		bGuiActive = false;
		return;
	}
	if( bCheck ){
		ofRectangle checkRect = checkboxRect;
		checkRect.x += b.x;
		checkRect.y += b.y;

		if( checkRect.inside(mx, my) ){
			bGuiActive = true;
		}else{
			bGuiActive = false;

		}
	}
	if( bGuiActive ){
		value = !value;
	}
}
