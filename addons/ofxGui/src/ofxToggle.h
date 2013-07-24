#pragma once

#include "ofParameter.h"
#include "ofxBaseGui.h"

class ofxToggle : public ofxBaseGui{
public:
	ofxToggle(){};
	~ofxToggle();
	ofxToggle(ofParameter<bool> _bVal, float width = defaultWidth, float height = defaultHeight);
	ofxToggle * setup(ofParameter<bool> _bVal, float width = defaultWidth, float height = defaultHeight);
	ofxToggle * setup(string toggleName, bool _bVal, float width = defaultWidth, float height = defaultHeight);
	

	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);
	virtual bool mouseReleased(ofMouseEventArgs & args);
	

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
	void render();
	ofRectangle checkboxRect;
	ofParameter<bool> value;
	bool bGuiActive;
	
	bool setValue(float mx, float my, bool bCheck);
	void generateDraw();
	void valueChanged(bool & value);
	ofPath bg,fg,cross;
	ofVboMesh textMesh;
};
