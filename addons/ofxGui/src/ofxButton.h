#pragma once

#include "ofxToggle.h"
#include "ofParameter.h"

class ofxButton : public ofxToggle{
	friend class ofPanel;
	
public:
	ofxButton();
	ofxButton(ofParameter<void> _bVal, float width = defaultWidth, float height = defaultHeight);
	~ofxButton();
	ofxButton* setup(ofParameter<void> _bVal, float width = defaultWidth, float height = defaultHeight);
    ofxButton* setup(const std::string& toggleName, float width = defaultWidth, float height = defaultHeight);

	virtual bool mouseReleased(ofMouseEventArgs & args);
	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		parameter.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		parameter.removeListener(listener,method);
	}
	virtual ofAbstractParameter & getParameter();

private:
	ofParameter<void> parameter;
	void valueChanged(bool & v);

};
