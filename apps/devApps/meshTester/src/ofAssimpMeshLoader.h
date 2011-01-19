#pragma once

// assimp include files. These three are usually needed.
#import "assimp.h"
#import "aiPostProcess.h"
#import "aiScene.h"

#include "ofMain.h"

//adapted by Keith Pasko from Anton Marini's ofxAssimpModelLoader addon

#include "ofMeshElement.h"

class ofAssimpMeshLoader{
	
public:
	~ofAssimpMeshLoader();
	ofAssimpMeshLoader();
	
	void loadMeshes(string modelName, vector<ofMeshElement>& m);
    
	void draw();
	
	// Our array of textures we load from the models path.
	//	vector <ofImage> textures;
	
	private:
	// the main Asset Import scene that does the magic.
	aiScene* scene;
	void aiMeshToOfMesh(const aiMesh* aim, ofMeshElement& ofm);
	ofColor aiColorToOfColor(const aiColor4D& c);
};
