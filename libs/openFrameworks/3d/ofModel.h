//TODO: should textures be ofTexture or ofImage (dynamically drawing to model texture?)
//TODO: de-pointerize the meshes if we decide we don't want to do advanced mesh classes
#pragma once

#include "ofMesh.h"
#include "ofUtils.h"
#include "ofImage.h"
#include "ofConstants.h"
#include "ofNode.h"
#include <map>

class ofModel{
public:
	ofModel();
	~ofModel();
		
	void drawWireframe();
	void drawFaces();
	void drawVertices();
	
	void enableTextures();
	void disableTextures();
	void enableNormals();
	void disableNormals();
	void enableColors();
	void disableColors();
	

	ofMesh* getMesh(unsigned int nIndex);
	ofMesh* getMesh(string sName);
	void listMeshNames();


	vector<ofNode> nodes;
	vector<ofMesh*> meshes;
	vector<ofImage> textures;
	map<int, int> textureLinks;
	

	map<string, ofMesh*> named_meshes;
	map<string, ofMesh*> named_primitives;
	
protected:
	bool bUsingTextures, bUsingNormals, bUsingColors;
	void bindTextureForMesh(int id);
	void unbindTextureForMesh(int id);

};
