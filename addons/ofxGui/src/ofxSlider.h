#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

template<typename Type>
class ofxSlider : public ofxBaseGui{
	friend class ofPanel;
	
public:	
	ofxSlider() : autoMin(false), autoMax(false) {}
	ofxSlider(string sliderName, ofxParameter<Type> _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);
	ofxSlider* setup(string sliderName, ofxParameter<Type> _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);
	
	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);
	
	virtual void saveToXml(ofxXmlSettings& xml);
	virtual void loadFromXml(ofxXmlSettings& xml);

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		value.addListener(listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		value.removeListener(listener,method);
	}

	virtual ofxParameter<Type> getMin() { return min; }
	virtual void setMin(Type v) { min = v; }

	virtual ofxParameter<Type> getMax() { return max; }
	virtual void setMax(Type v) { max = v; }

	virtual ofxParameter<bool> getAutoMin() { return autoMin; }
	virtual void setAutoMin(bool v) { autoMin = v; }

	virtual ofxParameter<bool> getAutoMax() { return autoMax; }
	virtual void setAutoMax(bool v) { autoMax = v; }

	double operator=(Type v);
	operator Type & ();

	void draw();
	
	ofxParameter<Type> value;

protected:
	ofxParameter<Type> min, max;
	ofxParameter<bool> autoMin, autoMax;

	void setValue(float mx, float my, bool bCheck);
};

typedef ofxSlider<float> ofxFloatSlider;
typedef ofxSlider<int> ofxIntSlider;
