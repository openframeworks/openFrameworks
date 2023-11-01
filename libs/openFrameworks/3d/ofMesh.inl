#ifndef OF_MESH_H
#include "ofMesh.h"
#endif

#include "ofAppRunner.h"
#include "ofGraphicsBaseTypes.h"
#include "ofVectorMath.h"
#include "ofMath.h"
#include "ofMathConstants.h"
#include "ofLog.h"
#include "ofColor.h"

#include <unordered_map>

//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T>::ofMesh_(){
	mode = OF_PRIMITIVE_TRIANGLES;
	bVertsChanged = false;
	bColorsChanged = false;
	bNormalsChanged = false;
	bTexCoordsChanged = false;
	bIndicesChanged = false;
	bFacesDirty = false;
	useColors = true;
	useTextures = true;
	useNormals = true;
	useIndices = true;
}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T>::ofMesh_(ofPrimitiveMode mode, const std::vector<V>& verts){
	bColorsChanged = false;
	bNormalsChanged = false;
	bTexCoordsChanged = false;
	useColors = true;
	useTextures = true;
	useNormals = true;
	useIndices = true;
	setMode(mode);
	addVertices(verts);
}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::clear(){
	if(!vertices.empty()){
		bVertsChanged = true;
		vertices.clear();
	}
	if(!colors.empty()){
		bColorsChanged = true;
		colors.clear();
	}
	if(!normals.empty()){
		bNormalsChanged = true;
		normals.clear();
	}
	if(!texCoords.empty()){
		bTexCoordsChanged = true;
		texCoords.clear();
	}
	if(!indices.empty()){
		bIndicesChanged = true;
		indices.clear();
	}
	bFacesDirty = true;
}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::haveVertsChanged(){
	if(bVertsChanged){
		bVertsChanged = false;
		return true;
	}else{
		return false;
	}
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::haveColorsChanged(){
	if(bColorsChanged){
		bColorsChanged = false;
		return true;
	}else{
		return false;
	}
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::haveNormalsChanged(){
	if(bNormalsChanged){
		bNormalsChanged = false;
		return true;
	}else{
		return false;
	}
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::haveTexCoordsChanged(){
	if(bTexCoordsChanged){
		bTexCoordsChanged = false;
		return true;
	}else{
		return false;
	}
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::haveIndicesChanged(){
	if(bIndicesChanged){
		bIndicesChanged = false;
		return true;
	}else{
		return false;
	}
}




//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::hasVertices() const{
	return !vertices.empty();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::hasColors() const{
	return !colors.empty();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::hasNormals() const{
	return !normals.empty();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::hasTexCoords() const{
	return !texCoords.empty();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::hasIndices() const{
	return !indices.empty();
}

//ADDERS



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addVertex(const V& v){
	vertices.push_back(v);
	bVertsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addVertices(const std::vector<V>& verts){
	vertices.insert(vertices.end(),verts.begin(),verts.end());
	bVertsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addVertices(const V* verts, std::size_t amt){
	vertices.insert(vertices.end(),verts,verts+amt);
	bVertsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addColor(const C& c){
	colors.push_back(c);
	bColorsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addColors(const std::vector<C>& cols){
	colors.insert(colors.end(),cols.begin(),cols.end());
	bColorsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addColors(const C* cols, std::size_t amt){
	colors.insert(colors.end(),cols,cols+amt);
	bColorsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addNormal(const N& n){
	normals.push_back(n);
	bNormalsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addNormals(const std::vector<N>& norms){
	normals.insert(normals.end(),norms.begin(),norms.end());
	bNormalsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addNormals(const N* norms, std::size_t amt){
	normals.insert(normals.end(),norms,norms+amt);
	bNormalsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addTexCoord(const T& t){
	//TODO: figure out if we add to all other arrays to match
	texCoords.push_back(t);
	bTexCoordsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addTexCoords(const std::vector<T>& tCoords){
	texCoords.insert(texCoords.end(),tCoords.begin(),tCoords.end());
	bTexCoordsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addTexCoords(const T* tCoords, std::size_t amt){
	texCoords.insert(texCoords.end(),tCoords,tCoords+amt);
	bTexCoordsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofIndexType ofMesh_<V,N,C,T>::getIndex(ofIndexType i) const{
	return indices[i];
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addIndex(ofIndexType i){
	indices.push_back(i);
	bIndicesChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addIndices(const std::vector<ofIndexType>& inds){
	indices.insert(indices.end(),inds.begin(),inds.end());
	bIndicesChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addIndices(const ofIndexType* inds, std::size_t amt){
	indices.insert(indices.end(),inds,inds+amt);
	bIndicesChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::addTriangle(ofIndexType index1, ofIndexType index2, ofIndexType index3) {
	addIndex(index1);
	addIndex(index2);
	addIndex(index3);
}

//REMOVERS


//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::removeVertex(ofIndexType index){
  if(index >= vertices.size()){
	ofLogError("ofMesh") << "removeVertex(): ignoring out of range index " << index << ", number of vertices is" << vertices.size();
  }else{
	vertices.erase(vertices.begin() + index);
	bVertsChanged = true;
	bFacesDirty = true;
  }
}

template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::removeVertices(ofIndexType startIndex, ofIndexType endIndex){
	if(startIndex >= vertices.size() || endIndex > vertices.size()){
		ofLogError("ofMesh") << "removeVertex(): ignoring out of range startIndex " << startIndex << " endIndex " << endIndex << ", number of vertices is" << vertices.size();
	}else{
		vertices.erase(vertices.begin() + startIndex, vertices.begin() + endIndex);
		bVertsChanged = true;
		bFacesDirty = true;
	}
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::removeNormal(ofIndexType index){
  if(index >= normals.size()){
	ofLogError("ofMesh") << "removeNormal(): ignoring out of range index " << index << ", number of normals is" << normals.size();
  }else{
	normals.erase(normals.begin() + index);
	bNormalsChanged = true;
	bFacesDirty = true;
  }
}

template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::removeNormals(ofIndexType startIndex, ofIndexType endIndex){
    if(startIndex >= normals.size() || endIndex > normals.size()){
        ofLogError("ofMesh") << "removeNormal(): ignoring out of range beginIndex " << startIndex << " endIndex " << endIndex << ", number of normals is" << normals.size();
    }else{
        normals.erase(normals.begin() + startIndex, normals.begin() + endIndex);
        bNormalsChanged = true;
        bFacesDirty = true;
    }
}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::removeColor(ofIndexType index){
  if(index >= colors.size()){
	ofLogError("ofMesh") << "removeColor(): ignoring out of range index " << index << ", number of colors is" << colors.size();
  }else{
	colors.erase(colors.begin() + index);
	bColorsChanged = true;
	bFacesDirty = true;
  }
}

template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::removeColors(ofIndexType startIndex, ofIndexType endIndex){
	if(startIndex >= colors.size() || endIndex > colors.size()){
		ofLogError("ofMesh") << "removeColor(): ignoring out of range startIndex " << startIndex << " endIndex " << endIndex << ", number of colors is" << colors.size();
	}else{
		colors.erase(colors.begin() + startIndex, colors.begin() + endIndex);
		bColorsChanged = true;
		bFacesDirty = true;
	}
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::removeTexCoord(ofIndexType index){
  if(index >= texCoords.size()){
	ofLogError("ofMesh") << "removeTexCoord(): ignoring out of range index " << index << ", number of tex coords is" << texCoords.size();
  }else{
	texCoords.erase(texCoords.begin() + index);
	bTexCoordsChanged = true;
	bFacesDirty = true;
  }
}

template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::removeTexCoords(ofIndexType startIndex, ofIndexType endIndex){
	if(startIndex >= texCoords.size() || endIndex >= texCoords.size()){
		ofLogError("ofMesh") << "removeTexCoord(): ignoring out of range startIndex " << startIndex << " endIndex " << endIndex << ", number of tex coords is" << texCoords.size();
	}else{
		texCoords.erase(texCoords.begin() + startIndex, texCoords.begin() + endIndex);
		bTexCoordsChanged = true;
		bFacesDirty = true;
	}
}




//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::removeIndex(ofIndexType index){
  if(index >= indices.size()){
	ofLogError("ofMesh") << "removeIndex(): ignoring out of range index " << index << ", number of indices is" << indices.size();
  }else{
	indices.erase(indices.begin() + index);
	bIndicesChanged = true;
	bFacesDirty = true;
  }
}

template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::removeIndices(ofIndexType startIndex, ofIndexType endIndex){
	if(startIndex >= indices.size() || endIndex > indices.size()){
		ofLogError("ofMesh") << "removeIndex(): ignoring out of range startIndex " << startIndex << " endIndex " << endIndex << ", number of indices is" << indices.size();;
	}else{
		indices.erase(indices.begin() + startIndex, indices.begin() + endIndex);
		bIndicesChanged = true;
		bFacesDirty = true;
	}
}



//GETTERS


//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofPrimitiveMode ofMesh_<V,N,C,T>::getMode() const{
	return mode;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
V ofMesh_<V,N,C,T>::getVertex(ofIndexType i) const{
	return vertices[i];
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
N ofMesh_<V,N,C,T>::getNormal(ofIndexType i) const{
	return normals[i];
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
C ofMesh_<V,N,C,T>::getColor(ofIndexType i) const{
	return colors[i];
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
T ofMesh_<V,N,C,T>::getTexCoord(ofIndexType i) const{
	return texCoords[i];
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::size_t ofMesh_<V,N,C,T>::getNumVertices() const{
	return vertices.size();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::size_t ofMesh_<V,N,C,T>::getNumColors() const{
	return colors.size();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::size_t ofMesh_<V,N,C,T>::getNumNormals() const{
	return normals.size();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::size_t ofMesh_<V,N,C,T>::getNumTexCoords() const{
	return texCoords.size();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::size_t ofMesh_<V,N,C,T>::getNumIndices() const{
	return indices.size();
}

/*


//--------------------------------------------------------------
template<class V, class N, class C, class T>
int ofPrimitive::getNumIndicesSolid(){
	return indicesSolid.size();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
int ofPrimitive::getNumIndicesWire(){
	return indicesWire.size();
}
 */



//--------------------------------------------------------------
template<class V, class N, class C, class T>
V* ofMesh_<V,N,C,T>::getVerticesPointer(){
	return vertices.data();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
C* ofMesh_<V,N,C,T>::getColorsPointer(){
	return colors.data();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
N* ofMesh_<V,N,C,T>::getNormalsPointer(){
	return normals.data();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
T* ofMesh_<V,N,C,T>::getTexCoordsPointer(){
	return texCoords.data();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofIndexType* ofMesh_<V,N,C,T>::getIndexPointer(){
	return indices.data();
}




//--------------------------------------------------------------
template<class V, class N, class C, class T>
const V* ofMesh_<V,N,C,T>::getVerticesPointer() const{
	return vertices.data();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
const C* ofMesh_<V,N,C,T>::getColorsPointer() const{
	return colors.data();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
const N* ofMesh_<V,N,C,T>::getNormalsPointer() const{
	return normals.data();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
const T* ofMesh_<V,N,C,T>::getTexCoordsPointer() const{
	return texCoords.data();
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
const ofIndexType * ofMesh_<V,N,C,T>::getIndexPointer() const{
	return indices.data();
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::vector<V> & ofMesh_<V,N,C,T>::getVertices(){
	bVertsChanged = true;
	bFacesDirty = true;
	return vertices;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::vector<C> & ofMesh_<V,N,C,T>::getColors(){
	bColorsChanged = true;
	bFacesDirty = true;
	return colors;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::vector<N> & ofMesh_<V,N,C,T>::getNormals(){
	bNormalsChanged = true;
	bFacesDirty = true;
	return normals;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::vector<T> & ofMesh_<V,N,C,T>::getTexCoords(){
	bTexCoordsChanged = true;
	bFacesDirty = true;
	return texCoords;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::vector<ofIndexType> & ofMesh_<V,N,C,T>::getIndices(){
	bIndicesChanged = true;
	bFacesDirty = true;
	return indices;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
const std::vector<V> & ofMesh_<V,N,C,T>::getVertices() const{
	return vertices;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
const std::vector<C> & ofMesh_<V,N,C,T>::getColors() const{
	return colors;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
const std::vector<N> & ofMesh_<V,N,C,T>::getNormals() const{
	return normals;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
const std::vector<T> & ofMesh_<V,N,C,T>::getTexCoords() const{
	return texCoords;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
const std::vector<ofIndexType> & ofMesh_<V,N,C,T>::getIndices() const{
	return indices;
}

/*


//--------------------------------------------------------------
template<class V, class N, class C, class T>
GLuint* ofPrimitive::getSolidIndexPointer(){
	return &indicesSolid[0];
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
GLuint* ofPrimitive::getWireIndexPointer(){
	return &indicesWire[0];
}
 */

/*


//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::vector<int>& ofPrimitive::getFace(int faceNum){
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




//--------------------------------------------------------------
template<class V, class N, class C, class T>
V ofMesh_<V,N,C,T>::getCentroid() const {
	if(vertices.size() == 0) {
		ofLogWarning("ofMesh") << "getCentroid(): mesh has no vertices, returning glm::vec3(0, 0, 0)";
		return glm::vec3(0, 0, 0);
	}

	V sum;
	for(ofIndexType i = 0; i < vertices.size(); i++) {
		sum += vertices[i];
	}
	sum /= vertices.size();
	return sum;
}

//SETTERS


//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::setMode(ofPrimitiveMode m){
	bIndicesChanged = true;
	mode = m;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::setVertex(ofIndexType index, const V& v){
	vertices[index] = v;
	bVertsChanged = true;
	bIndicesChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::setNormal(ofIndexType index, const N& n){
	normals[index] = n;
	bNormalsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::setColor(ofIndexType index, const C& c){
	colors[index] = c;
	bColorsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::setTexCoord(ofIndexType index, const T& t){
	texCoords[index] = t;
	bTexCoordsChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::setIndex(ofIndexType index, ofIndexType  val){
	indices[index] = val;
	bIndicesChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::setupIndicesAuto(){
	bIndicesChanged = true;
	bFacesDirty = true;
	indices.resize(vertices.size());
	for(ofIndexType i = 0; i < vertices.size();i++){
		indices[i]=i;
	}
}





//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::clearVertices(){
	vertices.clear();
	bVertsChanged=true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::clearNormals(){
	normals.clear();
	bNormalsChanged=true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::clearColors(){
	colors.clear();
	bColorsChanged=true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::clearTexCoords(){
	texCoords.clear();
	bTexCoordsChanged=true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::clearIndices(){
	indices.clear();
	bIndicesChanged = true;
	bFacesDirty = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::drawVertices() const{
	draw(OF_MESH_POINTS);
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::drawWireframe() const{
	draw(OF_MESH_WIREFRAME);
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::drawFaces() const{
	draw(OF_MESH_FILL);
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::draw() const{
	draw(OF_MESH_FILL);
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::draw(ofPolyRenderMode renderType) const{
	if(getNumVertices()==0) return;
	ofGetCurrentRenderer()->draw(*this,renderType,useColors,useTextures,useNormals);
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::enableColors(){
	useColors = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::enableTextures(){
	useTextures = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::enableNormals(){
	useNormals = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::enableIndices(){
	useIndices = true;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::disableColors(){
	useColors = false;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::disableTextures(){
	useTextures = false;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::disableNormals(){
	useNormals = false;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::disableIndices(){
	useIndices = false;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::usingColors() const{
	return useColors;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::usingTextures() const{
	return useTextures;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::usingNormals() const{
	return useNormals;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMesh_<V,N,C,T>::usingIndices() const{
	return useIndices;
}




//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::append(const ofMesh_<V,N,C,T> & mesh){
	ofIndexType prevNumVertices = static_cast<ofIndexType>(vertices.size());
	if(mesh.getNumVertices()){
		vertices.insert(vertices.end(),mesh.getVertices().begin(),mesh.getVertices().end());
	}
	if(mesh.getNumTexCoords()){
		texCoords.insert(texCoords.end(),mesh.getTexCoords().begin(),mesh.getTexCoords().end());
	}
	if(mesh.getNumColors()){
		colors.insert(colors.end(),mesh.getColors().begin(),mesh.getColors().end());
	}
	if(mesh.getNumNormals()){
		normals.insert(normals.end(),mesh.getNormals().begin(),mesh.getNormals().end());
	}
	if(mesh.getNumIndices()){
		for(auto index: mesh.getIndices()){
			indices.push_back(index+prevNumVertices);
		}
	}
}




//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::load(const of::filesystem::path& path){
	ofFile is = {path, ofFile::ReadOnly};
	auto & data = *this;

	std::string error;
	ofBuffer buffer(is);
	auto backup = data;

	int orderVertices=-1;
	int orderIndices=-1;

	ofIndexType vertexCoordsFound=0;
	ofIndexType colorCompsFound=0;
	ofIndexType texCoordsFound=0;
	ofIndexType normalsCoordsFound=0;

	ofIndexType currentVertex = 0;
	ofIndexType currentFace = 0;

	bool colorTypeIsUChar = false; /// flag to distinguish between uchar (more common) and float (less common) color format in ply file
	
	enum State{
		Header,
		VertexDef,
		FaceDef,
		Vertices,
		Normals,
		Faces
	};

	
	enum Attribute {
		Position,
		Color,
		Normal,
		TexCoord,
	};
	
	std::vector<Attribute> meshDefinition;
	
	data.clear();
	State state = Header;

	int lineNum = 0;
	ofBuffer::Lines lines = buffer.getLines();
	ofBuffer::Line line = lines.begin();
	lineNum++;
	if(*line!="ply"){
		error = "wrong format, expecting 'ply'";
		goto clean;
	}

	line++;
	lineNum++;
	if(*line!="format ascii 1.0"){
		error = "wrong format, expecting 'format ascii 1.0'";
		goto clean;
	}

	for(;line != lines.end(); ++line){
		lineNum++;
		std::string lineStr = *line;
		if(lineStr.find("comment")==0 || lineStr.empty()){
			continue;
		}

		if((state==Header || state==FaceDef) && lineStr.find("element vertex")==0){
			state = VertexDef;
			orderVertices = std::max(orderIndices, 0)+1;
			data.getVertices().resize(ofTo<size_t>(lineStr.substr(15)));
			continue;
		}

		if((state==Header || state==VertexDef) && lineStr.find("element face")==0){
			state = FaceDef;
			orderIndices = std::max(orderVertices, 0)+1;
			data.getIndices().resize(ofTo<size_t>(lineStr.substr(13))*3);
			continue;
		}

		if(state==VertexDef && (lineStr.find("property float x")==0 || lineStr.find("property float y")==0 || lineStr.find("property float z")==0
                || lineStr.find("property double x")==0 || lineStr.find("property double y")==0 || lineStr.find("property double z")==0)){
			meshDefinition.push_back(Position);
			vertexCoordsFound++;
			continue;
		}

		if(state==VertexDef && (lineStr.find("property float r")==0 || lineStr.find("property float g")==0 || lineStr.find("property float b")==0 || lineStr.find("property float a")==0)){
			colorCompsFound++;
			meshDefinition.push_back(Color);
			data.getColors().resize(data.getVertices().size());
			continue;
		}

		if(state==VertexDef && (lineStr.find("property uchar red")==0 || lineStr.find("property uchar green")==0 || lineStr.find("property uchar blue")==0 || lineStr.find("property uchar alpha")==0)){
			colorTypeIsUChar = true;
			colorCompsFound++;
			meshDefinition.push_back(Color);
			data.getColors().resize(data.getVertices().size());
			continue;
		}

		if(state==VertexDef && (lineStr.find("property float u")==0 || lineStr.find("property float v")==0|| lineStr.find("property float s")==0 || lineStr.find("property float t")==0)){
			texCoordsFound++;
			meshDefinition.push_back(TexCoord);
			data.getTexCoords().resize(data.getVertices().size());
			continue;
		}

		if(state==VertexDef && (lineStr.find("property float nx")==0 || lineStr.find("property float ny")==0 || lineStr.find("property float nz")==0)){
			normalsCoordsFound++;
			meshDefinition.push_back(Normal);
			if (normalsCoordsFound==3) data.getNormals().resize(data.getVertices().size());
			continue;
		}

		if(state==FaceDef && lineStr.find("property list")!=0 && lineStr!="end_header"){
			error = "wrong face definition";
			goto clean;
		}

		if(lineStr=="end_header"){
			if(data.hasColors() && colorCompsFound!=3 && colorCompsFound!=4){
				error =  "data has color coordiantes but not correct number of components. Found " + ofToString(colorCompsFound) + " expecting 3 or 4";
				goto clean;
			}
			if(data.hasNormals() && normalsCoordsFound!=3){
				error = "data has normal coordiantes but not correct number of components. Found " + ofToString(normalsCoordsFound) + " expecting 3";
				goto clean;
			}
			if(!data.hasVertices()){
				ofLogWarning("ofMesh") << "load(): mesh loaded from \"" << path << "\" has no vertices";
			}
			if(orderVertices==-1) orderVertices=9999;
			if(orderIndices==-1) orderIndices=9999;

			if(orderVertices < orderIndices){
				state = Vertices;
			}else {
				state = Faces;
			}
			continue;
		}

		if(state==Vertices){
			if(data.getNumVertices()<=currentVertex){
				error = "found more vertices: " + ofToString(currentVertex+1) + " than specified in header: " + ofToString(data.getNumVertices());
				goto clean;
			}
			std::stringstream sline(lineStr);
			
			// read in a line of vertex elements
			// and split it into attributes,
			// based attribute order specified in file header
			ofIndexType vAttr = 0;
			ofIndexType nAttr = 0;
			ofIndexType tAttr = 0;
			ofIndexType cAttr = 0;
			for(auto s:meshDefinition){
				switch (s) {
					case Position:
						sline >> *(&data.getVertices()[currentVertex].x + (vAttr++)%vertexCoordsFound);
						break;
					case Color:
						if (colorTypeIsUChar){
							int c = 0;
							sline >> c;
							*(&data.getColors()[currentVertex].r + (cAttr++)%colorCompsFound) = c/255.f;
						} else {
							sline >> *(&data.getColors()[currentVertex].r + (cAttr++)%colorCompsFound);
						}
						break;
					case Normal:
						sline >> *(&data.getNormals()[currentVertex].x + (nAttr++)%normalsCoordsFound);
						break;
					case TexCoord:
						sline >> *(&data.getTexCoords()[currentVertex].x + (tAttr++)%texCoordsFound);
						break;
					default:
						break;
				}
			}
			if (vAttr != vertexCoordsFound || cAttr!= colorCompsFound || nAttr!=normalsCoordsFound || tAttr!=texCoordsFound){
				error = "attribute data does not match definition in header";
				goto clean;
			}
			
			currentVertex++;
			if(currentVertex==data.getNumVertices()){
				if(orderVertices<orderIndices){
					state = Faces;
				}else{
					state = Vertices;
				}
			}
			continue;
		}

		if(state==Faces){
			if(data.getNumIndices()/3<currentFace){
				error = "found more faces than specified in header";
				goto clean;
			}
			std::stringstream sline(lineStr);
			int numV;
			sline >> numV;
			if(numV!=3){
				error = "face not a triangle";
				goto clean;
			}
			ofIndexType i;
			sline >> i;
			data.getIndices()[currentFace*3] = i;
			sline >> i;
			data.getIndices()[currentFace*3+1] = i;
			sline >> i;
			data.getIndices()[currentFace*3+2] = i;

			currentFace++;
			if(currentFace==data.getNumIndices()/3){
				if(orderVertices<orderIndices){
					state = Vertices;
				}else{
					state = Faces;
				}
			}
			continue;
		}
	}


	return;
	clean:
	ofLogError("ofMesh") << "load(): " << lineNum << ":" << error;
	ofLogError("ofMesh") << "load(): \"" << *line << "\"";
	data = backup;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::save(const of::filesystem::path& path, bool useBinary) const{
	ofFile os(path, ofFile::WriteOnly);
	const auto & data = *this;

	os << "ply" << std::endl;
	if(useBinary) {
		os << "format binary_little_endian 1.0" << std::endl;
	} else {
		os << "format ascii 1.0" << std::endl;
	}

	if(data.getNumVertices()){
		os << "element vertex " << data.getNumVertices() << std::endl;
		os << "property float x" << std::endl;
		os << "property float y" << std::endl;
		os << "property float z" << std::endl;
		if(data.getNumColors()){
			os << "property uchar red" << std::endl;
			os << "property uchar green" << std::endl;
			os << "property uchar blue" << std::endl;
			os << "property uchar alpha" << std::endl;
		}
		if(data.getNumTexCoords()){
			os << "property float u" << std::endl;
			os << "property float v" << std::endl;
		}
		if(data.getNumNormals()){
			os << "property float nx" << std::endl;
			os << "property float ny" << std::endl;
			os << "property float nz" << std::endl;
		}
	}

	uint8_t faceSize = 3;
	if(data.getNumIndices()){
		os << "element face " << data.getNumIndices() / faceSize << std::endl;
		os << "property list uchar int vertex_indices" << std::endl;
	} else if(data.getMode() == OF_PRIMITIVE_TRIANGLES) {
		os << "element face " << data.getNumVertices() / faceSize << std::endl;
		os << "property list uchar int vertex_indices" << std::endl;
	} else if(data.getMode() == OF_PRIMITIVE_TRIANGLE_STRIP && data.getNumVertices() >= 4) {
		os << "element face " << data.getNumVertices() - 2 << std::endl;
		os << "property list uchar int vertex_indices" << std::endl;
	}

	os << "end_header" << std::endl;

	for(std::size_t i = 0; i < data.getNumVertices(); i++){
		if(useBinary) {
			os.write((char*) &data.getVertices()[i], sizeof(V));
		} else {
			os << data.getVertex(i).x << " " << data.getVertex(i).y << " " << data.getVertex(i).z;
		}
		if(data.getNumColors()){
			// VCG lib / MeshLab don't support float colors, so we have to cast
			ofColor cur = data.getColors()[i];
			if(useBinary) {
				os.write((char*) &cur, sizeof(ofColor));
			} else {
				os << " " << (int) cur.r << " " << (int) cur.g << " " << (int) cur.b << " " << (int) cur.a;
			}
		}
		if(data.getNumTexCoords()){
			if(useBinary) {
				os.write((char*) &data.getTexCoords()[i], sizeof(T));
			} else {
				os << " " << data.getTexCoord(i).x << " " << data.getTexCoord(i).y;
			}
		}
		if(data.getNumNormals()){
			if(useBinary) {
				os.write((char*) &data.getNormals()[i], sizeof(V));
			} else {
				os << " " << data.getNormal(i).x << " " << data.getNormal(i).y << " " << data.getNormal(i).z;
			}
		}
		if(!useBinary) {
			os << std::endl;
		}
	}

	if(data.getNumIndices()) {
		for(uint32_t i = 0; i < data.getNumIndices(); i += faceSize) {
			if(useBinary) {
				os.write((char*) &faceSize, sizeof(unsigned char));
				os.write((char*)&data.getIndices()[i], faceSize);
			} else {
				os << (std::size_t) faceSize << " " << data.getIndex(i) << " " << data.getIndex(i+1) << " " << data.getIndex(i+2) << std::endl;
			}
		}
	} else if(data.getMode() == OF_PRIMITIVE_TRIANGLES) {
		for(uint32_t i = 0; i < data.getNumVertices(); i += faceSize) {
			uint32_t indices[] = {i, i + 1, i + 2};
			if(useBinary) {
				os.write((char*) &faceSize, sizeof(unsigned char));
				os.write((char*) indices, sizeof(indices));
			} else {
				os << (std::size_t) faceSize << " " << indices[0] << " " << indices[1] << " " << indices[2] << std::endl;
			}
		}
	} else if(data.getMode() == OF_PRIMITIVE_TRIANGLE_STRIP && data.getNumVertices() >= 4) {
		for(uint32_t i = 0; i < data.getNumVertices() - 2; i += 2) {
			uint32_t indices1[] = {i, i + 1, i + 2};
			uint32_t indices2[] = {i + 1, i + 3, i + 2};
			if(useBinary) {
				os.write((char*) &faceSize, sizeof(unsigned char));
				os.write((char*) indices1, sizeof(indices1));
				os.write((char*) &faceSize, sizeof(unsigned char));
				os.write((char*) indices2, sizeof(indices2));
			} else {
				os << (std::size_t) faceSize << " " << indices1[0] << " " << indices1[1] << " " << indices1[2] << std::endl;
				os << (std::size_t) faceSize << " " << indices2[0] << " " << indices2[1] << " " << indices2[2] << std::endl;
			}
		}
	}

	//TODO: add index generation for other OF_PRIMITIVE cases
}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::setColorForIndices( ofIndexType startIndex, ofIndexType endIndex, C color ) {
	if(!hasColors()) {
		// no colors for vertices, so we must set them here //
		getColors().resize( getNumVertices() );
	}

	for(ofIndexType i = startIndex; i < endIndex; i++) {
		setColor( getIndex(i), color);
	}
}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T> ofMesh_<V,N,C,T>::getMeshForIndices( ofIndexType startIndex, ofIndexType endIndex ) const {
	ofIndexType startVertIndex = 0;
	ofIndexType endVertIndex = 0;

	if(startIndex >= getNumIndices() ) {
		startVertIndex = 0;
	} else {
		startVertIndex = getIndex( startIndex );
	}

	if(endIndex >= getNumIndices() ) {
		// set to the total, because the vector assign does not include the last element //
		endVertIndex = getNumVertices();
	} else {
		endVertIndex = getIndex( endIndex );
	}
	return getMeshForIndices(startIndex, endIndex, startVertIndex, endVertIndex );
}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T> ofMesh_<V,N,C,T>::getMeshForIndices( ofIndexType startIndex, ofIndexType endIndex, ofIndexType startVertIndex, ofIndexType endVertIndex ) const{

	ofMesh_<V,N,C,T> mesh;
	mesh.setMode( getMode() );

	mesh.getVertices().assign( getVertices().begin()+startVertIndex, getVertices().begin()+endVertIndex );

	if( hasColors() ) {
		std::vector<ofFloatColor> colors;
		mesh.getColors().assign( getColors().begin()+startVertIndex, getColors().begin()+endVertIndex );
		if( usingColors()) mesh.enableColors();
		else mesh.disableColors();
	}

	if( hasTexCoords() ) {
		mesh.getTexCoords().assign( getTexCoords().begin()+startVertIndex, getTexCoords().begin()+endVertIndex );
		if( usingTextures() ) mesh.enableTextures();
		else mesh.disableTextures();
	}

	if( hasNormals() ) {
		mesh.getNormals().assign( getNormals().begin()+startVertIndex, getNormals().begin()+endVertIndex );
		if( usingNormals() ) mesh.enableNormals();
		else mesh.disableNormals();
	}

	ofIndexType offsetIndex = getIndex(startIndex);
	bool bFoundLessThanZero = false;
	for(ofIndexType i = startIndex; i < endIndex; i++) {
		ofIndexType index;
		if(getIndex(i)<offsetIndex){
			index = 0;
			bFoundLessThanZero = true;
		}else{
			index = getIndex(i) - offsetIndex;
		}
		mesh.addIndex( index );
	}

	if(bFoundLessThanZero) {
		ofLogWarning( "ofMesh :: getMeshForIndices : found some indices less than 0, setting them to 0"  );
	}

	return mesh;
}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::mergeDuplicateVertices() {

	std::vector<V> verts = getVertices();
	std::vector<ofIndexType> indices = getIndices();

	//get indexes to share single point - TODO: try j < i
	for(ofIndexType i = 0; i < indices.size(); i++) {
		for(ofIndexType j = 0; j < indices.size(); j++ ) {
			if(i==j) continue;

			ofIndexType i1 = indices[i];
			ofIndexType i2 = indices[j];
			const V & v1 = verts[ i1 ];
			const V & v2 = verts[ i2 ];

			if( v1 == v2 && i1 != i2) {
				indices[j] = i1;
				break;
			}
		}
	}

	//indices array now has list of unique points we need
	//but we need to delete the old points we're not using and that means the index values will change
	//so we are going to create a new list of points and new indexes - we will use a map to map old index values to the new ones
	std::vector <V> newPoints;
	std::vector <ofIndexType> newIndexes;
	std::unordered_map <ofIndexType, bool> ptCreated;
	std::unordered_map <ofIndexType, ofIndexType> oldIndexNewIndex;

	std::vector<ofFloatColor> newColors;
	std::vector<ofFloatColor>& colors = getColors();
	std::vector<T> newTCoords;
	std::vector<T>& tcoords = getTexCoords();
	std::vector<N> newNormals;
	std::vector<N>& normals = getNormals();

	for(ofIndexType i = 0; i < indices.size(); i++){
		ptCreated[i] = false;
	}

	for(ofIndexType i = 0; i < indices.size(); i++){
		ofIndexType index = indices[i];
		const auto & p = verts[ index ];

		if( ptCreated[index] == false ){
			oldIndexNewIndex[index] = newPoints.size();
			newPoints.push_back( p );
			if(hasColors()) {
				newColors.push_back(colors[index]);
			}
			if(hasTexCoords()) {
				newTCoords.push_back(tcoords[index]);
			}
			if(hasNormals()) {
				newNormals.push_back(normals[index]);
			}

			ptCreated[index] = true;
		}

		//ofLogNotice("ofMesh") << "[" << i << "]: old " << index << " --> " << oldIndexNewIndex[index];
		newIndexes.push_back( oldIndexNewIndex[index] );
	}

	verts.clear();
	verts = newPoints;

	indices.clear();
	indices = newIndexes;

	clearIndices();
	addIndices(indices);
	clearVertices();
	addVertices( verts );

	if(hasColors()) {
		clearColors();
		addColors( newColors );
	}

	if(hasTexCoords()) {
		clearTexCoords();
		addTexCoords( newTCoords );
	}

	if(hasNormals()) {
		clearNormals();
		addNormals( newNormals );
	}

}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMeshFace_<V,N,C,T> ofMesh_<V,N,C,T>::getFace(ofIndexType faceId) const{
	const std::vector<ofMeshFace_<V,N,C,T>> & faces = getUniqueFaces();
	if(faces.size()>faceId){
		return faces[faceId];
	}else{
		ofLogError() << "couldn't find face " << faceId;
		return ofMeshFace_<V,N,C,T>();
	}
}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
const std::vector<ofMeshFace_<V,N,C,T>> & ofMesh_<V,N,C,T>::getUniqueFaces() const{
	if(bFacesDirty){
		// if we are doing triangles, we have to use a vert and normal for each triangle
		// that way we can calculate face normals and use getFaceNormal();
		faces.resize( indices.size()/3 );

		int index	   = 0;
		int triindex	= 0;

		bool bHasColors	 = hasColors();
		bool bHasNormals	= hasNormals();
		bool bHasTexcoords  = hasTexCoords();

		if( getMode() == OF_PRIMITIVE_TRIANGLES) {
			for(std::size_t j = 0; j < indices.size(); j += 3) {
				ofMeshFace_<V,N,C,T> & tri = faces[triindex];
				for(std::size_t k = 0; k < 3; k++) {
					index = indices[j+k];
					tri.setVertex( k, vertices[index] );
					if(bHasNormals)
						tri.setNormal(k, normals[index] );
					if(bHasTexcoords)
						tri.setTexCoord(k, texCoords[index] );
					if(bHasColors)
						tri.setColor(k, colors[index] );
				}
				triindex++;
			}

		} else {
			ofLogWarning("ofMesh") << "getUniqueFaces(): only works with primitive mode OF_PRIMITIVE_TRIANGLES";
		}

		bFacesDirty = false;
	}

	return faces;

}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
std::vector<N> ofMesh_<V,N,C,T>::getFaceNormals( bool perVertex ) const{
	// default for ofPrimitiveBase is vertex normals //
	std::vector<N> faceNormals;

	if( hasVertices() ) {
		if(vertices.size() > 3 && indices.size() > 3) {
			if(perVertex){
				faceNormals.resize(indices.size()*3);
			}else{
				faceNormals.resize(indices.size());
			}
			ofMeshFace_<V,N,C,T> face;
			N n;
			for(ofIndexType i = 0; i < indices.size(); i+=3) {
				face.setVertex( 0, vertices[indices[i+0]] );
				face.setVertex( 1, vertices[indices[i+1]] );
				face.setVertex( 2, vertices[indices[i+2]] );

				n = face.getFaceNormal();

				faceNormals[i]=n;
				if(perVertex) {
					faceNormals[i+1]=n;
					faceNormals[i+2]=n;
				}
			}
		}
	}

	return faceNormals;
}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::setFromTriangles( const std::vector<ofMeshFace_<V,N,C,T>>& tris, bool bUseFaceNormal ) {
	if(tris.empty()) {
		ofLogWarning("ofMesh") << "setFromTriangles(): ignoring empty tris vector";
		return;
	}

	typename std::vector<ofMeshFace_<V,N,C,T>>::const_iterator it;

	vertices.resize(tris.size()*3 );
	it = tris.begin();
	// if the first tri has data, assume the rest do as well //
	if(it->hasNormals()){
		normals.resize(tris.size()*3);
	}else{
		normals.clear();
	}
	if(it->hasColors()){
		colors.resize(tris.size()*3);
	}else{
		colors.clear();
	}
	if(it->hasTexcoords()){
		texCoords.resize(tris.size()*3);
	}else{
		texCoords.clear();
	}

	int i = 0;
	for(it = tris.begin(); it != tris.end(); it++) {
		for(std::size_t k = 0; k < 3; k++) {
			vertices[i] = it->getVertex(k);
			if(it->hasTexcoords())
				texCoords[i] = it->getTexCoord(k);
			if(it->hasColors())
				colors[i] = it->getColor(k);
			if(bUseFaceNormal)
				normals[i] = it->getFaceNormal();
			else if(it->hasNormals())
				normals[i] = it->getNormal(k);
			i++;
		}
	}

	setupIndicesAuto();
	bVertsChanged = true;
	bIndicesChanged = true;
	bNormalsChanged = true;
	bColorsChanged = true;
	bTexCoordsChanged = true;

	bFacesDirty = false;
	faces = tris;
}


//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::smoothNormals( float angle ) {

	if( getMode() == OF_PRIMITIVE_TRIANGLES) {
		std::vector<ofMeshFace_<V,N,C,T>> triangles = getUniqueFaces();
		std::vector<V> verts;
		for(ofIndexType i = 0; i < triangles.size(); i++) {
			for(ofIndexType j = 0; j < 3; j++) {
				verts.push_back( triangles[i].getVertex(j) );
			}
		}

		std::unordered_map<int, int> removeIds;

		float epsilon = .01f;
		for(ofIndexType i = 0; i < verts.size()-1; i++) {
			for(ofIndexType j = i+1; j < verts.size(); j++) {
				if(i != j) {
					const auto& v1 = toGlm(verts[i]);
					const auto& v2 = toGlm(verts[j]);
					if( glm::distance(v1, v2) <= epsilon ) {
						// average the location //
						verts[i] = (v1+v2)/2.f;
						verts[j] = verts[i];
						removeIds[j] = 1;
					}
				}
			}
		}

		// string of vertex in 3d space to triangle index //
		std::unordered_map<std::string, std::vector<int> > vertHash;

		//ofLogNotice("ofMesh") << "smoothNormals(): num verts = " << verts.size() << " tris size = " << triangles.size();

		std::string xStr, yStr, zStr;

		for(ofIndexType i = 0; i < verts.size(); i++ ) {
			xStr = "x"+ofToString(verts[i].x==-0?0:verts[i].x);
			yStr = "y"+ofToString(verts[i].y==-0?0:verts[i].y);
			zStr = "z"+ofToString(verts[i].z==-0?0:verts[i].z);
			std::string vstring = xStr+yStr+zStr;
			if(vertHash.find(vstring) == vertHash.end()) {
				for(ofIndexType j = 0; j < triangles.size(); j++) {
					for(ofIndexType k = 0; k < 3; k++) {
						if(verts[i].x == triangles[j].getVertex(k).x) {
							if(verts[i].y == triangles[j].getVertex(k).y) {
								if(verts[i].z == triangles[j].getVertex(k).z) {
									vertHash[vstring].push_back( j );
								}
							}
						}
					}
				}
			}
		}

//		for( std::unordered_map<std::string, std::vector<int> >::iterator it = vertHash.begin(); it != vertHash.end(); ++it) {
//			//for( std::unordered_map<std::string, int >::iterator it = vertHash.begin(); it != vertHash.end(); ++it) {
//			ofLogNotice("ofMesh") << "smoothNormals(): " << it->first << "  num = " << it->second.size();
//		}

		V vert;
		N normal;
		float angleCos = cos(ofDegToRad(angle));
		float numNormals=0;

		for(ofIndexType j = 0; j < triangles.size(); j++) {
			for(ofIndexType k = 0; k < 3; k++) {
				vert = triangles[j].getVertex(k);
				xStr = "x"+ofToString(vert.x==-0?0:vert.x);
				yStr = "y"+ofToString(vert.y==-0?0:vert.y);
				zStr = "z"+ofToString(vert.z==-0?0:vert.z);

				std::string vstring = xStr+yStr+zStr;
				numNormals=0;
				normal = {0.f,0.f,0.f};
				if(vertHash.find(vstring) != vertHash.end()) {
					for(ofIndexType i = 0; i < vertHash[vstring].size(); i++) {
						auto f1 = triangles[j].getFaceNormal();
						auto f2 = triangles[vertHash[vstring][i]].getFaceNormal();
						if(glm::dot(toGlm(f1), toGlm(f2)) >= angleCos ) {
							normal += f2;
							numNormals+=1.f;
						}
					}
					//normal /= (float)vertHash[vstring].size();
					normal /= numNormals;

					triangles[j].setNormal(k, normal);
				}
			}
		}

		//ofLogNotice("ofMesh") << "smoothNormals(): setting from triangles ";
		setFromTriangles( triangles );

	}
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMesh_<V,N,C,T>::flatNormals() {
    if( getMode() == OF_PRIMITIVE_TRIANGLES) {
        
        // get copy original mesh data
        auto indices = getIndices();
        auto verts = getVertices();
        auto texCoords = getTexCoords();
        auto colors = getColors();
        
        // remove all data to start from scratch
        clear();
        
        // add mesh data back, duplicating vertices and recalculating normals
        N normal;
        for(ofIndexType i = 0; i < indices.size(); i++) {
            ofIndexType indexCurr = indices[i];
    
            if(i % 3 == 0) {
                ofIndexType indexNext1 = indices[i + 1];
                ofIndexType indexNext2 = indices[i + 2];
                auto e1 = verts[indexCurr] - verts[indexNext1];
                auto e2 = verts[indexNext2] - verts[indexNext1];
                normal = glm::normalize(glm::cross(e1, e2));
            }
    
            addIndex(i);
            addNormal(normal);
    
            if(indexCurr < texCoords.size()) {
                addTexCoord(texCoords[indexCurr]);
            }
    
            if(indexCurr < verts.size()) {
                addVertex(verts[indexCurr]);
            }
    
            if(indexCurr < colors.size()) {
                addColor(colors[indexCurr]);
            }
        }
    }
}

// PLANE MESH //


//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T> ofMesh_<V,N,C,T>::plane(float width, float height, int columns, int rows, ofPrimitiveMode mode ) {
	ofMesh_<V,N,C,T> mesh;

	if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
		ofLogWarning("ofMesh") << "ofGetPlaneMesh(): primtive mode " << mode << " not supported, setting to OF_PRIMITIVE_TRIANGLES";
		mode = OF_PRIMITIVE_TRIANGLES;
	}

	mesh.setMode(mode);

	V vert;
	N normal(0, 0, 1); // always facing forward //
	T texcoord;

	// the origin of the plane is at the center //
	float halfW = width  * 0.5f;
	float halfH = height * 0.5f;
	
	// add the vertexes //
	for(int iy = 0; iy != rows; iy++) {
		for(int ix = 0; ix != columns; ix++) {

			// normalized tex coords //
			texcoord.x =       ((float)ix/((float)columns-1));
			texcoord.y = 1.f - ((float)iy/((float)rows-1));

			vert.x = texcoord.x * width - halfW;
			vert.y = -(texcoord.y-1) * height - halfH;

			mesh.addVertex(vert);
			mesh.addTexCoord(texcoord);
			mesh.addNormal(normal);
		}
	}
	if(mode == OF_PRIMITIVE_TRIANGLE_STRIP) {
		for(int y = 0; y < rows-1; y++) {
			// even rows //
			if((y&1)==0) {
				for(int x = 0; x < columns; x++) {
					mesh.addIndex( (y) * columns + x );
					mesh.addIndex( (y+1) * columns + x);
				}
			} else {
				for(int x = columns-1; x >0; x--) {
					mesh.addIndex( (y+1) * columns + x );
					mesh.addIndex( y * columns + x-1 );
				}
			}
		}

		if(rows%2!=0) mesh.addIndex(mesh.getNumVertices()-columns);
	} else {
		// Triangles //
		for(int y = 0; y < rows-1; y++) {
			for(int x = 0; x < columns-1; x++) {
				// first triangle //
				mesh.addIndex((y)*columns + x);
				mesh.addIndex((y)*columns + x+1);
				mesh.addIndex((y+1)*columns + x);

				// second triangle //
				mesh.addIndex((y)*columns + x+1);
				mesh.addIndex((y+1)*columns + x+1);
				mesh.addIndex((y+1)*columns + x);
			}
		}
	}

	return mesh;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T> ofMesh_<V,N,C,T>::sphere( float radius, int res, ofPrimitiveMode mode ) {

	ofMesh_<V,N,C,T> mesh;

	float doubleRes = res*2.f;
	float polarInc = glm::pi<float>()/(res); // ringAngle
	float azimInc = glm::two_pi<float>()/(doubleRes); // segAngle //

	if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
		mode = OF_PRIMITIVE_TRIANGLE_STRIP;
	}
	mesh.setMode(mode);

	V vert;
	T tcoord;

	for(float i = 0; i < res+1; i++) {

		float tr = sin( glm::pi<float>()-i * polarInc );
		float ny = cos( glm::pi<float>()-i * polarInc );

		tcoord.y = 1.f - (i / res);

		for(float j = 0; j <= doubleRes; j++) {

			float nx = tr * sin(j * azimInc);
			float nz = tr * cos(j * azimInc);

			tcoord.x = j / (doubleRes);

			vert = {nx, ny, nz};
			mesh.addNormal(vert);
			vert *= radius;
			mesh.addVertex(vert);
			mesh.addTexCoord(tcoord);
		}
	}

	int nr = doubleRes+1;
	if(mode == OF_PRIMITIVE_TRIANGLES) {

		ofIndexType index1, index2, index3;

		for(float iy = 0; iy < res; iy++) {
			for(float ix = 0; ix < doubleRes; ix++) {

				// first tri //
				if(iy > 0) {
					index1 = (iy+0) * (nr) + (ix+0);
					index2 = (iy+0) * (nr) + (ix+1);
					index3 = (iy+1) * (nr) + (ix+0);

					mesh.addIndex(index1);
					mesh.addIndex(index3);
					mesh.addIndex(index2);
				}

				if(iy < res-1 ) {
					// second tri //
					index1 = (iy+0) * (nr) + (ix+1);
					index2 = (iy+1) * (nr) + (ix+1);
					index3 = (iy+1) * (nr) + (ix+0);

					mesh.addIndex(index1);
					mesh.addIndex(index3);
					mesh.addIndex(index2);

				}
			}
		}

	} else {
		for(int y = 0; y < res; y++) {
			for(int x = 0; x <= doubleRes; x++) {
				mesh.addIndex( (y)*nr + x );
				mesh.addIndex( (y+1)*nr + x );
			}
		}
	}


	return mesh;
}

/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural
 
 For the latest info, see http://code.google.com/p/ogre-procedural/
 
 Copyright (c) 2010 Michael Broutin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
// http://code.google.com/p/ogre-procedural/source/browse/library/src/ProceduralIcoSphereGenerator.cpp


//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T> ofMesh_<V,N,C,T>::icosahedron(float radius) {
        auto mesh = icosphere(radius, 0);
        mesh.flatNormals();
	return mesh;
}

// based on code by Michael Broutin for the ogre-procedural library //
// http://code.google.com/p/ogre-procedural/source/browse/library/src/ProceduralIcoSphereGenerator.cpp
// For the latest info, see http://code.google.com/p/ogre-procedural/ //

//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T> ofMesh_<V,N,C,T>::icosphere(float radius, std::size_t iterations) {
	ofMesh_<V,N,C,T> sphere;

	/// Step 1 : Generate icosahedron
	const float sqrt5 = sqrt(5.0f);
	const float phi = (1.0f + sqrt5) * 0.5f;
	const float invnorm = 1/sqrt(phi*phi+1);

    sphere.addVertex(invnorm * V(-1,  phi, 0));//0
	sphere.addVertex(invnorm * V( 1,  phi, 0));//1
	sphere.addVertex(invnorm * V(0,   1,  -phi));//2
	sphere.addVertex(invnorm * V(0,   1,   phi));//3
	sphere.addVertex(invnorm * V(-phi,0,  -1));//4
	sphere.addVertex(invnorm * V(-phi,0,   1));//5
	sphere.addVertex(invnorm * V( phi,0,  -1));//6
	sphere.addVertex(invnorm * V( phi,0,   1));//7
	sphere.addVertex(invnorm * V(0,   -1, -phi));//8
	sphere.addVertex(invnorm * V(0,   -1,  phi));//9
	sphere.addVertex(invnorm * V(-1,  -phi,0));//10
	sphere.addVertex(invnorm * V( 1,  -phi,0));//11
       
        ofIndexType firstFaces[] = {
		0,1,2,
		0,3,1,
		0,4,5,
		1,7,6,
		1,6,2,
		1,3,7,
		0,2,4,
		0,5,3,
		2,6,8,
		2,8,4,
		3,5,9,
		3,9,7,
		11,6,7,
		10,5,4,
		10,4,8,
		10,9,5,
		11,8,6,
		11,7,9,
		10,8,11,
		10,11,9
	};

        for(ofIndexType i = 0; i < 60; i+=3) {
		sphere.addTriangle(firstFaces[i], firstFaces[i+1], firstFaces[i+2]);
	}
        
	auto& vertices = sphere.getVertices();
	auto& faces = sphere.getIndices();

	ofIndexType size = faces.size();

	/// Step 2 : tessellate
	for (ofIndexType iteration = 0; iteration < iterations; iteration++)
	{
		size*=4;
		std::vector<ofIndexType> newFaces;
		for (ofIndexType i=0; i<size/12; i++)
		{
			auto i1 = faces[i*3];
			auto i2 = faces[i*3+1];
			auto i3 = faces[i*3+2];
			auto i12 = vertices.size();
			auto i23 = i12+1;
			auto i13 = i12+2;
			auto v1 = vertices[i1];
			auto v2 = vertices[i2];
			auto v3 = vertices[i3];
			//make 1 vertice at the center of each edge and project it onto the sphere
			vertices.push_back(glm::normalize(toGlm(v1+v2)));
			vertices.push_back(glm::normalize(toGlm(v2+v3)));
			vertices.push_back(glm::normalize(toGlm(v1+v3)));
			//now recreate indices
			newFaces.push_back(i1);
			newFaces.push_back(i12);
			newFaces.push_back(i13);
			newFaces.push_back(i2);
			newFaces.push_back(i23);
			newFaces.push_back(i12);
			newFaces.push_back(i3);
			newFaces.push_back(i13);
			newFaces.push_back(i23);
			newFaces.push_back(i12);
			newFaces.push_back(i23);
			newFaces.push_back(i13);
		}
		faces.swap(newFaces);
	}

	/// Step 3 : generate texcoords
	std::vector<T> texCoords;
	for (ofIndexType i=0;i<vertices.size();i++)
	{
		const auto& vec = vertices[i];
		float u, v;
		float r0 = sqrtf(vec.x*vec.x+vec.z*vec.z);
		float alpha;
		alpha = atan2f(vec.z,vec.x);
		u = alpha/glm::two_pi<float>()+.5f;
		v = atan2f(vec.y, r0)/glm::pi<float>() + .5f;
		// reverse the u coord, so the default is texture mapped left to
		// right on the outside of a sphere 
		// reverse the v coord, so that texture origin is at top left
		texCoords.push_back(T(1.0-u,1.f-v));
	}

	/// Step 4 : fix texcoords
	// find vertices to split
	std::vector<ofIndexType> indexToSplit;

	for (ofIndexType i=0;i<faces.size()/3;i++)
	{
		T& t0 = texCoords[faces[i*3+0]];
		T& t1 = texCoords[faces[i*3+1]];
		T& t2 = texCoords[faces[i*3+2]];

		if (std::abs(t2.x-t0.x)>0.5)
		{
			if (t0.x<0.5)
				indexToSplit.push_back(faces[i*3]);
			else
				indexToSplit.push_back(faces[i*3+2]);
		}
		if (std::abs(t1.x-t0.x)>0.5)
		{
			if (t0.x<0.5)
				indexToSplit.push_back(faces[i*3]);
			else
				indexToSplit.push_back(faces[i*3+1]);
		}
		if (std::abs(t2.x-t1.x)>0.5)
		{
			if (t1.x<0.5)
				indexToSplit.push_back(faces[i*3+1]);
			else
				indexToSplit.push_back(faces[i*3+2]);
		}
	}

	//split vertices
	for (ofIndexType i=0;i<indexToSplit.size();i++)
	{
		ofIndexType index = indexToSplit[i];
		//duplicate vertex
		V v = vertices[index];
		T t = texCoords[index] + T(1.f, 0.f);
		vertices.push_back(v);
		texCoords.push_back(t);
		ofIndexType newIndex = vertices.size()-1;
		//reassign indices
		for (ofIndexType j=0;j<faces.size();j++)
		{
			if (faces[j]==index)
			{
				ofIndexType index1 = faces[(j+1)%3+(j/3)*3];
				ofIndexType index2 = faces[(j+2)%3+(j/3)*3];
				if ((texCoords[index1].x>0.5) || (texCoords[index2].x>0.5))
				{
					faces[j] = newIndex;
				}
			}
		}
	}

	// tig: flip face(=triangle) winding order, so that we are consistent with all other ofPrimitives.
	// i wish there was a more elegant way to do this, but anything happening before "split vertices"
	// makes things very, very complicated.
	
	for (ofIndexType i = 0; i < faces.size(); i+=3) {
		std::swap(faces[i+1], faces[i+2]);
	}

	sphere.addNormals( vertices );
	sphere.addTexCoords( texCoords );

	for(ofIndexType i = 0; i < vertices.size(); i++ ) {
		vertices[i] *= radius;
	}

	return  sphere;
}
/*
 -----------------------------------------------------------------------------
 // END OGRE
 -----------------------------------------------------------------------------
 */



// Cylinder Mesh

//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T> ofMesh_<V,N,C,T>::cylinder( float radius, float height, int radiusSegments, int heightSegments, int numCapSegments, bool bCapped, ofPrimitiveMode mode ) {
	ofMesh_<V,N,C,T> mesh;
	if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
		mode = OF_PRIMITIVE_TRIANGLE_STRIP;
	}
	mesh.setMode(mode);

	radiusSegments = radiusSegments+1;
	int capSegs = numCapSegments;
	capSegs = capSegs+1;
	heightSegments = heightSegments+1;
	if(heightSegments < 2) heightSegments = 2;
	if( capSegs < 2 ) bCapped = false;
	if(!bCapped) capSegs=1;

	float angleIncRadius = -1 * (glm::two_pi<float>()/((float)radiusSegments-1.f));
	float heightInc = height/((float)heightSegments-1.f);
	float halfH = height*.5f;

	float newRad;
	V vert;
	T tcoord;
	N normal;
	glm::vec3 up(0,1,0);

	std::size_t vertOffset = 0;

	float maxTexY   = heightSegments-1.f;
	if(capSegs > 0) {
		maxTexY += (capSegs*2)-2.f;
	}
	float maxTexYNormalized = (capSegs-1.f) / maxTexY;

	// add the top cap //
	if(bCapped && capSegs > 0) {
		normal = {0.f, -1.f, 0.f};
		for(int iy = 0; iy < capSegs; iy++) {
			for(int ix = 0; ix < radiusSegments; ix++) {
				newRad = ofMap((float)iy, 0, capSegs-1, 0.0, radius);
				vert.x = cos((float)ix*angleIncRadius) * newRad;
				vert.z = sin((float)ix*angleIncRadius) * newRad;
				vert.y = -halfH;

				tcoord.x = (float)ix/((float)radiusSegments-1.f);
				tcoord.y = 1.f - ofMap(iy, 0, capSegs-1, 0, maxTexYNormalized);

				mesh.addTexCoord( tcoord );
				mesh.addVertex( vert );
				mesh.addNormal( normal );
			}
		}

		if(mode == OF_PRIMITIVE_TRIANGLES) {
			for(int y = 0; y < capSegs-1; y++) {
				for(int x = 0; x < radiusSegments-1; x++) {
					if(y > 0) {
						// first triangle //
						mesh.addIndex( (y)*radiusSegments + x + vertOffset );
						mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
						mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
					}

					// second triangle //
					mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
					mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset);
					mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
				}
			}
		} else {
			for(int y = 0; y < capSegs-1; y++) {
				for(int x = 0; x < radiusSegments; x++) {
					mesh.addIndex( (y)*radiusSegments + x + vertOffset );
					mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
				}
			}
		}

		vertOffset = mesh.getNumVertices();

	}

	//maxTexY			 = heightSegments-1.f + capSegs-1.f;
	float minTexYNormalized = 0;
	if(bCapped) minTexYNormalized = maxTexYNormalized;
	maxTexYNormalized   = 1.f;
	if(bCapped) maxTexYNormalized = (heightSegments) / maxTexY;

	// cylinder vertices //
	for(int iy = 0; iy < heightSegments; iy++) {
		normal = {1.f, 0.f, 0.f};
		for(int ix = 0; ix < radiusSegments; ix++) {

			//newRad = ofMap((float)iy, 0, heightSegments-1, 0.0, radius);
			vert.x = cos(ix*angleIncRadius) * radius;
			vert.y = heightInc*float(iy) - halfH;
			vert.z = sin(ix*angleIncRadius) * radius;

			tcoord.x = float(ix)/(float(radiusSegments)-1.f);
			tcoord.y = 1.f - ofMap(iy, 0, heightSegments-1, minTexYNormalized, maxTexYNormalized );

			mesh.addTexCoord( tcoord );
			mesh.addVertex( vert );
			mesh.addNormal( normal );

			normal = glm::rotate(toGlm(normal), -angleIncRadius, up);

		}
	}

	if(mode == OF_PRIMITIVE_TRIANGLES) {
		for(int y = 0; y < heightSegments-1; y++) {
			for(int x = 0; x < radiusSegments-1; x++) {
				// first triangle //
				mesh.addIndex( (y)*radiusSegments + x + vertOffset);
				mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset );
				mesh.addIndex( (y+1)*radiusSegments + x + vertOffset );

				// second triangle //
				mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset );
				mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset );
				mesh.addIndex( (y+1)*radiusSegments + x + vertOffset );
			}
		}
	} else {
		for(int y = 0; y < heightSegments-1; y++) {
			for(int x = 0; x < radiusSegments; x++) {
				mesh.addIndex( (y)*radiusSegments + x + vertOffset );
				mesh.addIndex( (y+1)*radiusSegments + x + vertOffset );
			}
		}
	}

	vertOffset = mesh.getNumVertices();

	// add the bottom cap
	if(bCapped && capSegs > 0) {
		minTexYNormalized = maxTexYNormalized;
		maxTexYNormalized   = 1.f;

		normal = {0.f, 1.f, 0.f};
		for(int iy = 0; iy < capSegs; iy++) {
			for(int ix = 0; ix < radiusSegments; ix++) {
				newRad = ofMap((float)iy, 0, capSegs-1, radius, 0.0);
				vert.x = cos((float)ix*angleIncRadius) * newRad;
				vert.z = sin((float)ix*angleIncRadius) * newRad;
				vert.y = halfH;

				tcoord.x = (float)ix/((float)radiusSegments-1.f);
				tcoord.y = 1.f - ofMap(iy, 0, capSegs-1, minTexYNormalized, maxTexYNormalized);

				mesh.addTexCoord( tcoord );
				mesh.addVertex( vert );
				mesh.addNormal( normal );
			}
		}

		if(mode == OF_PRIMITIVE_TRIANGLES) {
			for(int y = 0; y < capSegs-1; y++) {
				for(int x = 0; x < radiusSegments-1; x++) {
					// first triangle //
					mesh.addIndex( (y)*radiusSegments + x + vertOffset );
					mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
					mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);

					if(y < capSegs -1 && capSegs > 2) {
						// second triangle //
						mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
						mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset);
						mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
					}
				}
			}
		} else {
			for(int y = 0; y < capSegs-1; y++) {
				for(int x = 0; x < radiusSegments; x++) {
					mesh.addIndex( (y)*radiusSegments + x + vertOffset );
					mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
				}
			}
		}

		vertOffset = mesh.getNumVertices();

	}

	return mesh;
}

// Cone Mesh //


//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T> ofMesh_<V,N,C,T>::cone( float radius, float height, int radiusSegments, int heightSegments, int capSegments, ofPrimitiveMode mode ) {
	ofMesh_<V,N,C,T> mesh;
	if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
		mode = OF_PRIMITIVE_TRIANGLE_STRIP;
	}
	mesh.setMode(mode);

	radiusSegments = radiusSegments+1;
	capSegments = capSegments+1;
	heightSegments = heightSegments+1;
	if(heightSegments < 2) heightSegments = 2;
	int capSegs = capSegments;
	if( capSegs < 2 ) {
		capSegs = 0;
	}


	float angleIncRadius = -1.f * ((glm::two_pi<float>()/((float)radiusSegments-1.f)));
	float heightInc = height/((float)heightSegments-1);
	float halfH = height*.5f;

	float newRad;
	V vert;
	N normal;
	T tcoord;
	glm::vec3 up(0,1,0);

	std::size_t vertOffset = 0;

	float maxTexY = heightSegments-1.f;
	if(capSegs > 0) {
		maxTexY += capSegs-1.f;
	}

	V startVec(0, -halfH-1.f, 0);

	// cone vertices //
	for(int iy = 0; iy < heightSegments; iy++) {
		for(int ix = 0; ix < radiusSegments; ix++) {

			newRad = ofMap((float)iy, 0, heightSegments-1, 0.0, radius);
			vert.x = cos((float)ix*angleIncRadius) * newRad;
			vert.y = heightInc*((float)iy) - halfH;
			vert.z = sin((float)ix*angleIncRadius) * newRad;

			tcoord.x = (float)ix/((float)radiusSegments-1.f);
			tcoord.y = 1.f - (float)iy/((float)maxTexY);

			mesh.addTexCoord( tcoord );
			mesh.addVertex( vert );

			if(iy == 0) {
				newRad = 1.f;
				vert.x = cos((float)ix*angleIncRadius) * newRad;
				vert.y = heightInc*((float)iy) - halfH;
				vert.z = sin((float)ix*angleIncRadius) * newRad;
			}

			auto diff = toGlm(vert - startVec);
			auto crossed = glm::cross(up, toGlm(vert));
			normal = glm::cross(crossed, diff);
			mesh.addNormal( glm::normalize(toGlm(normal)) );

		}
	}

	if(mode == OF_PRIMITIVE_TRIANGLES) {
		for(int y = 0; y < heightSegments-1; y++) {
			for(int x = 0; x < radiusSegments-1; x++) {
				if(y > 0){
					// first triangle //
					mesh.addIndex( (y)*radiusSegments + x );
					mesh.addIndex( (y)*radiusSegments + x+1 );
					mesh.addIndex( (y+1)*radiusSegments + x );
				}

				// second triangle //
				mesh.addIndex( (y)*radiusSegments + x+1 );
				mesh.addIndex( (y+1)*radiusSegments + x+1 );
				mesh.addIndex( (y+1)*radiusSegments + x );
			}
		}
	} else {
		for(int y = 0; y < heightSegments-1; y++) {
			for(int x = 0; x < radiusSegments; x++) {
				mesh.addIndex( (y)*radiusSegments + x );
				mesh.addIndex( (y+1)*radiusSegments + x );
			}
		}
	}

	vertOffset = mesh.getNumVertices();
	float maxTexYNormalized = (heightSegments-1.f) / maxTexY;

	// add the cap //
	normal= {0.f,1.f,0.f};
	for(int iy = 0; iy < capSegs; iy++) {
		for(int ix = 0; ix < radiusSegments; ix++) {
			newRad = ofMap((float)iy, 0, capSegs-1, radius, 0.0);
			vert.x = cos((float)ix*angleIncRadius) * newRad;
			vert.z = sin((float)ix*angleIncRadius) * newRad;
			vert.y = halfH;

			tcoord.x = (float)ix/((float)radiusSegments-1.f);
			tcoord.y = 1.f - ofMap(iy, 0, capSegs-1, maxTexYNormalized, 1.f);

			mesh.addTexCoord( tcoord );
			mesh.addVertex( vert );
			mesh.addNormal( normal );
		}
	}

	if(mode == OF_PRIMITIVE_TRIANGLES) {
		if( capSegs > 0 ) {
			for(int y = 0; y < capSegs-1; y++) {
				for(int x = 0; x < radiusSegments-1; x++) {
					// first triangle //
					mesh.addIndex( (y)*radiusSegments + x + vertOffset );
					mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
					mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);

					if(y < capSegs-1) {
						// second triangle //
						mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
						mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset);
						mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
					}
				}
			}
		}
	} else {
		if(capSegs > 0 ) {
			for(int y = 0; y < capSegs-1; y++) {
				for(int x = 0; x < radiusSegments; x++) {
					mesh.addIndex( (y)*radiusSegments + x + vertOffset );
					mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
				}
			}
		}
	}

	return mesh;
}


// Box Mesh //


//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T> ofMesh_<V,N,C,T>::box( float width, float height, float depth, int resX, int resY, int resZ ) {
	// mesh only available as triangles //
	ofMesh_<V,N,C,T> mesh;
	mesh.setMode( OF_PRIMITIVE_TRIANGLES );

	resX = resX + 1;
	resY = resY + 1;
	resZ = resZ + 1;

	if( resX < 2 ) resX = 0;
	if( resY < 2 ) resY = 0;
	if( resZ < 2 ) resZ = 0;

	// halves //
	float halfW = width * .5f;
	float halfH = height * .5f;
	float halfD = depth * .5f;

	V vert;
	T texcoord;
	N normal;
	std::size_t vertOffset = 0;

	// TRIANGLES //

	// Front Face //
	normal = {0.f, 0.f, 1.f};
	// add the vertexes //
	for(int iy = 0; iy < resY; iy++) {
		for(int ix = 0; ix < resX; ix++) {

			// normalized tex coords //
			texcoord.x = ((float)ix/((float)resX-1.f));
			texcoord.y = 1.f - ((float)iy/((float)resY-1.f));

			vert.x = texcoord.x * width - halfW;
			vert.y = -(texcoord.y-1.f) * height - halfH;
			vert.z = halfD;

			mesh.addVertex(vert);
			mesh.addTexCoord(texcoord);
			mesh.addNormal(normal);
		}
	}

	for(int y = 0; y < resY-1; y++) {
		for(int x = 0; x < resX-1; x++) {
			// first triangle //
			mesh.addIndex((y)*resX + x + vertOffset);
			mesh.addIndex((y+1)*resX + x + vertOffset);
			mesh.addIndex((y)*resX + x+1 + vertOffset);

			// second triangle //
			mesh.addIndex((y)*resX + x+1 + vertOffset);
			mesh.addIndex((y+1)*resX + x + vertOffset);
			mesh.addIndex((y+1)*resX + x+1 + vertOffset);
		}
	}

	vertOffset = mesh.getNumVertices();


	// Right Side Face //
	normal = {1.f, 0.f, 0.f};
	// add the vertexes //
	for(int iy = 0; iy < resY; iy++) {
		for(int ix = 0; ix < resZ; ix++) {

			// normalized tex coords //
			texcoord.x = ((float)ix/((float)resZ-1.f));
			texcoord.y = 1.f - ((float)iy/((float)resY-1.f));

			//vert.x = texcoord.x * width - halfW;
			vert.x = halfW;
			vert.y = -(texcoord.y-1.f) * height - halfH;
			vert.z = texcoord.x * -depth + halfD;

			mesh.addVertex(vert);
			mesh.addTexCoord(texcoord);
			mesh.addNormal(normal);
		}
	}

	for(int y = 0; y < resY-1; y++) {
		for(int x = 0; x < resZ-1; x++) {
			// first triangle //
			mesh.addIndex((y)*resZ + x + vertOffset);
			mesh.addIndex((y+1)*resZ + x + vertOffset);
			mesh.addIndex((y)*resZ + x+1 + vertOffset);

			// second triangle //
			mesh.addIndex((y)*resZ + x+1 + vertOffset);
			mesh.addIndex((y+1)*resZ + x + vertOffset);
			mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
		}
	}

	vertOffset = mesh.getNumVertices();

	// Left Side Face //
	normal = {-1.f, 0.f, 0.f};
	// add the vertexes //
	for(int iy = 0; iy < resY; iy++) {
		for(int ix = 0; ix < resZ; ix++) {

			// normalized tex coords //
			texcoord.x = ((float)ix/((float)resZ-1.f));
			texcoord.y = 1.f-((float)iy/((float)resY-1.f));

			//vert.x = texcoord.x * width - halfW;
			vert.x = -halfW;
			vert.y = -(texcoord.y-1.f) * height - halfH;
			vert.z = texcoord.x * depth - halfD;

			mesh.addVertex(vert);
			mesh.addTexCoord(texcoord);
			mesh.addNormal(normal);
		}
	}

	for(int y = 0; y < resY-1; y++) {
		for(int x = 0; x < resZ-1; x++) {
			// first triangle //
			mesh.addIndex((y)*resZ + x + vertOffset);
			mesh.addIndex((y+1)*resZ + x + vertOffset);
			mesh.addIndex((y)*resZ + x+1 + vertOffset);

			// second triangle //
			mesh.addIndex((y)*resZ + x+1 + vertOffset);
			mesh.addIndex((y+1)*resZ + x + vertOffset);
			mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
		}
	}

	vertOffset = mesh.getNumVertices();


	// Back Face //
	normal = {0.f, 0.f, -1.f};
	// add the vertexes //
	for(int iy = 0; iy < resY; iy++) {
		for(int ix = 0; ix < resX; ix++) {

			// normalized tex coords //
			texcoord.x = ((float)ix/((float)resX-1.f));
			texcoord.y = 1.f-((float)iy/((float)resY-1.f));

			vert.x = texcoord.x * -width + halfW;
			vert.y = -(texcoord.y-1.f) * height - halfH;
			vert.z = -halfD;

			mesh.addVertex(vert);
			mesh.addTexCoord(texcoord);
			mesh.addNormal(normal);
		}
	}

	for(int y = 0; y < resY-1; y++) {
		for(int x = 0; x < resX-1; x++) {
			// first triangle //
			mesh.addIndex((y)*resX + x + vertOffset);
			mesh.addIndex((y+1)*resX + x + vertOffset);
			mesh.addIndex((y)*resX + x+1 + vertOffset);

			// second triangle //
			mesh.addIndex((y)*resX + x+1 + vertOffset);
			mesh.addIndex((y+1)*resX + x + vertOffset);
			mesh.addIndex((y+1)*resX + x+1 + vertOffset);
		}
	}

	vertOffset = mesh.getNumVertices();


	// Top Face //
	normal = {0.f, -1.f, 0.f};
	// add the vertexes //
	for(int iy = 0; iy < resZ; iy++) {
		for(int ix = 0; ix < resX; ix++) {

			// normalized tex coords //
			texcoord.x = ((float)ix/((float)resX-1.f));
			texcoord.y = 1.f-((float)iy/((float)resZ-1.f));

			vert.x = texcoord.x * width - halfW;
			//vert.y = -(texcoord.y-1.f) * height - halfH;
			vert.y = -halfH;
			vert.z = texcoord.y * depth - halfD;

			mesh.addVertex(vert);
			mesh.addTexCoord(texcoord);
			mesh.addNormal(normal);
		}
	}

	for(int y = 0; y < resZ-1; y++) {
		for(int x = 0; x < resX-1; x++) {
			// first triangle //
			mesh.addIndex((y)*resX + x + vertOffset);
			mesh.addIndex((y)*resX + x+1 + vertOffset);
			mesh.addIndex((y+1)*resX + x + vertOffset);

			// second triangle //
			mesh.addIndex((y)*resX + x+1 + vertOffset);
			mesh.addIndex((y+1)*resX + x+1 + vertOffset);
			mesh.addIndex((y+1)*resX + x + vertOffset);
		}
	}

	vertOffset = mesh.getNumVertices();


	// Bottom Face //
	normal = {0.f, 1.f, 0.f};
	// add the vertexes //
	for(int iy = 0; iy < resZ; iy++) {
		for(int ix = 0; ix < resX; ix++) {

			// normalized tex coords //
			texcoord.x = ((float)ix/((float)resX-1.f));
			texcoord.y = 1.f-((float)iy/((float)resZ-1.f));

			vert.x = texcoord.x * width - halfW;
			//vert.y = -(texcoord.y-1.f) * height - halfH;
			vert.y = halfH;
			vert.z = texcoord.y * -depth + halfD;

			mesh.addVertex(vert);
			mesh.addTexCoord(texcoord);
			mesh.addNormal(normal);
		}
	}

	for(int y = 0; y < resZ-1; y++) {
		for(int x = 0; x < resX-1; x++) {
			// first triangle //
			mesh.addIndex((y)*resX + x + vertOffset);
			mesh.addIndex((y)*resX + x+1 + vertOffset);
			mesh.addIndex((y+1)*resX + x + vertOffset);

			// second triangle //
			mesh.addIndex((y)*resX + x+1 + vertOffset);
			mesh.addIndex((y+1)*resX + x+1 + vertOffset);
			mesh.addIndex((y+1)*resX + x + vertOffset);
		}
	}

	return mesh;
}




//--------------------------------------------------------------
/// Returns an ofMesh representing an XYZ coordinate system.
template<class V, class N, class C, class T>
ofMesh_<V,N,C,T> ofMesh_<V,N,C,T>::axis( float size ) {
	ofMesh_<V,N,C,T> mesh;

	// mesh only available as wireframe //
	mesh.setMode(OF_PRIMITIVE_LINES);

	V vertices[6] = {
		V(0,0,0),
		V(size,0,0),
		V(0,0,0),
		V(0,size,0),
		V(0,0,0),
		V(0,0,size),
	};
	C colors[6] = {
		C::red,
		C::red,
		C::green,
		C::green,
		C::blue,
		C::blue,
	};

	mesh.addVertices(vertices, 6);
	mesh.addColors(colors, 6);

	return mesh;
}



//--------------------------------------------------------------
template<class V, class N, class C, class T>
ofMeshFace_<V,N,C,T>::ofMeshFace_()
:bHasNormals(false)
,bHasColors(false)
,bHasTexcoords(false)
,bFaceNormalDirty(false)
{
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
const N & ofMeshFace_<V,N,C,T>::getFaceNormal() const{
	if(bFaceNormalDirty) calculateFaceNormal();
	return faceNormal;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMeshFace_<V,N,C,T>::calculateFaceNormal() const{
	glm::vec3 u, v;

	u = toGlm(vertices[1]-vertices[0]);
	v = toGlm(vertices[2]-vertices[0]);

	faceNormal = glm::cross(u, v);
	faceNormal = glm::normalize(toGlm(faceNormal));
	bFaceNormalDirty = false;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMeshFace_<V,N,C,T>::setVertex( ofIndexType index, const V& v ) {
	vertices[index] = v;
	bFaceNormalDirty = true;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
const V& ofMeshFace_<V,N,C,T>::getVertex( ofIndexType index ) const{
	return vertices[index];
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMeshFace_<V,N,C,T>::setNormal( ofIndexType index, const N& n ) {
	normals[index] = n;
	bHasNormals = true;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
const N& ofMeshFace_<V,N,C,T>::getNormal( ofIndexType index ) const{
	return normals[ index ];
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMeshFace_<V,N,C,T>::setColor( ofIndexType index, const C& color ) {
	colors[index] = color;
	bHasColors = true;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
const C& ofMeshFace_<V,N,C,T>::getColor( ofIndexType index) const{
	return colors[index];
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMeshFace_<V,N,C,T>::setTexCoord( ofIndexType index, const T& tCoord ) {
	texCoords[index] = tCoord;
	bHasTexcoords = true;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
const T& ofMeshFace_<V,N,C,T>::getTexCoord( ofIndexType index ) const{
	return texCoords[index];
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMeshFace_<V,N,C,T>::setHasColors( bool bColors ) {
	bHasColors = bColors;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMeshFace_<V,N,C,T>::setHasNormals( bool bNormals ) {
	bHasNormals = bNormals;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
void ofMeshFace_<V,N,C,T>::setHasTexcoords( bool bTexcoords ) {
	bHasTexcoords = bTexcoords;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMeshFace_<V,N,C,T>::hasColors() const{
	return bHasColors;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMeshFace_<V,N,C,T>::hasNormals() const{
	return bHasNormals;
}

//--------------------------------------------------------------
template<class V, class N, class C, class T>
bool ofMeshFace_<V,N,C,T>::hasTexcoords() const{
	return bHasTexcoords;
}
