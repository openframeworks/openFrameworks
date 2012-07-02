#pragma once

#include "ofParameter.h"
#include "ofxBaseGui.h"

class ofxGuiImage: public ofxBaseGui {
public:
    ofxGuiImage() : showName(true), imgBorder(1) {}
    virtual ~ofxGuiImage(){}

    // Set size explicitly (includes name if showName is true).
    ofxGuiImage * setup(string _name, ofImage* _imgPtr, bool _showName, float width, float height );
    // Set width, scale height based on img and showName to preserve aspect.
    ofxGuiImage * setup(string _name, ofImage* _imgPtr, bool _showName = true, float width = defaultWidth );

	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);

	virtual void saveToXml(ofxXmlSettings& xml) ;
	virtual void loadFromXml(ofxXmlSettings& xml);

	void draw();

    template<class ListenerClass>
    void addListener(ListenerClass * listener, void ( ListenerClass::*method )(bool&)){
        value.addListener(listener,method);
    }

    template<class ListenerClass>
    void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(bool&)){
        value.removeListener(listener,method);
    }

    ofxParameter<int> getImgBorder() { return imgBorder; }
    void setImgBorder(int v)         { imgBorder = v; }

	bool operator=(bool v) { value = v; return v; }
	operator bool & ()     { return value; }

protected:
	ofImage* imgPtr;
    ofxParameter<bool> value;
	void setValue(float mx, float my, bool bCheck);
	bool showName;
	ofxParameter<int> imgBorder; // pixels of border around img
};
