#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "ofAssimpMeshLoader.h"
#import "aiConfig.h"

//--------------------------------------------------------------
ofAssimpMeshLoader::ofAssimpMeshLoader(){
	scene = NULL;
}

//--------------------------------------------------------------
ofAssimpMeshLoader::~ofAssimpMeshLoader(){
}

//-------------------------------------------
ofColor ofAssimpMeshLoader::aiColorToOfColor(const aiColor4D& c){
	return ofColor(255*c.r,255*c.g,255*c.b,255*c.a);
}

void ofAssimpMeshLoader::aiMeshToOfMesh(const aiMesh* aim, ofMeshElement& ofm){
	
	// default to triangle mode
	ofm.setMode(OF_TRIANGLES_ELEMENT);
	
	// copy vertices
	for (int i=0; i < aim->mNumVertices;i++){
		ofm.addVertex(ofVec3f(aim->mVertices[i].x,aim->mVertices[i].y,aim->mVertices[i].z));
	}
	
	for (int i=0; i < aim->mNumFaces;i++){	
		if(aim->mFaces[i].mNumIndices>3){
			ofLog(OF_LOG_WARNING,"non-triangular face found: model face # " + ofToString(i));
		}
		for (int j=0; j<aim->mFaces[i].mNumIndices; j++){
			ofm.addIndex(aim->mFaces[i].mIndices[j]);
		}
	}	

	if(aim->HasNormals()){
		for (int i=0; i < aim->mNumVertices;i++){
			ofm.addNormal(ofVec3f(aim->mNormals[i].x,aim->mNormals[i].y,aim->mNormals[i].z));
		}
	}
	
	// aiVector3D * 	mTextureCoords [AI_MAX_NUMBER_OF_TEXTURECOORDS]
	// just one for now
	if(aim->GetNumUVChannels()>0){
		for (int i=0; i < aim->mNumVertices;i++){
			ofm.addTexCoord(ofVec2f(aim->mTextureCoords[0][i].x,aim->mTextureCoords[0][i].y));
		}
	}
	
	//aiColor4D * 	mColors [AI_MAX_NUMBER_OF_COLOR_SETS]	
	// just one for now		
	if(aim->GetNumColorChannels()>0){
		for (int i=0; i < aim->mNumVertices;i++){
			ofm.addColor(aiColorToOfColor(aim->mColors[0][i]));
		}
	}
	
	// copy name
	//ofm.name = string(aim->mName.data);
	
	// copy face type
	//TODO: we are only grabbing the index of the first face, should probably do something smarter
	//ofm.faceType = aim->mFaces[0].mNumIndices;
	
	//	ofm.materialId = aim->mMaterialIndex;	
}

//--------------------------------------------------------------
void ofAssimpMeshLoader::loadMeshes(string modelName,vector<ofMeshElement>& m){
	
    // if we have a model loaded, unload the fucker. (pardon anton's french)
    if(scene != NULL){
        aiReleaseImport(scene);
        scene = NULL; 
    }
    
    // Load our new path.
    string filepath = ofToDataPath(modelName);
	
    ofLog(OF_LOG_VERBOSE, "loading meshes from %s", filepath.c_str());
    
	//TODO: import modes
    // only ever give us triangles.
    aiSetImportPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
    aiSetImportPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, true);
    
    // aiProcess_FlipUVs is for VAR code. Not needed otherwise. Not sure why.
    scene = (aiScene*) aiImportFile(filepath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph | aiProcess_Triangulate | aiProcess_FlipUVs | 0 );	
    if(scene){        
        ofLog(OF_LOG_VERBOSE, "initted scene with %i meshes", scene->mNumMeshes);
		
		m.resize(scene->mNumMeshes);
		
		for (unsigned int i = 0; i < scene->mNumMeshes; i++){
			ofLog(OF_LOG_VERBOSE, "loading mesh %u", i);
			// current mesh we are introspecting
			aiMesh* aMesh = scene->mMeshes[i];
				
			m[i] = ofMeshElement();
			aiMeshToOfMesh(aMesh,m[i]);
		}
    }
}	
