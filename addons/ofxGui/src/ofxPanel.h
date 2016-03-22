#pragma once

#include "ofxGuiGroup.h"

class ofxGuiGroup;

class ofxPanelHeader : public ofxGuiGroupHeader {

	public:

		ofxPanelHeader();
		ofxPanelHeader(const ofJson &config);

		~ofxPanelHeader(){
		}

		virtual bool mousePressed(ofMouseEventArgs & args) override;

		ofEvent<void> loadPressedE;
		ofEvent<void> savePressedE;

	protected:

		virtual void generateDraw() override;
		virtual void render() override;
		virtual void loadIcons();

		ofRectangle loadBox, saveBox;
		static ofImage loadIcon, saveIcon;

};

class ofxPanel : public ofxGuiGroup {

	public:

		ofxPanel(const std::string& collectionName="");
		ofxPanel(const std::string& collectionName, const ofJson & config);
		ofxPanel(const ofParameterGroup & parameters, const ofJson & config = ofJson());
		ofxPanel(const std::string& collectionName, const std::string& filename, float x = 10, float y = 10);
		ofxPanel(const ofParameterGroup & parameters, const std::string& filename, float x = 10, float y = 10);

		~ofxPanel();

		void setup();

		void onHeaderMove(MoveEventArgs& args);
		void onLoadPressed();
		void onSavePressed();

	private:

		bool headerListenersLoaded;

};
