#pragma once

#include "ofxBaseGui.h"
#include "ofConstants.h"
#include "ofxXmlSettings.h"
#include "ofxSlider.h"
#include "ofxButton.h"
#include "ofxToggle.h"


class ofxPanel : public ofxBaseGui{
public:
	ofxPanel();

	virtual ~ofxPanel();
	
	void setup(string collectionName="", string _filename="settings.xml", float x = 10, float y = 10);
	
	virtual void saveToXml(ofxXmlSettings& xml);
	virtual void loadFromXml(ofxXmlSettings& xml);
	
	void add(ofxBaseGui * element);
	
	void clear();
	
	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);
	
	void draw();
	
	vector<string> getControlNames();

	ofxIntSlider getIntSlider(string name);
	ofxFloatSlider getFloatSlider(string name);
	ofxToggle getToggle(string name);
	ofxButton getButton(string name);

protected:
	void setValue(float mx, float my, bool bCheck);

	template<class ControlType>
	ControlType getControl(string name);

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
ControlType ofxPanel::getControl(string name){
	for(int i=0; i<(int)collection.size(); i++){
		if(collection[i]->getName()==name){
			ControlType * control = dynamic_cast<ControlType*>(collection[i]);
			if(control)	return *control;
		}
	}
	return ControlType();
}
