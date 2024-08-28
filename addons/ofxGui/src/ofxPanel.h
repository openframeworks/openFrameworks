#pragma once

#include "ofxGuiGroup.h"
#include "ofImage.h"

#ifndef TARGET_EMSCRIPTEN
constexpr const char* ofxPanelDefaultFilename = "settings.xml";
#else
constexpr const char* ofxPanelDefaultFilename = "settings.json";
#endif

class ofxGuiGroup;

class ofxPanel : public ofxGuiGroup {
public:
	ofxPanel();
	ofxPanel(const ofParameterGroup & parameters, const std::string& filename=ofxPanelDefaultFilename, float x = 10, float y = 10);
	~ofxPanel();

	ofxPanel * setup(const std::string& collectionName="", const std::string& filename=ofxPanelDefaultFilename, float x = 10, float y = 10);
	ofxPanel * setup(const ofParameterGroup & parameters, const std::string& filename=ofxPanelDefaultFilename, float x = 10, float y = 10);

	bool mousePressed(ofMouseEventArgs & args);
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
	ofImage loadIcon, saveIcon;
    
    glm::vec3 grabPt;
	bool bGrabbed;
};
