#include "ofxGuiTabs.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiTabs::ofxGuiTabs() :
	ofxGuiGroup(){

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
	if(tabs){
		tabs->getActiveToggleIndex().removeListener(this, &ofxGuiTabs::setActiveTab);
	}
	ofRemoveListener(childAdded, this, &ofxGuiTabs::onChildAdd);
}

void ofxGuiTabs::setup(){

	tabs = nullptr;
	pages.clear();
	activeToggle = nullptr;
	activePage = nullptr;
	clear();

	setBackgroundColor(ofColor(0,0,0,0));
	setShowHeader(false);
	setBorderWidth(0);
	tabWidth.set("tab width", 100);
	tabHeight.set("tab height", defaultHeight);

	ofAddListener(childAdded, this, &ofxGuiTabs::onChildAdd);

}

void ofxGuiTabs::clear(){

	if(tabs){
		tabs->getActiveToggleIndex().removeListener(this, &ofxGuiTabs::setActiveTab);
	}

	ofxGuiGroup::clear();

	tabs = add<ofxGuiGroup>("tabs");
	tabs->setShowHeader(false);
	tabs->setExclusiveToggles(true);
	tabs->setBorderWidth(0);
	tabs->setPercentalWidth(true, 1);
	tabs->setMargin(0, 0, -2, 0);
	tabs->setBackgroundColor(ofColor(0,0,0,0));
	tabs->getActiveToggleIndex().addListener(this, &ofxGuiTabs::setActiveTab);

}

void ofxGuiTabs::onResize(ResizeEventArgs &args){
	ofxGuiGroup::onResize(args);
	if(tabs){
		tabs->setWidth(args.shape().width);
	}

	for(auto &e : pages){
		e->setWidth(args.shape().width);
	}
}

void ofxGuiTabs::onChildAdd(ElementEventArgs &args){

	unsigned int nopages = 2;
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
			page->setHidden(true);
			page->setPercentalWidth(true, 1);
			page->setMargin(0);
		}

		ofJson toggleconfig = {
			{"float", "left"},
			{"width", tabWidth.get()},
			{"height", tabHeight.get()},
			{"type", "fullsize"}
		};
		ofxBaseGui* tab = tabs->add<ofxToggle>(name, toggleconfig);
		tab->setTextAlignment(TextAlignment::CENTERED);
		tab->setFillColor(page->getBackgroundColor());
		tab->setMargin(0);
		tab->setBackgroundColor(ofColor(page->getBackgroundColor(), 50));

		if(pages.size() == 2){
			int index = 0;
			setActiveTab(index);
		}

		setNeedsRedraw();

	}

}

void ofxGuiTabs::setActiveTab(int &index){
	activeToggle = tabs->getControlType<ofxToggle>(index);
	activeToggle->getParameter().cast<bool>().set(true);
	for(auto &e : pages){
		e->setHidden(true);
	}
	pages.at(index)->setHidden(false);
	activePage = pages.at(index);
}

ofParameter<int>& ofxGuiTabs::getActiveTabIndex(){
	return tabs->getActiveToggleIndex();
}

Element *ofxGuiTabs::getActiveTab(){
	return activePage;
}

void ofxGuiTabs::setTabHeight(int h){
	tabHeight = h;
	for(auto & e: tabs->children()){
		e->setHeight(h);
	}
}

void ofxGuiTabs::setTabWidth(int w){
	tabWidth = w;
	for(auto & e: tabs->children()){
		e->setWidth(w);
	}
}

void ofxGuiTabs::minimize(){
	minimized = true;

	for(auto& child : children()){
		if(child != header){
			child->setHidden(true);
		}
	}

	invalidateChildShape();
	setNeedsRedraw();
}

void ofxGuiTabs::minimizeAll(){
	for(auto & e: children()){
		if(e != header){
			ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>(e);
			if(group){
				group->minimize();
			}
		}
	}
}

void ofxGuiTabs::maximize(){
	minimized = false;
	activePage->setHidden(false);
	invalidateChildShape();
	setNeedsRedraw();
}

void ofxGuiTabs::maximizeAll(){
	maximize();
}
