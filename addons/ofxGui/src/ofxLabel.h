#pragma once

#include "ofxBaseGui.h"

class ofxLabel: public ofxBaseGui {
public:
    ofxLabel(){}
    virtual ~ofxLabel() {}
    // We need a string version here as literals are not getting converted to
    // ofxParameter<string>. For status.setup("Status", "") we get:
    // error: no matching function for call to ‘ofxLabel::setup(const char [7], const char [1])’
    // note:   no known conversion for argument 2 from ‘const char [1]’ to ‘ofxParameter<std::basic_string<char> >’
    ofxLabel * setup(string labelName, string _label, float width = defaultWidth, float height = defaultHeight);
    ofxLabel * setup(string labelName, ofxParameter<string> _label, float width = defaultWidth, float height = defaultHeight);

    // Abstract methods we must implement, but have no need for!
    virtual void mouseMoved(ofMouseEventArgs & args){}
    virtual void mousePressed(ofMouseEventArgs & args){}
    virtual void mouseDragged(ofMouseEventArgs & args){}
    virtual void mouseReleased(ofMouseEventArgs & args){}

	virtual void saveTo(ofBaseSerializer& serializer){};
	virtual void loadFrom(ofBaseSerializer& serializer){};

    virtual void setValue(float mx, float my, bool bCheckBounds){}

    void draw();

    template<class ListenerClass>
    void addListener(ListenerClass * listener, void ( ListenerClass::*method )(string&)){
        label.addListener(listener,method);
    }

    template<class ListenerClass>
    void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(string&)){
        label.removeListener(listener,method);
    }

    string operator=(string v) { label = v; return v; }
    operator string & ()       { return label; }

protected:
    ofxParameter<string> label;
};
