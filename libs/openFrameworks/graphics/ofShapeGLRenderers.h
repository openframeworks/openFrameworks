#pragma once
#include "ofBaseTypes.h"
#include "ofVboMesh.h"
class ofShape;

class ofVboShapeRenderer: public ofBaseShapeRenderer{
public:
	ofVboShapeRenderer(){}
	~ofVboShapeRenderer(){}
	void setShape(ofShape & shape_);
	void draw();

private:
	ofShape *shape;
	vector<ofVboMesh> vbos;
};


class ofVAShapeRenderer: public ofBaseShapeRenderer{
public:
	ofVAShapeRenderer(){}
	~ofVAShapeRenderer(){}
	void setShape(ofShape & shape_);
	void draw();

private:
	ofShape *shape;
};
