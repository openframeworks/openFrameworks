#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

class ofxSliderType{
	public:
	enum Type{
		/// \brief Default. Shows slider as a vertical or horizontal bar.
		STRAIGHT,
		/// \brief Displays circular slider.
		CIRCULAR
	};
};

template<typename DataType>
class ofxSlider : public ofxBaseGui, public ofxSliderType{
public:

	ofxSlider();
	ofxSlider(const ofJson & config);
	ofxSlider(ofParameter<DataType> _val, const ofJson & config = ofJson());
	ofxSlider(ofParameter<DataType> _val, float width, float height = defaultHeight);
	ofxSlider(const std::string& sliderName, DataType _val, DataType _min, DataType _max, float width = defaultWidth, float height = defaultHeight);

	~ofxSlider();

	void setup();

	void setMin(DataType min);
	DataType getMin();
	void setMax(DataType max);
	DataType getMax();

	void setType(const std::string &type);
	void setType(const Type &type);
	Type getType();

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

	double operator=(DataType v);
	operator const DataType & ();

	ofAbstractParameter & getParameter();

protected:
	virtual void _setConfig(const ofJson & config) override;
	virtual void render() override;

	virtual void resized(ResizeEventArgs&);

	ofParameter<DataType> value;
	ofParameter<ofxSliderType::Type> type;
	virtual bool setValue(float mx, float my, bool bCheck) override;
	virtual void generateDraw() override;
	virtual void generateText();
	virtual void _generateText(std::string valStr);
	void valueChanged(DataType & value);
	ofPath bar;
	ofVboMesh textMesh;

	ofParameter<bool> updateOnReleaseOnly;
	ofParameter<unsigned int> precision;
	/// \brief The Slider orientation.
	bool horizontal;

	bool hasFocus;

	//circular type
	void arcStrip(ofPath & path, ofPoint center, float outer_radius, float inner_radius, float percent);
	float _mouseOffset;


};

typedef ofxSlider<float> ofxFloatSlider;
typedef ofxSlider<int> ofxIntSlider;
