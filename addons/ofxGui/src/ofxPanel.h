#pragma once

#include "ofxBaseGui.h"
#include "ofConstants.h"
#include "ofxXmlSettings.h"
#include "ofxSlider.h"
#include "ofxButton.h"
#include "ofxToggle.h"
#include "ofParameterGroup.h"


class ofxPanel : public ofxBaseGui{
public:
	ofxPanel();

	virtual ~ofxPanel();
	
	ofxPanel * setup(string collectionName="", string _filename="settings.xml", float x = 10, float y = 10);
	ofxPanel * setup(const ofParameterGroup & parameters, string _filename="settings.xml", float x = 10, float y = 10);
	
	void add(ofxBaseGui * element);
	void add(ofParameter<float> & parameter);
	void add(ofParameter<int> & parameter);
	void add(ofParameter<bool> & parameter);
	
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


	ofAbstractParameter & getParameter();

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
	ofParameterGroup parameters;
	
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
