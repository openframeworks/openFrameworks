#include "ofxTabbedPages.h"
#include "ofGraphics.h"
#include "ofxMinimalToggle.h"
using namespace std;

ofxTabbedPages::ofxTabbedPages() :
	ofxGuiPage(){
}

ofxTabbedPages::~ofxTabbedPages(){
	//
}

ofxTabbedPages & ofxTabbedPages::setup(string collectionName, string filename, float x, float y){
	ofxGuiPage::setup(collectionName, filename, x, y);
	tabs.setup("tabs");
	tabs.setShowHeader(false);
	tabs.setExclusiveToggles(true);
	tabs.setLayout(ofxBaseGui::Horizontal);
	tabs.setBorderColor(ofColor(0, 0, 0, 0));
	tabs.setDefaultBackgroundColor(thisBorderColor);
	tabs.setDefaultBorderColor(thisBorderColor);
	tabs.setDefaultFillColor(thisBackgroundColor);
	tabHeight = 20;
	tabWidth = 0;
	sizeChangedCB();
	collection.push_back(&tabs);
	tabs.unregisterMouseEvents();
	parameters.add(tabs.getParameter());
	updateContentShapes();
	return *this;
}

void ofxTabbedPages::add(ofxGuiGroup *element){
	collection.push_back(element);

	parameters_tabs.push_back(ofParameter <bool>(element->getName(), false));
	ofxMinimalToggle::Config config;
	config.shape.setSize(tabWidth, tabHeight);
	config.textLayout = ofxBaseGui::Centered;
	if(element->getBackgroundColor() != thisBackgroundColor){
		config.backgroundColor = element->getBackgroundColor();
		config.borderColor = element->getBackgroundColor();
	}
	tabs.add <ofxMinimalToggle>(parameters_tabs.at(parameters_tabs.size() - 1), config);

	element->setPosition(pagesShape.getPosition());
	element->setShowHeader(false);
	element->setBorderColor(ofColor(0, 0, 0, 0));
	setSizeToElement(element);

	element->setSize(pagesShape.width, pagesShape.height);

	element->unregisterMouseEvents();
	ofAddListener(element->sizeChangedE, this, &ofxTabbedPages::sizeChangedCB);

	parameters.add(element->getParameter());

	if(collection.size() == 2){
		setActiveTab(0);
	}

	setNeedsRedraw();
}

void ofxTabbedPages::clear(){
	collection.resize(1);
	parameters.clear();
	parameters.add(tabs.getParameter());
	b.height = spacing + spacingNextElement;
	if(bShowHeader){
		b.height += header;
	}
	tabs.clear();
	sizeChangedCB();
}

void ofxTabbedPages::generateDraw(){

	updateContentShapes();

	bg.clear();
	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(pagesShape.x, pagesShape.y, pagesShape.width + 1, pagesShape.height);

	border.clear();
	border.setStrokeColor(thisBorderColor);
	border.setStrokeWidth(1);
	border.setFilled(false);
	border.rectangle(pagesShape.x, pagesShape.y + 1, pagesShape.width + 1, pagesShape.height);

	if(bShowHeader){
		generateDrawHeader();
	}
}

void ofxTabbedPages::render(){

	/*
	 * TODO: call the first part when a tab toggle is clicked
	 * in this case call needsRedraw
	 * move the second part to generateDraw
	 */
	for(int i = 1; i < (int)collection.size(); i++){
		if(parameters_tabs.at(i - 1).get()){
			activePage = collection[i];
			activeToggle = tabs.getControl(i - 1);
		}
	}

	tabBorder.clear();
	if(activePage){
		tabBorder.setStrokeWidth(3);
		tabBorder.setFilled(false);
		tabBorder.setStrokeColor(thisBackgroundColor);
		tabBorder.moveTo(activeToggle->getShape().getBottomLeft() - ofPoint(-1, 1));
		tabBorder.lineTo(activeToggle->getShape().getBottomRight() - ofPoint(1, 1));

		this->setBorderColor(activeToggle->getBorderColor());
	}
	/*
	 * end TODO
	 */

	bg.draw();
	tabs.draw();
	border.draw();
	tabBorder.draw();
	if(bShowHeader){
		headerBg.draw();
	}

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofColor c = ofGetStyle().color;

	if(bShowHeader){
		renderHeader();
	}

	if(activePage){
		activePage->draw();
	}

	ofSetColor(c);
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}

}

void ofxTabbedPages::updateContentShapes(){
	ofPoint newpos = b.getPosition();
	if(bShowHeader){
		newpos.y += header + spacing;
	}

	tabShape.setPosition(newpos);
	tabShape.setWidth(b.width);
	tabShape.setHeight(tabHeight);

	newpos.y += tabHeight;
	pagesShape.setPosition(newpos);
	pagesShape.setWidth(b.width - 1 /*border*/);
	pagesShape.setHeight(b.getBottom() - newpos.y - 1 /*border*/);
}

void ofxTabbedPages::setSizeToElement(ofxBaseGui * element){
	updateContentShapes();
	if(element->getWidth() > pagesShape.width){
		b.width = element->getWidth() + 1 /*border*/;
	}
	if(element->getHeight() > pagesShape.getHeight()){
		b.height = element->getHeight() + tabHeight + 1 /*border*/;
		if(bShowHeader){
			b.height += spacing + header;
		}
	}
	sizeChangedCB();
}

void ofxTabbedPages::sizeChangedCB(){
	updateContentShapes();
	tabs.setShape(tabShape);

	for(int i = 1; i < (int)collection.size(); i++){
		collection[i]->sizeChangedE.disable();
		collection[i]->setShape(pagesShape);
		collection[i]->sizeChangedE.enable();
	}
	ofNotifyEvent(sizeChangedE,this);
	setNeedsRedraw();
}

void ofxTabbedPages::setActiveTab(int index){
	tabs.setActiveToggle(index);
	activeToggle = tabs.getControl(index);
}

int ofxTabbedPages::getActiveTabIndex(){
	return tabs.getActiveToggleIndex();
}

ofxGuiPage * ofxTabbedPages::getActiveTab(){
	return (ofxGuiPage *)collection.at(tabs.getActiveToggleIndex() + 1);
}

void ofxTabbedPages::setTabHeight(int h){
	tabHeight = h;
	sizeChangedCB();
}

void ofxTabbedPages::setTabWidth(int w){
	tabWidth = w;
	sizeChangedCB();
}

void ofxTabbedPages::setBorderColor(const ofColor &color){
	ofxGuiPage::setBorderColor(color);
	tabs.setDefaultBackgroundColor(color);
	tabs.setDefaultBorderColor(thisBorderColor);
}

void ofxTabbedPages::setBackgroundColor(const ofColor &color){
	ofxGuiPage::setBackgroundColor(color);
	tabs.setDefaultFillColor(color);
}
