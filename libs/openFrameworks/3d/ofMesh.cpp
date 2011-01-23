#include "ofMesh.h"

//--------------------------------------------------------------
ofMesh::ofMesh(){

}

//--------------------------------------------------------------	
ofMesh::~ofMesh(){}

//--------------------------------------------------------------	
void ofMesh::clear(){
	elements.clear();
}

void ofMesh::addElement(ofTriangleMode mode, const vector<ofVec3f>& verts){
	elements.push_back(ofMeshElement());
	elements.back().setMode(mode);
	elements.back().addVertices(verts);
}