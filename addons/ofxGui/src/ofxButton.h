#pragma once

#include "ofxToggle.h"
#include "ofParameter.h"

class ofxButton : public ofxToggle{
	friend class ofPanel;
	
public:

	ofxButton* setup(string toggleName, float width = defaultWidth, float height = defaultHeight);

	virtual void mouseReleased(ofMouseEventArgs & args);
	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);
	

	virtual void saveToXml(ofxXmlSettings& xml) {} // does't make sense to save buttons;
	virtual void loadFromXml(ofxXmlSettings& xml) {}

};
