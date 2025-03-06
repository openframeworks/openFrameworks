#include "ofVboMesh.h"
#include "ofGLBaseTypes.h"

#ifdef TARGET_ANDROID
#include "ofxAndroidUtils.h"
#endif

ofVboMesh::ofVboMesh(){
	usage= GL_STATIC_DRAW;
	vboNumIndices = 0;
	vboNumVerts = 0;
	vboNumColors = 0;
	vboNumTexCoords = 0;
	vboNumNormals = 0;
	updateSet = false;
}

ofVboMesh::ofVboMesh(const ofMesh & mom)
:ofMesh(mom)
{
	usage= GL_STATIC_DRAW;
	vboNumIndices = 0;
	vboNumVerts = 0;
	vboNumColors = 0;
	vboNumTexCoords = 0;
	vboNumNormals = 0;
	updateSet = false;
}

void ofVboMesh::operator=(const ofMesh & mom)
{
	(*(ofMesh*)this) = mom;
	getVertices();
	getColors();
	getTexCoords();
	getNormals();
	getIndices();
}

ofVboMesh::~ofVboMesh(){
#ifdef TARGET_ANDROID
	ofRemoveListener(ofxAndroidEvents().unloadGL,this,&ofVboMesh::unloadVbo);
#endif
}

void ofVboMesh::setUsage(int _usage){
	usage = _usage;
}

void ofVboMesh::enableColors(){
	vbo.enableColors();
    ofMesh::enableColors();
}

void ofVboMesh::enableTextures(){
	vbo.enableTexCoords();
	ofMesh::enableTextures();
}

void ofVboMesh::enableNormals(){
	vbo.enableNormals();
	ofMesh::enableNormals();
}

void ofVboMesh::enableIndices(){
	vbo.enableIndices();
	ofMesh::enableIndices();
}

void ofVboMesh::disableColors(){
	vbo.disableColors();
	ofMesh::disableColors();
}

void ofVboMesh::disableTextures(){
	vbo.disableTexCoords();
	ofMesh::disableTextures();
}

void ofVboMesh::disableNormals(){
	vbo.disableNormals();
	ofMesh::disableNormals();
}

void ofVboMesh::disableIndices(){
	vbo.disableIndices();
	ofMesh::disableIndices();
}

bool ofVboMesh::usingColors() const {
	return vbo.getUsingColors();
}

bool ofVboMesh::usingTextures() const {
	return vbo.getUsingTexCoords();
}

bool ofVboMesh::usingNormals() const {
	return vbo.getUsingNormals();
}

bool ofVboMesh::usingIndices() const {
	return vbo.getUsingIndices();
}

ofVbo & ofVboMesh::getVbo() {
	updateVbo();
	return vbo;
};

const ofVbo & ofVboMesh::getVbo() const{
	const_cast<ofVboMesh*>(this)->updateVbo();
	return vbo;
}

void ofVboMesh::drawInstanced(ofPolyRenderMode drawMode, int primCount) const{
	if(getNumVertices()==0) return;
	ofGetGLRenderer()->drawInstanced(*this,drawMode,primCount);
}

void ofVboMesh::draw(ofPolyRenderMode drawMode) const{
	if(getNumVertices()==0) return;
	drawInstanced(drawMode, 1);
}

void ofVboMesh::unloadVbo(){
	vbo.clear();
}

void ofVboMesh::updateVbo(){
	if(!vbo.getIsAllocated()){
		#ifdef TARGET_ANDROID
			ofAddListener(ofxAndroidEvents().unloadGL,this,&ofVboMesh::unloadVbo);
		#endif
		if(getNumVertices()){
			vbo.setVertexData(getVerticesPointer(),getNumVertices(),usage);
		}
		if(getNumColors()){
			vbo.setColorData(getColorsPointer(),getNumColors(),usage);
		}
		if(getNumNormals()){
			vbo.setNormalData(getNormalsPointer(),getNumNormals(),usage);
		}
		if(getNumTexCoords()){
			vbo.setTexCoordData(getTexCoordsPointer(),getNumTexCoords(),usage);
		}
		if(getNumIndices()){
			vbo.setIndexData(getIndexPointer(),getNumIndices(),usage);
		}
		vboNumIndices = getNumIndices();
		vboNumVerts = getNumVertices();
		vboNumColors = getNumColors();
		vboNumTexCoords = getNumTexCoords();
		vboNumNormals = getNumNormals();

		// we need to call these to reset the changed flags or else next frame
		// data will be updated again.
		haveVertsChanged();
		haveColorsChanged();
		haveNormalsChanged();
		haveTexCoordsChanged();
		haveIndicesChanged();
	}else{

		if(haveVertsChanged()){
			if(getNumVertices()==0){
				vbo.clearVertices();
				vboNumVerts = getNumVertices();
			}else if(vboNumVerts<getNumVertices() || updateSet){
				vbo.setVertexData(getVerticesPointer(),getNumVertices(),usage);
				vboNumVerts = getNumVertices();
			}else{
				vbo.updateVertexData(getVerticesPointer(),getNumVertices());
			}
		}

		if(haveColorsChanged()){
			if(getNumColors()==0){
				vbo.clearColors();
				vboNumColors = getNumColors();
			}else if(vboNumColors<getNumColors() || updateSet){
				vbo.setColorData(getColorsPointer(),getNumColors(),usage);
				vboNumColors = getNumColors();
			}else{
				vbo.updateColorData(getColorsPointer(),getNumColors());
			}
		}

		if(haveNormalsChanged()){
			if(getNumNormals()==0){
				vbo.clearNormals();
				vboNumNormals = getNumNormals();
			}else if(vboNumNormals<getNumNormals() || updateSet){
				vbo.setNormalData(getNormalsPointer(),getNumNormals(),usage);
				vboNumNormals = getNumNormals();
			}else{
				vbo.updateNormalData(getNormalsPointer(),getNumNormals());
			}
		}

		if(haveTexCoordsChanged()){
			if(getNumTexCoords()==0){
				vbo.clearTexCoords();
				vboNumTexCoords = getNumTexCoords();
			}else if(vboNumTexCoords<getNumTexCoords() || updateSet){
				vbo.setTexCoordData(getTexCoordsPointer(),getNumTexCoords(),usage);
				vboNumTexCoords = getNumTexCoords();
			}else{
				vbo.updateTexCoordData(getTexCoordsPointer(),getNumTexCoords());
			}
		}

		if(haveIndicesChanged()){
			if(getNumIndices()==0){
				vbo.clearIndices();
				vboNumIndices = getNumIndices();
			}else if(vboNumIndices<getNumIndices() || updateSet){
				vbo.setIndexData(getIndexPointer(),getNumIndices(),usage);
				vboNumIndices = getNumIndices();
			}else{
				vbo.updateIndexData(getIndexPointer(),getNumIndices());
			}
		}
		if(updateSet) updateSet = false;
	}
}

void ofVboMesh::removeVertex(ofIndexType index) {
    ofMesh::removeVertex(index);
    vboNumVerts = getNumVertices();
	updateSet = true;
}

void ofVboMesh::removeVertex(ofIndexType startIndex, ofIndexType endIndex) {
    ofMesh::removeVertices(startIndex, endIndex);
    vboNumVerts = getNumVertices();
	updateSet = true;
}

void ofVboMesh::removeColor(ofIndexType index) {
    ofMesh::removeColor(index);
    vboNumColors = getNumColors();
	updateSet = true;
}

void ofVboMesh::removeColor(ofIndexType startIndex, ofIndexType endIndex) {
    ofMesh::removeColors(startIndex, endIndex);
    vboNumColors = getNumColors();
	updateSet = true;
}


