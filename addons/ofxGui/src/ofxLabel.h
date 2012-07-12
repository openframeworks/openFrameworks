#pragma once

#include "ofxBaseGui.h"

class ofxLabel: public ofxBaseGui {
public:
    ofxLabel(){}
    virtual ~ofxLabel() {}
    ofxLabel * setup(string labelName, string _label, float width = defaultWidth, float height = defaultHeight);

    void draw();

    // Abstract methods we must implement, but have no need for!
    virtual void mouseMoved(ofMouseEventArgs & args){}
    virtual void mousePressed(ofMouseEventArgs & args){}
    virtual void mouseDragged(ofMouseEventArgs & args){}
    virtual void mouseReleased(ofMouseEventArgs & args){}

	virtual void saveTo(ofBaseSerializer& serializer){};
	virtual void loadFrom(ofBaseSerializer& serializer){};

    virtual void setValue(float mx, float my, bool bCheckBounds){}

    void setLabel(string _label) { label = _label; }
    string getLabel()            { return label; }

    string operator=(string v) { label = v; return v; }
    operator string & ()       { return label; }

	ofAbstractParameter & getParameter();

protected:
    ofParameter<string> label;
};
