#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

template<typename Type>
class ofxSlider : public ofxBaseGui{
	friend class ofPanel;
	
public:	
	ofxSlider(){}
	ofxSlider(string sliderName, ofParameter<Type> _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);
	ofxSlider* setup(string sliderName, ofParameter<Type> _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);
	
	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		value.addListener(listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		value.removeListener(listener,method);
	}


	double operator=(Type v);
	operator Type & ();

	void draw();
	

	ofAbstractParameter & getParameter();

protected:
	Type min, max;
	
	ofParameter<Type> value;

	void setValue(float mx, float my, bool bCheck);
};

typedef ofxSlider<float> ofxFloatSlider;
typedef ofxSlider<int> ofxIntSlider;
