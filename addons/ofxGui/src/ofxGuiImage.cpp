#include "ofxGuiImage.h"

ofxGuiImage * ofxGuiImage::setup(string _name, ofBaseDraws* _imgPtr, bool _showName, float width ) {
	name     = _name;
	showName = _showName;
	b.width  = width;
	b.height = _imgPtr->getHeight()*(width/_imgPtr->getWidth());
	imgPtr   = _imgPtr;
	if (showName)
	    b.height += defaultHeight;

	ofRegisterMouseEvents(this);
	return this;
}

ofxGuiImage * ofxGuiImage::setup(string _name, ofBaseDraws* _imgPtr, bool _showName, float width, float height ) {
	name     = _name;
	showName = _showName;
	b.width  = width;
	b.height = height;
	imgPtr   = _imgPtr;

	ofRegisterMouseEvents(this);
	return this;
}

void ofxGuiImage::mouseMoved(ofMouseEventArgs & args){
}

void ofxGuiImage::mousePressed(ofMouseEventArgs & args){
    setValue(args.x, args.y, true);
}

void ofxGuiImage::mouseDragged(ofMouseEventArgs & args){
}

void ofxGuiImage::mouseReleased(ofMouseEventArgs & args){
    setValue(args.x, args.y, false);
    bGuiActive = false;
}

void ofxGuiImage::saveToXml(ofxXmlSettings& xml) {
}

void ofxGuiImage::loadFromXml(ofxXmlSettings& xml) {
}

void ofxGuiImage::draw(){
	currentFrame = ofGetFrameNum();

	ofPushStyle();
	ofPushMatrix();

	ofSetColor(backgroundColor);
	ofRect(b);

	ofTranslate(b.x, b.y);

	if (imgPtr != NULL) {
        ofSetColor(ofColor::white);
        float h = b.height-2*imgBorder;
        if (showName)
            h -= defaultHeight;
	    imgPtr->draw(imgBorder, imgBorder, b.width-2*imgBorder, h);
	}

	if (showName) {
        ofSetColor(textColor);
        ofDrawBitmapString(name, textPadding, b.height-textPadding);
	}

	ofPopMatrix();
	ofPopStyle();
}

void ofxGuiImage::setValue(float mx, float my, bool bCheck){
    if( ofGetFrameNum() - currentFrame > 1 ){
        bGuiActive = false;
        return;
    }
    if( bCheck ){
        if( b.inside(mx, my) ){
            bGuiActive = true;
        }else{
            bGuiActive = false;
        }
    }
    if( bGuiActive ){
        value = !value;
    }
}
