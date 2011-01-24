#include "ofMeshElement.h"

//--------------------------------------------------------------
ofMeshElement::ofMeshElement(){
	mode = OF_TRIANGLES_ELEMENT;
	bVertsChanged = true;
	bColorsChanged = true;
	bNormalsChanged = true;
	bTexCoordsChanged = true;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
ofMeshElement::~ofMeshElement(){
}

//--------------------------------------------------------------
void ofMeshElement::clear(){
	bVertsChanged = true;
	bColorsChanged = true;
	bNormalsChanged = true;
	bTexCoordsChanged = true;
	bIndicesChanged = true;
	
	vertices.clear();
	colors.clear();
	normals.clear();
	texCoords.clear();
	indices.clear();
}

//--------------------------------------------------------------
bool ofMeshElement::haveVertsChanged(){
	if(bVertsChanged){
		bVertsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMeshElement::haveColorsChanged(){
	if(bColorsChanged){
		bColorsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMeshElement::haveNormalsChanged(){
	if(bNormalsChanged){
		bNormalsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMeshElement::haveTexCoordsChanged(){
	if(bTexCoordsChanged){
		bTexCoordsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMeshElement::haveIndicesChanged(){
	if(bIndicesChanged){
		bIndicesChanged = false;
		return true;
	}else{
		return false;
	}
}

//ADDERS

//--------------------------------------------------------------
void ofMeshElement::addVertex(const ofVec3f& v){
	//TODO: figure out if we add to all other arrays to match
	vertices.push_back(v);
	indices.push_back(vertices.size()-1);
	bVertsChanged = true;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::addVertices(const vector<ofVec3f>& verts){
	vertices.insert(vertices.end(),verts.begin(),verts.end());
	setupIndices();
	bVertsChanged = true;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::addColor(const ofColor& c){
	//TODO: figure out if we add to all other arrays to match
	colors.push_back(c);
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::addColors(const vector<ofColor>& cols){
	colors.insert(colors.end(),cols.begin(),cols.end());
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::addNormal(const ofVec3f& n){
	//TODO: figure out if we add to all other arrays to match
	normals.push_back(n);
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::addNormals(const vector<ofVec3f>& norms){
	normals.insert(normals.end(),norms.begin(),norms.end());
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::addTexCoord(const ofVec2f& t){
	//TODO: figure out if we add to all other arrays to match
	texCoords.push_back(t);
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::addTexCoords(const vector<ofVec2f>& tCoords){
	texCoords.insert(texCoords.end(),tCoords.begin(),tCoords.end());
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::addIndex(int i){
	indices.push_back(i);
	bIndicesChanged = true;
}


//--------------------------------------------------------------
void ofMeshElement::addIndices(const vector<GLuint>& inds){
	indices.insert(indices.end(),inds.begin(),inds.end());
	bIndicesChanged = true;
}

//GETTERS
//--------------------------------------------------------------
ofTriangleMode ofMeshElement::getMode(){
	return mode;
}

//--------------------------------------------------------------
ofVec3f ofMeshElement::getVertex(int i){
	return vertices[i];
}

//--------------------------------------------------------------
ofVec3f ofMeshElement::getNormal(int i){
	return normals[i];
}

//--------------------------------------------------------------
ofColor ofMeshElement::getColor(int i){
	return colors[i];
}

//--------------------------------------------------------------
ofVec2f ofMeshElement::getTexCoord(int i){
	return texCoords[i];
}

//--------------------------------------------------------------
int ofMeshElement::getNumVertices() const{
	return vertices.size();
}

//--------------------------------------------------------------
int ofMeshElement::getNumColors() const{
	return colors.size();
}

//--------------------------------------------------------------
int ofMeshElement::getNumNormals() const{
	return normals.size();
}

//--------------------------------------------------------------
int ofMeshElement::getNumTexCoords() const{
	return texCoords.size();
}

//--------------------------------------------------------------
int ofMeshElement::getNumIndices() const{
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
void ofMeshElement::setMode(ofTriangleMode m){
	bIndicesChanged = true;
	mode = m;
}

//--------------------------------------------------------------
void ofMeshElement::setVertex(int index, const ofVec3f& v){
	vertices[index] = v;
	bVertsChanged = true;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::setNormal(int index, const ofVec3f& n){
	normals[index] = n;
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::setColor(int index, const ofColor& c){
	colors[index] = c;
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::setTexCoord(int index, const ofVec2f& t){
	texCoords[index] = t;
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::setIndex(int i, int val){
	indices[i] = val;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMeshElement::setupIndices(){
	bIndicesChanged = true;
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
