#include "ofMeshElement.h"

//--------------------------------------------------------------
ofMeshElement::ofMeshElement(){
	mode = OF_TRIANGLES_ELEMENT;
}

//--------------------------------------------------------------
ofMeshElement::~ofMeshElement(){
}

//ADDERS

//--------------------------------------------------------------
void ofMeshElement::addVertex(const ofVec3f& v){
	//TODO: figure out if we add to all other arrays to match
	vertices.push_back(v);
}

//--------------------------------------------------------------
void ofMeshElement::addColor(const ofColor& c){
	//TODO: figure out if we add to all other arrays to match
	colors.push_back(c);
}

//--------------------------------------------------------------
void ofMeshElement::addNormal(const ofVec3f& n){
	//TODO: figure out if we add to all other arrays to match
	normals.push_back(n);
}

//--------------------------------------------------------------
void ofMeshElement::addTexCoord(const ofVec2f& t){
	//TODO: figure out if we add to all other arrays to match
	texCoords.push_back(t);
}

//--------------------------------------------------------------
void ofMeshElement::addIndex(int i){
	indices.push_back(i);
}

//GETTERS

//--------------------------------------------------------------
int ofMeshElement::getNumVertices(){
	return vertices.size();
}

//--------------------------------------------------------------
int ofMeshElement::getNumColors(){
	return colors.size();
}

//--------------------------------------------------------------
int ofMeshElement::getNumNormals(){
	return normals.size();
}

//--------------------------------------------------------------
int ofMeshElement::getNumTexCoords(){
	return texCoords.size();
}

//--------------------------------------------------------------
int ofMeshElement::getNumIndices(){
	return indices.size();
}

/*
//--------------------------------------------------------------
int ofMeshElement::getNumIndicesSolid(){
	return indicesSolid.size();
}

//--------------------------------------------------------------
int ofMeshElement::getNumIndicesWire(){
	return indicesWire.size();
}
 */

//--------------------------------------------------------------
ofVec3f* ofMeshElement::getVerticesPointer(){
	return &vertices[0];
}

//--------------------------------------------------------------
ofColor* ofMeshElement::getColorsPointer(){
	return &colors[0];
}

//--------------------------------------------------------------
ofVec3f* ofMeshElement::getNormalsPointer(){
	return &normals[0];
}

//--------------------------------------------------------------
ofVec2f* ofMeshElement::getTexCoordsPointer(){
	return &texCoords[0];
}

//--------------------------------------------------------------
GLuint* ofMeshElement::getIndexPointer(){
	return &indices[0];
}

/*
//--------------------------------------------------------------
GLuint* ofMeshElement::getSolidIndexPointer(){
	return &indicesSolid[0];
}

//--------------------------------------------------------------
GLuint* ofMeshElement::getWireIndexPointer(){
	return &indicesWire[0];
}
 */

/*
//--------------------------------------------------------------
vector<int>& ofMeshElement::getFace(int faceNum){
	switch(mode){
		//GL_QUADS
		indices[faceNum*4+0];
		indices[faceNum*4+1];
		indices[faceNum*4+2];
		indices[faceNum*4+3];

		//GL_TRIANGLES
		indices[faceNum*3+0];
		indices[faceNum*3+1];
		indices[faceNum*3+2];

		//GL_TRIANGLE_FAN
		// 1 element per fan
		indices[0];
		indices[faceNum+1];
		indices[faceNum+2];

		//GL_TRIANGLE_STRIP
		// 1 element per strip
		indices[faceNum+0];
		indices[faceNum+1];
		indices[faceNum+2];
		default:break;
	}
}
 */

//SETTERS

//--------------------------------------------------------------
void ofMeshElement::setMode(int m){
	mode = m;
}


//--------------------------------------------------------------
void ofMeshElement::setupIndices(){
	indices.clear();
	switch(mode){
		case(OF_TRIANGLES_ELEMENT):
			for(int i = 0; i < vertices.size();i++){
				indices.push_back((GLuint)i);
				indices.push_back((GLuint)i+1);
				indices.push_back((GLuint)i+2);
			}
		break;
		case(OF_TRIANGLE_STRIP_ELEMENT):
			for(int i = 0; i < vertices.size();i++){
				indices.push_back((GLuint)i);
			}
		break;
		case(OF_TRIANGLE_FAN_ELEMENT):
			indices.push_back((GLuint)0);
			for(int i = 1; i < vertices.size();i++){
				indices.push_back((GLuint)i);
			}
		break;
		default:break;
	}
}

