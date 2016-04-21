#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

template<typename Type>
class ofxSlider : public ofxBaseGui{
public:
	struct Config: public ofxBaseGui::Config{
		Config(){}
		Config(const ofxBaseGui::Config & c)
		:ofxBaseGui::Config(c){}
		bool updateOnReleaseOnly = false;
		unsigned int precision = 6;
	};

	ofxSlider();
	~ofxSlider();
	ofxSlider(ofParameter<Type> _val, const Config & config = Config());
	ofxSlider & setup(ofParameter<Type> _val, const Config & config);
	ofxSlider & setup(ofParameter<Type> _val, float width = defaultWidth, float height = defaultHeight);
	ofxSlider & setup(const std::string& sliderName, Type _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);

	void setMin(Type min);
	Type getMin();
	void setMax(Type max);
	Type getMax();

	void setPrecision(int precision);

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



	ofAbstractParameter & getParameter();

protected:
	virtual void render();
	ofParameter<Type> value;
	unsigned int precision;
	bool bUpdateOnReleaseOnly;
	bool bGuiActive;
	bool mouseInside;
	bool setValue(float mx, float my, bool bCheck);
	virtual void generateDraw();
	virtual void generateText();
	void valueChanged(Type & value);
	ofPath bg, bar, border;
	ofVboMesh textMesh;
};

typedef ofxSlider<float> ofxFloatSlider;
typedef ofxSlider<int> ofxIntSlider;
