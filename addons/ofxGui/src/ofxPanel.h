#pragma once

#include "ofxGuiGroup.h"

class ofxGuiGroup;

class ofxPanel : public ofxGuiGroup {
public:
	struct Config: public ofxGuiGroup::Config{
		Config():ofxGuiGroup::Config(){
			shape.x = 10;
			shape.y = 10;
		}
		Config(const ofxGuiGroup::Config & config):ofxGuiGroup::Config(config){}
		Config(const ofxBaseGui::Config & config):ofxGuiGroup::Config(config){}
	};

	ofxPanel();
	ofxPanel(const ofParameterGroup & parameters, const Config & groupConfig = ofxPanel::Config(), const Config &itemConfig = ofxBaseGui::Config());
	~ofxPanel();

	ofxPanel & setup(const Config & config = ofxPanel::Config());
	ofxPanel & setup(const std::string& collectionName, const Config & config = ofxPanel::Config());
	ofxPanel & setup(const ofParameterGroup & parameters, const Config & groupConfig = ofxPanel::Config(), const Config &itemConfig = ofxBaseGui::Config());
	ofxPanel & setup(const std::string& collectionName, const std::string& filename, float x = 10, float y = 10);
	ofxPanel & setup(const ofParameterGroup & parameters, const std::string& filename, float x = 10, float y = 10);

	virtual void setSize(float w, float h);
	virtual void setShape(ofRectangle r);
	virtual void setShape(float x, float y, float w, float h);

	bool mouseReleased(ofMouseEventArgs & args);

	ofEvent<void> loadPressedE;
	ofEvent<void> savePressedE;
protected:
	virtual void sizeChangedCB();
	virtual void render();
	virtual void renderHeader();
	virtual bool setValue(float mx, float my, bool bCheck);
	virtual void generateDraw();
	virtual void generateDrawHeader();
	virtual void loadIcons();
private:
	ofRectangle loadBox, saveBox;
	static ofImage loadIcon, saveIcon;

	ofPoint grabPt;
	bool bGrabbed;
};
