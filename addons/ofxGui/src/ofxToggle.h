#pragma once

#include "ofParameter.h"
#include "ofxBaseGui.h"

class ofxToggle : public ofxBaseGui{
public:

	enum class Type
	{
		/// \brief Shows toggle as checkbox (default).
		CHECKBOX,
		/// \brief Shows toggle as radio toggle.
		RADIO,
		/// \brief Uses the whole element as toggle.
		FULLSIZE

	};

	ofxToggle();
	ofxToggle(const std::string& toggleName);
	ofxToggle(const std::string& toggleName, const ofJson & config);
	ofxToggle(ofParameter<bool>& _bVal, const ofJson & config = ofJson());
	ofxToggle(ofParameter<bool>& _bVal, float width, float height = defaultHeight);
	ofxToggle(const std::string& toggleName, bool _bVal, float width = defaultWidth, float height = defaultHeight);

	~ofxToggle();

	void setup();

	void setType(const std::string &type);
	void setType(const Type &type);
	ofxToggle::Type getType();

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

	ofParameter<ofxToggle::Type> type;
	bool hasFocus;

	virtual bool setValue(float mx, float my, bool bCheck);
	virtual void generateDraw();
	void valueChanged(bool & value);
	ofPath fg,cross;
	ofVboMesh textMesh;
};
