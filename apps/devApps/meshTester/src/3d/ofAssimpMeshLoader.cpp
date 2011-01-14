#include "ofAssimpMeshLoader.h"
#import "aiConfig.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//-------------------------------------------
static ofColor aiColorToOfColor(const aiColor4D& c){
	return ofColor(255*c.r,255*c.g,255*c.b,255*c.a);
}

static void aiMeshToOfMesh(const aiMesh* aim, ofMesh& ofm){
	// copy vertices
	for (int i=0; i < aim->mNumVertices;i++){
		ofm.vertices.push_back(ofVec3f(aim->mVertices[i].x,aim->mVertices[i].y,aim->mVertices[i].z));
	}
	
	//copy faces
	for (int i=0; i < aim->mNumFaces;i++){	
		ofm.faces.push_back(ofFace());
		for (int j=0; j<aim->mFaces[i].mNumIndices; j++){
			ofm.faces.back().indices.push_back(aim->mFaces[i].mIndices[j]);
		}
		if(ofm.faces.back().indices.size()>3){
			ofLog(OF_LOG_WARNING,"non-triangular face found: face # " + ofToString(ofm.faces.size()-1));
		}
	}	
	
	if(aim->HasNormals()){
		for (int i=0; i < aim->mNumVertices;i++){
			ofm.normals.push_back(ofVec3f(aim->mNormals[i].x,aim->mNormals[i].y,aim->mNormals[i].z));
		}
	}else{
		ofm.bUsingNormals = false;
	}
	
	// aiVector3D * 	mTextureCoords [AI_MAX_NUMBER_OF_TEXTURECOORDS]
	// just one for now
	if(aim->GetNumUVChannels()>0){
		for (int i=0; i < aim->mNumVertices;i++){
			ofm.texCoords.push_back(ofVec2f(aim->mTextureCoords[0][i].x,aim->mTextureCoords[0][i].y));
		}
	}else{
		ofm.bUsingTexCoords = false;
	}
	
	//aiColor4D * 	mColors [AI_MAX_NUMBER_OF_COLOR_SETS]
	
	// just one for now		
	if(aim->GetNumColorChannels()>0){
		for (int i=0; i < aim->mNumVertices;i++){
			ofm.colors.push_back(aiColorToOfColor(aim->mColors[0][i]));
		}
	}else{
		ofm.bUsingColors = false;
	}	
	// copy name
	ofm.name = string(aim->mName.data);
	// copy face type
	ofm.faceType = aim->mPrimitiveTypes;
	
	//	ofm->materialId = aim->mMaterialIndex;	
}


ofxAssimpModelLoader::ofxAssimpModelLoader(){
	scene = NULL;
}

//------------------------------------------
void ofxAssimpModelLoader::loadMeshes(string modelName,vector<ofVboMesh>& m){
	
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
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
			ofLog(OF_LOG_VERBOSE, "loading mesh %u", i);
			// current mesh we are introspecting
			aiMesh* aMesh = scene->mMeshes[i];
			m.push_back(ofVboMesh());
			ofVboMesh& curMesh = m.back();
			curMesh.createVbo();
			aiMeshToOfMesh(aMesh,*curMesh.createMesh());
			curMesh.setupVertices(OF_VBO_STATIC);
			curMesh.setupColors(OF_VBO_STATIC);
		}
    }
}

//-------------------------------------------
ofxAssimpModelLoader::~ofxAssimpModelLoader(){
}	
