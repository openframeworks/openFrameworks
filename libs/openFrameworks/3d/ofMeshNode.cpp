#include "ofMeshNode.h"

//--------------------------------------------------------------
ofMeshNode::ofMeshNode(){
}

//--------------------------------------------------------------
ofMeshNode::~ofMeshNode(){
}

//--------------------------------------------------------------
ofMeshNode& ofMeshNode::addMesh(ofMesh* m){
	meshes.push_back(m);
	return *this;
}

//--------------------------------------------------------------
ofMeshNode& ofMeshNode::addMesh(ofMesh& m){
	meshes.push_back(&m);
	return *this;
}

//--------------------------------------------------------------
void ofMeshNode::setName(string n){
	name = n;
}

//--------------------------------------------------------------
string ofMeshNode::getName(){
	return name;
}
