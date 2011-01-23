#include "ofMesh.h"

//--------------------------------------------------------------
ofMesh::ofMesh(){

}

//--------------------------------------------------------------	
ofMesh::~ofMesh(){}

//--------------------------------------------------------------	
void ofMesh::clear(){

}
/*
 
//--------------------------------------------------------------
int ofMesh::addVertex(ofVec3f pos, ofVec3f normal, ofColor color, ofVec2f texCoord) {
	vertices.push_back(ofVec3f(pos));
	if (bUsingNormals) normals.push_back(ofVec3f(normal));
	if (bUsingColors) colors.push_back(ofColor(color));
	if (bUsingTexCoords) texCoords.push_back(ofVec2f(texCoord));
	return vertices.size()-1;	// keeping 0-begin indexing 
}

//--------------------------------------------------------------	
int ofMesh::addVertex(const ofVec3f& pos) {
	vertices.push_back(pos);
	if(bUsingNormals) normals.push_back(ofVec3f(0,0,0));
	if(bUsingColors) colors.push_back(ofColor(255,255,255));
	if(bUsingTexCoords) texCoords.push_back(ofVec2f(0,0));
	return vertices.size()-1;
}

//--------------------------------------------------------------	
int ofMesh::addFace(int vi1, int vi2, int vi3){
	int indexMax = vertices.size()-1;
	if(max(vi3,max(vi1,vi2)) > indexMax){
		ofLog(OF_LOG_WARNING,"addFace(int, int, int) - vertex index out of range");
		return -1;
	}else{
		faces.push_back(ofFace(vi1,vi2,vi3));
		return faces.size()-1;
	}
}

//--------------------------------------------------------------
int ofMesh::getFaceVertexId(const ofFace& f, int index){
	int isize = f.indices.size();
	if(index < isize){
		return f.indices[index];
	}else{
		ofLog(OF_LOG_WARNING,"getFaceVertexId - face vertex index out of range");
		return -1;
	}
}

//--------------------------------------------------------------
//would like to be reference, but what if there is none?
ofVec3f* ofMesh::getFaceVertex(const ofFace& f, int index){
	int isize = f.indices.size();
	if(index < isize){
		return &vertices[f.indices[index]];
	}else{
		ofLog(OF_LOG_WARNING,"getFaceVertexId - face vertex index out of range");
		return NULL;
	}
}

//--------------------------------------------------------------
int ofMesh::numVertices(){
	return vertices.size();
}

//--------------------------------------------------------------	
int ofMesh::numFaces(){
	return faces.size();
}

//--------------------------------------------------------------	
int ofMesh::numNormals(){
	return normals.size();			
}

//--------------------------------------------------------------		
int ofMesh::numTexCoords(){
	return texCoords.size();			
}

//--------------------------------------------------------------	
int ofMesh::numColors(){
	return colors.size();			
}
*/
