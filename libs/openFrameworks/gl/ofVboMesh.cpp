#include "ofVboMesh.h"

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
	return vbo;
};

void ofVboMesh::drawInstanced(ofPolyRenderMode drawMode, int primCount){
	if(getNumVertices()==0) return;
	updateVbo();
	GLuint mode = ofGetGLPrimitiveMode(getMode());
#ifndef TARGET_OPENGLES
	if (!ofIsGLProgrammableRenderer()) {
		// this is deprecated in GL3.2+
		glPushAttrib(GL_POLYGON_BIT);
	}
	glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(drawMode));
	if(getNumIndices() && drawMode!=OF_MESH_POINTS){
		if (primCount <= 1) {
			vbo.drawElements(mode,getNumIndices());
		} else {
			vbo.drawElementsInstanced(mode,getNumIndices(),primCount);
		}
	}else{
		if (primCount <= 1) {
			vbo.draw(mode,0,getNumVertices());
		} else {
			vbo.drawInstanced(mode,0,getNumVertices(),primCount);
		}
	}
	if (!ofIsGLProgrammableRenderer()){
		glPopAttrib();
	}
#else
	if(drawMode == OF_MESH_POINTS){
		vbo.draw(GL_POINTS,0,getNumVertices());
	}else if(drawMode == OF_MESH_WIREFRAME){
		if(getNumIndices()){
			vbo.drawElements(GL_LINES,getNumIndices());
		}else{
			vbo.draw(GL_LINES,0,getNumVertices());
		}
	}else{
		if(getNumIndices()){
			vbo.drawElements(mode,getNumIndices());
		}else{
			vbo.draw(mode,0,getNumVertices());
		}
	}
#endif
}

void ofVboMesh::draw(ofPolyRenderMode drawMode){
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
