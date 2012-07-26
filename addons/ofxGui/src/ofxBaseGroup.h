#pragma once
#include "ofxBaseGui.h"

#include "ofxSlider.h"
#include "ofxButton.h"

class ofxBaseGroup : public ofxBaseGui {
public:	    
    virtual ~ofxBaseGroup() {}
    ofxBaseGroup * setup(string collectionName="", string filename="settings.xml", float x = 10, float y = 10);
	ofxBaseGroup * setup(const ofParameterGroup & parameters, string filename="settings.xml", float x = 10, float y = 10);
    
	void add(ofxBaseGui * element);
    void add(const ofParameterGroup & parameters);
	void add(ofParameter<float> & parameter);
	void add(ofParameter<int> & parameter);
	void add(ofParameter<bool> & parameter);
	
	void clear();
	
	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);
	
	virtual void draw();
	
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
    ofxBaseGroup(string collectionName="", string filename="settings.xml", float x = 10, float y = 10);
	ofxBaseGroup(const ofParameterGroup & parameters, string _filename="settings.xml", float x = 10, float y = 10);
    virtual void setValue(float mx, float my, bool bCheck);
    
	float spacing;
	float header;
	
    template<class ControlType>
	ControlType getControlType(string name);
    
    ofPoint grabPt;
	bool bGrabbed;

	vector <ofxBaseGui *> collection;
	ofParameterGroup parameters;
	
	string filename;
};

template<class ControlType>
ControlType ofxBaseGroup::getControlType(string name){
	ControlType * control = dynamic_cast<ControlType*>(getControl(name));
	if(control) return *control;
	else return ControlType();
}