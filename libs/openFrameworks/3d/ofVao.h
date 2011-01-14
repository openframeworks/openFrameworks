#pragma once
#include "ofVbo.h"

class ofVao{
public:
	
	ofVao();
	~ofVao();
	
    ofVbo * vbo;
	
	void bind();
	
private:
	GLuint handle;	
};