#include "ofMesh.h"
#include "ofVbo.h"

class ofVboMesh{
public:
	ofVboMesh(){
	};
	~ofVboMesh(){
	};
	
	ofVbo* createVbo(){
		vbo = new ofVbo();
		return vbo; 
	}
	
	ofMesh* createMesh(){
		mesh = new ofMesh();
		return mesh; 
	}
	
	void setupVertices(int usage){
		if(vbo!=NULL){
			vbo->setVertexData(&mesh->vertices[0], mesh->vertices.size(), usage);
		}
	}

	void setupColors(int usage){
		if(vbo!=NULL){
			vbo->setColorData(&mesh->colors[0], mesh->numColors(), usage);
			mesh->bUsingColors = true; 
		}
	}

	void setupIndices(){
		if(vbo!=NULL){
			vbo->setIndexData(&mesh->colors[0], mesh->faces.size());
		}
	}
	
	/*
	 void setupNormals(int usage){
		vbo->setNormalData(&mesh->normals[0], mesh->numNormals(), usage);
	 }
	 void setupTextures(int usage){
		vbo->setTexCoordData(&mesh->texCoords[0], mesh->numTexCoords(), usage);
	 }
	 */
	 
	void drawVertices(){
		vbo->draw(GL_POINTS,0,mesh->vertices.size());
	}
	void drawWireframe(){
		vbo->draw(GL_TRIANGLES,0,mesh->vertices.size());
	}
	void drawFaces(){
		vbo->draw(GL_POLYGON,0,mesh->vertices.size());
	}
	
	void addTriangles(const vector<ofVec3f>& verts){
		if(verts.size()%3==0){
		}else {
			//ofLog
			return;
		}
	}
	
	void addTriangleFan(const vector<ofVec3f>& verts){
		//>3
	}
	
	void addTriangleStr(const vector<ofVec3f>& verts){
		//>3
	}
	
    ofMesh* mesh;
	ofVbo* vbo;
	
	
};
