/*
 * ofPanel.cpp
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#include "ofxPanel.h"

ofImage ofxPanel::loadIcon;
ofImage ofxPanel::saveIcon;

ofxPanel::ofxPanel(string collectionName, string filename, float x, float y) : ofxBaseGroup(collectionName, filename, x, y){
	this->registerMouseEvents();
}

ofxPanel::ofxPanel(const ofParameterGroup & parameters, string filename, float x, float y) : ofxBaseGroup(parameters, filename, x, y){
	this->registerMouseEvents();
}

void ofxPanel::draw(){
	if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
		unsigned char loadIconData[] = {0x38,0x88,0xa,0x6,0x7e,0x60,0x50,0x11,0x1c};
		unsigned char saveIconData[] = {0xff,0x4a,0x95,0xea,0x15,0xa8,0x57,0xa9,0x7f};
		loadIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
		saveIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
		loadStencilFromHex(loadIcon, loadIconData);
		loadStencilFromHex(saveIcon, saveIconData);
	}


	int iconSpacing = 6;
	loadBox.x = b.width - (loadIcon.getWidth() + saveIcon.getWidth() + iconSpacing + textPadding);
	loadBox.y = header / 2 - loadIcon.getHeight() / 2;
	loadBox.width = loadIcon.getWidth();
	loadBox.height = loadIcon.getHeight();
	saveBox.set(loadBox);
	saveBox.x += loadIcon.getWidth() + iconSpacing;

	ofPushStyle();
	ofPushMatrix();

	currentFrame = ofGetFrameNum();

	ofTranslate(b.x, b.y);

	ofFill();
	ofSetColor(headerBackgroundColor);
	ofRect(0, 0, b.width, header);

	ofSetColor(textColor);
	ofDrawBitmapString(name, textPadding, header / 2 + 4);

	ofPushMatrix();
	loadIcon.draw(loadBox.x, loadBox.y);
	saveIcon.draw(saveBox.x, saveBox.y);
	ofPopMatrix();

	for(int i = 0; i < (int)collection.size(); i++){
		collection[i]->draw();
	}

	ofPopMatrix();
	ofPopStyle();
}

void ofxPanel::mouseReleased(ofMouseEventArgs & args){
    this->bGrabbed = false;
    ofxBaseGroup::mouseReleased(args);
}

void ofxPanel::setValue(float mx, float my, bool bCheck){

	if( ofGetFrameNum() - currentFrame > 1 ){
		bGrabbed = false;
		bGuiActive = false;
		return;
	}
	if( bCheck ){
		if( b.inside(mx, my) ){
			bGuiActive = true;

			if( my > b.y && my <= b.y + header ){
				bGrabbed = true;
				grabPt.set(mx-b.x, my-b.y);
			} else{
				bGrabbed = false;
			}

			if(loadBox.inside(mx - b.x, my - b.y)) {
				loadFromFile(filename);
			}
			if(saveBox.inside(mx - b.x, my - b.y)) {
				saveToFile(filename);
			}
		}
	} else if( bGrabbed ){
		b.x = mx - grabPt.x;
		b.y = my - grabPt.y;
	}
}