#include "ofxGuiTabs.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiTabs::ofxGuiTabs() :
	ofxPanel(){

	setup();

}

ofxGuiTabs::ofxGuiTabs(const string &collectionName, const ofJson &config) :
	ofxGuiTabs(){

	setName(collectionName);
	_setConfig(config);

}

ofxGuiTabs::ofxGuiTabs(string collectionName, string filename, float x, float y)
	:ofxGuiTabs(collectionName){

	this->filename = filename;
	setPosition(x,y);

}

ofxGuiTabs::~ofxGuiTabs(){
	tabs->getActiveToggleIndex().removeListener(this, &ofxGuiTabs::setActiveTab);
	ofRemoveListener(childAdded, this, &ofxGuiTabs::onChildAdd);
}

void ofxGuiTabs::setup(){

	tabs = nullptr;
	pages.clear();

	setBackgroundColor(ofColor(0,0,0,0));
	setShowHeader(false);
	tabWidth.set("tab width", 100);
	tabHeight.set("tab height", defaultHeight);

	clear();

	tabs->getActiveToggleIndex().addListener(this, &ofxGuiTabs::setActiveTab);
	ofAddListener(childAdded, this, &ofxGuiTabs::onChildAdd);

}

void ofxGuiTabs::clear(){
	ofxPanel::clear();

	tabs = add<ofxGuiGroup>("tabs");
	tabs->setShowHeader(false);
	tabs->setExclusiveToggles(true);
	tabs->setBorderWidth(0);
	tabs->setBackgroundColor(ofColor(0,0,0,0));

}

void ofxGuiTabs::onResize(ResizeEventArgs &args){
	ofxPanel::onResize(args);
	if(tabs){
		tabs->setWidth(args.shape().width);
	}

	for(auto &e : pages){
		e->setWidth(args.shape().width);
	}
}

void ofxGuiTabs::onChildAdd(ElementEventArgs &args){

	int nopages = 2;
	if(this->children().size() < nopages){
		return;
	}

	if(tabs){

		Element* _page = args.element();

		pages.push_back(_page);

		std::string name = "Page " + ofToString(pages.size());
		ofxBaseGui* page = dynamic_cast<ofxBaseGui*>(_page);
		if(page){
			name = page->getName();
		}

		ofJson toggleconfig = {
			{"float", "left"},
			{"width", tabWidth.get()},
			{"height", tabHeight.get()}
		};
		ofxBaseGui* tab = tabs->add<ofxToggle>(name, toggleconfig);
		tab->setTextAlignment(TextAlignment::Centered);

		if(pages.size() == 2){
			int index = 0;
			setActiveTab(index);
		}

		setNeedsRedraw();

	}

}

void ofxGuiTabs::setActiveTab(int &index){
	//+1 because of header element
	activeToggle = tabs->getControlType<ofxToggle>(index);
	activeToggle->getParameter().cast<bool>().set(true);
	for(auto &e : pages){
		e->setHidden(true);
	}
	pages.at(index)->setHidden(false);
}

ofParameter<int>& ofxGuiTabs::getActiveTabIndex(){
	return tabs->getActiveToggleIndex();
}

ofxBaseGui *ofxGuiTabs::getActiveTab(){
	return (ofxBaseGui*)pages.at(tabs->getActiveToggleIndex());
}

void ofxGuiTabs::setTabHeight(int h){
	tabHeight = h;
//	sizeChangedCB();
}

void ofxGuiTabs::setTabWidth(int w){
	tabWidth = w;
//	sizeChangedCB();
}

