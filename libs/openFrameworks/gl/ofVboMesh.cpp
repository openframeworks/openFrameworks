#include "ofVboMesh.h"
#include "ofBaseTypes.h"

ofVboMesh::ofVboMesh(){
	usage= GL_STATIC_DRAW;
	vboNumIndices = 0;
	vboNumVerts = 0;
	vboNumColors = 0;
	vboNumTexCoords = 0;
	vboNumNormals = 0;
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
}

void ofVboMesh::operator=(const ofMesh & mom)
{
	((ofMesh&)(*this)) = mom;
}

void ofVboMesh::setUsage(int _usage){
	usage = _usage;
}

void ofVboMesh::enableColors(){
	vbo.enableColors();
}

void ofVboMesh::enableTextures(){
	vbo.enableTexCoords();
}

void ofVboMesh::enableNormals(){
	vbo.enableNormals();
}

void ofVboMesh::enableIndices(){
	vbo.enableIndices();
}

void ofVboMesh::disableColors(){
	vbo.disableColors();
}

void ofVboMesh::disableTextures(){
	vbo.disableTexCoords();
}

void ofVboMesh::disableNormals(){
	vbo.disableNormals();
}

void ofVboMesh::disableIndices(){
	vbo.disableIndices();
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

void ofVboMesh::updateVbo(){
	if(!vbo.getIsAllocated()){
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
	}else{

		if(haveVertsChanged()){
			if(getNumVertices()==0){
				vbo.clearVertices();
				vboNumVerts = getNumVertices();
			}else if(vboNumVerts<getNumVertices()){
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
			}else if(vboNumColors<getNumColors()){
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
			}else if(vboNumNormals<getNumNormals()){
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
			}else if(vboNumTexCoords<getNumTexCoords()){
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
			}else if(vboNumIndices<getNumIndices()){
				vbo.setIndexData(getIndexPointer(),getNumIndices(),usage);
				vboNumIndices = getNumIndices();
			}else{
				vbo.updateIndexData(getIndexPointer(),getNumIndices());
			}
		}
	}
}
