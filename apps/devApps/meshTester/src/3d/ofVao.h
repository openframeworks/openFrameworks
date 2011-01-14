#pragma once
#include "ofVbo.h"

enum{
	OF_VAO_POINTS,
	OF_VAO_LINES,
	OF_VAO_TRIANGLES
};

class ofVao{
public:
	
	ofVao();
	~ofVao();
	void setIndexData(const int * indices, int total, int buffers);
	
    ofVbo * vbo;
	
	void bind();
	
private:
	GLuint indexId;
	GLuint handle;	
};