#pragma once

#include "ofxPanel.h"

class ofxGuiTabs : public ofxPanel {

	public:

		ofxGuiTabs();
		ofxGuiTabs(const std::string &collectionName, const ofJson & config = ofJson());
		ofxGuiTabs(std::string collectionName, std::string filename, float x = 10, float y = 10);
		~ofxGuiTabs();

		void setup();

		using ofxGuiGroup::add;

		template <typename GuiType>
		GuiType* add(std::unique_ptr<GuiType> element);

		void clear();

		void setActiveTab(int &index);
		ofParameter<int> &getActiveTabIndex();
		ofxBaseGui * getActiveTab();

		void setTabHeight(int h);
		void setTabWidth(int w);

	protected:

		void onResize(ResizeEventArgs &) override;
		void onChildAdd(ElementEventArgs& args);

		ofParameter<float> tabWidth;
		ofParameter<float> tabHeight;

		ofxGuiGroup * tabs;
		vector<Element*> pages;
		ofPath tabBorder;
		ofxBaseGui * activePage;
		ofxBaseGui * activeToggle;

	private:
};
