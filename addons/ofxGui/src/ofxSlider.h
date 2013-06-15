#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

template<typename Type>
class ofxSlider : public ofxBaseGui{
	friend class ofPanel;
	
public:	
	ofxSlider();
	~ofxSlider();
	ofxSlider(ofParameter<Type> _val, float width = defaultWidth, float height = defaultHeight);
	ofxSlider* setup(ofParameter<Type> _val, float width = defaultWidth, float height = defaultHeight);
	ofxSlider* setup(string sliderName, Type _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);
	
	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);
	virtual bool mouseReleased(ofMouseEventArgs & args);

	void setUpdateOnReleaseOnly(bool bUpdateOnReleaseOnly);

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		value.addListener(listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		value.removeListener(listener,method);
	}


	double operator=(Type v);
	operator const Type & ();

	

	ofAbstractParameter & getParameter();

protected:
	void render();
	ofParameter<Type> value;
	bool bUpdateOnReleaseOnly;
	bool bGuiActive;
	bool setValue(float mx, float my, bool bCheck);
	void generateDraw();
	void generateText();
	void valueChanged(Type & value);
	ofPath bg, bar;
	ofVboMesh textMesh;
};

typedef ofxSlider<float> ofxFloatSlider;
typedef ofxSlider<int> ofxIntSlider;
