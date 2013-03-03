/*
 * ofPanel.cpp
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#include "ofxPanel.h"
#include "ofGraphics.h"
#include "ofImage.h"

ofImage ofxPanel::loadIcon;
ofImage ofxPanel::saveIcon;

ofxPanel::ofxPanel()
:bGrabbed(false){}

ofxPanel::ofxPanel(const ofParameterGroup & parameters, string filename, float x, float y)
: ofxGuiGroup(parameters, filename, x, y)
, bGrabbed(false){
	this->registerMouseEvents();
}

ofxPanel * ofxPanel::setup(string collectionName, string filename, float x, float y){
	if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
		unsigned char loadIconData[] = {0x38,0x88,0xa,0x6,0x7e,0x60,0x50,0x11,0x1c};
		unsigned char saveIconData[] = {0xff,0x4a,0x95,0xea,0x15,0xa8,0x57,0xa9,0x7f};
		loadIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
		saveIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
		loadStencilFromHex(loadIcon, loadIconData);
		loadStencilFromHex(saveIcon, saveIconData);
	}
	return (ofxPanel*)ofxGuiGroup::setup(collectionName,filename,x,y);
}

ofxPanel * ofxPanel::setup(const ofParameterGroup & parameters, string filename, float x, float y){
	if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
		unsigned char loadIconData[] = {0x38,0x88,0xa,0x6,0x7e,0x60,0x50,0x11,0x1c};
		unsigned char saveIconData[] = {0xff,0x4a,0x95,0xea,0x15,0xa8,0x57,0xa9,0x7f};
		loadIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
		saveIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
		loadStencilFromHex(loadIcon, loadIconData);
		loadStencilFromHex(saveIcon, saveIconData);
	}
	return (ofxPanel*)ofxGuiGroup::setup(parameters,filename,x,y);
}

void ofxPanel::generateDraw(){
	border.clear();
	border.setStrokeColor(thisBorderColor);
	border.setStrokeWidth(1);
	border.setFilled(false);
	border.moveTo(b.x,b.y);
	border.lineTo(b.x+b.width+1,b.y);
	border.lineTo(b.x+b.width+1,b.y+b.height-spacingNextElement);
	border.lineTo(b.x,b.y+b.height-spacingNextElement);
	border.close();


	headerBg.clear();
	headerBg.setFillColor(ofColor(thisHeaderBackgroundColor,180));
	headerBg.setFilled(true);
	headerBg.moveTo(b.x,b.y+1);
	headerBg.lineTo(b.x+b.width,b.y+1);
	headerBg.lineTo(b.x+b.width,b.y+header+1);
	headerBg.lineTo(b.x,b.y+header+1);
	headerBg.close();

	int iconSpacing = 6;
	loadBox.x = b.x + b.width - (loadIcon.getWidth() + saveIcon.getWidth() + iconSpacing + textPadding);
	loadBox.y = b.y + header / 2 - loadIcon.getHeight() / 2;
	loadBox.width = loadIcon.getWidth();
	loadBox.height = loadIcon.getHeight();
	saveBox.set(loadBox);
	saveBox.x += loadIcon.getWidth() + iconSpacing;

	textMesh = font.getStringMesh(getName(), textPadding + b.x, header / 2 + 4 + b.y);
}

void ofxPanel::render(){
	border.draw();
	headerBg.draw();

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofColor c = ofGetStyle().color;
	ofSetColor(thisTextColor);
	font.getFontTexture().bind();

	textMesh.draw();
	font.getFontTexture().unbind();

	loadIcon.draw(loadBox.x, loadBox.y);
	saveIcon.draw(saveBox.x, saveBox.y);

	for(int i = 0; i < (int)collection.size(); i++){
		collection[i]->draw();
	}

	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

void ofxPanel::mouseReleased(ofMouseEventArgs & args){
    this->bGrabbed = false;
    ofxGuiGroup::mouseReleased(args);
    if(isGuiDrawing() && b.inside(ofPoint(args.x,args.y))){
    	ofEventMarkAttended();
    }
}

bool ofxPanel::setValue(float mx, float my, bool bCheck){

	if( !isGuiDrawing() ){
		bGrabbed = false;
		bGuiActive = false;
		return false;
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

			if(loadBox.inside(mx, my)) {
				loadFromFile(filename);
				ofNotifyEvent(loadPressedE,this);
				return true;
			}
			if(saveBox.inside(mx, my)) {
				saveToFile(filename);
				ofNotifyEvent(savePressedE,this);
				return true;
			}
		}
	} else if( bGrabbed ){
		setPosition(mx - grabPt.x,my - grabPt.y);
		return true;
	}
	return false;
}
