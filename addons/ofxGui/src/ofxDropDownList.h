#pragma once

#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofxBaseGui.h"
#include "ofxGuiGroup.h"

class ofxDropDownList : public ofxBaseGui{
public:
	ofxDropDownList(){};
	~ofxDropDownList();
	ofxDropDownList(string dorpDownListName, vector<string> itemName, int index = 0, float width = defaultWidth, float height = defaultHeight);
	ofxGuiGroup * setup(string dorpDownListName, vector<string> itemName, int index = 0, float width = defaultWidth, float height = defaultHeight);
	
	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);
	virtual bool mouseReleased(ofMouseEventArgs & args);
	
	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}

	virtual ofAbstractParameter & getParameter();
	bool setValue(float mx, float my, bool bCheck);
	void addData(ofAbstractParameter & parameter);
	string getItemName(int index);
	int getItemNum();
	int getCurItemId();
	void setCurItemId(int id);
	
	void minimize();
    void maximize();
    void minimizeAll();
    void maximizeAll();

protected:
	void render();
	void generateDraw();
	void valueChanged(bool & value);

	ofVboMesh textMesh;
	bool bGuiActive;

	ofxGuiGroup dropDownList;
	ofParameterGroup parameters;
	ofParameter<int> value;
	vector<ofParameter<string>> items;   
	vector<ofParameter<bool>> itemValue;
	vector<ofPath> background,foreground;
	vector<ofRectangle> rect;
	vector<ofVboMesh> textMH;
};
