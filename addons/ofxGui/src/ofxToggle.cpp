#include "ofxToggle.h"
#include "ofGraphics.h"

ofxToggle::ofxToggle(ofParameter<bool> _bVal, float width, float height){
	setup(_bVal,width,height);
}

ofxToggle * ofxToggle::setup(ofParameter<bool> _bVal, float width, float height){
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

ofxToggle * ofxToggle::setup(string toggleName, bool _bVal, float width, float height){
	value.set(toggleName,_bVal);
	return setup(value,width,height);
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

void ofxToggle::draw(){
	currentFrame = ofGetFrameNum();

	bool currentFill = ofGetStyle().bFill;
	ofColor c = ofGetStyle().color;
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
	ofDrawBitmapString(getName(), textPadding + checkboxRect.width, 0);

	ofPopMatrix();
	ofSetColor(c);
	if(!currentFill) ofNoFill();
}

bool ofxToggle::operator=(bool v){
	value = v;
	return v;
}

ofxToggle::operator const bool & (){
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

ofAbstractParameter & ofxToggle::getParameter(){
	return value;
}
