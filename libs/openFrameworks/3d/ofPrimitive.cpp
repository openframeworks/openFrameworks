#include "ofPrimitive.h"
#include "ofGraphics.h"

//--------------------------------------------------------------
ofPrimitive::ofPrimitive(){
	mode = OF_TRIANGLES_MODE;
	bVertsChanged = true;
	bColorsChanged = true;
	bNormalsChanged = true;
	bTexCoordsChanged = true;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
ofPrimitive::ofPrimitive(ofPrimitiveMode mode, const vector<ofVec3f>& verts){
	setMode(mode);
	addVertices(verts);
}

//--------------------------------------------------------------
ofPrimitive::~ofPrimitive(){
}

//--------------------------------------------------------------
void ofPrimitive::clear(){
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
bool ofPrimitive::haveVertsChanged(){
	if(bVertsChanged){
		bVertsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofPrimitive::haveColorsChanged(){
	if(bColorsChanged){
		bColorsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofPrimitive::haveNormalsChanged(){
	if(bNormalsChanged){
		bNormalsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofPrimitive::haveTexCoordsChanged(){
	if(bTexCoordsChanged){
		bTexCoordsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofPrimitive::haveIndicesChanged(){
	if(bIndicesChanged){
		bIndicesChanged = false;
		return true;
	}else{
		return false;
	}
}

//ADDERS

//--------------------------------------------------------------
void ofPrimitive::addVertex(const ofVec3f& v){
	vertices.push_back(v);
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addVertices(const vector<ofVec3f>& verts){
	vertices.insert(vertices.end(),verts.begin(),verts.end());
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addVertices(const ofVec3f* verts, int amt){
	for (int i = 0; i < amt;i++){
		addVertex(verts[i]);
	}
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addColor(const ofColor& c){
	colors.push_back(c);
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addColors(const vector<ofColor>& cols){
	colors.insert(colors.end(),cols.begin(),cols.end());
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addColors(const ofColor* cols, int amt){
	for (int i = 0; i < amt;i++){
		addColor(cols[i]);
	}
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addNormal(const ofVec3f& n){
	normals.push_back(n);
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addNormals(const vector<ofVec3f>& norms){
	normals.insert(normals.end(),norms.begin(),norms.end());
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addNormals(const ofVec3f* norms, int amt){
	for (int i = 0; i < amt;i++){
		addNormal(norms[i]);
	}
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addTexCoord(const ofVec2f& t){
	//TODO: figure out if we add to all other arrays to match
	texCoords.push_back(t);
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addTexCoords(const vector<ofVec2f>& tCoords){
	texCoords.insert(texCoords.end(),tCoords.begin(),tCoords.end());
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addTexCoords(const ofVec2f* tCoords, int amt){
	for (int i = 0; i < amt;i++){
		addTexCoord(tCoords[i]);
	}
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
ofIndexType ofPrimitive::getIndex(int i){
	return indices[i];
}

//--------------------------------------------------------------
void ofPrimitive::addIndex(ofIndexType i){
	indices.push_back(i);
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addIndices(const vector<ofIndexType>& inds){
	indices.insert(indices.end(),inds.begin(),inds.end());
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::addIndices(const ofIndexType* inds, int amt){
	for (int i = 0; i < amt;i++){
		addIndex(inds[i]);
	}
	bIndicesChanged = true;
}

//GETTERS
//--------------------------------------------------------------
ofPrimitiveMode ofPrimitive::getMode() const{
	return mode;
}

//--------------------------------------------------------------
ofVec3f ofPrimitive::getVertex(int i){
	return vertices[i];
}

//--------------------------------------------------------------
ofVec3f ofPrimitive::getNormal(int i){
	return normals[i];
}

//--------------------------------------------------------------
ofColor ofPrimitive::getColor(int i){
	return colors[i];
}

//--------------------------------------------------------------
ofVec2f ofPrimitive::getTexCoord(int i){
	return texCoords[i];
}

//--------------------------------------------------------------
int ofPrimitive::getNumVertices() const{
	return (int)vertices.size();
}

//--------------------------------------------------------------
int ofPrimitive::getNumColors() const{
	return (int)colors.size();
}

//--------------------------------------------------------------
int ofPrimitive::getNumNormals() const{
	return (int)normals.size();
}

//--------------------------------------------------------------
int ofPrimitive::getNumTexCoords() const{
	return (int)texCoords.size();
}

//--------------------------------------------------------------
int ofPrimitive::getNumIndices() const{
	return (int)indices.size();
}

/*
//--------------------------------------------------------------
int ofPrimitive::getNumIndicesSolid(){
	return indicesSolid.size();
}

//--------------------------------------------------------------
int ofPrimitive::getNumIndicesWire(){
	return indicesWire.size();
}
 */

//--------------------------------------------------------------
float* ofPrimitive::getVerticesPointer(){
	return &vertices[0].x;
}

//--------------------------------------------------------------
float* ofPrimitive::getColorsPointer(){
	return &colors[0].r;
}

//--------------------------------------------------------------
float* ofPrimitive::getNormalsPointer(){
	return &normals[0].x;
}

//--------------------------------------------------------------
float* ofPrimitive::getTexCoordsPointer(){
	return &texCoords[0].x;
}

//--------------------------------------------------------------
ofIndexType* ofPrimitive::getIndexPointer(){
	return &indices[0];
}


//--------------------------------------------------------------
const float* ofPrimitive::getVerticesPointer() const{
	return &vertices[0].x;
}

//--------------------------------------------------------------
const float* ofPrimitive::getColorsPointer() const{
	return &colors[0].r;
}

//--------------------------------------------------------------
const float* ofPrimitive::getNormalsPointer() const{
	return &normals[0].x;
}

//--------------------------------------------------------------
const float* ofPrimitive::getTexCoordsPointer() const{
	return &texCoords[0].x;
}

//--------------------------------------------------------------
const ofIndexType * ofPrimitive::getIndexPointer() const{
	return &indices[0];
}

/*
//--------------------------------------------------------------
GLuint* ofPrimitive::getSolidIndexPointer(){
	return &indicesSolid[0];
}

//--------------------------------------------------------------
GLuint* ofPrimitive::getWireIndexPointer(){
	return &indicesWire[0];
}
 */

/*
//--------------------------------------------------------------
vector<int>& ofPrimitive::getFace(int faceNum){
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
void ofPrimitive::setMode(ofPrimitiveMode m){
	bIndicesChanged = true;
	mode = m;
}

//--------------------------------------------------------------
void ofPrimitive::setVertex(int index, const ofVec3f& v){
	vertices[index] = v;
	bVertsChanged = true;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::setNormal(int index, const ofVec3f& n){
	normals[index] = n;
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::setColor(int index, const ofColor& c){
	colors[index] = c;
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::setTexCoord(int index, const ofVec2f& t){
	texCoords[index] = t;
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::setIndex(int i, ofIndexType  val){
	indices[i] = val;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofPrimitive::setName(string name_){
	name = name_;
}

//--------------------------------------------------------------
void ofPrimitive::setupIndicesAuto(){
	bIndicesChanged = true;
	indices.clear();
	for(int i = 0; i < (int)vertices.size();i++){
		indices.push_back((ofIndexType)i);
	}
}
