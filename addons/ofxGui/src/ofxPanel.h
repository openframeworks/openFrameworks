#pragma once

#include "ofxGuiGroup.h"

class ofxGuiGroup;

class ofxPanel : public ofxGuiGroup {
public:
	struct Config: public ofxGuiGroup::Config{
		Config(){
			shape.x = 10;
			shape.y = 10;
		}
		Config(const ofxGuiGroup::Config & config){}
		Config(const ofxBaseGui::Config & config){}
	};

	ofxPanel();
	ofxPanel(const ofParameterGroup & parameters, const Config & config);
    ~ofxPanel();

	ofxPanel & setup(const ofParameterGroup & parameters, const Config & config);
	ofxPanel & setup(const std::string& collectionName="", const std::string& filename="settings.xml", float x = 10, float y = 10);
	ofxPanel & setup(const ofParameterGroup & parameters, const std::string& filename="settings.xml", float x = 10, float y = 10);

	bool mouseReleased(ofMouseEventArgs & args);

	ofEvent<void> loadPressedE;
	ofEvent<void> savePressedE;
protected:
	void render();
    void renderHeader();
	bool setValue(float mx, float my, bool bCheck);
	void generateDraw();
    void generateDrawHeader();
	void loadIcons();
private:
	ofRectangle loadBox, saveBox;
	static ofImage loadIcon, saveIcon;
    
    ofPoint grabPt;
	bool bGrabbed;
};
