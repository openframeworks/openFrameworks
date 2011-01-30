#pragma once

#include "ofMesh.h"
#include "ofUtils.h"
#include "ofTexture.h"

class ofModel{
public:
	ofModel();
	~ofModel();
		
	void drawWireframe();
	void drawFaces();
	void drawVertices();
	
	void enableTexCoords();
	void disableTexCoords();
	void enableNormals();
	void disableNormals();
	void enableColors();
	void disableColors();
	
	void setRenderMethod(meshRenderMethod m);
	
	vector<ofMesh> meshes;
	vector<ofTexture> textures;

private:
	meshRenderMethod renderMethod;
};
