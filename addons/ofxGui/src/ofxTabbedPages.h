#pragma once

#include "ofxGuiPage.h"

class ofxTabbedPages : public ofxGuiPage {

	public:

		struct Config: public ofxGuiPage::Config{
			Config(){}
			Config(const ofxGuiPage::Config & c)
			:ofxGuiPage::Config(c){}
			Config(const ofxGuiGroup::Config & c)
			:ofxGuiPage::Config(c){}
			Config(const ofxBaseGui::Config & c)
			:ofxGuiPage::Config(c){}

			float tabWidth = 100;
			float tabHeight = defaultHeight;
		};

		ofxTabbedPages();
		ofxTabbedPages(const Config & config);
		~ofxTabbedPages();

		ofxTabbedPages & setup(std::string collectionName = "", std::string filename = "settings.xml", float x = 10, float y = 10);

		using ofxGuiGroup::add;

		void clear();

		void setActiveTab(int index);
		int getActiveTabIndex();
		ofxGuiPage * getActiveTab();

		void setTabHeight(int h);
		void setTabWidth(int w);

		void setBorderColor(const ofColor &color);
		void setBackgroundColor(const ofColor &color);

	protected:
		void render();
		void generateDraw();
		void updateContentShapes();
		void setSizeToElement(ofxBaseGui * element);
		virtual void sizeChangedCB();
		virtual void add(ofxGuiGroup * element);

	private:

		ofxGuiGroup tabs;
		std::vector <ofParameter <bool> > parameters_tabs;
		float tabHeight, tabWidth;
		ofRectangle pagesShape, tabShape;
		ofPath bg;
		ofPath tabBorder;
		ofxBaseGui * activePage;
		ofxBaseGui * activeToggle;
};
