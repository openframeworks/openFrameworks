#pragma once

#include "ofAppBaseWindow.h"

class ofWindow: public ofAppBaseWindow{
public:
	ofWindow();
	~ofWindow();  
    
	virtual void enableContext() = 0;
	
	void update();
	void draw();
protected:
private:
};
