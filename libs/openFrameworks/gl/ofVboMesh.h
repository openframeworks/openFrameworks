#pragma once

#include "ofMesh.h"
#include "ofVbo.h"

class ofVboMesh: public ofMesh{
public:
	using ofMesh::draw;
	ofVboMesh();
	ofVboMesh(const ofMesh & mom);
	void setUsage(int usage);

protected:
	void draw(ofPolyRenderMode drawMode);

private:
	ofVbo vbo;
	int usage;
};
