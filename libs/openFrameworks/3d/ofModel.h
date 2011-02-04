//TODO: should textures be ofTexture or ofImage (dynamically drawing to model texture?)

#pragma once

#include "ofMain.h"
#include "ofMesh.h"
#include "ofUtils.h"

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
	
	void bindTextureForMesh(int id);
	void unbindTextureForMesh(int id);
	
	void setRenderMethod(meshRenderMethod m);
	
	vector<ofMesh> meshes;
	vector<ofImage> textures;
	map<int, int> textureLinks;
	
protected:
	bool bUsingTextures;

private:
	meshRenderMethod renderMethod;
};
