#include "ofxAssimpSrcScene.h"
#include "ofxAssimpUtils.h"
#include "ofLight.h"
#include "ofImage.h"
#include "ofPixels.h"
#include "ofGraphics.h"
#include "ofConstants.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/DefaultLogger.hpp>

#include <glm/gtx/matrix_decompose.hpp>

//--------------------------------------------------------------
using std::shared_ptr;
using std::vector;

using namespace ofx::assimp;

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
	
	ofLogVerbose("ofx::assimp::SrcScene") << "load(): loading from memory buffer \"." << extension << "\"";
	
	if(scene.get() != nullptr){
		clear();
		// we reset the shared_ptr explicitly here, to force the old
		// aiScene to be deleted **before** a new aiScene is created.
		scene.reset();
	}
	
	// sets various properties & flags to a default preference
	unsigned int flags = initImportProperties(assimpOptimizeFlags);
	
	// 	//enable assimp logging based on ofGetLogLevel
	//	if( ofGetLogLevel() < OF_LOG_NOTICE ){
	//		auto logLevel = Assimp::DefaultLogger::LogSeverity::VERBOSE;
	//		Assimp::DefaultLogger::create(ASSIMP_DEFAULT_LOG_NAME, logLevel, aiDefaultLogStream_DEBUGGER | aiDefaultLogStream_STDOUT );
	//	}
	
	// loads scene from memory buffer - note this will not work for multipart files (obj, md3, etc)
	const aiScene * scenePtr = importer.ReadFileFromMemory(buffer.getData(), buffer.size(), flags, extension);
	
	// this is funky but the scenePtr is managed by assimp and so we can't put it in our shared_ptr without disabling the deleter with: [](const aiScene*){}
	scene = shared_ptr<const aiScene>(scenePtr,[](const aiScene*){});
	
	ImportSettings tsettings;
	tsettings.assimpOptimizeFlags = assimpOptimizeFlags;
	bool bOk = processScene(tsettings);
	return bOk;
}

//------------------------------------------
bool SrcScene::load( const ImportSettings& asettings ) {
	mFile.open(asettings.filePath, ofFile::ReadOnly, true); // Since it may be a binary file we should read it in binary -Ed
	if(!mFile.exists()) {
		ofLogVerbose("ofx::assimp::SrcScene") << "load(): model does not exist: \"" << asettings.filePath << "\"";
		return false;
	}
	
	ofLogVerbose("ofx::assimp::SrcScene") << "load(): loading \"" << mFile.getFileName() << "\" from \"" << mFile.getEnclosingDirectory() << "\"";
	
	if(scene.get() != nullptr){
		clear();
		// we reset the shared_ptr explicitly here, to force the old
		// aiScene to be deleted **before** a new aiScene is created.
		scene.reset();
	}
	
	// sets various properties & flags to a default preference
	unsigned int flags = initImportProperties(asettings.assimpOptimizeFlags);
	// loads scene from file
	std::string path = mFile.getAbsolutePath();
	const aiScene * scenePtr = importer.ReadFile(path.c_str(), flags);
	
	//this is funky but the scenePtr is managed by assimp and so we can't put it in our shared_ptr without disabling the deleter with: [](const aiScene*){}
	scene = shared_ptr<const aiScene>(scenePtr,[](const aiScene*){});
	
	bool bOk = processScene(asettings);
	return bOk;
}

unsigned int SrcScene::initImportProperties(int assimpOptimizeFlags) {
	store.reset(aiCreatePropertyStore(), aiReleasePropertyStore);
	
	// only ever give us triangles.
	aiSetImportPropertyInteger(store.get(), AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
	aiSetImportPropertyInteger(store.get(), AI_CONFIG_PP_PTV_NORMALIZE, true);
	
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
		
		// we need the armature data to help with bone parsing
		flags |= aiProcess_PopulateArmatureData;
		// this fixes things for OF both tex uvs and model not flipped in z
		flags |= aiProcess_ConvertToLeftHanded;
	}
	
	return flags;
}

//-------------------------------------------
void SrcScene::optimizeScene(){
	if( scene ) {
		aiApplyPostProcessing(scene.get(),aiProcess_ImproveCacheLocality | aiProcess_OptimizeGraph |
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
	
	ofLogVerbose("ofx::assimp::SrcScene") << "clear(): deleting GL resources";
	
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
	
//	ofLogNotice( "PRINTING ALL SCENE NODES" );
//	printAllNodeNames( scene->mRootNode, 0 );
//	ofLogNotice( "DONE PRINTING ALL SCENE NODES" ) << std::endl << std::endl;
	
//	mBSceneBoundsDirty = true;
//	normalizeFactor = ofGetWidth() / 2.0;
	
	if(scene){
//		if( mSettings.importAnimations ) {
//			unsigned int numOfAnimations = scene->mNumAnimations;
//			for (unsigned int i = 0; i<numOfAnimations; i++) {
//				aiAnimation * animation = scene->mAnimations[i];
//				mAnimations.push_back(Animation(scene, animation));
//			}
//		}
		
//		ofLogNotice("ofx::assimp::SrcScene::processScene : scale: ") << getScale() << " global Scale: " << getGlobalScale();
		processNodes();
//		processBones();
		processAnimations();
		
//		std::stringstream ss;
//		for( auto& srcNode : mSrcNodes ) {
//			ss << srcNode->getAsString(0);
//		}
//		std::cout << ss.str();
		
//		loadGLResources();
		
//		// now associate the meshes with the bones
//		for( auto& modelMesh : mSrcMeshes ) {
//			//			mBones
//			// loop through all vertex weights of all bones
//			auto* mesh = modelMesh->getAiMesh();
//			for(unsigned int a = 0; a < mesh->mNumBones; ++a) {
//				aiBone* bone = mesh->mBones[a];
//				if( !bone ) {
//					ofLogError("ofx::assimp::SrcScene : NULL bone! - ") << a;
//					continue;
//				}
//				aiNode* node = scene->mRootNode->FindNode(bone->mName);
//				if( !node ) {
//					ofLogError("ofx::assimp::SrcScene : unable to find scene node for bone!") << bone->mName.data;
//					continue;
//				}
//				if( auto sbone = getSrcBone( node ) ) {
//					ofLogNotice("ofx::assimp::SrcScene : adding bone to mesh") << sbone->getName();
//					modelMesh->mBones.push_back(sbone);
//				} else {
//					ofLogWarning("ofx::assimp::SrcScene : unable to find source bone!") << bone->mName.data;
//				}
//			}
//
//			//			ofLogNotice("--------------------------------------");
//			//			ofLogNotice("processScene ") << modelMesh->getName() << " num ai bones: " << mesh->mNumBones << " num ofx bones: " << modelMesh->mBones.size();
//		}
		
		// now parent the bones
//		for( auto srcBone : mSrcBones ) {
//			//			srcBone->setParent( *this, false );
//			srcBone->parentRecursive(this);
//		}
		
//		ofLogNotice("-- ModelLoader::processScene : scale: ") << getScale() << " global Scale: " << getGlobalScale();
		
//		update();
//		calculateDimensions();
		
		if(mAnimations.size() > 0)
			ofLogVerbose("ofx::assimp::SrcScene") << "load(): scene has " << mAnimations.size() << "animations";
		else {
			ofLogVerbose("ofx::assimp::SrcScene") << "load(): no animations";
		}
		
		return true;
	}else{
		ofLogError("ofx::assimp::SrcScene") << "load(): " + (string) aiGetErrorString();
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
	//	if( anode->mParent ) {
	//		ss << " -> parent: " << anode->mParent->mName.data;
	//	}
	//ss << " -> num bones: " <<  anode->mNumBones ;
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
	//	ofLogNotice("Node: " ) << anode->mName.data <<;
	for (unsigned int i = 0; i < anode->mNumChildren; ++i){
		printAllNodeNames( anode->mChildren[i], alevel );
	}
}

//-------------------------------------------
void SrcScene::processNodes() {
	// lets load in the node hierarchy here //
//	processNodesRecursive(scene->mRootNode, shared_ptr<SrcNode>() );
	mSrcMeshes.clear();
	mSrcMeshes.assign( scene->mNumMeshes, shared_ptr<ofx::assimp::SrcMesh>() );
//	processNodesRecursive(scene->mRootNode, shared_ptr<SrcNode>() );
	processMeshes(scene->mRootNode, shared_ptr<SrcNode>());
	for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; i++ ){
		processNodesRecursive(scene->mRootNode->mChildren[i], shared_ptr<SrcNode>() );
	}
}

//-------------------------------------------
void SrcScene::processNodesRecursive(aiNode* anode, std::shared_ptr<SrcNode> aParentNode) {
	if( !anode ) return;
	
	std::shared_ptr<SrcNode> sNode;
	if( isBone(anode) || isArmature(anode) ) {
//	if( aiBone* tAiBone = getAiBoneForAiNode(anode) ) {
		aiBone* tAiBone = getAiBoneForAiNode(anode);
		auto sBone = std::make_shared<ofx::assimp::SrcBone>();
		sBone->setAiBone(tAiBone, anode);
		sBone->bRoot = isRootBone(anode) || !isBone(anode);
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
			sNode = std::make_shared<ofx::assimp::SrcNode>();
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
		processNodesRecursive(anode->mChildren[i], sNode );
	}
}

//-------------------------------------------
void SrcScene::processMeshes(aiNode* anode, std::shared_ptr<SrcNode> aSrcNode) {
	// the meshes are per node it seems
	for(unsigned int i = 0; i < anode->mNumMeshes; i++) {
		unsigned int meshIndex = anode->mMeshes[i];
		if( meshIndex >= mSrcMeshes.size() ) {
			ofLogError("SrcScene::processNodesRecursive") << " mesh index out of range: " << meshIndex << " / " << mSrcMeshes.size() << " num in scene: " << scene->mNumMeshes;
			continue;
		}
		if( mSrcMeshes[meshIndex] ) {
			ofLogWarning("SrcScene::processNodesRecursive") << " hmm we already have a SrcMesh at index: " << meshIndex << " " << anode->mName.data;
		} else {
			ofLogNotice("SrcScene::processNodesRecursive") << " going to process mesh: " << scene->mMeshes[meshIndex]->mName.data << " from node: " << anode->mName.data << " num scene meshes: " << scene->mNumMeshes;
			// ok, open slot for a src mesh //
			auto srcMesh = std::make_shared<ofx::assimp::SrcMesh>();
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
//void SrcScene::processBones() {
//	mSrcBones.clear();
//
//	ofLogNotice( "SrcScene processBones: " ) << scene->mRootNode->mName.data << " skeletons: " << scene->mNumSkeletons;
////	ofLogNotice( "PRINTING ALL SCENE NODES" );
////	printAllNodeNames( scene->mRootNode, 0 );
////	ofLogNotice( "DONE PRINTING ALL SCENE NODES" ) << std::endl << std::endl;
//	//	for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; ++i){
//	//		ofLogNotice("processBones : ") << i << " - " << scene->mRootNode->mChildren[i]->mName.data;
//	//	}
//
//	// lets go through and try to find the armatures as src bones
//	for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
//		// current mesh we are introspecting
//		aiMesh* mesh = scene->mMeshes[i];
//		if( mesh == nullptr ) {
//			continue;
//		}
//
//		for(unsigned int a = 0; a < mesh->mNumBones; ++a) {
//			aiBone* bone = mesh->mBones[a];
//
//			if( bone == nullptr ) {
//				ofLogNotice("ProcessBones : BONE IS NULL!") << a;
//				continue;
//			}
//			if( bone->mArmature == nullptr ) {
//				ofLogNotice("ProcessBones : bone->mArmature IS NULL!") << bone->mName.data;
//				continue;
//			}
//			if(!getSrcBone(bone->mArmature)) {
//				auto srcBone = std::make_shared<ofx::assimp::SrcBone>();
//				srcBone->setAiBone(bone, bone->mArmature);
//				srcBone->bRoot = true;
//				mSrcBones.push_back(srcBone);
//				ofLogNotice("ofx::assimp::processBones ") << " root bone: " << srcBone->getName();
//			}
//		}
//	}
//
////	std::cout << std::endl << std::endl;
////	ofLogNotice("------------------------------");
//
//	// ok now we have some parent bones aka armatures to start with //
//	// lets get the babies //
//	for( auto& srcRootBone : mSrcBones ) {
//		recursiveAddSrcBones(srcRootBone);
//	}
//
////	for( auto& srcRootBone : mSrcBones ) {
////		std::cout << srcRootBone->getAsString() << std::endl;
////		std::cout << "----------------------------------------" << std::endl;
////	}
//}

//-------------------------------------------
bool SrcScene::isBone( aiNode* aAiNode ) {
//	if(isRootBone( aAiNode )) {
//		return true;
//	}
	return (getAiBoneForAiNode(aAiNode) != nullptr);
}

//-------------------------------------------
bool SrcScene::isArmature( aiNode* aAiNode ) {
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
		// current mesh we are introspecting
		aiMesh* mesh = scene->mMeshes[i];
		if( mesh == nullptr ) {
			continue;
		}
		for(unsigned int a = 0; a < mesh->mNumBones; ++a) {
			aiBone* bone = mesh->mBones[a];
			if( bone != nullptr ) {
				if( aAiNode == bone->mArmature ) {
					return true;
				}
			}
		}
	}
	return false;
}

//-------------------------------------------
bool SrcScene::isRootBone( aiNode* aAiNode ) {
	// check if there are parent bones
	bool bHasBoneParent = false;
	auto temp = aAiNode->mParent;
	while( temp ) {
		if( getAiBoneForAiNode(temp)) {
			bHasBoneParent=true;
			break;
		}
		if( temp != scene->mRootNode && isArmature(temp)) {
			bHasBoneParent=true;
			break;
		}
		temp = temp->mParent;
	}
	
	return !bHasBoneParent;
	
//	return false;
}

//-------------------------------------------
aiBone*  SrcScene::getAiBoneForAiNode( aiNode* aAiNode ) {
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
		ofLogVerbose("ofx::assimp::SrcScene") << "loadGLResources(): loading mesh " << i;
		// current mesh we are introspecting
		aiMesh* mesh = scene->mMeshes[i];
		if( mesh == nullptr ) {
			continue;
		}
		for(unsigned int a = 0; a < mesh->mNumBones; ++a) {
			aiBone* bone = mesh->mBones[a];
			if( bone != nullptr ) {
				if( aAiNode->mName == bone->mName ) {
//				if( bone->mNode == aAiNode) {
					return bone;
				}
			}
		}
	}
	return nullptr;
}

//-------------------------------------------
void SrcScene::recursiveAddSrcBones( std::shared_ptr<ofx::assimp::SrcBone> abone ) {
	for( unsigned int i = 0; i < abone->getAiNode()->mNumChildren; i++ ) {
		// aiNode, now we need to get the associated bone
		auto boneNode = abone->getAiNode()->mChildren[i];
		if( boneNode ) {
			auto tbone = getAiBoneForAiNode( boneNode );
			if( tbone ) {
				auto nSrcBone = std::make_shared<ofx::assimp::SrcBone>();
				nSrcBone->setAiBone(tbone, boneNode);
//				abone->childBones[nSrcBone->getName()] = nSrcBone;
				abone->childBones.push_back(nSrcBone);
//				nSrcBone->setParent( *abone, false );
//				std::cout << abone->getName() << " - Bone child: " << nSrcBone->getName() << std::endl;
			}
		}
	}
	
	for( auto iter : abone->childBones ) {
		recursiveAddSrcBones(iter);
	}
}

//-------------------------------------------
std::shared_ptr<ofx::assimp::SrcNode> SrcScene::getSrcNodeForAiNodeName( const std::string& aAiNodeName ) {
	std::shared_ptr<ofx::assimp::SrcNode> rnode;
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
			mLights[i].setOrientation(aiVecToOfVec(scene->mLights[i]->mDirection));
		}
		if(scene->mLights[i]->mType!=aiLightSource_POINT){
			mLights[i].setSpotlight();
			mLights[i].setPosition(aiVecToOfVec(scene->mLights[i]->mPosition));
		}
		mLights[i].setAmbientColor(aiColorToOfColor(scene->mLights[i]->mColorAmbient));
		mLights[i].setDiffuseColor(aiColorToOfColor(scene->mLights[i]->mColorDiffuse));
		mLights[i].setSpecularColor(aiColorToOfColor(scene->mLights[i]->mColorSpecular));
	}
}

//-------------------------------------------
void SrcScene::processAnimations() {
	if( !mSettings.importAnimations || !scene ) {
		mAnimations.clear();
		return;
	}
	
//	unsigned int numOfAnimations = scene->mNumAnimations;
//	for (unsigned int i = 0; i<numOfAnimations; i++) {
//		aiAnimation * animation = scene->mAnimations[i];
////		mAnimations.push_back(Animation(scene, animation));
//	}
	
	for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
		aiAnimation* animation = scene->mAnimations[i];
		if(animation == nullptr) {
			continue;
		}
//		mAnimations.push_back(Animation(scene, animation));
		mAnimations.push_back(Animation(i, animation));
//		auto srcAnim = std::make_shared<ofx::assimp::Animation>( scene, animation );
//		mAnimations.push_back(srcAnim);
		// now lets parse the animation by getting all of the channels
		for( unsigned int j = 0; j < animation->mNumChannels; j++ ) {
			// grab the node animation that holds all of the key frames
			aiNodeAnim* nodeAnim = animation->mChannels[j];
			// the nodeAnim contains the name of the node it affects,
			// according to the docs, it must exist and the name must be unique
			std::string nodeName = nodeAnim->mNodeName.data;
			if(std::shared_ptr<ofx::assimp::SrcNode> snode = getSrcNodeForAiNodeName( nodeName )) {
				// SrcNode contains a pointer to the aiNode
				processKeyframes(snode, nodeAnim, i);
			} else {
				ofLogWarning("SrcScene::processAnimations") << "unable to find node " << nodeName << " for animation: " << animation->mName.data;
			}
		}
	}
}

//-------------------------------------------
void SrcScene::processKeyframes(std::shared_ptr<ofx::assimp::SrcNode> aSrcNode, aiNodeAnim* aNodeAnim, unsigned int aAnimIndex) {
	if(!aSrcNode || !aNodeAnim) {
		return;
	}
	auto& anim = mAnimations[aAnimIndex];
	ofx::assimp::SrcAnimKeyCollection& keyCollection = aSrcNode->getKeyCollection(aAnimIndex);
	keyCollection.clear();
	keyCollection.setup( aNodeAnim, anim.getDurationInTicks() );
	
	ofLogNotice("SrcScene processKeyframes: ") << aSrcNode->getName() << " node anim: " << aNodeAnim->mNodeName.data << " node anim num pos keyframes: " << aNodeAnim->mNumPositionKeys << " scale: " << aNodeAnim->mNumScalingKeys << " rot: " << aNodeAnim->mNumRotationKeys;
	
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
			ofLogError("ofx::assimp::SrcScene : getGLFormatFromAiFormat") << " can't parse format " << formatStr;
		}
	}
	
	ofLogWarning("ofx::assimp::SrcScene : getGLFormatFromAiFormat") << " can't parse format " << formatStr << " returning GL_RGB";
	return GL_RGB;
}

//-------------------------------------------
void SrcScene::loadGLResources(std::shared_ptr<ofx::assimp::SrcMesh> aSrcMesh, aiMesh* amesh){
	
	ofLogVerbose("ofx::assimp::SrcScene") << "loadGLResources(): starting";
	
	// we do this as we have textures and vbos and in meshHelper and we don't want to copy them
	// this should really be a vector of shared_ptr's but keeping it as objects for legacy reasons
//	mSrcMeshes.clear();
//	mSrcMeshes.reserve(scene->mNumMeshes);
	
	// create OpenGL buffers and populate them based on each meshes pertinant info.
//	for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
//		ofLogVerbose("ofx::assimp::SrcScene") << "loadGLResources(): loading mesh " << i;
//		// current mesh we are introspecting
//		aiMesh* mesh = scene->mMeshes[i];
//		if( mesh == nullptr ) {
//			continue;
//		}
	if( amesh ) {
		
		// the current meshHelper we will be populating data into.
		//		modelMeshes.push_back( std::make_shared<MeshHelper>());
		//		MeshHelper & meshHelper = modelMeshes[i];
//		auto srcMesh = std::make_shared<ofx::assimp::SrcMesh>();
//		mSrcMeshes.push_back(srcMesh);
		//ofxAssimpMeshHelper meshHelper;
//		meshHelper->setParent( *this, false );
		
		//meshHelper.texture = NULL;
//		srcMesh->setAiMesh(mesh, aiNode* aAiNode); // also makes the material
		
		// Handle material info
		aiMaterial* mtl = scene->mMaterials[amesh->mMaterialIndex];
		aiColor4D tcolor;
		
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &tcolor)){
			auto col = aiColorToOfColor(tcolor);
			aSrcMesh->material->setDiffuseColor(col);
		}
		
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &tcolor)){
			auto col = aiColorToOfColor(tcolor);
			aSrcMesh->material->setSpecularColor(col);
		}
		
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &tcolor)){
			auto col = aiColorToOfColor(tcolor);
			aSrcMesh->material->setAmbientColor(col);
		}
		
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &tcolor)){
			auto col = aiColorToOfColor(tcolor);
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
		
		// Culling
		unsigned int max = 1;
		int two_sided=0;
		if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided){
			aSrcMesh->twoSided = true;
			ofLogVerbose("ofx::assimp::SrcScene") <<" loadGLResources: mesh is two sided";
		}else{
			aSrcMesh->twoSided = false;
			ofLogVerbose("ofx::assimp::SrcScene") <<" loadGLResources: mesh is one sided";
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
					
					//this is a solution to support older versions of assimp. see the weak defination above
					if( aiTextureTypeToString ){
						ofLogVerbose("ofx::assimp::SrcScene") << "loadGLResource(): loading " <<  aiTextureTypeToString((aiTextureType)d) << " image from \"" << texPath.data << "\"";
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
							ofLogVerbose("ofx::assimp::SrcScene") << "loadGLResource() texture " << realPath.filename() << " is embedded ";
						}else{
							ofLogError("ofx::assimp::SrcScene") << "loadGLResource(): texture doesn't exist: \""
							<< mFile.getFileName() + "\" in \"" << realPath.string() << "\"";
						}
					}
#endif
					
					bool bTextureAlreadyExists = false;
					if(mAssimpTextures.count(realPath)){
						bTextureAlreadyExists = true;
					}
					
					if(bTextureAlreadyExists) {
						//					ofxAssimpTexture assimpTexture;
						
						//					assimpTexture.setup(*textures[realPath].get(), realPath, bWrap);
						//					assimpTexture.setTextureType((aiTextureType)d);
						auto assimpTexture = mAssimpTextures[realPath];
						aSrcMesh->addTexture(assimpTexture);
						
						ofLogVerbose("ofx::assimp::SrcScene") << "loadGLResource(): texture already loaded: \""
						<< mFile.getFileName() + "\" from \"" << realPath.string() << "\"" << " adding texture as " << assimpTexture->getTextureTypeAsString() ;
					} else {
						
						//					shared_ptr<ofTexture> texture = std::make_shared<ofTexture>();
						auto assimpTexture = std::make_shared<ofx::assimp::Texture>();
						
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
								
								ofLogVerbose("ofx::assimp::SrcScene") << "loadGLResource() texture size is " << tmp.getWidth() << "x" << tmp.getHeight();
								
								assimpTexture->getTextureRef().loadData(tmp.getPixels());
							}else{
								//uncompressed texture - might need swizzling from argb to rgba?
								auto glFormat = getGLFormatFromAiFormat(embeddedTexture->achFormatHint);
								assimpTexture->getTextureRef().loadData((const uint8_t *)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight, glFormat);
							}
#endif
						}else{
							//						ofLoadImage(*texture.get(), realPath);
							ofLoadImage(assimpTexture->getTextureRef(), realPath );
						}
						
						if(assimpTexture && assimpTexture->getTextureRef().isAllocated()){
							//						ofxAssimpTexture tmpTex;
							//						tmpTex.setup(*texture.get(), realPath, bWrap);
							assimpTexture->setup( realPath, bWrap );
							assimpTexture->setTextureType((aiTextureType)d);
							mAssimpTextures[realPath] = assimpTexture;
							ofLogNotice( "ofx::assimp::SrcScene") << " assimpTexture type: " << assimpTexture->getTextureTypeAsString() << " path: " << assimpTexture->getTexturePath();
							//						tmpTex.setTextureType((aiTextureType)d);
							aSrcMesh->addTexture( assimpTexture );
							
							ofLogVerbose("ofx::assimp::SrcScene") << "loadGLResource(): texture " << assimpTexture->getTextureTypeAsString() << " loaded, dimensions: " << assimpTexture->getTextureRef().getWidth() << "x" << assimpTexture->getTextureRef().getHeight();
						}else{
							ofLogError("ofx::assimp::SrcScene") << "loadGLResource(): couldn't load texture: \""
							<< mFile.getFileName() + "\" from \"" << realPath.string() << "\"";
						}
					}
				}
			}
		}
		
		if( bWasUsingArb ) {
			ofEnableArbTex();
		}
		
		//		meshHelper.mesh = mesh;
		//		meshHelper.setAiMesh(mesh);
		//		aiMeshToOfMesh(mesh, meshHelper->cachedMesh, meshHelper.get() );
		//		meshHelper->cachedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
//		aSrcMesh->validCache = true;
//		aSrcMesh->hasChanged = false;
		
		
		
//		if(mAnimations.size()){
//			meshHelper->animatedPos.resize(mesh->mNumVertices);
//			if(mesh->HasNormals()){
//				meshHelper->animatedNorm.resize(mesh->mNumVertices);
//			}
//		}
		
		
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
		
		ofMesh tempMesh;
		if( aSrcMesh->hasTexture() ) {
			aiMeshToOfMesh(amesh, tempMesh, &aSrcMesh->getTexture() );
		} else {
			aiMeshToOfMesh(amesh, tempMesh, nullptr);
		}
		
		aSrcMesh->calculateLocalBounds(tempMesh);
		
		aSrcMesh->vbo->setVertexData(&amesh->mVertices[0].x,3,amesh->mNumVertices,usage,sizeof(aiVector3D));
		if(amesh->HasVertexColors(0)){
			aSrcMesh->vbo->setColorData(&amesh->mColors[0][0].r,amesh->mNumVertices,GL_STATIC_DRAW,sizeof(aiColor4D));
		}
		if(amesh->HasNormals()){
			aSrcMesh->vbo->setNormalData(&amesh->mNormals[0].x,amesh->mNumVertices,usage,sizeof(aiVector3D));
		}
//		if (meshHelper->cachedMesh.hasTexCoords()){
//			meshHelper->vbo.setTexCoordData(&meshHelper->cachedMesh.getTexCoords()[0].x, mesh->mNumVertices,GL_STATIC_DRAW,sizeof(glm::vec2));
//		}
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
		
		aSrcMesh->vbo->setIndexData(&aSrcMesh->indices[0],aSrcMesh->indices.size(),GL_STATIC_DRAW);
		ofLogNotice("Setting indices on ") << aSrcMesh->getName() << " " << aSrcMesh->indices.size();
		
		//modelMeshes.push_back(meshHelper);
	}
	ofLogVerbose("ofx::assimp::SrcScene") << "loadGLResource(): finished";
}
