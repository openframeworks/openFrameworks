#include "ofVboMesh.h"

//--------------------------------------------------------------
ofVboMesh::ofVboMesh(){
	vbo = ofVbo();
	meshElement = NULL;
}

//--------------------------------------------------------------
ofVboMesh::~ofVboMesh(){
}

//--------------------------------------------------------------
ofVboMesh::ofVboMesh(const ofVboMesh& v){
	clone(v);
}

//--------------------------------------------------------------
ofVboMesh& ofVboMesh::operator=(const ofVboMesh& v){
	clone(v);
	return *this;
}

//--------------------------------------------------------------
void ofVboMesh::clone(const ofVboMesh& v){
	meshElement = v.getMeshElement();
	vbo = v.vbo;
}

//--------------------------------------------------------------
void ofVboMesh::setMeshElement(ofMeshElement* m){
	meshElement = m;
}

//--------------------------------------------------------------
ofMeshElement* ofVboMesh::getMeshElement(){
	return meshElement;
}

//--------------------------------------------------------------
ofMeshElement* ofVboMesh::getMeshElement() const{
	return meshElement;
}

//--------------------------------------------------------------
bool ofVboMesh::setupVertices(int usage){
	int size = meshElement->getNumVertices();
	if(size){
		vbo.setVertexData(meshElement->getVerticesPointer(),size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupVertices - no vertices in mesh.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofVboMesh::setupColors(int usage){
	int size = meshElement->getNumColors();
	if(size){
		vbo.setColorData(meshElement->getColorsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupColors - no colors in meshElement.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofVboMesh::setupNormals(int usage){
	int size = meshElement->getNumNormals();
	if(size){
		vbo.setNormalData(meshElement->getNormalsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupNormals - no normals in meshElement.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofVboMesh::setupTexCoords(int usage){
	int size = meshElement->getNumTexCoords();
	if(size){
		vbo.setTexCoordData(meshElement->getTexCoordsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupTexCoords - no texCoords in meshElement.");
		return false;
	}
}

//--------------------------------------------------------------
/*
void ofVboMesh::setupIndices(int indexMode){
	switch(indexMode){
		case(OF_MESH_POINTS):
		case(OF_MESH_FILL):
			indices.clear();
			for (int i = 0; i < meshElement->faces.size();i++){
				for (int j=0; j< meshElement->faces.at(i).indices.size(); j++){
					indices.push_back((GLuint)meshElement->faces.at(i).indices.at(j));
				}
			}
			vbo.setIndexData(&indices[0], indices.size() );
			break;
		case(OF_MESH_WIREFRAME):
			wfIndices.clear();
			for (int i=0;i<meshElement->faces.size();i++){
				
				int maxIndex = meshElement->faces.at(i).indices.size()-1;
				
				for (int j=0;j<maxIndex;j++){
					wfIndices.push_back( (GLuint) meshElement->faces.at(i).indices.at(j) );
					wfIndices.push_back( (GLuint) meshElement->faces.at(i).indices.at(j+1) );
				}
				
				wfIndices.push_back((GLuint)meshElement->faces.at(i).indices.at(maxIndex));
				wfIndices.push_back((GLuint)meshElement->faces.at(i).indices.at(0));
			}
			break;
		default:break;
	}
}
 */


/*
//--------------------------------------------------------------
void ofVboMesh::addTriangles(const vector<ofVec3f>& verts){
	if(verts.size()%3==0){
		//store current size of the mesh vertices
		int curSize = meshElement->getNumVertices();
		addMeshVertices(verts);
		for (int i = 0; i < verts.size()-2;i++){
			int index = i+curSize;
			meshElement->addFace(index, index+1, index+2);
			indices.push_back(index);
			indices.push_back(index+1);
			indices.push_back(index+2);
		}
	}else {
		ofLog(OF_LOG_ERROR,"ofVboMesh::addTriangles: trying to create triangles from non-divisble-by-three array");
		return;
	}
}
 */

/*
//--------------------------------------------------------------
void ofVboMesh::addTriangleFan(const vector<ofVec3f>& verts){
	if(verts.size()>=3){
		//store current size of the mesh vertices
		int curSize = mesh->vertices.size();
		addMeshVertices(verts);
		for (int i = 1; i < verts.size()-1;i++){
			int index = i+curSize;
			mesh->addFace(0, index, index+1);
			indices.push_back(0);
			indices.push_back(index);
			indices.push_back(index+1);
		}
	}else {
		ofLog(OF_LOG_ERROR,"ofVboMesh::addTriangleFan: must supply at least 3 vertices");
		return;
	}
}
*/

/*
//--------------------------------------------------------------
void ofVboMesh::addTriangleStrip(const vector<ofVec3f>& verts){
	if(verts.size()>=3){
		//store current size of the mesh vertices
		int curSize = meshElement->vertices.size();
		addMeshVertices(verts);
		for (int i = 0; i < verts.size()-1;i++){
			int index = i+curSize;
			meshElement->addFace(index, index+1, index+2);
		}
	}else {
		ofLog(OF_LOG_ERROR,"ofVboMesh::addTriangleStrip: must supply at least 3 vertices");
		return;
	}
}
 */

//--------------------------------------------------------------
void ofVboMesh::addMeshVertices(const vector<ofVec3f>& verts){
	for (int i =0; i < verts.size(); i++){
		//copy vec3 info
		meshElement->addVertex(verts[i]);
		/*
		//if we want to use this info, we need to match the vert size
		if(mesh->bUsingTexCoords) mesh->texCoords.push_back(ofVec2f(0,0));
		if(mesh->bUsingColors) mesh->colors.push_back(ofColor(255,255,255,255));
		 */
	}
}

//--------------------------------------------------------------
void ofVboMesh::drawVertices(){
	//make sure we have vertices setup in the VBO
	if(!vbo.getIsAllocated()){
		setupVertices(OF_VBO_STATIC);
	}else{

		//make sure we have indices
		if(!meshElement->getNumIndices()){
			meshElement->setupIndices();
		}
		
		//make sure they're sent to the vbo
		if(!vbo.getUsingIndices()){
			vbo.setIndexData(meshElement->getIndexPointer(), meshElement->getNumIndices() );
		}
		
		/* mesh element should now know what mode its in, so we won't need this
		//feed vbo the correct index data
		if(mode == OF_MESH_WIREFRAME){
			vbo.setIndexData(&indices[0], indices.size() );
		}
		 */
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		vbo.draw(GL_POINTS,0,meshElement->getNumVertices());
	}
}

//--------------------------------------------------------------
void ofVboMesh::drawWireframe(){
	//make sure we have vertices
	if(!vbo.getIsAllocated()){
		setupVertices(OF_VBO_STATIC);
	}else{
		//make sure we have indices
		if(!meshElement->getNumIndices()){
			meshElement->setupIndices();
		}
		
		//make sure they're sent to the vbo
		if(!vbo.getUsingIndices()){
			vbo.setIndexData(meshElement->getIndexPointer(), meshElement->getNumIndices() );
		}
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		vbo.drawElements(GL_TRIANGLES,meshElement->getNumIndices());
	}
}

//--------------------------------------------------------------
void ofVboMesh::drawFaces(){
	//make sure we have vertices
	if(!vbo.getIsAllocated()){
		setupVertices(OF_VBO_STATIC);
	}else{
		//make sure we have indices
		if(!meshElement->getNumIndices()){
			meshElement->setupIndices();
		}
		
		//make sure they're sent to the vbo
		if(!vbo.getUsingIndices()){
			vbo.setIndexData(meshElement->getIndexPointer(), meshElement->getNumIndices() );
		}
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		vbo.drawElements(GL_TRIANGLES,meshElement->getNumIndices());
	}
}
