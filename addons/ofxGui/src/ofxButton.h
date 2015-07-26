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
		std::string name;
	};

	ofxButton(const Config & config = Config());
	~ofxButton();
    ofxButton & setup(const std::string& toggleName, float width = defaultWidth, float height = defaultHeight);

	virtual bool mouseReleased(ofMouseEventArgs & args);
	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		ofAddListener(triggerEvent,listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		ofRemoveListener(triggerEvent,listener,method);
	}

private:
	ofEvent<void> triggerEvent;
	void valueChanged(bool & v);

};
