#pragma once

#include "ofxGuiPage.h"

class ofxTabbedPages : public ofxGuiGroup {

	public:

		ofxTabbedPages(const std::string &collectionName, const ofJson & config = ofJson());
		ofxTabbedPages(std::string collectionName, std::string filename, float x = 10, float y = 10);
		~ofxTabbedPages();

		void setup(const ofJson &config = ofJson());

		using ofxGuiGroup::add;
		template <typename GuiType>
		GuiType* add(std::unique_ptr<GuiType> element);

		void clear();

		void setActiveTab(int index);
		int getActiveTabIndex();
		ofxGuiPage * getActiveTab();

		void setTabHeight(int h);
		void setTabWidth(int w);

		void setBorderColor(const ofColor &color);
		void setBackgroundColor(const ofColor &color);

	protected:
//		void render();
//		void generateDraw();
		void updateContentShapes();

		ofParameter<float> tabWidth;
		ofParameter<float> tabHeight;

		ofxGuiGroup * tabs;
		ofxGuiGroup * pages;
		ofPath tabBorder;
		ofxBaseGui * activePage;
		ofxBaseGui * activeToggle;

	private:
};

// TODO (what was this good for?)
template <typename GuiType>
GuiType* ofxTabbedPages::add(std::unique_ptr<GuiType> element){

	Element::add(element);

	if(exclusiveToggles) {
		setOneToggleActive();
	}

	// TODO

//	collection.push_back(element);

//	parameters_tabs.push_back(ofParameter <bool>(element->getName(), false));
//	ofxMinimalToggle::Config config;
//	config.shape.setSize(tabWidth, tabHeight);
//	config.textAlignment = ofxBaseGui::Centered;
//	if(element->getBackgroundColor() != thisBackgroundColor){
//		config.backgroundColor = element->getBackgroundColor();
//		config.borderColor = element->getBackgroundColor();
//	}
//	tabs.add <ofxMinimalToggle>(parameters_tabs.at(parameters_tabs.size() - 1), config);

//	element->setPosition(pagesShape.getPosition());
//	element->setShowHeader(false);
//	element->setBorderColor(ofColor(0, 0, 0, 0));
//	setSizeToElement(element);

//	element->setSize(pagesShape.width, pagesShape.height);

//	element->unregisterMouseEvents();
//	ofAddListener(element->sizeChangedE, this, &ofxTabbedPages::sizeChangedCB);

//	parameters.add(element->getParameter());

//	if(collection.size() == 2){
//		setActiveTab(0);
//	}

//	setNeedsRedraw();
}
