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

void ofMesh::addElement(ofPrimitiveMode mode, const vector<ofVec3f>& verts){
	elements.push_back(ofVertexData());
	elements.back().setMode(mode);
	elements.back().addVertices(verts);
}