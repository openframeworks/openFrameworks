//TODO: should textures be ofTexture or ofImage (dynamically drawing to model texture?)
//TODO: de-pointerize the meshes if we decide we don't want to do advanced mesh classes
#pragma once

#include "ofMesh.h"
#include "ofUtils.h"
#include "ofImage.h"
#include "ofConstants.h"
#include "ofMeshRenderer.h"
#include "ofMeshNode.h"
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
	
	void setRenderMethod(meshRenderMethod m);
	

	ofMesh* getMesh(unsigned int nIndex);
	ofMesh* getMesh(string sName);
	void listMeshNames();


	vector<ofMeshNode> meshNodes;
	vector<ofMesh*> meshes;
	vector<ofMeshRenderer> renderers;
	vector<ofImage> textures;
	map<int, int> textureLinks;
	

	map<string, ofMesh*> named_meshes;
	map<string, ofPrimitive*> named_primitives;
	
protected:
	bool bUsingTextures, bUsingNormals, bUsingColors;
	void updateRenderers();
	void bindTextureForMesh(int id);
	void unbindTextureForMesh(int id);

private:
	meshRenderMethod renderMethod;
};
