#include "ofVertexData.h"
#include "ofGraphics.h"

//--------------------------------------------------------------
ofVertexData::ofVertexData(){
	mode = OF_TRIANGLES_MODE;
	bVertsChanged = true;
	bColorsChanged = true;
	bNormalsChanged = true;
	bTexCoordsChanged = true;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
ofVertexData::ofVertexData(ofPrimitiveMode mode, const vector<ofVec3f>& verts){
	setMode(mode);
	addVertices(verts);
}

//--------------------------------------------------------------
ofVertexData::~ofVertexData(){
}

//--------------------------------------------------------------
void ofVertexData::clear(){
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
bool ofVertexData::haveVertsChanged(){
	if(bVertsChanged){
		bVertsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofVertexData::haveColorsChanged(){
	if(bColorsChanged){
		bColorsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofVertexData::haveNormalsChanged(){
	if(bNormalsChanged){
		bNormalsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofVertexData::haveTexCoordsChanged(){
	if(bTexCoordsChanged){
		bTexCoordsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofVertexData::haveIndicesChanged(){
	if(bIndicesChanged){
		bIndicesChanged = false;
		return true;
	}else{
		return false;
	}
}

//ADDERS

//--------------------------------------------------------------
void ofVertexData::addVertex(const ofVec3f& v){
	vertices.push_back(v);
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addVertices(const vector<ofVec3f>& verts){
	vertices.insert(vertices.end(),verts.begin(),verts.end());
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addVertices(const ofVec3f* verts, int amt){
	for (int i = 0; i < amt;i++){
		addVertex(verts[i]);
	}
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addColor(const ofColor& c){
	colors.push_back(c);
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addColors(const vector<ofColor>& cols){
	colors.insert(colors.end(),cols.begin(),cols.end());
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addColors(const ofColor* cols, int amt){
	for (int i = 0; i < amt;i++){
		addColor(cols[i]);
	}
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addNormal(const ofVec3f& n){
	normals.push_back(n);
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addNormals(const vector<ofVec3f>& norms){
	normals.insert(normals.end(),norms.begin(),norms.end());
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addNormals(const ofVec3f* norms, int amt){
	for (int i = 0; i < amt;i++){
		addNormal(norms[i]);
	}
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addTexCoord(const ofVec2f& t){
	//TODO: figure out if we add to all other arrays to match
	texCoords.push_back(t);
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addTexCoords(const vector<ofVec2f>& tCoords){
	texCoords.insert(texCoords.end(),tCoords.begin(),tCoords.end());
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addTexCoords(const ofVec2f* tCoords, int amt){
	for (int i = 0; i < amt;i++){
		addTexCoord(tCoords[i]);
	}
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addIndex(int i){
	indices.push_back(GLuint(i));
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addIndices(const vector<GLuint>& inds){
	indices.insert(indices.end(),inds.begin(),inds.end());
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::addIndices(const GLuint* inds, int amt){
	for (int i = 0; i < amt;i++){
		addIndex(inds[i]);
	}
	bIndicesChanged = true;
}

//GETTERS
//--------------------------------------------------------------
ofPrimitiveMode ofVertexData::getMode() const{
	return mode;
}

//--------------------------------------------------------------
ofVec3f ofVertexData::getVertex(int i){
	return vertices[i];
}

//--------------------------------------------------------------
ofVec3f ofVertexData::getNormal(int i){
	return normals[i];
}

//--------------------------------------------------------------
ofColor ofVertexData::getColor(int i){
	return colors[i];
}

//--------------------------------------------------------------
ofVec2f ofVertexData::getTexCoord(int i){
	return texCoords[i];
}

//--------------------------------------------------------------
int ofVertexData::getNumVertices() const{
	return vertices.size();
}

//--------------------------------------------------------------
int ofVertexData::getNumColors() const{
	return colors.size();
}

//--------------------------------------------------------------
int ofVertexData::getNumNormals() const{
	return normals.size();
}

//--------------------------------------------------------------
int ofVertexData::getNumTexCoords() const{
	return texCoords.size();
}

//--------------------------------------------------------------
int ofVertexData::getNumIndices() const{
	return indices.size();
}

/*
//--------------------------------------------------------------
int ofVertexData::getNumIndicesSolid(){
	return indicesSolid.size();
}

//--------------------------------------------------------------
int ofVertexData::getNumIndicesWire(){
	return indicesWire.size();
}
 */

//--------------------------------------------------------------
float* ofVertexData::getVerticesPointer(){
	return &vertices[0].x;
}

//--------------------------------------------------------------
float* ofVertexData::getColorsPointer(){
	return &colors[0].r;
}

//--------------------------------------------------------------
float* ofVertexData::getNormalsPointer(){
	return &normals[0].x;
}

//--------------------------------------------------------------
float* ofVertexData::getTexCoordsPointer(){
	return &texCoords[0].x;
}

//--------------------------------------------------------------
GLuint* ofVertexData::getIndexPointer(){
	return &indices[0];
}


//--------------------------------------------------------------
const float* ofVertexData::getVerticesPointer() const{
	return &vertices[0].x;
}

//--------------------------------------------------------------
const float* ofVertexData::getColorsPointer() const{
	return &colors[0].r;
}

//--------------------------------------------------------------
const float* ofVertexData::getNormalsPointer() const{
	return &normals[0].x;
}

//--------------------------------------------------------------
const float* ofVertexData::getTexCoordsPointer() const{
	return &texCoords[0].x;
}

//--------------------------------------------------------------
const GLuint* ofVertexData::getIndexPointer() const{
	return &indices[0];
}

/*
//--------------------------------------------------------------
GLuint* ofVertexData::getSolidIndexPointer(){
	return &indicesSolid[0];
}

//--------------------------------------------------------------
GLuint* ofVertexData::getWireIndexPointer(){
	return &indicesWire[0];
}
 */

/*
//--------------------------------------------------------------
vector<int>& ofVertexData::getFace(int faceNum){
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
void ofVertexData::setMode(ofPrimitiveMode m){
	bIndicesChanged = true;
	mode = m;
}

//--------------------------------------------------------------
void ofVertexData::setVertex(int index, const ofVec3f& v){
	vertices[index] = v;
	bVertsChanged = true;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::setNormal(int index, const ofVec3f& n){
	normals[index] = n;
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::setColor(int index, const ofColor& c){
	colors[index] = c;
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::setTexCoord(int index, const ofVec2f& t){
	texCoords[index] = t;
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::setIndex(int i, int val){
	indices[i] = val;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofVertexData::setName(string name_){
	name = name_;
}

//--------------------------------------------------------------
void ofVertexData::setupIndicesAuto(){
	bIndicesChanged = true;
	indices.clear();
	for(int i = 0; i < (int)vertices.size();i++){
		indices.push_back((GLuint)i);
	}
}
