#pragma once

#include "ofxToggle.h"
#include "ofParameter.h"

class ofxButton : public ofxToggle{
public:

	ofxButton();
	ofxButton(const std::string& buttonName);
	ofxButton(const std::string& buttonName, const ofJson & config);
	ofxButton(ofParameter<void>& _val, const ofJson & config = ofJson());
	ofxButton(ofParameter<bool>& _bVal, const ofJson & config = ofJson());
	ofxButton(ofParameter<void>& _val, float width, float height = defaultHeight);
	ofxButton(ofParameter<bool>& _bVal, float width, float height = defaultHeight);
	ofxButton(const std::string& buttonName, float width, float height = defaultHeight);

	void setup();

	~ofxButton();

	virtual bool mouseReleased(ofMouseEventArgs & args) override;

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
	bool setValue(float mx, float my, bool bCheck);
	ofParameter<void> voidvalue;
	bool useVoidValue {false};

};
