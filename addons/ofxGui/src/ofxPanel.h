#pragma once

#include "ofxGuiGroup.h"

class ofxGuiGroup;

class ofxPanel : public ofxGuiGroup {
public:
	ofxPanel();
	ofxPanel(const ofParameterGroup & parameters, const std::string& filename="settings.xml", float x = 10, float y = 10);
	~ofxPanel();

	ofxPanel * setup(const std::string& collectionName="", const std::string& filename="settings.xml", float x = 10, float y = 10);
	ofxPanel * setup(const ofParameterGroup & parameters, const std::string& filename="settings.xml", float x = 10, float y = 10);

	bool mouseReleased(ofMouseEventArgs & args);

	ofEvent<void> loadPressedE;
	ofEvent<void> savePressedE;
protected:
	void render();
	bool setValue(float mx, float my, bool bCheck);
	void generateDraw();
	void loadIcons();
private:
	ofRectangle loadBox, saveBox;
	static ofImage loadIcon, saveIcon;
    
    ofPoint grabPt;
	bool bGrabbed;
};
