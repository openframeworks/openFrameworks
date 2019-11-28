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

ofxPanel::ofxPanel()
:bGrabbed(false){}

ofxPanel::ofxPanel(const ofParameterGroup & parameters, const std::string& filename, float x, float y)
: ofxGuiGroup(parameters, filename, x, y)
, bGrabbed(false){
	loadIcons();
	registerMouseEvents();
	setNeedsRedraw();
}

ofxPanel::~ofxPanel(){
	//
}

ofxPanel * ofxPanel::setup(const std::string& collectionName, const std::string& filename, float x, float y){
	loadIcons();
	registerMouseEvents();
	return (ofxPanel*)ofxGuiGroup::setup(collectionName,filename,x,y);
}

ofxPanel * ofxPanel::setup(const ofParameterGroup & parameters, const std::string& filename, float x, float y){
	loadIcons();
	registerMouseEvents();
	return (ofxPanel*)ofxGuiGroup::setup(parameters,filename,x,y);
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
	ofxGuiGroup::generateDraw();
	if(bHeaderEnabled){
		float iconHeight = headerRect.height*.5;
		float iconWidth = loadIcon.getWidth()/loadIcon.getHeight()*iconHeight;
		int iconSpacing = iconWidth*.5;
	
		loadBox.x = minimizeRect.getX() - (iconWidth + iconSpacing) * 2;
		loadBox.y = headerRect.y + headerRect.height / 2. - iconHeight / 2.;
		loadBox.width = iconWidth;
		loadBox.height = iconHeight;
		saveBox.set(loadBox);
		saveBox.x += iconWidth + iconSpacing;
	}
}

void ofxPanel::render(){
	ofxGuiGroup::render();
	
	if(bHeaderEnabled){
		ofBlendMode blendMode = ofGetStyle().blendingMode;
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableAlphaBlending();
		}
		ofColor c = ofGetStyle().color;
		ofSetColor(thisTextColor);
				
		bool texHackEnabled = ofIsTextureEdgeHackEnabled();
		ofDisableTextureEdgeHack();
		loadIcon.draw(loadBox);
		saveIcon.draw(saveBox);
		if(texHackEnabled){
			ofEnableTextureEdgeHack();
		}
		ofSetColor(c);
		
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableBlendMode(blendMode);
		}
	}
}
bool ofxPanel::mousePressed(ofMouseEventArgs & args){
	if(!ofxGuiGroup::mousePressed(args)){
		//this is to avoid keeping dragging when more than one ofxPanel instance is present
		this->bGrabbed = false;
		return false;
	}
	return true;
}
bool ofxPanel::mouseReleased(ofMouseEventArgs & args){
    this->bGrabbed = false;
	return ofxGuiGroup::mouseReleased(args);
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
			if( bHeaderEnabled){
				if(loadBox.inside(mx, my)) {
					if(!ofNotifyEvent(loadPressedE,this)){
						loadFromFile(filename);
					}
					return true;
				}
				if(saveBox.inside(mx, my)) {
					if(!ofNotifyEvent(savePressedE,this)){
						saveToFile(filename);
					}
					return true;
				}
				if(minimizeRect.inside(mx, my)){
					if(!minimized){
						minimize();
					}else{
						maximize();
					}
					return true;
				}
				
				if(headerRect.inside(mx, my)){
					bGrabbed = true;
					grabPt = {mx-b.x, my-b.y, 0};
					return true;
				}
			} else{
				bGrabbed = false;
			}
		}
	} else if( bGrabbed ){
		setPosition(mx - grabPt.x,my - grabPt.y);
		return true;
	}
	return false;
}
