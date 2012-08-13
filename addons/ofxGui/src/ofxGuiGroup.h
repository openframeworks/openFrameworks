#pragma once
#include "ofxBaseGui.h"

#include "ofxSlider.h"
#include "ofxButton.h"

class ofxGuiGroup : public ofxBaseGui {
public:	    
    virtual ~ofxGuiGroup() {}
    ofxGuiGroup * setup(string collectionName="", string filename="settings.xml", float x = 10, float y = 10);
	ofxGuiGroup * setup(const ofParameterGroup & parameters, string filename="settings.xml", float x = 10, float y = 10);
    
	void add(ofxBaseGui * element);
    void add(const ofParameterGroup & parameters);
	void add(ofParameter<float> & parameter);
	void add(ofParameter<int> & parameter);
	void add(ofParameter<bool> & parameter);
    void add(ofParameter<ofVec2f> & parameter);
    void add(ofParameter<ofVec3f> & parameter);
    void add(ofParameter<ofVec4f> & parameter);
    void add(ofParameter<ofColor> & parameter);
    void add(ofParameter<ofShortColor> & parameter);
    void add(ofParameter<ofFloatColor> & parameter);
	
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
    ofxGuiGroup(string collectionName="", string filename="settings.xml", float x = 10, float y = 10);
	ofxGuiGroup(const ofParameterGroup & parameters, string _filename="settings.xml", float x = 10, float y = 10);
    void registerMouseEvents();
    virtual void setValue(float mx, float my, bool bCheck);
    void sizeChangedCB();
    
	float spacing;
	float header;
	
    template<class ControlType>
	ControlType getControlType(string name);

	vector <ofxBaseGui *> collection;
	ofParameterGroup parameters;
	
	string filename;
	bool minimized;

	ofxGuiGroup * parent;
};

template<class ControlType>
ControlType ofxGuiGroup::getControlType(string name){
	ControlType * control = dynamic_cast<ControlType*>(getControl(name));
	if(control) return *control;
	else return ControlType();
}
