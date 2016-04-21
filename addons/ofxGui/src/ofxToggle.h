#pragma once

#include "ofParameter.h"
#include "ofxBaseGui.h"

class ofxToggle : public ofxBaseGui{
public:
	struct Config: public ofxBaseGui::Config{
		Config(){}
		Config(const ofxBaseGui::Config & c)
		:ofxBaseGui::Config(c){}
	};

	ofxToggle();
	ofxToggle(ofParameter<bool>& _bVal, const Config & config = Config());
	~ofxToggle();
	ofxToggle & setup(const std::string& toggleName, const Config & config);
	ofxToggle & setup(ofParameter<bool>& _bVal, const Config & config);
	ofxToggle & setup(ofParameter<bool>& _bVal, float width = defaultWidth, float height = defaultHeight);
	ofxToggle & setup(const std::string& toggleName, bool _bVal, float width = defaultWidth, float height = defaultHeight);

	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);
	virtual bool mouseReleased(ofMouseEventArgs & args);
	virtual bool mouseScrolled(ofMouseEventArgs & args){return false;}


	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}



	bool operator=(bool v);
	operator const bool & ();

	virtual ofAbstractParameter & getParameter();

protected:
	virtual void render();
	ofRectangle checkboxRect;
	ofParameter<bool> value;
	bool bGuiActive;

	virtual bool setValue(float mx, float my, bool bCheck);
	virtual void generateDraw();
	void valueChanged(bool & value);
	ofPath bg,fg,cross;
	ofVboMesh textMesh;
};
