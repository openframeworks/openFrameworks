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

ofImage ofxPanel::loadIcon;
ofImage ofxPanel::saveIcon;

ofxPanel::ofxPanel()
:bGrabbed(false){}

ofxPanel::ofxPanel(const ofParameterGroup & parameters, const Config & groupConfig, const Config &itemConfig)
:ofxGuiGroup(parameters,groupConfig,itemConfig)
,bGrabbed(false){
	if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
		loadIcons();
	}
	registerMouseEvents();
	setNeedsRedraw();
}

ofxPanel::~ofxPanel(){
	//
}

ofxPanel & ofxPanel::setup(const Config & config){
	if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
		loadIcons();
	}
	return (ofxPanel&)ofxGuiGroup::setup(config);
}

ofxPanel & ofxPanel::setup(const std::string& collectionName, const Config & config){
	setName(collectionName);
	return setup(config);
}

ofxPanel & ofxPanel::setup(const ofParameterGroup & parameters, const Config & groupConfig, const Config &itemConfig){
	if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
		loadIcons();
	}
	return (ofxPanel&)ofxGuiGroup::setup(parameters, groupConfig, itemConfig);
}

ofxPanel & ofxPanel::setup(const std::string& collectionName, const std::string& filename, float x, float y){
	ofxPanel::Config config;
	config.filename = filename;
	config.shape.x = x;
	config.shape.y = y;
	return setup(collectionName, config);
}

ofxPanel & ofxPanel::setup(const ofParameterGroup & parameters, const std::string& filename, float x, float y){
	ofxPanel::Config config;
	config.filename = filename;
	config.shape.x = x;
	config.shape.y = y;
	return setup(parameters, config);
}

void ofxPanel::loadIcons(){
	unsigned char loadIconData[] = {0x38,0x88,0xa,0x6,0x7e,0x60,0x50,0x11,0x1c};
	unsigned char saveIconData[] = {0xff,0x4a,0x95,0xea,0x15,0xa8,0x57,0xa9,0x7f};
	loadIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
	saveIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
	loadStencilFromHex(loadIcon, loadIconData);
	loadStencilFromHex(saveIcon, saveIconData);

	loadIcon.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
	saveIcon.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
}

void ofxPanel::generateDraw(){
	border.clear();
	border.setFillColor(thisBorderColor);
	border.setFilled(true);
	border.rectangle(b.x,b.y,b.width+1,b.height);

	if(bShowHeader){
		generateDrawHeader();
	}
}

void ofxPanel::generateDrawHeader(){
	headerBg.clear();
	headerBg.setFillColor(thisHeaderBackgroundColor);
	headerBg.setFilled(true);
	headerBg.rectangle(b.x,b.y+1,b.width,header);

	float iconHeight = header*.5;
	float iconWidth = loadIcon.getWidth()/loadIcon.getHeight()*iconHeight;
	int iconSpacing = iconWidth*.5;

	loadBox.x = b.getMaxX() - (iconWidth * 2 + iconSpacing + textPadding);
	loadBox.y = b.y + header / 2. - iconHeight / 2.;
	loadBox.width = iconWidth;
	loadBox.height = iconHeight;
	saveBox.set(loadBox);
	saveBox.x += iconWidth + iconSpacing;

	if(bShowName){
		textMesh = getTextMesh(getName(), textPadding + b.x, header / 2 + 4 + b.y);
	}
}

void ofxPanel::render(){
	border.draw();
	if(bShowHeader){
		headerBg.draw();
	}

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofColor c = ofGetStyle().color;

	if(bShowHeader){
		renderHeader();
	}

	for(std::size_t i = 0; i < collection.size(); i++){
		collection[i]->draw();
	}

	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

void ofxPanel::renderHeader() {
	ofSetColor(thisTextColor);

	if(bShowName){
		bindFontTexture();
		textMesh.draw();
		unbindFontTexture();
	}

	bool texHackEnabled = ofIsTextureEdgeHackEnabled();
	ofDisableTextureEdgeHack();
	loadIcon.draw(loadBox);
	saveIcon.draw(saveBox);
	if(texHackEnabled){
		ofEnableTextureEdgeHack();
	}
}

bool ofxPanel::mouseReleased(ofMouseEventArgs & args){
	this->bGrabbed = false;
	if(ofxGuiGroup::mouseReleased(args)) return true;
	if(isGuiDrawing() && b.inside(ofPoint(args.x,args.y))){
		return true;
	}else{
		return false;
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

			if(bShowHeader){
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
		}
	} else if( bGrabbed ){
		setPosition(mx - grabPt.x,my - grabPt.y);
		return true;
	}
	return false;
}

void ofxPanel::sizeChangedCB(){
	float x = b.x;
	float y = b.y + spacingFirstElement;
	if(bShowHeader){
		y += header;
	}

	if(layout == ofxBaseGui::Vertical){
		for(auto & e: collection){
			e->setPosition(e->getPosition().x,y + spacing);
			e->sizeChangedE.disable();
			e->setSize(b.width-1, e->getHeight());
			e->sizeChangedE.enable();
			y += e->getHeight()+spacing;
		}
		b.height = y - b.y;
	}else{
		float max_h = 0;
		for(auto & e: collection){
			e->setPosition(x,y + spacing);
			x += e->getWidth() + spacing;
			if(max_h < e->getHeight()){
				max_h = e->getHeight();
			}
		}
		y += max_h+spacing;
		b.width = x - b.x;
		b.height = y - b.y;
	}
	sizeChangedE.notify(this);
	setNeedsRedraw();
}

void ofxPanel::setSize(float w, float h){
	ofxBaseGui::setSize(w,h);
}

void ofxPanel::setShape(ofRectangle r){
	ofxBaseGui::setShape(r);
}

void ofxPanel::setShape(float x, float y, float w, float h){
	ofxBaseGui::setShape(x,y,w,h);
}
