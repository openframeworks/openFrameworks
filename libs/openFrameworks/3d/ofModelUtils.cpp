//TODO: include multiple textures per mesh/different texture types?
//TODO: materials

#include "ofModelUtils.h"
#include "ofConstants.h"
#if !defined(TARGET_OF_IPHONE)
#include "aiConfig.h"
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"
#include "ofColor.h"

//--------------------------------------------------------------
static inline ofColor aiColorToOfColor(const aiColor4D& c){
	return ofColor(255*c.r,255*c.g,255*c.b,255*c.a);
}

//--------------------------------------------------------------
void aiMeshToOfPrimitive(const aiMesh* aim, ofPrimitive& ofm){
	// default to triangle mode
	ofm.setMode(OF_TRIANGLES_MODE);
	
	// copy vertices
	for (int i=0; i < (int)aim->mNumVertices;i++){
		ofm.addVertex(ofVec3f(aim->mVertices[i].x,aim->mVertices[i].y,aim->mVertices[i].z));
	}

	if(aim->HasNormals()){
		for (int i=0; i < (int)aim->mNumVertices;i++){
			ofm.addNormal(ofVec3f(aim->mNormals[i].x,aim->mNormals[i].y,aim->mNormals[i].z));
		}
	}
	
	// aiVector3D * 	mTextureCoords [AI_MAX_NUMBER_OF_TEXTURECOORDS]
	// just one for now
	if(aim->GetNumUVChannels()>0){
		for (int i=0; i < (int)aim->mNumVertices;i++){
			ofm.addTexCoord(ofVec2f(aim->mTextureCoords[0][i].x ,aim->mTextureCoords[0][i].y));
		}
	}
	
	//aiColor4D * 	mColors [AI_MAX_NUMBER_OF_COLOR_SETS]	
	// just one for now		
	if(aim->GetNumColorChannels()>0){
		for (int i=0; i < (int)aim->mNumVertices;i++){
			ofm.addColor(aiColorToOfColor(aim->mColors[0][i]));
		}
	}
	
	for (int i=0; i <(int) aim->mNumFaces;i++){
		if(aim->mFaces[i].mNumIndices>3){
			ofLog(OF_LOG_WARNING,"non-triangular face found: model face # " + ofToString(i));
		}
		for (int j=0; j<(int)aim->mFaces[i].mNumIndices; j++){
			ofm.addIndex(aim->mFaces[i].mIndices[j]);
		}
	}	
	
	ofm.setName(string(aim->mName.data));
	//	ofm.materialId = aim->mMaterialIndex;	
}

//--------------------------------------------------------------
void aiMatrix4x4ToOfMatrix4x4(const aiMatrix4x4& aim, ofMeshNode& ofm){
	float m[16] = { aim.a1,aim.a2,aim.a3,aim.a4,
					aim.b1,aim.b2,aim.b3,aim.b4,
					aim.c1,aim.c2,aim.c3,aim.c4,
					aim.d1,aim.d2,aim.d3,aim.d4 };

	ofm.setTransformMatrix(	m);
}

//--------------------------------------------------------------
void aiNodeToOfNode(const aiNode* ain, ofMeshNode& ofn, const ofModel& model){
	aiMatrix4x4ToOfMatrix4x4(ain->mTransformation, ofn);
	for (int i =0; i < (int)ain->mNumMeshes;i++){
		ofn.addMesh(model.meshes.at(ain->mMeshes[i]));
	}
	ofn.setName(string(ain->mName.data));
}

//--------------------------------------------------------------
int createNodes(const aiNode* curNode, ofModel& model){
	//lets only make nodes that have meshes for now
	if(curNode->mNumMeshes){
		model.meshNodes.push_back(ofMeshNode());
		aiNodeToOfNode(curNode, model.meshNodes.back(), model);
	}
	
	if (curNode->mNumChildren>0){
		for (int i =0; i<(int)curNode->mNumChildren;i++){
			createNodes(curNode->mChildren[i], model);
		}
	}else return 0;

#warning "this is returning nothing ig nNumChildren i>0  fix it"
}

//--------------------------------------------------------------
void createBones(const aiScene* scene, ofModel& model){
	for (int i =0; i < (int)scene->mNumMeshes;i++){
		aiMesh& curMesh = *scene->mMeshes[i];
		if(curMesh.HasBones()){
			for (int j=0; j < (int)curMesh.mNumBones;j++){
				aiNode* boneNode = scene->mRootNode->FindNode(curMesh.mBones[j]->mName);
			}
		}
	}
}

//--------------------------------------------------------------
bool ofLoadModel(string modelName, ofModel & model){
    string filepath = ofToDataPath(modelName);
	
    ofLog(OF_LOG_VERBOSE, "loading meshes from %s", filepath.c_str());
    
	//TODO: import modes
    // only ever give us triangles.
    aiSetImportPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
    aiSetImportPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, true);
    
    // aiProcess_FlipUVs is for VAR code. Not needed otherwise. Not sure why.
    aiScene * scene = (aiScene*) aiImportFile(filepath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph | aiProcess_Triangulate | aiProcess_FlipUVs | 0 );	
	if(scene == NULL) {
		return false;
	}
	else {
        ofLog(OF_LOG_VERBOSE, "initted scene with %i meshes", scene->mNumMeshes);
		
		model.meshes.resize(scene->mNumMeshes);
		
		map<string,int> texPathMap;
		for (unsigned int i = 0; i < scene->mNumMeshes; i++){
			model.textureLinks[i]=-1;
			
			
			ofLog(OF_LOG_VERBOSE, "loading mesh %u", i);
			// current mesh we are introspecting
			aiMesh* aMesh = scene->mMeshes[i];

			model.meshes[i] = new ofMesh();
			ofMesh& curMesh = *model.meshes[i];
			curMesh.primitives.push_back(ofPrimitive());
			aiMeshToOfPrimitive(aMesh,curMesh.primitives.back());			
		
			//load texture
			aiMaterial* mtl = scene->mMaterials[aMesh->mMaterialIndex];
			aiString texPath;
			//defaults to only get the 0-index texture for now
			if(mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS){
				if(texPathMap.find(texPath.data)==texPathMap.end()){
					model.textures.push_back(ofImage());
					ofLog(OF_LOG_VERBOSE, "loading image from %s", texPath.data);
					string modelFolder = ofFileUtils::getEnclosingDirectoryFromPath(filepath);
					cout << modelFolder << " --- " << texPath.data << endl; 

					if(ofFileUtils::isAbsolute(texPath.data) && ofFileUtils::doesFileExist(texPath.data)) {
						model.textures.back().loadImage(texPath.data);
					}
					else {
						model.textures.back().loadImage(modelFolder + texPath.data);
					}

					model.textures.back().update();
					texPathMap[texPath.data] = model.textures.size()-1;
				}
				
				model.textureLinks[i] = texPathMap[texPath.data];
			}
			
			// add named meshes: 
			// TODO: how to handle duplicate names (roxlu)
			if(aMesh->mName.length > 0) {
				model.named_meshes[aMesh->mName.data] = &curMesh;
				// TODO: workout primitives
				//model.named_vertices.insert(ofNamedVertexData(aMesh->mName.data,curMesh.primitives));
			}
		}
		
		if(scene->mRootNode!=NULL){
			createNodes(scene->mRootNode, model);
			//createBones(scene,model);
		}
		
		return true;
		
	}
	return true;
}

#endif
