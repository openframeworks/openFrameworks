#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

template<typename Type>
class ofxValueLabel: public ofxBaseGui {
public:

	ofxValueLabel();
	ofxValueLabel(ofParameter<Type> _label, const ofJson & config = ofJson());
	ofxValueLabel(const std::string& labelName, const ofJson & config = ofJson());
	ofxValueLabel(ofParameter<Type> _label, float width, float height);
	ofxValueLabel(const std::string& labelName, const Type & label, const ofJson & config = ofJson());
	ofxValueLabel(const std::string& labelName, const Type & label, float width, float height);

	virtual ~ofxValueLabel();

	void setup();

	virtual void saveTo(ofBaseSerializer& serializer){}
	virtual void loadFrom(ofBaseSerializer& serializer){}


	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		label.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		label.removeListener(listener,method);
	}


	Type operator=(Type v) { label = v; return v; }
	operator const Type & () { return label; }

	ofAbstractParameter & getParameter();

protected:
	virtual void render() override;
	ofParameter<Type> label;
	virtual void generateDraw() override;
	void valueChanged(Type & value);
	bool setValue(float mx, float my){return false;}
	ofVboMesh textMesh;
};

typedef ofxValueLabel<std::string> ofxLabel;
typedef ofxValueLabel<int> ofxIntLabel;
typedef ofxValueLabel<float> ofxFloatLabel;
typedef ofxValueLabel<bool> ofxBoolLabel;
