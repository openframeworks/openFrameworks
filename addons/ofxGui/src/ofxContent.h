#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"
#include "ofxPanel.h"
class ofxContent: public ofxBaseGui {
public:
	ofxContent(){}
    ofxContent(string contentName, ofBaseDraws &content, float width = defaultWidth, float height = defaultHeight);
	virtual ~ofxContent();

    ofxContent * setup(string contentName, ofBaseDraws &content, float width = defaultWidth, float height = defaultHeight);

    // Abstract methods we must implement, but have no need for!
    virtual bool mouseMoved(ofMouseEventArgs & args){return false;}
    virtual bool mousePressed(ofMouseEventArgs & args);
    virtual bool mouseDragged(ofMouseEventArgs & args){return false;}
    virtual bool mouseReleased(ofMouseEventArgs & args){return false;}

	virtual void saveTo(ofBaseSerializer& serializer){};
	virtual void loadFrom(ofBaseSerializer& serializer){};


	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}

    ofAbstractParameter & getParameter();

protected:
    void render();
    ofParameter<string> label;
	ofPath fg;
    void generateDraw();
    void valueChanged(bool & value);
    bool setValue(float mx, float my, bool bCheckBounds){return false;}
    ofPath bg;
    ofVboMesh textMesh;
	ofBaseDraws * content;
	string name;

public:
	static ofBaseDraws * getCurrentContent();
	static ofBaseDraws * contentSelect;
	static string nameSelect;
	ofParameter<bool> value;
};
