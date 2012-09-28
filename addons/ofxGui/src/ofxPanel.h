#pragma once

#include "ofxGuiGroup.h"

class ofxGuiGroup;

class ofxPanel : public ofxGuiGroup {
public:
    ofxPanel(string collectionName="", string filename="settings.xml", float x = 10, float y = 10);
	ofxPanel(const ofParameterGroup & parameters, string filename="settings.xml", float x = 10, float y = 10);
	void draw();
	virtual void mouseReleased(ofMouseEventArgs & args);

	ofEvent<bool> loadPressedE;
	ofEvent<bool> savePressedE;
protected:
	void setValue(float mx, float my, bool bCheck);
private:
	ofRectangle loadBox, saveBox;
	static ofImage loadIcon, saveIcon;
    
    ofPoint grabPt;
	bool bGrabbed;
};
