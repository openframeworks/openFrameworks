#pragma once

#import "assimp.h"
#import "aiPostProcess.h"
#import "aiScene.h"
#include "ofColor.h"
#include "ofMesh.h"

static inline ofColor aiColorToOfColor(const aiColor4D& c){
	return ofColor(255*c.r,255*c.g,255*c.b,255*c.a);
}

void loadMeshes(string modelName, vector<ofMesh>& m);
void aiMeshToOfVertexData(const aiMesh* aim, ofVertexData& ofm);
