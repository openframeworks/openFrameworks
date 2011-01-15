#include "ofMesh.h"

//--------------------------------------------------------------	
ofFace::ofFace(){
}

//--------------------------------------------------------------
ofFace::ofFace(const ofFace& f){
	indices = f.indices;
}

//--------------------------------------------------------------
ofFace::ofFace(int vi0, int vi1, int vi2){
	indices.push_back(vi0);
	indices.push_back(vi1);
	indices.push_back(vi2);
}

//--------------------------------------------------------------
ofFace::~ofFace(){}

/* unchecked operators
 inline bool operator==(const ofFace& f){
 if (f.indices.size()!=indices.size()){
 return false;
 }else{
 return equal(f.indices.begin(),f.indices.end(),indices.begin());
 }
 }
 
 inline bool operator!= (const ofFace& f){
 return !((*this)==f);
 }
 */

//--------------------------------------------------------------	
bool ofFace::operator=(const ofFace& f){
	//indices.assign(f.indices.begin(), f.indices.end());
	indices = f.indices;
}

//--------------------------------------------------------------
ofMesh::ofMesh(){
	name = "";
	bUsingNormals = false;
	bUsingColors = false;
	bUsingTexCoords = false;
	faceType = 3;
}

//--------------------------------------------------------------	
ofMesh::~ofMesh(){}

//--------------------------------------------------------------	
void ofMesh::clear(){
	vertices.clear();
	normals.clear();
	texCoords.clear();
	faces.clear();
	colors.clear();
}

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
/*
 int addFace(){
 int indexMax = vertices.size()-1;
 if(indexMax>=2){
 faces.push_back(ofFace(indexMax,indexMax-1,indexMax-2));
 return faces.size()-1;
 }else{
 ofLog(OF_LOG_WARNING,"addFace() requires at least three existing vertices");
 return -1;
 }
 }
 */

//--------------------------------------------------------------

void ofMesh::flipNormals(){
	it = normals.begin();
	while(it!=normals.end()){
		(*it) *= -1;
		++it;
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

/*
 int numTangents(){
 return tangents.size();
 }
 
 int numBiTangents(){
 return biTangents.size();
 }
 
 int getNumColorMaps(){
 return colors.size();
 }
 
 int numTexMaps(){
 return texCoords.size();
 }
 */