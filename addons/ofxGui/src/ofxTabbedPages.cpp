#include "ofxTabbedPages.h"
#include "ofGraphics.h"
#include "ofxMinimalToggle.h"
using namespace std;

ofxTabbedPages::ofxTabbedPages(const string &collectionName, const ofJson &config) :
	ofxGuiGroup(collectionName, config){

	setup(config);

}

ofxTabbedPages::ofxTabbedPages(string collectionName, string filename, float x, float y)
	:ofxTabbedPages(collectionName){

	this->filename = filename;
	setPosition(x,y);

}

ofxTabbedPages::~ofxTabbedPages(){
	//
}

void ofxTabbedPages::setup(const ofJson &){

	tabWidth.set("tab width", 0);
	tabHeight.set("tab height", defaultHeight);
	tabs = nullptr;
	pages = nullptr;

	tabs = add<ofxGuiGroup>();
	tabs->setup("tabs");
	tabs->setShowHeader(false);
	tabs->setExclusiveToggles(true);
	// TODO set alignment
//	tabs->setAlignment(ofxBaseGui::Horizontal);
	tabs->setBorderColor(ofColor(0, 0, 0, 0));
	tabs->setDefaultBackgroundColor(borderColor);
	tabs->setDefaultBorderColor(borderColor);
	tabs->setDefaultFillColor(backgroundColor);
	updateContentShapes();
	pages = add<ofxGuiGroup>();
	pages->setup("pages");
	pages->setShowHeader(false);

}

void ofxTabbedPages::clear(){
	tabs->clear();
	pages->clear();

	// TODO may not be needed
//	b.height = spacing + spacingNextElement;
//	if(bShowHeader){
//		b.height += header;
//	}

}

//void ofxTabbedPages::generateDraw(){

//	updateContentShapes();

//	bg.clear();
//	bg.setFillColor(backgroundColor);
//	bg.setFilled(true);
//	bg.rectangle(pages->getShape().x, pages->getShape().y, pages->getShape().width + 1, pages->getShape().height);

//	border.clear();
//	border.setStrokeColor(borderColor);
//	border.setStrokeWidth(1);
//	border.setFilled(false);
//	border.rectangle(pages->getShape().x, pages->getShape().y + 1, pages->getShape().width + 1, pages->getShape().height);

//	if(showHeader){
//		generateDrawHeader();
//	}
//}

// TODO is this needed?


//void ofxTabbedPages::render(){

//	/*
//	 * TODO: call the first part when a tab toggle is clicked
//	 * in this case call needsRedraw
//	 * move the second part to generateDraw
//	 */
//	for(int i = 1; i < (int)collection.size(); i++){
//		if(parameters_tabs.at(i - 1).get()){
//			activePage = collection[i];
//			activeToggle = tabs.getControl(i - 1);
//		}
//	}

//	tabBorder.clear();
//	if(activePage){
//		tabBorder.setStrokeWidth(3);
//		tabBorder.setFilled(false);
//		tabBorder.setStrokeColor(thisBackgroundColor);
//		tabBorder.moveTo(activeToggle->getShape().getBottomLeft() - ofPoint(-1, 1));
//		tabBorder.lineTo(activeToggle->getShape().getBottomRight() - ofPoint(1, 1));

//		this->setBorderColor(activeToggle->getBorderColor());
//	}
//	/*
//	 * end TODO
//	 */

//	bg.draw();
//	tabs.draw();
//	border.draw();
//	tabBorder.draw();
//	if(bShowHeader){
//		headerBg.draw();
//	}

//	ofBlendMode blendMode = ofGetStyle().blendingMode;
//	if(blendMode != OF_BLENDMODE_ALPHA){
//		ofEnableAlphaBlending();
//	}
//	ofColor c = ofGetStyle().color;

//	if(bShowHeader){
//		renderHeader();
//	}

//	if(activePage){
//		activePage->draw();
//	}

//	ofSetColor(c);
//	if(blendMode != OF_BLENDMODE_ALPHA){
//		ofEnableBlendMode(blendMode);
//	}

//}

void ofxTabbedPages::updateContentShapes(){
	// TODO needed?
//	ofPoint newpos = b.getPosition();
//	if(bShowHeader){
//		newpos.y += header + spacing;
//	}

//	tabShape.setPosition(newpos);
//	tabShape.setWidth(b.width);
//	tabShape.setHeight(tabHeight);

//	newpos.y += tabHeight;
//	pages->getShape().setPosition(newpos);
//	pages->getShape().setWidth(b.width - 1 /*border*/);
//	pages->getShape().setHeight(b.getBottom() - newpos.y - 1 /*border*/);
}

// TODO needed?

//void ofxTabbedPages::setSizeToElement(ofxBaseGui * element){
//	updateContentShapes();
//	if(element->getWidth() > pages->getShape().width){
//		b.width = element->getWidth() + 1 /*border*/;
//	}
//	if(element->getHeight() > pages->getShape().getHeight()){
//		b.height = element->getHeight() + tabHeight + 1 /*border*/;
//		if(bShowHeader){
//			b.height += spacing + header;
//		}
//	}
//	sizeChangedCB();
//}

//void ofxTabbedPages::sizeChangedCB(){
//	updateContentShapes();
//	tabs.setShape(tabShape);

//	for(int i = 1; i < (int)collection.size(); i++){
//		collection[i]->sizeChangedE.disable();
//		collection[i]->setShape(pages->getShape());
//		collection[i]->sizeChangedE.enable();
//	}
//	ofNotifyEvent(sizeChangedE,this);
//	setNeedsRedraw();
//}

void ofxTabbedPages::setActiveTab(int index){
	tabs->setActiveToggle(index);
	activeToggle = tabs->getControl(index);
}

int ofxTabbedPages::getActiveTabIndex(){
	return tabs->getActiveToggleIndex();
}

ofxGuiPage * ofxTabbedPages::getActiveTab(){
	return (ofxGuiPage *)pages->children().at(tabs->getActiveToggleIndex());
}

void ofxTabbedPages::setTabHeight(int h){
	tabHeight = h;
//	sizeChangedCB();
}

void ofxTabbedPages::setTabWidth(int w){
	tabWidth = w;
//	sizeChangedCB();
}

void ofxTabbedPages::setBorderColor(const ofColor &color){
	pages->setBorderColor(color);
	tabs->setDefaultBackgroundColor(color);
	tabs->setDefaultBorderColor(borderColor);
}

void ofxTabbedPages::setBackgroundColor(const ofColor &color){
	pages->setBackgroundColor(color);
	tabs->setDefaultFillColor(color);
}
