#pragma once

#include "ofxToggle.h"
#include "ofParameter.h"

class ofxButton : public ofxToggle{
	friend class ofPanel;
	
public:
	ofxButton* setup(string toggleName, float width = defaultWidth, float height = defaultHeight){
		name = toggleName;
		b.x = 0;
		b.y = 0;
		b.width = width;
		b.height = height;
		currentFrame = 0;			
		bGuiActive = false;
		value = false;
		checkboxRect.set(1, 1, b.height - 2, b.height - 2);
		return this;
	}
	
	virtual void mouseReleased(ofMouseEventArgs & args){
		setValue(args.x, args.y, false);
		bGuiActive = false;		
	}	
	
};
