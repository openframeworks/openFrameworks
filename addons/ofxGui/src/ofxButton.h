#pragma once

#include "ofxToggle.h"
#include "ofParameter.h"

class ofxButton : public ofxToggle{
	friend class ofPanel;
	
public:
	ofxButton();
	ofxButton* setup(string toggleName, float width = defaultWidth, float height = defaultHeight);

	virtual void mouseReleased(ofMouseEventArgs & args);
	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )()){
		ofAddListener(triggerEvent,listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )()){
		ofRemoveListener(triggerEvent,listener,method);
	}
	using ofxToggle::addListener;
	using ofxToggle::removeListener;
private:
	ofEvent<void> triggerEvent;
	void valueChanged(bool & v);

};
