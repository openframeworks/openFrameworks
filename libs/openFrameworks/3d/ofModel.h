#pragma once

#include "ofMesh.h"
#include "ofUtils.h"

class ofModel{
public:
	ofModel();
	~ofModel();
	
	vector<ofMesh> meshes;
};
