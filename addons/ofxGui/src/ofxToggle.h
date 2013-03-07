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
	

	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);
	
	void draw();

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(bool&)){
		value.addListener(listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(bool&)){
		value.removeListener(listener,method);
	}


	bool operator=(bool v);
	operator const bool & ();

	virtual ofAbstractParameter & getParameter();

protected:
	ofRectangle checkboxRect;
	ofParameter<bool> value;
	
	void setValue(float mx, float my, bool bCheck);
	void generateDraw();
	void valueChanged(bool & value);
	ofPath bg,fg,cross;
	ofVboMesh textMesh;
};
