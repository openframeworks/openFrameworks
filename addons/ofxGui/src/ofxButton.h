#pragma once

#include "ofxToggle.h"
#include "ofParameter.h"

class ofxButton : public ofxToggle{
public:
	struct Config: public ofxToggle::Config{
		Config(){}
		Config(const ofxToggle::Config & c)
		:ofxToggle::Config(c){}
		Config(const ofxBaseGui::Config & c)
		:ofxToggle::Config(c){}
	};

	ofxButton();
	ofxButton(ofParameter<void> _val, const Config & config);
	ofxButton(ofParameter<bool> _bVal, const Config & config);
	~ofxButton();
	ofxButton & setup(const std::string& buttonName, const Config & config);
	ofxButton & setup(ofParameter<void>& _val, const Config & config);
	ofxButton & setup(ofParameter<bool>& _bVal, const Config & config);
	ofxButton & setup(const std::string& buttonName, float width = defaultWidth, float height = defaultHeight);
	ofxButton & setup(ofParameter<void>& _val, float width = defaultWidth, float height = defaultHeight);
	ofxButton & setup(ofParameter<bool>& _bVal, float width = defaultWidth, float height = defaultHeight);

	virtual bool mouseReleased(ofMouseEventArgs & args);
	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);

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
