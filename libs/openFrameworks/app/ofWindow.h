#pragma once

#include "ofAppBaseWindow.h"

class ofWindow: public ofAppBaseWindow{
public:
	ofWindow();
	~ofWindow();  
    
	virtual bool open() = 0;
protected:
private:
};
