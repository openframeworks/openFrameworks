#pragma once

#include "ofxToggle.h"
#include "ofParameter.h"

class ofxButton : public ofxToggle{
public:

	ofxButton(const std::string& buttonName="", const ofJson & config = ofJson());
	ofxButton(ofParameter<void>& _val, const ofJson & config = ofJson());
	ofxButton(ofParameter<bool>& _bVal, const ofJson & config = ofJson());
//	ofxButton(const std::string& buttonName, const ofJson & config = ofJson());
	ofxButton(ofParameter<void>& _val, float width, float height = defaultHeight);
	ofxButton(ofParameter<bool>& _bVal, float width, float height = defaultHeight);
	ofxButton(const std::string& buttonName, float width, float height = defaultHeight);

	void setup(const ofJson & config = ofJson());

	~ofxButton();

	virtual void pointerReleased(PointerUIEventArgs& e);

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		voidvalue.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		voidvalue.removeListener(listener,method);
	}

protected:
	virtual void generateDraw();
	bool setValue(float mx, float my);
	ofParameter<void> voidvalue;
	bool useVoidValue {false};

};
