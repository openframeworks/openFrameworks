#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

template<typename Type>
class ofxSlider : public ofxBaseGui{
	friend class ofPanel;
	
public:	
	ofxSlider* setup(string sliderName, ofxParameter<Type> _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);
	
	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);
	
	virtual void saveToXml(ofxXmlSettings& xml);
	virtual void loadFromXml(ofxXmlSettings& xml);

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		ofAddListener(value.changedE,listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		ofRemoveListener(value.changedE,listener,method);
	}


	double operator=(Type v);
	operator Type & ();

	void draw();
	
	ofxParameter<Type> value;

protected:
	Type min, max;
	
	void setValue(float mx, float my, bool bCheck);
};

typedef ofxSlider<float> ofxFloatSlider;
typedef ofxSlider<int> ofxIntSlider;
