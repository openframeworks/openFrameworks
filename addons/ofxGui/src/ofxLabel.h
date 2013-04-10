#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

class ofxLabel: public ofxBaseGui {
public:
    ofxLabel(){}
    ofxLabel(ofParameter<string> _label, float width = defaultWidth, float height = defaultHeight);
    virtual ~ofxLabel();

    ofxLabel * setup(ofParameter<string> _label, float width = defaultWidth, float height = defaultHeight);
    ofxLabel * setup(string labelName, string label, float width = defaultWidth, float height = defaultHeight);

    // Abstract methods we must implement, but have no need for!
    virtual bool mouseMoved(ofMouseEventArgs & args){return false;}
    virtual bool mousePressed(ofMouseEventArgs & args){return false;}
    virtual bool mouseDragged(ofMouseEventArgs & args){return false;}
    virtual bool mouseReleased(ofMouseEventArgs & args){return false;}

	virtual void saveTo(ofBaseSerializer& serializer){};
	virtual void loadFrom(ofBaseSerializer& serializer){};


    template<class ListenerClass>
    void addListener(ListenerClass * listener, void ( ListenerClass::*method )(string&)){
        label.addListener(listener,method);
    }

    template<class ListenerClass>
    void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(string&)){
        label.removeListener(listener,method);
    }

    string operator=(string v) { label = v; return v; }
    operator const string & ()       { return label; }

    ofAbstractParameter & getParameter();

protected:
    void render();
    ofParameter<string> label;
    void generateDraw();
    void valueChanged(string & value);
    bool setValue(float mx, float my, bool bCheckBounds){return false;}
    ofPath bg;
    ofVboMesh textMesh;
};
