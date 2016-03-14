#pragma once

#include "ofParameter.h"
#include "ofxBaseGui.h"

class ofxToggle : public ofxBaseGui{
public:

	ofxToggle(const std::string& toggleName="", const ofJson & config = ofJson());
	ofxToggle(ofParameter<bool>& _bVal, const ofJson & config = ofJson());
	ofxToggle(ofParameter<bool>& _bVal, float width, float height = defaultHeight);
	ofxToggle(const std::string& toggleName, bool _bVal, float width = defaultWidth, float height = defaultHeight);

	~ofxToggle();

	void setup(const ofJson &config = ofJson());

	virtual void pointerPressed(PointerUIEventArgs& e);

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}

	bool operator=(bool v);
	operator const bool & ();

	virtual ofAbstractParameter & getParameter();

protected:
	virtual void render();
	ofRectangle checkboxRect;
	ofParameter<bool> value;

	virtual bool setValue(float mx, float my);
	virtual void generateDraw();
	void valueChanged(bool & value);
	ofPath fg,cross;
	ofVboMesh textMesh;
};
