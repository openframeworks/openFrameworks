#include "ofxAssimpSrcScene.h"
#include "ofxAssimpUtils.h"
#include "ofLight.h"
#include "ofImage.h"
#include "ofPixels.h"
#include "ofGraphics.h"
#include "ofConstants.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

//--------------------------------------------------------------
using std::shared_ptr;
using std::vector;

using namespace ofxAssimp;

//------------------------------------------
SrcScene::SrcScene(){
	clear();
}

//------------------------------------------
SrcScene::~SrcScene(){
	clear();
}

//------------------------------------------
bool SrcScene::load(std::string aPathToFile, int assimpOptimizeFlags){
	ImportSettings tsettings;
	tsettings.filePath = aPathToFile;
	tsettings.assimpOptimizeFlags = assimpOptimizeFlags;
	return load(tsettings);
}

//------------------------------------------
bool SrcScene::load(ofBuffer & buffer, int assimpOptimizeFlags, const char * extension){
	
	ofLogVerbose("ofxAssimp::SrcScene") << "load(): loading from memory buffer \"." << extension << "\"";
	
	if(scene.get() != nullptr){
		clear();
		// we reset the shared_ptr explicitly here, to force the old
		// aiScene to be deleted **before** a new aiScene is created.
		scene.reset();
	}
	
	ImportSettings tsettings;
	tsettings.assimpOptimizeFlags = assimpOptimizeFlags;
	
	// sets various properties & flags to a default preference
	unsigned int flags = initImportProperties(assimpOptimizeFlags,tsettings);
	
	// 	//enable assimp logging based on ofGetLogLevel
	//	if( ofGetLogLevel() < OF_LOG_NOTICE ){
	//		auto logLevel = Assimp::DefaultLogger::LogSeverity::VERBOSE;
	//		Assimp::DefaultLogger::create(ASSIMP_DEFAULT_LOG_NAME, logLevel, aiDefaultLogStream_DEBUGGER | aiDefaultLogStream_STDOUT );
	//	}
	
	// loads scene from memory buffer - note this will not work for multipart files (obj, md3, etc)
	const aiScene * scenePtr = importer.ReadFileFromMemory(buffer.getData(), buffer.size(), flags, extension);
	
	// this is funky but the scenePtr is managed by assimp and so we can't put it in our shared_ptr without disabling the deleter with: [](const aiScene*){}
	scene = shared_ptr<const aiScene>(scenePtr,[](const aiScene*){});
	bool bOk = processScene(tsettings);
	return bOk;
}

//------------------------------------------
bool SrcScene::load( const ImportSettings& asettings ) {
	mFile.open(asettings.filePath, ofFile::ReadOnly, true); // Since it may be a binary file we should read it in binary -Ed
	if(!mFile.exists()) {
		ofLogVerbose("ofxAssimp::SrcScene") << "load(): model does not exist: \"" << asettings.filePath << "\"";
		return false;
	}
	
	ofLogVerbose("ofxAssimp::SrcScene") << "load(): loading \"" << mFile.getFileName() << "\" from \"" << mFile.getEnclosingDirectory() << "\"";
	
	if(scene.get() != nullptr){
		clear();
		// we reset the shared_ptr explicitly here, to force the old
		// aiScene to be deleted **before** a new aiScene is created.
		scene.reset();
	}
	
	// sets various properties & flags to a default preference
	unsigned int flags = initImportProperties(asettings.assimpOptimizeFlags, asettings);
	// loads scene from file
	std::string path = mFile.getAbsolutePath();
	const aiScene * scenePtr = importer.ReadFile(path.c_str(), flags);
	
	if(!scenePtr || scenePtr->mFlags & AI_SCENE_FLAGS_INCOMPLETE ) {
		ofLogError("ofxAssimp::SrcScene") << "load: " << importer.GetErrorString();
		return false;
	}
	
	//this is funky but the scenePtr is managed by assimp and so we can't put it in our shared_ptr without disabling the deleter with: [](const aiScene*){}
	scene = shared_ptr<const aiScene>(scenePtr,[](const aiScene*){});
	
	bool bOk = processScene(asettings);
	return bOk;
}

unsigned int SrcScene::initImportProperties(int assimpOptimizeFlags, const ImportSettings& asettings ) {
	// only ever give us triangles; newer c++ api
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
	importer.SetPropertyBool(AI_CONFIG_PP_PTV_NORMALIZE, true);
	// when preserve fbx pivots is true, assimp splits pivots out into multiple, one for transform, rotation, etc.
	// this causes incorrect parsing and transforms.
	// we just want a single bone without extra added nodes.
	// this is apparent in FBX Mixamo models, among others.
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	
	unsigned int flags = assimpOptimizeFlags;
	
	if( flags <= OFX_ASSIMP_OPTIMIZE_HIGH ){
		
		if( flags == OFX_ASSIMP_OPTIMIZE_NONE ){
			flags = 0;
		}else if( flags == OFX_ASSIMP_OPTIMIZE_DEFAULT || flags == OFX_ASSIMP_OPTIMIZE_HIGH ){
			flags =     aiProcess_CalcTangentSpace              |  \
			aiProcess_GenSmoothNormals              |  \
			aiProcess_JoinIdenticalVertices         |  \
			aiProcess_ImproveCacheLocality          |  \
			aiProcess_LimitBoneWeights              |  \
			aiProcess_RemoveRedundantMaterials      |  \
			aiProcess_SplitLargeMeshes              |  \
			aiProcess_Triangulate                   |  \
			aiProcess_GenUVCoords                   |  \
			aiProcess_SortByPType                   |  \
			aiProcess_FindDegenerates               |  \
			aiProcess_FindInstances                 |  \
			aiProcess_OptimizeMeshes;
		}
		
		if( flags == OFX_ASSIMP_OPTIMIZE_HIGH ){
			flags |= aiProcess_OptimizeGraph |  \
			aiProcess_FindInstances |  \
			aiProcess_ValidateDataStructure;
		}
		if( asettings.fixInfacingNormals ) {
			flags |= aiProcess_FixInfacingNormals;
		}
		
		if( asettings.aiFlags > 0 ) {
			flags |= asettings.aiFlags;
		}
		
		// we need the armature data to help with bone parsing
		flags |= aiProcess_PopulateArmatureData;
		// this fixes things for OF both tex uvs and model not flipped in z
		if(asettings.convertToLeftHanded) {
			flags |= aiProcess_ConvertToLeftHanded;
		}
	}
	
	return flags;
}

//-------------------------------------------
void SrcScene::optimizeScene(){
	if( scene ) {
		importer.ApplyPostProcessing(aiProcess_ImproveCacheLocality | aiProcess_OptimizeGraph |
									 aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices |
									 aiProcess_RemoveRedundantMaterials);
	}
}

//-------------------------------------------
void SrcScene::clear(){
	if( scene ){
		scene.reset();
		importer.FreeScene();
	}
	
	ofLogVerbose("ofxAssimp::SrcScene") << "clear(): deleting GL resources";
	
	// clear out everything.
	mSrcMeshes.clear();
	mAnimations.clear();
	mLights.clear();
	mSrcNodes.clear();
	mAssimpTextures.clear();
}

//-------------------------------------------
bool SrcScene::processScene(const ImportSettings& asettings) {
	mSettings = asettings;
	
	if(scene){
		processNodes();
		processAnimations();
		
		if(mAnimations.size() > 0)
			ofLogVerbose("ofxAssimp::SrcScene") << "load(): scene has " << mAnimations.size() << "animations";
		else {
			ofLogVerbose("ofxAssimp::SrcScene") << "load(): no animations";
		}
		
		return true;
	}else{
		ofLogError("ofxAssimp::SrcScene") << "load(): " + (std::string)importer.GetErrorString();
		clear();
		return false;
	}
	
	return false;
}

//-------------------------------------------
void SrcScene::printAllNodeNames( aiNode* anode, int alevel ) {
	if( !anode ) return;
	
	std::stringstream ss;
	for( int i = 0; i < alevel; i++ ) {
		ss << "  ";
	}
	ss << anode->mName.data << " num mehses: " << anode->mNumMeshes;
	for( int i = 0; i < anode->mNumMeshes; i++ ) {
		aiMesh* mesh = scene->mMeshes[anode->mMeshes[i]];
		if(mesh) {
			for( int k = 0; k < alevel; k++ ) {
				ss << "  ";
			}
			ss << std::endl << "  -" << mesh->mName.data << std::endl;
		}
	}
	ofLogNotice(ss.str());
	
	alevel++;
	for (unsigned int i = 0; i < anode->mNumChildren; ++i){
		printAllNodeNames( anode->mChildren[i], alevel );
	}
}


//-------------------------------------------
void SrcScene::processNodes() {
	// lets load in the node hierarchy here //
	mSrcMeshes.clear();
	mSrcMeshes.assign( scene->mNumMeshes, shared_ptr<ofxAssimp::SrcMesh>() );
	processMeshes(scene->mRootNode, shared_ptr<SrcNode>());
	
	std::unordered_map<std::string, aiBone*> boneMap;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
		ofLogVerbose("ofxAssimp::SrcScene") << "getAiBoneForAiNode(): loading mesh " << i;
		// current mesh we are introspecting
		aiMesh* mesh = scene->mMeshes[i];
		if( mesh == nullptr ) {
			continue;
		}
		for(unsigned int a = 0; a < mesh->mNumBones; ++a) {
			aiBone* bone = mesh->mBones[a];
			if( bone != nullptr ) {
				boneMap[std::string(bone->mName.C_Str())] = bone;
			}
		}
	}
	
	
	for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; i++ ){
		processNodesRecursive(scene->mRootNode->mChildren[i], shared_ptr<SrcNode>(), boneMap );
	}
}

//-------------------------------------------
void SrcScene::processNodesRecursive(aiNode* anode, std::shared_ptr<SrcNode> aParentNode, std::unordered_map<std::string, aiBone*>& aBoneMap ) {
	if( !anode ) return;
	
	std::shared_ptr<SrcNode> sNode;
	if( isBone(anode, aBoneMap) || isArmature(anode, aBoneMap) ) {
//	if( aiBone* tAiBone = getAiBoneForAiNode(anode) ) {
		aiBone* tAiBone = getAiBoneForAiNode(anode, aBoneMap);
		auto sBone = std::make_shared<ofxAssimp::SrcBone>();
		sBone->setAiBone(tAiBone, anode);
		sBone->bRoot = isRootBone(anode, aBoneMap) || !isBone(anode, aBoneMap);
//		sBone->bRoot = isArmature(anode);
		if(tAiBone && tAiBone->mArmature) {
//			std::cout << "SrcScene :: processNodes: " << tAiBone->mArmature->mName.data << std::endl;
		}
		sNode = sBone;
	} else {
		
		std::string sname = anode->mName.data;
		bool bExclude = false;
		for( auto& es : mSettings.excludeNodesContainingStrings ) {
			if( ofIsStringInString(sname, es) ) {
				bExclude = true;
				break;
			}
		}
		if(!bExclude) {
			sNode = std::make_shared<ofxAssimp::SrcNode>();
			sNode->setAiNode(anode);
		}
	}
	
	if(sNode) {
		if( aParentNode ) {
			aParentNode->addChild(sNode);
		} else {
			// lets add to the srcNodes vector
			mSrcNodes.push_back(sNode);
		}
		processMeshes(anode, sNode);
	}
	
	for (unsigned int i = 0; i < anode->mNumChildren; i++ ){
		processNodesRecursive(anode->mChildren[i], sNode, aBoneMap );
	}
}

//-------------------------------------------
void SrcScene::processMeshes(aiNode* anode, std::shared_ptr<SrcNode> aSrcNode) {
	// the meshes are per node it seems
	for(unsigned int i = 0; i < anode->mNumMeshes; i++) {
		unsigned int meshIndex = anode->mMeshes[i];
		if( meshIndex >= mSrcMeshes.size() ) {
			ofLogError("ofxAssimp::Scene::processNodesRecursive") << " mesh index out of range: " << meshIndex << " / " << mSrcMeshes.size() << " num in scene: " << scene->mNumMeshes;
			continue;
		}
		if( mSrcMeshes[meshIndex] ) {
			ofLogWarning("ofxAssimp::SrcScene::processNodesRecursive") << " hmm we already have a SrcMesh at index: " << meshIndex << " " << anode->mName.data;
		} else {
			ofLogVerbose("ofxAssimp::SrcScene::processNodesRecursive") << " going to process mesh: " << scene->mMeshes[meshIndex]->mName.data << " from node: " << anode->mName.data << " num scene meshes: " << scene->mNumMeshes;
			// ok, open slot for a src mesh //
			auto srcMesh = std::make_shared<ofxAssimp::SrcMesh>();
			srcMesh->setAiMesh(scene->mMeshes[meshIndex], anode );
			loadGLResources(srcMesh, srcMesh->getAiMesh());
			mSrcMeshes.push_back(srcMesh);
			if(aSrcNode) {
				aSrcNode->addChild(srcMesh);
			} else {
				mSrcNodes.push_back(srcMesh);
			}
		}
	}
}

//-------------------------------------------
bool SrcScene::isBone( aiNode* aAiNode, const std::unordered_map<std::string, aiBone*>& aBoneMap ) {
	if( aBoneMap.count(std::string(aAiNode->mName.C_Str())) > 0 ) {
		return true;
	}
	return false;
	
//	if(isRootBone( aAiNode )) {
//		return true;
//	}
//	return (getAiBoneForAiNode(aAiNode) != nullptr);
}

//-------------------------------------------
bool SrcScene::isArmature( aiNode* aAiNode, const std::unordered_map<std::string, aiBone*>& aBoneMap ) {
	for( const auto& iter : aBoneMap ) {
		if( iter.second != nullptr ) {
			if( iter.second->mArmature == aAiNode ) {
				return true;
			}
		}
	}
	return false;
	
//	for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
//		// current mesh we are introspecting
//		aiMesh* mesh = scene->mMeshes[i];
//		if( mesh == nullptr ) {
//			continue;
//		}
//		for(unsigned int a = 0; a < mesh->mNumBones; ++a) {
//			aiBone* bone = mesh->mBones[a];
//			if( bone != nullptr ) {
//				if( aAiNode == bone->mArmature ) {
//					return true;
//				}
//			}
//		}
//	}
//	return false;
}

//-------------------------------------------
bool SrcScene::isRootBone( aiNode* aAiNode, std::unordered_map<std::string, aiBone*>& aBoneMap ) {
	// check if there are parent bones
	bool bHasBoneParent = false;
	auto temp = aAiNode->mParent;
	while( temp ) {
		if( getAiBoneForAiNode(temp, aBoneMap)) {
			bHasBoneParent=true;
			break;
		}
		if( temp != scene->mRootNode && isArmature(temp, aBoneMap)) {
			bHasBoneParent=true;
			break;
		}
		temp = temp->mParent;
	}
	
	return !bHasBoneParent;
	
//	return false;
}

//-------------------------------------------
aiBone* SrcScene::getAiBoneForAiNode( aiNode* aAiNode, std::unordered_map<std::string, aiBone*>& aBoneMap ) {
	
	std::string bstr = std::string(aAiNode->mName.C_Str());
	if( aBoneMap.count(bstr) > 0) {
		return aBoneMap[bstr];
	}
	return nullptr;

}

//-------------------------------------------
std::shared_ptr<ofxAssimp::SrcNode> SrcScene::getSrcNodeForAiNodeName( const std::string& aAiNodeName ) {
	std::shared_ptr<ofxAssimp::SrcNode> rnode;
	for( auto tnode : mSrcNodes ) {
		if( tnode->getName() == aAiNodeName ) {
			rnode = tnode;
			break;
		}
		if( auto knode = tnode->getNode(aAiNodeName)) {
			rnode = knode;
			break;
		}
	}
	return rnode;
}

//-------------------------------------------
void SrcScene::processLights(){
	mLights.clear();
	mLights.resize(scene->mNumLights);
	for(unsigned int i = 0; i < scene->mNumLights; i++){
		mLights[i].enable();
		if(scene->mLights[i]->mType==aiLightSource_DIRECTIONAL){
			mLights[i].setDirectional();
			mLights[i].setOrientation(ofxAssimp::Utils::aiVecToOfVec(scene->mLights[i]->mDirection));
		}
		if(scene->mLights[i]->mType!=aiLightSource_POINT){
			mLights[i].setSpotlight();
			mLights[i].setPosition(ofxAssimp::Utils::aiVecToOfVec(scene->mLights[i]->mPosition));
		}
		mLights[i].setAmbientColor(ofxAssimp::Utils::aiColorToOfColor(scene->mLights[i]->mColorAmbient));
		mLights[i].setDiffuseColor(ofxAssimp::Utils::aiColorToOfColor(scene->mLights[i]->mColorDiffuse));
		mLights[i].setSpecularColor(ofxAssimp::Utils::aiColorToOfColor(scene->mLights[i]->mColorSpecular));
	}
}

//-------------------------------------------
void SrcScene::processAnimations() {
	if( !mSettings.importAnimations || !scene ) {
		mAnimations.clear();
		return;
	}
	
	for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
		aiAnimation* animation = scene->mAnimations[i];
		if(animation == nullptr) {
			continue;
		}
//		mAnimations.push_back(Animation(scene, animation));
		mAnimations.push_back(Animation(i, animation));
		// now lets parse the animation by getting all of the channels
		for( unsigned int j = 0; j < animation->mNumChannels; j++ ) {
			// grab the node animation that holds all of the key frames
			aiNodeAnim* nodeAnim = animation->mChannels[j];
			// the nodeAnim contains the name of the node it affects,
			// according to the docs, it must exist and the name must be unique
			std::string nodeName = nodeAnim->mNodeName.data;
			if(std::shared_ptr<ofxAssimp::SrcNode> snode = getSrcNodeForAiNodeName( nodeName )) {
				// SrcNode contains a pointer to the aiNode
				processKeyframes(snode, nodeAnim, i);
			} else {
				ofLogWarning("SrcScene::processAnimations") << "unable to find node " << nodeName << " for animation: " << animation->mName.data;
			}
		}
	}
}

//-------------------------------------------
void SrcScene::processKeyframes(std::shared_ptr<ofxAssimp::SrcNode> aSrcNode, aiNodeAnim* aNodeAnim, unsigned int aAnimIndex) {
	if(!aSrcNode || !aNodeAnim) {
		return;
	}
	auto& anim = mAnimations[aAnimIndex];
	ofxAssimp::SrcAnimKeyCollection& keyCollection = aSrcNode->getKeyCollection(aAnimIndex);
	keyCollection.clear();
	keyCollection.setup( aNodeAnim, anim.getDurationInTicks() );
	
//	ofLogNotice("ofxAssimp::SrcScene processKeyframes: ") << aSrcNode->getName() << " node anim: " << aNodeAnim->mNodeName.data << " node anim num pos keyframes: " << aNodeAnim->mNumPositionKeys << " scale: " << aNodeAnim->mNumScalingKeys << " rot: " << aNodeAnim->mNumRotationKeys;
	
	double startTime = 0.0; // seconds
	double endTime = anim.getDurationInTicks();// seconds;
	
	keyCollection.positionKeys = keyCollection.getAnimVectorKeysForTime(startTime, endTime, aNodeAnim->mNumPositionKeys, aNodeAnim->mPositionKeys );
	keyCollection.scaleKeys = keyCollection.getAnimVectorKeysForTime(startTime, endTime, aNodeAnim->mNumScalingKeys, aNodeAnim->mScalingKeys );
	keyCollection.rotationKeys = keyCollection.getAnimRotationKeysForTime(startTime, endTime, aNodeAnim->mNumRotationKeys, aNodeAnim->mRotationKeys );
	
}

#ifndef TARGET_WIN32
//this is a hack to allow for weak definations of functions that might not exist in older assimp versions
const char *aiTextureTypeToString(enum aiTextureType in)__attribute__((weak));
#endif

//-------------------------------------------
static GLint getGLFormatFromAiFormat(const char * aiFormat){
	
	std::string formatStr(aiFormat);
	
	if( formatStr.size() >= 8 ){
		if( formatStr.substr(0, 4) == "rgba" ){
			if(formatStr.substr(4,4) == "8888"){
				return GL_RGBA;
			}else if(formatStr.substr(4,4) == "8880"){
				return GL_RGB;
			}
		}else{
			ofLogError("ofxAssimp::SrcScene : getGLFormatFromAiFormat") << " can't parse format " << formatStr;
		}
	}
	
	ofLogWarning("ofxAssimp::SrcScene : getGLFormatFromAiFormat") << " can't parse format " << formatStr << " returning GL_RGB";
	return GL_RGB;
}

//-------------------------------------------
void SrcScene::loadGLResources(std::shared_ptr<ofxAssimp::SrcMesh> aSrcMesh, aiMesh* amesh){
	
	ofLogVerbose("ofxAssimp::SrcScene") << "loadGLResources(): starting";
	
	// we do this as we have textures and vbos and in mesh and we don't want to copy them
	// create OpenGL buffers and populate them based on each meshes pertinant info.
	if( amesh ) {
		aSrcMesh->bConvertedToLeftHand = mSettings.convertToLeftHanded;
		if( mSettings.importMaterials ) {
			if( !aSrcMesh->material ) {
				aSrcMesh->material = std::make_shared<ofMaterial>();
			}
		}
		
		// Handle material info
		aiMaterial* mtl = scene->mMaterials[amesh->mMaterialIndex];
		aiColor4D tcolor;
		if( mSettings.importMaterials ) {
			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &tcolor)){
				auto col = ofxAssimp::Utils::aiColorToOfColor(tcolor);
				aSrcMesh->material->setDiffuseColor(col);
			}
			
			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &tcolor)){
				auto col = ofxAssimp::Utils::aiColorToOfColor(tcolor);
				aSrcMesh->material->setSpecularColor(col);
			}
			
			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &tcolor)){
				auto col = ofxAssimp::Utils::aiColorToOfColor(tcolor);
				aSrcMesh->material->setAmbientColor(col);
			}
			
			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &tcolor)){
				auto col = ofxAssimp::Utils::aiColorToOfColor(tcolor);
				ofLogVerbose("ofxAssimp::SrcScene") << "setting emissive color: " << col;
				aSrcMesh->material->setEmissiveColor(col);
			}
			
			float shininess;
			if(AI_SUCCESS == aiGetMaterialFloat(mtl, AI_MATKEY_SHININESS, &shininess)){
				aSrcMesh->material->setShininess(shininess);
			}
			
			int blendMode;
			if(AI_SUCCESS == aiGetMaterialInteger(mtl, AI_MATKEY_BLEND_FUNC, &blendMode)){
				if(blendMode==aiBlendMode_Default){
					aSrcMesh->blendMode=OF_BLENDMODE_ALPHA;
				}else{
					aSrcMesh->blendMode=OF_BLENDMODE_ADD;
				}
			}
			
			float metallic = 0.f;
			if( AI_SUCCESS == aiGetMaterialFloat(mtl, AI_MATKEY_METALLIC_FACTOR, &metallic) ) {
				ofLogVerbose("ofxAssimp::SrcScene") << " setting metallic value: " << metallic;
				aSrcMesh->material->setMetallic(metallic);
			}
			
			float roughness = 0.f;
			if( AI_SUCCESS == aiGetMaterialFloat(mtl, AI_MATKEY_ROUGHNESS_FACTOR, &roughness) ) {
				ofLogVerbose("ofxAssimp::SrcScene") << " setting roughness value: " << roughness;
				aSrcMesh->material->setRoughness(roughness);
			}
			
			float clearcoatFactor = 0.f;
			if( AI_SUCCESS == aiGetMaterialFloat(mtl, AI_MATKEY_CLEARCOAT_FACTOR, &clearcoatFactor) ) {
				ofLogVerbose("ofxAssimp::SrcScene") << " setting clearcoat value: " << clearcoatFactor;
				aSrcMesh->material->setClearCoatStrength(clearcoatFactor);
				if( clearcoatFactor > 0.0f ) {
					aSrcMesh->material->setClearCoatEnabled(true);
				} else {
					aSrcMesh->material->setClearCoatEnabled(false);
				}
			}
			
			float clearcoatRoughFactor = 0.f;
			if( AI_SUCCESS == aiGetMaterialFloat(mtl, AI_MATKEY_CLEARCOAT_ROUGHNESS_FACTOR, &clearcoatRoughFactor) ) {
				ofLogVerbose("ofxAssimp::SrcScene") << " setting clearcoat roughness value: " << clearcoatRoughFactor;
				aSrcMesh->material->setClearCoatRoughness(clearcoatRoughFactor);
			}
		}
		
		// Culling
		unsigned int max = 1;
		int two_sided=0;
		if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided){
			aSrcMesh->twoSided = true;
			ofLogVerbose("ofxAssimp::SrcScene") <<" loadGLResources: mesh is two sided";
		}else{
			aSrcMesh->twoSided = false;
			ofLogVerbose("ofxAssimp::SrcScene") <<" loadGLResources: mesh is one sided";
		}
		
		// Load Textures
		int texIndex = 0;
		aiString texPath;
		
		bool bWasUsingArb = ofGetUsingArbTex();
		if( mSettings.importAsTex2d) {
			ofDisableArbTex();
		}
		
		//this gets the wrapping in u and v coodinates
		//we don't support different wrapping in OF so just read u
		aiTextureMapMode texMapMode[2];
		if( mSettings.importTextures ) {
			for(int d = 0; d <= AI_TEXTURE_TYPE_MAX; d++){
				if(AI_SUCCESS == mtl->GetTexture((aiTextureType)d, texIndex, &texPath, NULL, NULL, NULL, NULL, &texMapMode[0])){
					
					// getTextureTypeAsString(const ofMaterialTextureType & aMaterialTextureType)
					auto matType = ofxAssimp::Texture::ofTextureTypeForAiType( (aiTextureType)d );
					auto texTypeString = ofMaterial::getTextureTypeAsString(matType);
					//this is a solution to support older versions of assimp. see the weak defination above
					if( aiTextureTypeToString ){
						ofLogVerbose("ofxAssimp::SrcScene") << "loadGLResource(): loading " <<  aiTextureTypeToString((aiTextureType)d) << " image from \"" << texPath.data << "\"";
					}
					
					if( matType == OF_MATERIAL_TEXTURE_NONE ) {
						ofLogWarning("ofxAssimp::SrcScene") << "unable to detect texture type: " << texPath.data;
						continue;
					}
					
					bool bWrap = (texMapMode[0]==aiTextureMapMode_Wrap);
					
					std::string texPathStr = texPath.C_Str();
					
					//deal with Blender putting "//" in front of local file paths
					if( texPathStr.size() > 2 && texPathStr.substr(0, 2) == "//" ){
						texPathStr = texPathStr.substr(2, texPathStr.size()-2);
					}
					
					//glb embedded texture file starts with *0
					bool bTryEmbed = false;
					if( texPathStr.size() >= 2 && texPathStr[0] == '*'){
						bTryEmbed = true;
					}
					
					//stuff for embedded textures
					auto ogPath = texPathStr;
					bool bHasEmbeddedTexture = false;
					
					auto modelFolder = ofFilePath::getEnclosingDirectory( mFile.path() );
					auto relTexPath = ofFilePath::getEnclosingDirectory(texPathStr,false);
					auto realPath = modelFolder / of::filesystem::path{ texPathStr };
					
					
#ifndef TARGET_LINUX_ARM
					if(bTryEmbed || ofFile::doesFileExist(realPath) == false) {
						auto embeddedTexture = scene->GetEmbeddedTexture(ogPath.c_str());
						if( embeddedTexture ){
							bHasEmbeddedTexture = true;
							ofLogVerbose("ofxAssimp::SrcScene") << "loadGLResource() texture " << realPath.filename() << " is embedded ";
						}else{
							ofLogError("ofxAssimp::SrcScene") << "loadGLResource(): texture doesn't exist: \""
							<< mFile.getFileName() + "\" in \"" << realPath.string() << "\"";
						}
					}
#endif
					
					
					
					bool bTextureAlreadyExists = false;
//					if(mAssimpTextures.count(realPath)){
					if(mTextureCacheMap.count(realPath)) {
						bTextureAlreadyExists = true;
					}
					
					if(bTextureAlreadyExists) {
						ofLogVerbose("ofxAssimp::SrcScene") << "loadGLResource(): texture already loaded: \""
						<< mFile.getFileName() + "\" from \"" << realPath.string() << "\"" << " adding texture as " << texTypeString;
					} else {
						shared_ptr<ofTexture> texture = std::make_shared<ofTexture>();
//						auto assimpTexture = std::make_shared<ofxAssimp::Texture>();
						
						if( bHasEmbeddedTexture ){
#ifndef TARGET_LINUX_ARM
							auto embeddedTexture = scene->GetEmbeddedTexture(ogPath.c_str());
							
							//compressed texture
							if( embeddedTexture->mHeight == 0 && embeddedTexture->mWidth > 0){
								ofImage tmp;
								ofBuffer buffer;
								buffer.set((char *)embeddedTexture->pcData, embeddedTexture->mWidth);
								
								tmp.setUseTexture(false);
								tmp.load(buffer);
								
								ofLogVerbose("ofxAssimp::SrcScene") << "loadGLResource() texture size is " << tmp.getWidth() << "x" << tmp.getHeight();
								
//								assimpTexture->getTextureRef().loadData(tmp.getPixels());
								texture->loadData(tmp.getPixels());
							}else{
								//uncompressed texture - might need swizzling from argb to rgba?
								auto glFormat = getGLFormatFromAiFormat(embeddedTexture->achFormatHint);
//								assimpTexture->getTextureRef().loadData((const uint8_t *)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight, glFormat);
								texture->loadData((const uint8_t *)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight, glFormat);
							}
#endif
						}else{
//							ofLoadImage(assimpTexture->getTextureRef(), realPath );
							ofLoadImage(*texture, realPath);
						}
						
						if(texture && texture->isAllocated()){
							mTextureCacheMap[realPath] = texture;
							ofLogVerbose("ofxAssimp::SrcScene") << "loadGLResource(): texture " << texTypeString << " loaded, dimensions: " << texture->getWidth() << "x" << texture->getHeight();
						}else{
							ofLogError("ofxAssimp::SrcScene") << "loadGLResource(): couldn't load texture: \""
							<< mFile.getFileName() + "\" from \"" << realPath.string() << "\"";
						}
					}
					
					
					if(mTextureCacheMap.count(realPath) > 0) {
						// create a key using the type of texture and the path
						std::string assimpTexKey = texTypeString+"_"+realPath.string();
						std::shared_ptr<ofxAssimp::Texture> assimpTexture;
						if( mAssimpTextures.count(assimpTexKey) ) {
							assimpTexture = mAssimpTextures[assimpTexKey];
						} else {
							assimpTexture = std::make_shared<ofxAssimp::Texture>();
							assimpTexture->setup( realPath, bWrap );
							assimpTexture->setAiTextureType((aiTextureType)d);
							assimpTexture->setTexture(mTextureCacheMap[realPath]);
							mAssimpTextures[assimpTexKey] = assimpTexture;
						}
						
						if( assimpTexture ) {
							ofLogVerbose("ofxAssimp::SrcScene") << "adding texture type: " << texTypeString << " to mesh: " << aSrcMesh->getName();
							aSrcMesh->addTexture( assimpTexture );
						}
					}
				}
			}
		}
		
		if( bWasUsingArb ) {
			ofEnableArbTex();
		}
		
		
		int usage;
		if(mAnimations.size()){
#ifndef TARGET_OPENGLES
			if(!ofIsGLProgrammableRenderer()){
				usage = GL_STATIC_DRAW;
			}else{
				usage = GL_STREAM_DRAW;
			}
#else
			usage = GL_DYNAMIC_DRAW;
#endif
		}else{
			usage = GL_STATIC_DRAW;
		}
		
		aSrcMesh->usage = usage;
		
		ofMesh tempMesh;
		if( aSrcMesh->hasTexture() ) {
			ofxAssimp::Utils::aiMeshToOfMesh(amesh, tempMesh, !mSettings.convertToLeftHanded, &aSrcMesh->getTexture() );
		} else {
			ofxAssimp::Utils::aiMeshToOfMesh(amesh, tempMesh, !mSettings.convertToLeftHanded, nullptr);
		}
		
		aSrcMesh->calculateLocalBounds(tempMesh);
		
		aSrcMesh->vbo->setVertexData(&amesh->mVertices[0].x,3,amesh->mNumVertices,usage,sizeof(aiVector3D));
		if(amesh->HasVertexColors(0)){
			aSrcMesh->vbo->setColorData(&amesh->mColors[0][0].r,amesh->mNumVertices,GL_STATIC_DRAW,sizeof(aiColor4D));
		}
		if(amesh->HasNormals()){
			aSrcMesh->vbo->setNormalData(&amesh->mNormals[0].x,amesh->mNumVertices,usage,sizeof(aiVector3D));
		}
		if (tempMesh.hasTexCoords()){
			aSrcMesh->vbo->setTexCoordData(&tempMesh.getTexCoords()[0].x, amesh->mNumVertices,GL_STATIC_DRAW,sizeof(glm::vec2));
		}
		
		aSrcMesh->indices.resize(amesh->mNumFaces * 3);
		int j=0;
		for (unsigned int x = 0; x < amesh->mNumFaces; ++x){
			for (unsigned int a = 0; a < amesh->mFaces[x].mNumIndices; ++a){
				aSrcMesh->indices[j++] = amesh->mFaces[x].mIndices[a];
			}
		}
		aSrcMesh->vbo->setIndexData(&aSrcMesh->indices[0],(int)aSrcMesh->indices.size(),GL_STATIC_DRAW);
	}
	ofLogVerbose("ofxAssimp::SrcScene") << "loadGLResource(): finished";
}
