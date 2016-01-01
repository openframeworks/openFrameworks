#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

template<typename Type>
class ofxValueLabel: public ofxBaseGui {
public:
	struct Config: public ofxBaseGui::Config{
		Config(){}
		Config(const ofxBaseGui::Config & c)
		:ofxBaseGui::Config(c){}
	};

	ofxValueLabel();

	ofxValueLabel(ofParameter<Type> _label, const Config & config = Config());
	virtual ~ofxValueLabel();

	ofxValueLabel & setup(ofParameter<Type> _label, const Config & config);
	ofxValueLabel & setup(ofParameter<Type> _label, float width = defaultWidth, float height = defaultHeight);
	ofxValueLabel & setup(const std::string& labelName, const Type & label, const Config & config);
	ofxValueLabel & setup(const std::string& labelName, const Type & label, float width = defaultWidth, float height = defaultHeight);

	// Abstract methods we must implement, but have no need for!
	virtual bool mouseMoved(ofMouseEventArgs & args){return false;}
	virtual bool mousePressed(ofMouseEventArgs & args){return false;}
	virtual bool mouseDragged(ofMouseEventArgs & args){return false;}
	virtual bool mouseReleased(ofMouseEventArgs & args){return false;}
	virtual bool mouseScrolled(ofMouseEventArgs & args){return false;}

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
	operator const Type & ()       { return label; }

	ofAbstractParameter & getParameter();

protected:
	void render();
	ofParameter<Type> label;
	void generateDraw();
	void valueChanged(Type & value);
	bool setValue(float mx, float my, bool bCheckBounds){return false;}
	ofPath bg;
	ofVboMesh textMesh;
};

typedef ofxValueLabel<std::string> ofxLabel;
typedef ofxValueLabel<int> ofxIntLabel;
typedef ofxValueLabel<float> ofxFloatLabel;
typedef ofxValueLabel<bool> ofxBoolLabel;
