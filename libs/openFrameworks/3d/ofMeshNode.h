#pragma once
#include "ofMesh.h"
#include "ofNode.h"
#include "ofUtils.h"

class ofMeshNode : public ofNode{
public:
	ofMeshNode();
	~ofMeshNode();
	ofMeshNode& addMesh(ofMesh* m);
	ofMeshNode& addMesh(ofMesh& m);
	void setName(string n);
	string getName();
protected:
	vector<ofMesh*> meshes;
	string name;
};
