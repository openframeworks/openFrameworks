#pragma once

#include "ofParameter.h"
#include "ofxBaseGui.h"

class ofxToggleType{
	public:
	enum Type {
		/// \brief Shows toggle as checkbox (default).
		CHECKBOX,
		/// \brief Shows toggle as radio toggle.
		RADIO,
		/// \brief Uses the whole element as toggle.
		FULLSIZE
	};
};

class ofxToggle : public ofxBaseGui{
public:

	ofxToggle();
	ofxToggle(const std::string& toggleName);
	ofxToggle(const std::string& toggleName, const ofJson & config);
	ofxToggle(ofParameter<bool>& _bVal, const ofJson & config = ofJson());
	ofxToggle(ofParameter<bool>& _bVal, float width, float height = defaultHeight);
	ofxToggle(const std::string& toggleName, bool _bVal, float width = defaultWidth, float height = defaultHeight);

	~ofxToggle();

	void setup();

	void setType(const std::string &type);
	void setType(const ofxToggleType::Type &type);
	ofxToggleType::Type getType();

	virtual void setWidth(float width) override;

	virtual bool mousePressed(ofMouseEventArgs & args) override;
	virtual bool mouseReleased(ofMouseEventArgs & args) override;

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
	virtual void _setConfig(const ofJson & config) override;
	virtual void render() override;
	ofRectangle checkboxRect;
	ofParameter<bool> value;

	ofParameter<ofxToggleType::Type> type;
	bool hasFocus;

	virtual bool setValue(float mx, float my, bool bCheck);
	virtual void generateDraw();
	void valueChanged(bool & value);
	ofPath fg,cross;
	ofVboMesh textMesh;
};
