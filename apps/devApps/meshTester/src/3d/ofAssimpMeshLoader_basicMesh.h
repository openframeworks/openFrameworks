// assimp include files. These three are usually needed.
#import "assimp.h"
#import "aiPostProcess.h"
#import "aiScene.h"

#pragma once

#include "ofMain.h"

//adapted by Keith Pasko from Anton Marini's ofxAssimpModelLoader addon

#import "ofVboMesh.h"

class ofxAssimpModelLoader{
	
public:
	~ofxAssimpModelLoader();
	ofxAssimpModelLoader();
	
	void loadModel(string modelName);
    
	void draw();
	
	// Our array of textures we load from the models path.
	vector <ofImage> textures;
    
	// TODO: convert to ofMesh or ofVBOMesh
	vector <ofVboMesh> vboMeshes;  
	
	private:
	// the main Asset Import scene that does the magic.
	aiScene* scene;
	
	// Initial VBO creation, etc
	void loadGLResources();
	void deleteGLResources();  
};
