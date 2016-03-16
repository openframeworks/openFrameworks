#pragma once

#include "ofxGuiGroup.h"

class ofxGuiTabs : public ofxGuiGroup {

	public:

		ofxGuiTabs();
		ofxGuiTabs(const std::string &collectionName, const ofJson & config = ofJson());
		ofxGuiTabs(std::string collectionName, std::string filename, float x = 10, float y = 10);
		~ofxGuiTabs();

		void setup();

		using ofxGuiGroup::add;

		template <typename GuiType>
		GuiType* add(std::unique_ptr<GuiType> element);

		virtual void clear() override;

		void setActiveTab(int &index);
		ofParameter<int> &getActiveTabIndex();
		Element * getActiveTab();

		void setTabHeight(int h);
		void setTabWidth(int w);

		virtual void minimize() override;
		virtual void maximize() override;
		virtual void minimizeAll() override;
		virtual void maximizeAll() override;

	protected:

		void onResize(ResizeEventArgs &) override;
		void onChildAdd(ElementEventArgs& args);

		ofParameter<float> tabWidth;
		ofParameter<float> tabHeight;

		ofxGuiGroup * tabs;
		vector<Element*> pages;
		Element * activePage;
		ofxBaseGui * activeToggle;


	private:
};
