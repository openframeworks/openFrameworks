#pragma once
#include "ofBaseTypes.h"
#include "ofVboMesh.h"
class ofShape;

class ofVboRenderer: public ofBaseRenderer{
public:
	ofVboRenderer(){}
	~ofVboRenderer(){}
	void draw(ofShape & shape);
	void draw(ofPath & path);
	bool rendersPathDirectly(){
		return false;
	}

private:
	vector<ofVboMesh> vbos;
	ofVbo vboOutline;
};


class ofVARenderer: public ofBaseRenderer{
public:
	ofVARenderer(){}
	~ofVARenderer(){}
	void draw(ofShape & shape);
	void draw(ofPath & path);
	bool rendersPathDirectly(){
		return false;
	}

};
