#pragma once

#include "ofxGuiPage.h"

class ofxTabbedPages : public ofxGuiPage {

	public:
		ofxTabbedPages();
		~ofxTabbedPages();

		ofxTabbedPages & setup(std::string collectionName = "", std::string filename = "settings.xml", float x = 10, float y = 10);

        virtual void add(ofxGuiGroup * element);

		void clear();

		void setActiveTab(int index);
		int getActiveTabIndex();
		ofxGuiPage * getActiveTab();

		void setTabHeight(int h);
        void setTabWidth(int w);

	protected:
		void render();
		void generateDraw();
		void updateContentShapes();
		void setSizeToElement(ofxBaseGui * element);
		virtual void sizeChangedCB();

	private:

		ofxGuiGroup tabs;
		vector <ofParameter <bool> > parameters_tabs;
		float tabHeight, tabWidth;
		ofRectangle pagesShape, tabShape;
		ofPath bg;
		ofPath tabBorder;
		ofxBaseGui * activePage;
		ofxBaseGui * activeToggle;
};
