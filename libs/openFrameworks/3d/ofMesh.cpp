#include "ofMesh.h"
#include "ofGraphics.h"

//--------------------------------------------------------------
ofMesh::ofMesh(){
	mode = OF_PRIMITIVE_TRIANGLES;
	bVertsChanged = true;
	bColorsChanged = true;
	bNormalsChanged = true;
	bTexCoordsChanged = true;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
ofMesh::ofMesh(ofPrimitiveMode mode, const vector<ofVec3f>& verts){
	setMode(mode);
	addVertices(verts);
}

//--------------------------------------------------------------
ofMesh::~ofMesh(){
}

//--------------------------------------------------------------
void ofMesh::clear(){
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
bool ofMesh::haveVertsChanged(){
	if(bVertsChanged){
		bVertsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::haveColorsChanged(){
	if(bColorsChanged){
		bColorsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::haveNormalsChanged(){
	if(bNormalsChanged){
		bNormalsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::haveTexCoordsChanged(){
	if(bTexCoordsChanged){
		bTexCoordsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::haveIndicesChanged(){
	if(bIndicesChanged){
		bIndicesChanged = false;
		return true;
	}else{
		return false;
	}
}


//--------------------------------------------------------------
bool ofMesh::hasVertices(){
	return !vertices.empty();
}

//--------------------------------------------------------------
bool ofMesh::hasColors(){
	return !colors.empty();
}

//--------------------------------------------------------------
bool ofMesh::hasNormals(){
	return !normals.empty();
}

//--------------------------------------------------------------
bool ofMesh::hasTexCoords(){
	return !texCoords.empty();
}

//--------------------------------------------------------------
bool ofMesh::hasIndices(){
	return !indices.empty();
}

//ADDERS

//--------------------------------------------------------------
void ofMesh::addVertex(const ofVec3f& v){
	vertices.push_back(v);
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addVertices(const vector<ofVec3f>& verts){
	vertices.insert(vertices.end(),verts.begin(),verts.end());
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addVertices(const ofVec3f* verts, int amt){
	vertices.insert(vertices.end(),verts,verts+amt);
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addColor(const ofFloatColor& c){
	colors.push_back(c);
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addColors(const vector<ofFloatColor>& cols){
	colors.insert(colors.end(),cols.begin(),cols.end());
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addColors(const ofFloatColor* cols, int amt){
	colors.insert(colors.end(),cols,cols+amt);
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addNormal(const ofVec3f& n){
	normals.push_back(n);
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addNormals(const vector<ofVec3f>& norms){
	normals.insert(normals.end(),norms.begin(),norms.end());
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addNormals(const ofVec3f* norms, int amt){
	normals.insert(normals.end(),norms,norms+amt);
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addTexCoord(const ofVec2f& t){
	//TODO: figure out if we add to all other arrays to match
	texCoords.push_back(t);
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addTexCoords(const vector<ofVec2f>& tCoords){
	texCoords.insert(texCoords.end(),tCoords.begin(),tCoords.end());
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addTexCoords(const ofVec2f* tCoords, int amt){
	texCoords.insert(texCoords.end(),tCoords,tCoords+amt);
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
ofIndexType ofMesh::getIndex(int i){
	return indices[i];
}

//--------------------------------------------------------------
void ofMesh::addIndex(ofIndexType i){
	indices.push_back(i);
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addIndices(const vector<ofIndexType>& inds){
	indices.insert(indices.end(),inds.begin(),inds.end());
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addIndices(const ofIndexType* inds, int amt){
	indices.insert(indices.end(),inds,inds+amt);
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addTriangle(ofIndexType index1, ofIndexType index2, ofIndexType index3) {
    addIndex(index1);
    addIndex(index2);
    addIndex(index3);
}


//GETTERS
//--------------------------------------------------------------
ofPrimitiveMode ofMesh::getMode() const{
	return mode;
}

//--------------------------------------------------------------
ofVec3f ofMesh::getVertex(int i){
	return vertices[i];
}

//--------------------------------------------------------------
ofVec3f ofMesh::getNormal(int i){
	return normals[i];
}

//--------------------------------------------------------------
ofFloatColor ofMesh::getColor(int i){
	return colors[i];
}

//--------------------------------------------------------------
ofVec2f ofMesh::getTexCoord(int i){
	return texCoords[i];
}

//--------------------------------------------------------------
int ofMesh::getNumVertices() const{
	return (int)vertices.size();
}

//--------------------------------------------------------------
int ofMesh::getNumColors() const{
	return (int)colors.size();
}

//--------------------------------------------------------------
int ofMesh::getNumNormals() const{
	return (int)normals.size();
}

//--------------------------------------------------------------
int ofMesh::getNumTexCoords() const{
	return (int)texCoords.size();
}

//--------------------------------------------------------------
int ofMesh::getNumIndices() const{
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
ofVec3f* ofMesh::getVerticesPointer(){
	return &vertices[0];
}

//--------------------------------------------------------------
ofFloatColor* ofMesh::getColorsPointer(){
	return &colors[0];
}

//--------------------------------------------------------------
ofVec3f* ofMesh::getNormalsPointer(){
	return &normals[0];
}

//--------------------------------------------------------------
ofVec2f* ofMesh::getTexCoordsPointer(){
	return &texCoords[0];
}

//--------------------------------------------------------------
ofIndexType* ofMesh::getIndexPointer(){
	return &indices[0];
}


//--------------------------------------------------------------
const float* ofMesh::getVerticesPointer() const{
	return &vertices[0].x;
}

//--------------------------------------------------------------
const float* ofMesh::getColorsPointer() const{
	return &colors[0].r;
}

//--------------------------------------------------------------
const float* ofMesh::getNormalsPointer() const{
	return &normals[0].x;
}

//--------------------------------------------------------------
const float* ofMesh::getTexCoordsPointer() const{
	return &texCoords[0].x;
}

//--------------------------------------------------------------
const ofIndexType * ofMesh::getIndexPointer() const{
	return &indices[0];
}

vector<ofVec3f> & ofMesh::getVertices(){
	return vertices;
}

vector<ofFloatColor> & ofMesh::getColors(){
	return colors;
}

vector<ofVec3f> & ofMesh::getNormals(){
	return normals;
}

vector<ofVec2f> & ofMesh::getTexCoords(){
	return texCoords;
}

vector<ofIndexType> & ofMesh::getIndices(){
	return indices;
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
void ofMesh::setMode(ofPrimitiveMode m){
	bIndicesChanged = true;
	mode = m;
}

//--------------------------------------------------------------
void ofMesh::setVertex(int index, const ofVec3f& v){
	vertices[index] = v;
	bVertsChanged = true;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::setNormal(int index, const ofVec3f& n){
	normals[index] = n;
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::setColor(int index, const ofFloatColor& c){
	colors[index] = c;
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::setTexCoord(int index, const ofVec2f& t){
	texCoords[index] = t;
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::setIndex(int i, ofIndexType  val){
	indices[i] = val;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::setName(string name_){
	name = name_;
}

//--------------------------------------------------------------
void ofMesh::setupIndicesAuto(){
	bIndicesChanged = true;
	indices.clear();
	indices.resize(vertices.size());
	for(int i = 0; i < (int)vertices.size();i++){
		indices[i]=(ofIndexType)i;
	}
}



//--------------------------------------------------------------
void ofMesh::clearVertices(){
	vertices.clear();
	bVertsChanged=true;
}

//--------------------------------------------------------------
void ofMesh::clearNormals(){
	normals.clear();
	bNormalsChanged=true;
}

//--------------------------------------------------------------
void ofMesh::clearColors(){
	colors.clear();
	bColorsChanged=true;
}

//--------------------------------------------------------------
void ofMesh::clearTexCoords(){
	texCoords.clear();
	bTexCoordsChanged=true;
}

//--------------------------------------------------------------
void ofMesh::clearIndices(){
	indices.clear();
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::drawVertices(){
	draw(OF_MESH_POINTS);
}

//--------------------------------------------------------------
void ofMesh::drawWireframe(){
	draw(OF_MESH_WIREFRAME);
}

//--------------------------------------------------------------
void ofMesh::drawFaces(){
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void ofMesh::draw(){
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void ofMesh::draw(ofPolyRenderMode renderType){
	ofGetCurrentRenderer()->draw(*this,renderType);
}

