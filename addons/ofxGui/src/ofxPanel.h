#pragma once

#include "ofxGuiGroup.h"

class ofxGuiGroup;

class ofxPanelHeader : public ofxGuiGroupHeader {

	public:

		ofxPanelHeader(const ofJson &config = ofJson());

		~ofxPanelHeader(){
			unregisterPointerEvents();
		}

		virtual void pointerPressed(PointerUIEventArgs & e);

		ofEvent<void> loadPressedE;
		ofEvent<void> savePressedE;

	protected:

		virtual void generateDraw() override;
		virtual void render() override;
		virtual void loadIcons();
		virtual bool setValue(float mx, float my) override;

		ofRectangle loadBox, saveBox;
		static ofImage loadIcon, saveIcon;

};

class ofxPanel : public ofxGuiGroup {

	public:

		ofxPanel(const std::string& collectionName="", const ofJson & config = ofJson());
		ofxPanel(const ofParameterGroup & parameters, const ofJson & config = ofJson());
		ofxPanel(const std::string& collectionName, const std::string& filename, float x = 10, float y = 10);
		ofxPanel(const ofParameterGroup & parameters, const std::string& filename, float x = 10, float y = 10);

		~ofxPanel();

		void setup(const ofJson &config = ofJson());

		virtual void clear() override;

		void onHeaderMove(MoveEventArgs& args);
		void onLoadPressed();
		void onSavePressed();

	private:

		bool headerListenersLoaded;

};
