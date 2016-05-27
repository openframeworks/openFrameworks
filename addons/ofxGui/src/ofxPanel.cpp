/*
 * ofPanel.cpp
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#include "ofxPanel.h"
#include "ofGraphics.h"
#include "ofImage.h"
using namespace std;

ofImage ofxPanelHeader::loadIcon;
ofImage ofxPanelHeader::saveIcon;

ofxPanelHeader::ofxPanelHeader():ofxGuiGroupHeader(){
	if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
		loadIcons();
	}
	registerMouseEvents();
	this->setDraggable(true);
}

ofxPanelHeader::ofxPanelHeader(const ofJson& config):ofxPanelHeader(){
	_setConfig(config);
}

void ofxPanelHeader::loadIcons(){
	unsigned char loadIconData[] = {0x38,0x88,0xa,0x6,0x7e,0x60,0x50,0x11,0x1c};
	unsigned char saveIconData[] = {0xff,0x4a,0x95,0xea,0x15,0xa8,0x57,0xa9,0x7f};
	loadIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
	saveIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
	loadStencilFromHex(loadIcon, loadIconData);
	loadStencilFromHex(saveIcon, saveIconData);

	loadIcon.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
	saveIcon.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
}

void ofxPanelHeader::generateDraw(){
	ofxBaseGui::generateDraw();

	float iconHeight = getHeight()*.5;
	float iconWidth = loadIcon.getWidth()/loadIcon.getHeight()*iconHeight;
	int iconSpacing = iconWidth*.5;

	loadBox.x = getWidth() - (iconWidth * 2 + iconSpacing + textPadding);
	loadBox.y = getHeight() / 2. - iconHeight / 2.;
	loadBox.width = iconWidth;
	loadBox.height = iconHeight;
	saveBox.set(loadBox);
	saveBox.x += iconWidth + iconSpacing;

	ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
	if(_parent){
		if(_parent->getShowName()){
			textMesh = getTextMesh(_parent->getName(), textPadding, getHeight() / 2 + 4);
		}
	}
}

void ofxPanelHeader::render() {

	ofxBaseGui::render();

	ofSetColor(textColor);

	ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
	if(_parent){
		if(_parent->getShowName()){
			bindFontTexture();
			textMesh.draw();
			unbindFontTexture();
		}
	}

	bool texHackEnabled = ofIsTextureEdgeHackEnabled();
	ofDisableTextureEdgeHack();
	loadIcon.draw(loadBox);
	saveIcon.draw(saveBox);
	if(texHackEnabled){
		ofEnableTextureEdgeHack();
	}
}

bool ofxPanelHeader::mousePressed(ofMouseEventArgs & args){

	if(!isHidden()){
		ofPoint pos = screenToLocal(ofPoint(args.x, args.y));
		if(loadBox.inside(pos)){
			ofNotifyEvent(loadPressedE,this);
			return true;
		}
		if(saveBox.inside(pos)) {
			ofNotifyEvent(savePressedE,this);
			return true;
		}

	}

	return ofxBaseGui::mousePressed(args);

}


/*
 * Panel
 */

ofxPanel::ofxPanel(const string &collectionName)
	:ofxGuiGroup(){

	parameters.setName(collectionName);
	setup();
	clear();

}

ofxPanel::ofxPanel(const string &collectionName, const ofJson & config)
	:ofxPanel(collectionName){

	_setConfig(config);

}

ofxPanel::ofxPanel(const ofParameterGroup & parameters, const ofJson & config)
:ofxPanel(parameters.getName()){

	addParametersFrom(parameters);
	_setConfig(config);

}

ofxPanel::ofxPanel(const std::string& collectionName, const std::string& filename, float x, float y)
	:ofxPanel(collectionName){

	// TODO set filename
	setPosition(x,y);

}

ofxPanel::ofxPanel(const ofParameterGroup & parameters, const std::string& filename, float x, float y)
	:ofxPanel(parameters.getName()){

	addParametersFrom(parameters);
//	config.filename = filename;
	// TODO set filename
	setPosition(x,y);

}

ofxPanel::~ofxPanel(){
	ofRemoveListener(header->move, this, &ofxPanel::onHeaderMove);
	ofRemoveListener(((ofxPanelHeader*)header)->loadPressedE, this, &ofxPanel::onLoadPressed);
	ofRemoveListener(((ofxPanelHeader*)header)->savePressedE, this, &ofxPanel::onSavePressed);
}

void ofxPanel::setup(){
	if(header){
		removeChild(header);
	}
	header = add<ofxPanelHeader>();
	header->setMargin(0);
	header->setHeight(headerHeight);
	header->setBackgroundColor(headerBackgroundColor);
	header->setBorderWidth(0);
	ofAddListener(header->move, this, &ofxPanel::onHeaderMove);
	ofAddListener(((ofxPanelHeader*)header)->loadPressedE, this, &ofxPanel::onLoadPressed);
	ofAddListener(((ofxPanelHeader*)header)->savePressedE, this, &ofxPanel::onSavePressed);
}

void ofxPanel::onHeaderMove(MoveEventArgs &args){
	ofPoint screenHeaderPos = localToScreen(args.position());
	ofPoint screenThisPos = getScreenPosition();
	ofPoint diff = screenHeaderPos-screenThisPos;

//	ofRectangle newshape = getShape();
//	newshape.setPosition(getPosition()+diff);
//	if(parent()){
//		if(newshape.x < parent()->getShape().getLeft()){
//			newshape.x = parent()->getShape().getLeft();
//		}
//		if(newshape.x + newshape.width > parent()->getShape().getRight()){
//			newshape.x = parent()->getShape().getRight() - newshape.width;
//		}
//		if(newshape.y < parent()->getShape().getTop()){
//			newshape.y = parent()->getShape().getTop();
//		}
//		if(newshape.y + newshape.height > parent()->getShape().getBottom()){
//			newshape.y = parent()->getShape().getBottom() - newshape.height;
//		}
//	}

	setPosition(getPosition()+diff);
}

void ofxPanel::onLoadPressed(){
	loadFromFile(filename);
}

void ofxPanel::onSavePressed(){
	saveToFile(filename);
}
