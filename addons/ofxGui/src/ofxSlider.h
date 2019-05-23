#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"
#include "ofxInputField.h"
#include "ofxGuiUtils.h"

template<typename Type>
class ofxSlider : public ofxBaseGui{
	friend class ofPanel;
	
public:	
	ofxSlider();
	~ofxSlider();
	ofxSlider(ofParameter<Type> _val, float width = defaultWidth, float height = defaultHeight);
	ofxSlider* setup(ofParameter<Type> _val, float width = defaultWidth, float height = defaultHeight);
	ofxSlider* setup(const std::string& sliderName, Type _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);
	
	void setMin(Type min);
	Type getMin();
	void setMax(Type max);
	Type getMax();

	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);
	virtual bool mouseReleased(ofMouseEventArgs & args);
    virtual bool mouseScrolled(ofMouseEventArgs & args);

	void setUpdateOnReleaseOnly(bool bUpdateOnReleaseOnly);


	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}



	double operator=(Type v);
	operator const Type & ();


	virtual void setPosition(const glm::vec3 & p);
	virtual void setPosition(float x, float y);
	virtual void setSize(float w, float h);
	virtual void setShape(ofRectangle r);
	virtual void setShape(float x, float y, float w, float h);
	
	ofAbstractParameter & getParameter();

private:

	ofxGuiRectMesh bg, bar;

protected:
	virtual void render();
	ofParameter<Type> value;
	bool bUpdateOnReleaseOnly;
	bool bGuiActive;
	bool mouseInside;
	bool overlappingLabel;
	bool setValue(float mx, float my, bool bCheck);
	virtual void generateDraw();
	virtual void generateText();
	void valueChanged(Type & value);

	ofVboMesh textMesh;
	ofxInputField<Type> input{ofxInputField<Type>::InsideSlider};

	enum State{
		Slider,
		Input,
	} state = Slider;
	ofEventListener listener;
	float errorTime = 0;
};

typedef ofxSlider<float> ofxFloatSlider;
typedef ofxSlider<int> ofxIntSlider;
