#pragma once

#include "ofPrimitive.h"
#include "ofVbo.h"

class ofVboMesh: public ofPrimitive{
public:
	ofVboMesh();
	ofVboMesh(const ofPrimitive & mom);
	void setUsage(int usage);
	void draw(polyMode drawMode);

private:
	ofVbo vbo;
	int usage;
};
