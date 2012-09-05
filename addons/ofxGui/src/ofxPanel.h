#pragma once

#include "ofxBaseGui.h"
#include "ofConstants.h"
#include "ofxXmlSettings.h"
#include "ofxSlider.h"
#include "ofxButton.h"
#include "ofxToggle.h"
#include "ofRectangle.h"
#include "ofImage.h"


class ofxPanel : public ofxBaseGui{
public:
	ofxPanel();

	virtual ~ofxPanel();
	
	ofxPanel * setup(string collectionName="", string _filename="settings.xml", float x = 10, float y = 10);
	
	virtual void saveToXml(ofxXmlSettings& xml);
	virtual void loadFromXml(ofxXmlSettings& xml);
	
	void add(ofxBaseGui * element);
	void add(ofxParameter<float> & parameter);
	void add(ofxParameter<int> & parameter);
	void add(ofxParameter<bool> & parameter);
	
	void clear();
	
	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);
	
	void draw();
	
	vector<string> getControlNames();
	int getNumControls();

	ofxIntSlider getIntSlider(string name);
	ofxFloatSlider getFloatSlider(string name);
	ofxToggle getToggle(string name);
	ofxButton getButton(string name);

	ofxBaseGui * getControl(string name);
	ofxBaseGui * getControl(int num);

protected:
	void setValue(float mx, float my, bool bCheck);

	template<class ControlType>
	ControlType getControlType(string name);

private:
	ofPoint grabPt;
	bool bGrabbed;
	float spacing;
	float header;
	vector <ofxBaseGui *> collection;
	
	string filename;
	ofRectangle loadBox, saveBox;
	static ofImage loadIcon, saveIcon;
};

template<class ControlType>
ControlType ofxPanel::getControlType(string name){
	ControlType * control = dynamic_cast<ControlType*>(getControl(name));
	if(control) return *control;
	else return ControlType();
}
