#include "ofxGuiImage.h"

ofxGuiImage * ofxGuiImage::setup(string _name, ofImage* _imgPtr, bool _showName, float width ) {
	name     = _name;
	showName = _showName;
	b.width  = width;
	b.height = _imgPtr->height*(width/_imgPtr->width);
	if (showName)
	    b.height += defaultHeight;
	imgPtr   = _imgPtr;
	return this;
}

ofxGuiImage * ofxGuiImage::setup(string _name, ofImage* _imgPtr, bool _showName, float width, float height ) {
	name     = _name;
	showName = _showName;
	b.width  = width;
	b.height = height;
	imgPtr   = _imgPtr;
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
	ofSetColor(ofColor::white);
	float h = b.height-2*imgBorder;
	if (showName)
	    h -= defaultHeight;
	imgPtr->draw(imgBorder, imgBorder, b.width-2*imgBorder, h);

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
