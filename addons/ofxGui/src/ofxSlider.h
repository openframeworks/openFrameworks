#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

template<typename Type>
class ofxSlider : public ofxBaseGui{
public:

	ofxSlider();
	ofxSlider(const ofJson & config);
	ofxSlider(ofParameter<Type> _val, const ofJson & config = ofJson());
	ofxSlider(ofParameter<Type> _val, float width, float height = defaultHeight);
	ofxSlider(const std::string& sliderName, Type _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);

	~ofxSlider();

	void setup();

	void setMin(Type min);
	Type getMin();
	void setMax(Type max);
	Type getMax();

	void setPrecision(int precision);

	void setUpdateOnReleaseOnly(bool bUpdateOnReleaseOnly);

	virtual bool mousePressed(ofMouseEventArgs & args) override;
	virtual bool mouseDragged(ofMouseEventArgs & args) override;
	virtual bool mouseReleased(ofMouseEventArgs & args) override;
	virtual bool mouseScrolled(ofMouseEventArgs & args) override;

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
	virtual void _setConfig(const ofJson & config) override;
	virtual void render() override;

	virtual void resized(ResizeEventArgs&);

	ofParameter<Type> value;
	virtual bool setValue(float mx, float my, bool bCheck) override;
	virtual void generateDraw() override;
	virtual void generateText();
	virtual void _generateText(std::string valStr);
	void valueChanged(Type & value);
	ofPath bar;
	ofVboMesh textMesh;

	ofParameter<bool> updateOnReleaseOnly;
	ofParameter<unsigned int> precision;
	/// \brief The Slider orientation.
	bool horizontal;

	bool hasFocus;

};

typedef ofxSlider<float> ofxFloatSlider;
typedef ofxSlider<int> ofxIntSlider;
