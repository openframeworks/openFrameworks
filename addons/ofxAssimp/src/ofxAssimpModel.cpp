#include "ofxAssimpModel.h"
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

using std::shared_ptr;
using std::vector;
using std::make_shared;
using std::string;

using namespace ofx::assimp;

Model::Model(){
	clear();
}

Model::~Model(){
	clear();
}

//------------------------------------------
bool Model::load(string aPathToFile, int assimpOptimizeFlags){
	ImportSettings tsettings;
	tsettings.filePath = aPathToFile;
	tsettings.assimpOptimizeFlags = assimpOptimizeFlags;
	return load(tsettings);
}

//------------------------------------------
bool Model::load(ofBuffer & buffer, int assimpOptimizeFlags, const char * extension){
	ofLogVerbose("ofx::assimp::Model") << "load(): loading from memory buffer \"." << extension << "\"";
	if( mSrcScene ) {
		mSrcScene->clear();
		mSrcScene.reset();
	}
	auto tscene = std::make_shared<ofx::assimp::SrcScene>();
	if(tscene->load( buffer, assimpOptimizeFlags, extension )) {
		bLoadedSrcScene = true;
		return setup(tscene);
	}
	clear();
	return false;
}

//------------------------------------------
bool Model::load( const ImportSettings& asettings ) {
	auto tscene = std::make_shared<ofx::assimp::SrcScene>();
	if(tscene->load(asettings)) {
		bLoadedSrcScene = true;
		return setup(tscene);
	}
	clear();
	return false;
}

//------------------------------------------
bool Model::setup( std::shared_ptr<ofx::assimp::SrcScene> ascene ) {
	clear();
	
	mSrcScene = ascene;
	bProcessedSceneSuccessfully = processScene();
	return bProcessedSceneSuccessfully;
}

//------------------------------------------
bool Model::isLoaded() {
	return bProcessedSceneSuccessfully;
}

//------------------------------------------
bool Model::processScene() {
//	mSettings = asettings;
	mAnimationIndex = 0;
	mBSceneBoundsDirty = true;
	normalizeFactor = ofGetWidth() / 2.0;

	if(mSrcScene){
		if( mSrcScene->getImportSettings().importAnimations ) {
//			auto scene = mSrcScene->getAiScene();
//			unsigned int numOfAnimations = scene->mNumAnimations;
//			for (unsigned int i = 0; i<numOfAnimations; i++) {
//				aiAnimation * animation = scene->mAnimations[i];
//				mAnimations.push_back(Animation(scene, animation));
//			}
			mAnimations = mSrcScene->getAnimations();
			if(mAnimations.size() > 0 ) {
				// add a default mixer
				mAnimMixer = std::make_shared<AnimationMixer>();
				mAnimMixer->add( AnimationClip( mAnimations[0], 1.0f ));
				mAnimMixer->getAnimationClip(0).animation.play();
			}
		}
		
		std::shared_ptr<ofx::assimp::Node> tempParent;
		auto sceneSrcNodes = mSrcScene->getRootNodes();
		for( auto sceneSrcNode : sceneSrcNodes ) {
			processSceneNodesRecursive(sceneSrcNode, tempParent);
		}
				
//		now associate the meshes with the bones
		for( auto& modelMesh : mMeshes ) {
			ofLogNotice("model mesh") << modelMesh->getName() << " indices: " << modelMesh->getNumIndices() << " vbo: " << modelMesh->vbo->getNumVertices();
			
			auto* mesh = modelMesh->getAiMesh();
			
			if(mAnimations.size()){
				modelMesh->animatedPos.resize(mesh->mNumVertices);
				if(mesh->HasNormals()){
					modelMesh->animatedNorm.resize(mesh->mNumVertices);
				}
			}
			
			
			for(unsigned int a = 0; a < mesh->mNumBones; ++a) {
				aiBone* bone = mesh->mBones[a];
				if( !bone ) {
					ofLogError("ofx::assimp::Model : NULL bone! - ") << a;
					continue;
				}
//				aiNode* node = scene->mRootNode->FindNode(bone->mName);
				aiNode* boneNode = bone->mNode;
				if( !boneNode ) {
					ofLogError("ofx::assimp::Model : unable to find scene node for bone!") << bone->mName.data;
					continue;
				}
				
				for( auto modelBone : mBones ) {
					if( modelBone->getSrcBone()->getAiNode() == boneNode ) {
						modelMesh->mBones.push_back(modelBone);
						break;
					}
				}
			}
		}
		
		ofLogNotice("-- Model::processScene : scale: ") << getScale() << " global Scale: " << getGlobalScale();
		
		update();
		calculateDimensions();

		if(hasAnimations()) {
			ofLogVerbose("ofx::assimp::Model") << "load(): scene has " << getNumAnimations() << "animations";
		} else {
			ofLogVerbose("ofx::assimp::Model") << "load(): no animations";
		}
		return true;
	}
	clear();
	return false;
}

//-------------------------------------------
void Model::processSceneNodesRecursive( std::shared_ptr<ofx::assimp::SrcNode> aSrcNode, std::shared_ptr<ofx::assimp::Node> aParentNode ) {
	if( !aSrcNode ) return;
	
	shared_ptr<ofx::assimp::Node> newNode;
	auto nodeType = aSrcNode->getType();
	if( nodeType == ofx::assimp::NodeType::OFX_ASSIMP_MESH ) {
		auto tmesh = make_shared<ofx::assimp::Mesh>();
		auto srcMesh = std::dynamic_pointer_cast<ofx::assimp::SrcMesh>(aSrcNode);
		tmesh->setSrcMesh( srcMesh );
		mMeshes.push_back(tmesh);
		newNode = tmesh;
	} else if( nodeType == ofx::assimp::NodeType::OFX_ASSIMP_BONE ) {
		
		auto srcBone = std::dynamic_pointer_cast<ofx::assimp::SrcBone>(aSrcNode);
		
		std::shared_ptr<ofx::assimp::Bone> tbone;
		if( srcBone->bRoot ) {
			auto skel = make_shared<ofx::assimp::Skeleton>();
			mSkeletons.push_back(skel);
			tbone = skel;
		} else {
			tbone = make_shared<ofx::assimp::Bone>();
		}
		tbone->setSrcBone( srcBone );
		mBones.push_back(tbone);
		newNode = tbone;
	} else if( nodeType == ofx::assimp::NodeType::OFX_ASSIMP_NODE ) {
		auto tnode = make_shared<ofx::assimp::Node>();
		mNullNodes.push_back(tnode);
		newNode = tnode;
	}
	
	if( newNode ) {
		if( aParentNode ) {
			aParentNode->addChild( newNode );
			newNode->setParent( *aParentNode );
			newNode->setParentNode( aParentNode );
		} else {
			newNode->setParent( *this );
			mKids.push_back( newNode );
		}
//		newNode->setRootOFNode(this);
		newNode->setSrcNode( aSrcNode );
//		mAllNodes.push_back( newNode );
	}
	
	for( unsigned int i = 0; i < aSrcNode->getNumChildren(); i++ ) {
		processSceneNodesRecursive( aSrcNode->getChildren()[i], newNode );
	}
}

////-------------------------------------------
//std::shared_ptr<ofx::assimp::Bone> Model::getBoneForSrcBone( std::shared_ptr<ofx::assimp::SrcBone> aSrcBone ) {
//	for( auto bone : mBones ) {
//		if( bone->getSrcBone().get() == aSrcBone.get() ) {
//			return bone;
//		}
//	}
//	return std::shared_ptr<ofx::assimp::Bone>();
//}

//-------------------------------------------
std::string Model::getHierarchyAsString() {
	std::stringstream ss;
	for( auto& node : mKids ) {
		ss << node->getAsString(0);
	}
	return ss.str();
}

//-------------------------------------------
void Model::clear(){
	
	if( bLoadedSrcScene && mSrcScene ){
		mSrcScene->clear();
		mSrcScene.reset();
	}
	bLoadedSrcScene = false;
	bProcessedSceneSuccessfully = false;
	
	ofLogVerbose("ofx::assimp::Model") << "clear(): deleting GL resources";
	
	if( mSrcNode ) {
		mSrcNode.reset();
	}
	if(mParentNode) {
		mParentNode.reset();
	}
	
	// clear out everything.
	mMeshes.clear();
	mBones.clear();
	mNullNodes.clear();
	mKids.clear();
	mAnimations.clear();
	mSkeletons.clear();
	
	setOrientation({0.f, 0.f, 0.f});
	setPosition(0.f, 0.f, 0.f);
	//	lights.clear();
	
	setScale(1.f, 1.f, 1.f);
	//	bNormalizeScale = true;
	bUsingMaterials = true;
	bUsingNormals = true;
	bUsingTextures = true;
	bUsingColors = true;
	
	removeAnimationMixer();
}

//-------------------------------------------
void Model::centerAndScaleToWindow() {
	centerAndScaleToViewRect( 0.f, 0.f, ofGetWidth(), ofGetHeight() );
}

//-------------------------------------------
void Model::centerAndScaleToViewRect(const ofRectangle& arect) {
	centerAndScaleToViewRect( arect.x, arect.y, arect.getWidth(), arect.getHeight());
}

//-------------------------------------------
void Model::centerAndScaleToViewRect(const ofRectangle& arect, float aNormalizeFactor) {
	centerAndScaleToViewRect( arect.x, arect.y, arect.getWidth(), arect.getHeight(), aNormalizeFactor );
}

//-------------------------------------------
void Model::centerAndScaleToViewRect( float awidth, float aheight ) {
	centerAndScaleToViewRect( 0.f, 0.f, awidth, aheight );
}

//-------------------------------------------
void Model::centerAndScaleToViewRect( float ax, float ay, float awidth, float aheight ) {
	centerAndScaleToViewRect( 0.f, 0.f, awidth, aheight, awidth / 2.0 );
}

//-------------------------------------------
void Model::centerAndScaleToViewRect( float ax, float ay, float awidth, float aheight, float aNormalizeFactor ) {
	calculateDimensions();
	normalizeFactor = aNormalizeFactor;
	
	normalizedScale = mSceneBoundsLocal.max.x-mSceneBoundsLocal.min.x;
	normalizedScale = std::max(double(mSceneBoundsLocal.max.y - mSceneBoundsLocal.min.y), normalizedScale);
	normalizedScale = std::max(double(mSceneBoundsLocal.max.z - mSceneBoundsLocal.min.z), normalizedScale);
	
	if (fabs(normalizedScale) < std::numeric_limits<float>::epsilon()){
		ofLogWarning("ofx::assimp::Model") << "Error calculating normalized scale of scene" << std::endl;
		normalizedScale = 1.0;
	} else {
		if( normalizedScale != 0.0f ) {
			normalizedScale = 1.f / normalizedScale;
		}
		normalizedScale *= normalizeFactor;
	}
	
	setScale( normalizedScale );
	
	float sceneHeight = mSceneBoundsLocal.getHeight() * normalizedScale;
	setPosition(0.f, -sceneHeight * 0.5f, 0.f);
	
}

////-------------------------------------------
//void Model::setNormalizationFactor(float factor){
//	normalizeFactor = factor;
//}

//-------------------------------------------
void Model::calculateDimensions(){
	if(!mSrcScene) return;
	ofLogVerbose("ofx::assimp::Model") << "calculateDimensions(): inited scene with "
	<< mSrcScene->getAiScene()->mNumMeshes << " meshes & " << mSrcScene->getAiScene()->mNumAnimations << " animations";
	
	mBSceneBoundsDirty = true;
	auto globalInvMat = glm::inverse(getGlobalTransformMatrix());
	mSceneBoundsLocal.clear();
	vector<glm::vec3> tMeshBoundsVerts;
	for( auto& meshHelper : mMeshes ) {
		meshHelper->recalculateBounds(true);
		mSceneBoundsLocal.include(globalInvMat, meshHelper->getGlobalBounds().getBoundingVerts());
//		tMeshBoundsVerts.push_back(meshHelper->getGlobalBounds().min);
//		tMeshBoundsVerts.push_back(meshHelper->getGlobalBounds().max);
//		tMeshBoundsVerts.insert(  tMeshBoundsVerts.end(), meshHelper->getLocalBounds().getBoundingVerts().begin(), meshHelper->getLocalBounds().getBoundingVerts().end() );
	}
//	mSceneBoundsLocal.include( tMeshBoundsVerts );

//	getBoundingBoxWithMinVector(&scene_min, &scene_max);
//	scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
//	scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
//	scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
//
//	// optional normalized scaling
//	normalizedScale = scene_max.x-scene_min.x;
//	normalizedScale = std::max(double(scene_max.y - scene_min.y), normalizedScale);
//	normalizedScale = std::max(double(scene_max.z - scene_min.z), normalizedScale);
//	if (fabs(normalizedScale) < std::numeric_limits<float>::epsilon()){
//		ofLogWarning("ofx::assimp::Model") << "Error calculating normalized scale of scene" << std::endl;
//		normalizedScale = 1.0;
//	} else {
//		if( normalizedScale != 0.0f ) {
//			normalizedScale = 1.f / normalizedScale;
//		}
//		normalizedScale *= normalizeFactor;
//	}

//	updateModelMatrix();
}

//------------------------------------------- update.
void Model::earlyUpdate() {
	if(!mSrcScene) return;
	updateAnimations();
//	aiMatrix4x4 trafo;
//	aiIdentityMatrix4(&trafo);
//	updateMeshTransforms(mSrcScene->getAiScene()->mRootNode, trafo );
	
	if(hasAnimations() == false) {
		return;
	}
//	for( auto& bone : mBones ) {
//		bone->updateFromSrcBone();
//	}
}

//------------------------------------------- update.
void Model::update() {
	earlyUpdate();
	lateUpdate();
}

void Model::lateUpdate() {
	if(hasAnimations() == false) {
		return;
	}
	
//	for( auto& bone : mBones ) {
//		bone->cacheGlobalMat();
//	}
	
	mBSceneBoundsDirty = true;
	updateMeshesFromBones();
	updateGLResources();
}

void Model::updateAnimations() {
	if( mAnimMixer ) {
		mAnimMixer->update(ofGetElapsedTimef());
		for( auto& kid : mKids ) {
			kid->update(mAnimMixer);
		}
	}
//	for(size_t i = 0; i < mAnimations.size(); i++) {
//		mAnimations[i].update();
//	}
}

void Model::updateMeshTransforms(aiNode * node, const aiMatrix4x4& parentMatrix) {
//	glm::mat4 globalInvMat = glm::inverse( getGlobalTransformMatrix() );
//	glm::mat4 gMat = globalInvMat * getGlobalTransformMatrix();
//	aiMatrix4x4 gBoneMat = glm2Assimp(gMat);
	
	// Not sure what is going on here ....

	aiMatrix4x4 m = node->mTransformation;
	m.Transpose();
//	ofMatrix4x4 matrix(m.a1, m.a2, m.a3, m.a4,
//					   m.b1, m.b2, m.b3, m.b4,
//					   m.c1, m.c2, m.c3, m.c4,
//					   m.d1, m.d2, m.d3, m.d4);
//	glm::mat4 	matrix(m.a1, m.a2, m.a3, m.a4,
//					   m.b1, m.b2, m.b3, m.b4,
//					   m.c1, m.c2, m.c3, m.c4,
//					   m.d1, m.d2, m.d3, m.d4);
	m *= parentMatrix;
//	matrix *= parentMatrix;
	
//	m = gBoneMat * m;
	
	// Decompose (aiVector3t<TReal>& scaling, aiQuaterniont<TReal>& rotation, aiVector3t<TReal>& position) const;
	
	aiVector3t<float> tAiScale;
	aiQuaterniont<float> tAiRotation;
	aiVector3t<float> tAiPosition;

	m.Decompose( tAiScale, tAiRotation, tAiPosition );

	glm::vec3 tpos = glm::vec3( tAiPosition.x, tAiPosition.y, tAiPosition.z );
	glm::quat tquat = glm::quat(tAiRotation.w, tAiRotation.x, tAiRotation.y, tAiRotation.z);
	glm::vec3 tscale = glm::vec3( tAiScale.x, tAiScale.y, tAiScale.z );
	
	
//	glm::vec3 tscale;
//    glm::quat tquat;
//    glm::vec3 tpos;
//	glm::vec3 skew;
//	glm::vec4 perspective;
//
//	glm::decompose(matrix, tscale, tquat, tpos, skew, perspective);
//	tquat=glm::conjugate(tquat);

	for(unsigned int i = 0; i < node->mNumMeshes; i++) {
		int meshIndex = node->mMeshes[i];
		if( meshIndex >= mMeshes.size() ) {
			ofLogError("Model::updateMeshes") << " mesh index out of range: " << meshIndex << " / " << mMeshes.size();
			continue;
		}
		
		auto meshHelper = mMeshes[meshIndex];
		
		const aiMesh* mesh = meshHelper->getAiMesh();
		if( mesh && mesh->mNumBones > 0 ) {
			//TODO: When this applied in addition to bone animation, the bones don't account for the mesh
			//transforms and the bones are given incorrect matrices even though the mesh renders
			continue;
		}
		
//		mesh.matrix = matrix;
//		meshHelper->setPosition( {0.f,0.f,0.f});
//		meshHelper->setOrientation( {0.f,0.f,0.f} );
//		meshHelper->setScale( {0.35f,0.35f,0.35f} );
		meshHelper->setPositionOrientationScale( tpos, tquat, tscale );
//		meshHelper->getGlobalTransformMatrix();
//		meshHelper->setGlobalPosition( gmat * tpos );
//		meshHelper->setGlobalOrientation( gmat * tquat );
//		meshHelper->setScale( gmat * tscale );
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++) {
//		updateMeshes(node->mChildren[i], matrix);
		updateMeshTransforms(node->mChildren[i], m);
	}
}

void Model::updateMeshesFromBones() {
	if (!hasAnimations()){
		return;
	}
//	auto gmat = getGlobalTransformMatrix();
//	auto globalInverse = scene->mRootNode->mTransformation;
//	globalInverse = globalInverse.Inverse();
	// try to update the src bones as a test //
//	for( auto& bone : mBones ) {
////		ofLogNotice("src bone: ") << srcBone->getName() << " scale: " << srcBone->getScale() << " global scale: " << srcBone->getGlobalScale();
//		bone->update();
//
//	}
	
	auto globalInvMat = glm::inverse(getGlobalTransformMatrix());
//	auto globalInvQuat = glm::inverse(getGlobalOrientation());
//	globalInvMat = getGlobalTransformMatrix();
//	globalInvMat = glm::mat4();
	
	// update mesh position for the animation
	for(size_t i = 0; i < mMeshes.size(); ++i) {
		// current mesh we are introspecting
		const aiMesh* mesh = mMeshes[i]->getAiMesh();
		if( mesh == nullptr ) {
			continue;
		}

		mMeshes[i]->animatedPos.assign(mMeshes[i]->animatedPos.size(), aiVector3D(0.0f));
//		if(mesh->HasNormals()){
		if(mMeshes[i]->hasNormals()) {
			mMeshes[i]->animatedNorm.assign(mMeshes[i]->animatedNorm.size(), aiVector3D(0.0f));
		}
		
		
		//-------------
		// loop through all vertex weights of all bones
//		ofLogNotice("Update Bones: ") << mesh->mNumBones << " src bones: " << modelMeshes[i]->mBones.size();
//		for(unsigned int a = 0; a < mesh->mNumBones; ++a) {
		for( unsigned int a = 0; a < mMeshes[i]->mBones.size(); ++a) {
			mMeshes[i]->hasChanged = true;
			mMeshes[i]->validCache = false;

			const aiBone* bone = mesh->mBones[a];
//			const aiMatrix4x4& posTrafo = boneMatrices[a];
			if( a >= mMeshes[i]->mBones.size() ) {
				ofLogError("Update Bones: ") << mesh->mNumBones << " src bones: " << mesh->mBones[a]->mName.data;
				continue;
			}
//
			auto sbone = mMeshes[i]->mBones[a];
//			ofLogNotice("Update sbone: ") << sbone->getName();
			if( !sbone ) {
				ofLogError("Update Bones: ") << mesh->mNumBones << " sbone is NULL: " << mesh->mBones[a]->mName.data;
				continue;
			}

			aiBone* tabone = sbone->getSrcBone()->getAiBone();

			if( !tabone) {
				ofLogError("Update Bones: ") << mesh->mNumBones << " bone is NULL: " << mesh->mBones[a]->mName.data;
				continue;
			}

//			const aiMatrix4x4& posTrafo = boneMatrices[a];

			glm::mat4 gMat = globalInvMat * sbone->getGlobalTransformMatrix();//sbone->getGlobalTransformCached();//sbone->getGlobalTransformMatrix();
//			gMat = (globalInvMat * modelMeshes[i]->getGlobalTransformMatrix()) * gMat;
			
			// puts the bones in the correct place, but not the vertices
//			gMat = globalInvMat * (glm::inverse(modelMeshes[i]->getGlobalTransformMatrix()) * sbone->getGlobalTransformMatrix());
			
			aiMatrix4x4 gBoneMat = glmMat4ToAiMatrix4x4(gMat);
			aiMatrix4x4 posTrafo = gBoneMat * sbone->getAiOffsetMatrix();//bone->mOffsetMatrix;
//			const aiMatrix4x4& posTrafo = boneMatrices[a];
//			continue;
			
//			for(unsigned int b = 0; b < sbone->numWeights; ++b) {
//				const aiVertexWeight& weight = sbone->weights[b];
//
//				size_t vertexId = weight.mVertexId;
//				const aiVector3D& srcPos = mesh->mVertices[vertexId];
//
//				modelMeshes[i]->animatedPos[vertexId] += weight.mWeight * (posTrafo * srcPos);
//			}
			
			for(unsigned int b = 0; b < bone->mNumWeights; ++b) {
				const aiVertexWeight& weight = bone->mWeights[b];

				size_t vertexId = weight.mVertexId;
				const aiVector3D& srcPos = mesh->mVertices[vertexId];

				mMeshes[i]->animatedPos[vertexId] += weight.mWeight * (posTrafo * srcPos);
			}
			if(mesh->HasNormals()){
				// 3x3 matrix, contains the bone matrix without the translation, only with rotation and possibly scaling
				aiMatrix3x3 normTrafo = aiMatrix3x3( posTrafo);
				for(unsigned int b = 0; b < bone->mNumWeights; ++b) {
					const aiVertexWeight& weight = bone->mWeights[b];
					size_t vertexId = weight.mVertexId;

					const aiVector3D& srcNorm = mesh->mNormals[vertexId];
					mMeshes[i]->animatedNorm[vertexId] += weight.mWeight * (normTrafo * srcNorm);
				}
			}
		}
//		--------------
		
	}
}

void Model::updateGLResources(){
	// now upload the result position and normal along with the other vertex attributes into a dynamic vertex buffer, VBO or whatever
	for (size_t i = 0; i < mMeshes.size(); ++i){
		if(mMeshes[i]->hasChanged){
			const aiMesh* mesh = mMeshes[i]->getAiMesh();
			if( mMeshes[i]->animatedPos.size() != mesh->mNumVertices ) {
				ofLogError( mMeshes[i]->getName() ) << " modelMeshes[i]->animatedPos.size(): " << mMeshes[i]->animatedPos.size() << " num verts: " <<mesh->mNumVertices ;
				continue;
			}

			if(hasAnimations()){
				mMeshes[i]->vbo->updateVertexData(&mMeshes[i]->animatedPos[0].x,mesh->mNumVertices);
				if(mesh->HasNormals()){
					mMeshes[i]->vbo->updateNormalData(&mMeshes[i]->animatedNorm[0].x,mesh->mNumVertices);
				}
			}
			mMeshes[i]->hasChanged = false;
		}
	}
}

//------------------------------------------- animations.
bool Model::hasAnimations() {
	return mAnimations.size() > 0;
}

unsigned int Model::getNumAnimations(){
	return mAnimations.size();
}

unsigned int Model::getCurrentAnimationIndex() {
	return mAnimationIndex;
}

ofx::assimp::Animation& Model::getCurrentAnimation() {
	if( mAnimations.size() < 1 ) {
		ofLogWarning("ofx::assimp::Model::getCurrentAnimation") << " no animations!!";
		return dummyAnimation;
	}
	if( mAnimMixer && mAnimMixer->getNumAnimationClips() > 0 ) {
		return mAnimMixer->getAnimationClips().back().animation;
	}
	ofLogWarning("ofx::assimp::Model::getCurrentAnimation") << " does not have current animation!";
	return dummyAnimation;
}

//ofx::assimp::Animation& Model::getAnimation(int aindex) {
//	if( mAnimations.size() < 1 ) {
//		ofLogWarning("ofx::assimp::Model : no animations!!");
//		return dummyAnimation;
//	}
//}
//
//ofx::assimp::Animation& Model::getAnimation(const std::string& aname) {
//	if( mAnimations.size() < 1 ) {
//		ofLogWarning("ofx::assimp::Model : no animations!!");
//		return dummyAnimation;
//	}
//}

bool Model::setCurrentAnimation( int aindex ) {
	if( mAnimations.size() < 1 ) {
		ofLogWarning("ofx::assimp::Model::setAnimation") << " no animations!!";
		return false;
	}
	if( aindex == mAnimationIndex ) {
		ofLogWarning("ofx::assimp::Model::setAnimation") << " already have this as the index!";
		return false;
	}
	mAnimationIndex = ofClamp(aindex, 0, mAnimations.size()-1);
	mAnimMixer->removeAll();
	mAnimMixer->add( AnimationClip( mAnimations[mAnimationIndex], 1.0f ));
	mAnimMixer->getAnimationClips().back().animation.reset();
	return true;
}

bool Model::setCurrentAnimation( const std::string& aname ) {
	int tindex = getAnimationIndex(aname);
	if( tindex < 0 ) {
		ofLogWarning("ofx::assimp::Model::setAnimation") << " could not find animation with name" << aname;
		return false;
	}
	return setCurrentAnimation(tindex);
}

bool Model::transitionCurrentAnimation( int aTargetAnimIndex, float aduration ) {
	if( mAnimations.size() < 1 ) {
		ofLogWarning("ofx::assimp::Model::setAnimation") << " no animations!!";
		return false;
	}
	mAnimationIndex = ofClamp(aTargetAnimIndex, 0, mAnimations.size()-1);
	mAnimMixer->add( AnimationClip( mAnimations[mAnimationIndex], 1.f-mAnimMixer->getTotalClipWeights() ));
	mAnimMixer->transition(mAnimations[mAnimationIndex].getName(), aduration );
	mAnimMixer->getAnimationClips().back().animation.reset();
	return true;
}

bool Model::transitionCurrentAnimation( const std::string& aTargetAnimName, float aduration ) {
	int tindex = getAnimationIndex(aTargetAnimName);
	if( tindex < 0 ) {
		ofLogWarning("ofx::assimp::Model::transitionCurrentAnimation") << " could not find animation with name" << aTargetAnimName;
		return false;
	}
	return transitionCurrentAnimation(tindex, aduration);
}

bool Model::hasAnimation( const std::string& aname ) {
	return getAnimationIndex(aname) > -1;
}

int Model::getAnimationIndex( const std::string& aname ) {
	int tindex = -1;
	for( int i = 0; i < (int)mAnimations.size(); i++ ) {
		if( mAnimations[i].getName() == aname ) {
			tindex = i;
			break;
		}
	}
	return tindex;
}

ofx::assimp::Animation& Model::getAnimation(int aindex) {
	if( mAnimations.size() < 1 ) {
		ofLogWarning("ofx::assimp::Model::getAnimation") << " no animations!!";
		return dummyAnimation;
	}
	aindex = ofClamp(aindex, 0, mAnimations.size()-1);
	return mAnimations[aindex];
}

ofx::assimp::Animation& Model::getAnimation(const std::string& aname) {
	int tindex = getAnimationIndex(aname);
	if( tindex < 0 ) {
		ofLogWarning("ofx::assimp::Model::getAnimation") << " could not find animation with name" << aname;
		return dummyAnimation;
	}
	return getAnimation(tindex);
}

bool Model::addAnimation( int aSrcAnimIndex, const std::string& aNewAnimName, float aStartTick, float aEndTick ) {
	return addAnimation( aSrcAnimIndex, aNewAnimName, aStartTick, aEndTick, OF_LOOP_NONE );
}

bool Model::addAnimation( int aSrcAnimIndex, const std::string& aNewAnimName, float aStartTick, float aEndTick, ofLoopType aLoopType ) {
	if( mAnimations.size() < 1 ) {
		ofLogWarning("ofx::assimp::Model::addAnimation") << " no animations!!";
		return false;
	}
	if(aSrcAnimIndex < 0 || aSrcAnimIndex >= mAnimations.size() ) {
		ofLogWarning("ofx::assimp::Model::addAnimation") << " aSrcAnimIndex out of range!!";
		return false;
	}
	auto& canim = mAnimations[aSrcAnimIndex];
	ofx::assimp::Animation nanim = mAnimations[aSrcAnimIndex];
	nanim.setup(aStartTick, aEndTick);
	nanim.setName(aNewAnimName);
	nanim.setLoopType(aLoopType);
	mAnimations.push_back(nanim);
	return true;
}

//void Model::playAllAnimations() {
//	for(size_t i = 0; i < mAnimations.size(); i++) {
//		mAnimations[i].play();
//	}
//}
//
//void Model::stopAllAnimations() {
//	for(size_t i = 0; i < mAnimations.size(); i++) {
//		mAnimations[i].stop();
//	}
//}
//
//void Model::resetAllAnimations() {
//	for(size_t i = 0; i < mAnimations.size(); i++) {
//		mAnimations[i].reset();
//	}
//}
//
//void Model::setPausedForAllAnimations(bool pause) {
//	for(size_t i = 0; i < mAnimations.size(); i++) {
//		mAnimations[i].setPaused(pause);
//	}
//}
//
//void Model::setLoopStateForAllAnimations(ofLoopType state) {
//	for(size_t i = 0; i < mAnimations.size(); i++) {
//		mAnimations[i].setLoopState(state);
//	}
//}
//
//void Model::setPositionForAllAnimations(float position) {
//	for(size_t i = 0; i < mAnimations.size(); i++) {
//		mAnimations[i].setPosition(position);
//	}
//}

//------------------------------------------- meshes.
bool Model::hasMeshes() {
	return mMeshes.size() > 0;
}

//-------------------------------------------
size_t Model::getNumMeshes(){
	return mMeshes.size();
}

//-------------------------------------------
vector<std::string> Model::getMeshNames(){
	if( mMeshes.size() < 1 ) {
		return std::vector<std::string>();
	}
	vector<string> names(mMeshes.size());
	for( size_t i = 0; i < mMeshes.size(); i++ ) {
		names[i] = mMeshes[i]->getName();
	}
	return names;
}

std::shared_ptr<ofx::assimp::Mesh> Model::getMesh(int meshIndex) {
	if( mMeshes.size() < 1 ) {
		return std::shared_ptr<ofx::assimp::Mesh>();
	}
	meshIndex = ofClamp(meshIndex, 0, mMeshes.size()-1);
	return mMeshes[meshIndex];
}

std::shared_ptr<ofx::assimp::Mesh> Model::getMesh(const std::string& aname) {
//	return getNodeAsType<ofx::assimp::Mesh>("*:"+aname);
	for( auto& mesh : mMeshes ) {
		if( mesh->getName() == aname ) {
			return mesh;
		}
	}
	return std::shared_ptr<ofx::assimp::Mesh>();
}

//-------------------------------------------
ofMesh& Model::getMeshForMesh( const std::string& aname ){
	auto helper = getMesh(aname);
	if( !helper ) {
		ofLogError("Model::getMesh : unable to find mesh with name") << aname << " returning dummy mesh";
		return dummyMesh;
	}
	return helper->getMesh();
}

//-------------------------------------------
ofMesh& Model::getMeshForMesh(unsigned int anum){
	auto helper = getMesh(anum);
	if( !helper ) {
		ofLogError("Model::getMesh : unable to find mesh with index") << anum << " returning dummy mesh";
		return dummyMesh;
	}
	return helper->getMesh();
}

//-------------------------------------------
ofMesh Model::getTransformedMeshForMesh(const std::string& aname) {
	auto helper = getMesh(aname);
	if( !helper ) {
		ofLogError("ofx::assimp::Model") << "getTransformedMesh(): unable to find mesh with name: " << aname;
		return dummyMesh;
	}
	return helper->getTransformedMesh();
}

//-------------------------------------------
ofMesh Model::getTransformedMeshForMesh(unsigned int anum) {
	auto helper = getMesh(anum);
	if( !helper ) {
		ofLogError("ofx::assimp::Model") << "getTransformedMesh(): unable to find mesh with index: " << anum;
		return dummyMesh;
	}
	return helper->getTransformedMesh();
}

//-------------------------------------------
std::shared_ptr<ofMaterial> Model::getMaterialForMesh(const std::string& aname){
	auto helper = getMesh(aname);
	if( !helper ) {
		ofLogError("ofx::assimp::Model") << "getMaterialForMesh(): couldn't find mesh: \"" + aname << "\"";
		return shared_ptr<ofMaterial>();
	}
	return helper->material;
}

//-------------------------------------------
std::shared_ptr<ofMaterial> Model::getMaterialForMesh(unsigned int anum){
	auto helper = getMesh(anum);
	if(!helper) {
		ofLogError("ofx::assimp::Model") << "getMaterialForMesh(): mesh id: " << anum << "out of range for total num meshes: " << mMeshes.size();
		return shared_ptr<ofMaterial>();
	}
	return helper->material;
}

//-------------------------------------------
ofTexture& Model::getTextureForMesh(const std::string& aname){
	auto helper = getMesh(aname);
	if( !helper ) {
		ofLogError("ofx::assimp::Model") << "getTextureForMesh(): couldn't find mesh: \"" << aname << "\"";
		return dummyTexture;
	}
	if(helper->hasTexture()) {
		return helper->getTexture();
	}
	ofLogError("ofx::assimp::Model") << "getTextureForMesh(): mesh: \"" << aname << "\" does not have a texture.";
	return dummyTexture;
}

//-------------------------------------------
ofTexture& Model::getTextureForMesh(unsigned int anum){
	auto helper = getMesh(anum);
	if(!helper) {
		ofLogError("ofx::assimp::Model") << "getTextureForMesh(): couldn't find mesh: \"" << anum << "\" / " << getNumMeshes();
		return dummyTexture;
	}
	if(helper->hasTexture()) {
		return helper->getTexture();
	}
	ofLogError("ofx::assimp::Model") << "getTextureForMesh(): mesh: \"" << anum << "\" does not have a texture.";
	return dummyTexture;
}

//-- Skeletons -----------------------------------------
bool Model::hasSkeletons() {
	return (mSkeletons.size() > 0);
}

//-------------------------------------------
size_t Model::getNumSkeletons() {
	return mSkeletons.size();
}

//-------------------------------------------
std::shared_ptr<ofx::assimp::Skeleton> Model::getSkeleton( const unsigned int& aindex ) {
	if( mSkeletons.size() < 1 ) {
		ofLogWarning("ofx::assimp::Model : no skeletons!!");
		return std::shared_ptr<ofx::assimp::Skeleton>();
	}
	return mSkeletons[ofClamp(aindex, 0, mSkeletons.size()-1)];
}

//-------------------------------------------
std::shared_ptr<ofx::assimp::Skeleton> Model::getSkeleton( const std::string& aname ) {
	for( auto skel : mSkeletons ) {
		if( skel->getName() == aname ) {
			return skel;
		}
	}
	return std::shared_ptr<ofx::assimp::Skeleton>();
}

//-------------------------------------------
std::vector< std::shared_ptr<ofx::assimp::Skeleton> > Model::getSkeletons() {
	return mSkeletons;
}

//-------------------------------------------
unsigned int Model::getNumBones() {
	unsigned int total = 0;
	for( auto skel : mSkeletons ) {
		total += skel->getAllNodesForType<ofx::assimp::Bone>().size();
	}
	return total;
}

//--------------------------------------------------------------
void Model::drawWireframe(){
	draw(OF_MESH_WIREFRAME);
}

//--------------------------------------------------------------
void Model::drawFaces(){
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void Model::drawVertices(){
	draw(OF_MESH_POINTS);
}

//--------------------------------------------------------------
void Model::drawBones() {
	for( auto& bone : mBones ) {
		bone->draw();
	}
}

//-------------------------------------------
void Model::enableCulling(int glCullType){
	mCullType = glCullType;
}

//-------------------------------------------
void Model::disableCulling(){
	mCullType = -1;
}

//-------------------------------------------
void Model::draw(ofPolyRenderMode renderType) {
	if(!mSrcScene) {
		return;
	}
	
//	transformGL();
//	for( auto& srcBone : mSrcBones ) {
//		srcBone->draw();
//	}
//	restoreTransformGL();
	ofPushStyle();

//	transformGL();
//	ofPushMatrix();
//	ofMultMatrix(modelMatrix);

#ifndef TARGET_OPENGLES
	glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
#endif

	for(size_t i = 0; i < mMeshes.size(); i++) {
		auto& mesh = mMeshes[i];

		mesh->transformGL();
//		ofPushMatrix();
//		ofMultMatrix(mesh.matrix);

		if(bUsingTextures && !bUsingMaterials){
			if(mesh->hasTexture()) {
				mesh->getTexture().bind();
			}
		}

		if(bUsingMaterials){
			mesh->material->begin();
		}

		//		this was broken / backwards
		if(!mesh->twoSided && mCullType >= 0) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(mCullType);
		}
		else {
			glDisable(GL_CULL_FACE);
		}
		
		
		ofEnableBlendMode(mesh->blendMode);

#ifndef TARGET_OPENGLES
		mesh->vbo->drawElements(GL_TRIANGLES,mesh->getNumIndices());
#else
		switch(renderType){
			case OF_MESH_FILL:
				mesh->vbo->drawElements(GL_TRIANGLES,mesh->getNumIndices());
				break;
			case OF_MESH_WIREFRAME:
				//note this won't look the same as on non ES renderers.
				//there is no easy way to convert GL_TRIANGLES to outlines for each triangle
				mesh->vbo->drawElements(GL_LINES,mesh->getNumIndices());
				break;
			case OF_MESH_POINTS:
				mesh->vbo->drawElements(GL_POINTS,mesh->getNumIndices());
				break;
		}
#endif

		if(bUsingTextures && !bUsingMaterials){
			if(mesh->hasTexture()) {
				mesh->getTexture().unbind();
			}
		}

		if(!mesh->twoSided) {
			glDisable(GL_CULL_FACE);
		}

		if(bUsingMaterials){
			mesh->material->end();
		}
		
		mesh->restoreTransformGL();
//		ofPopMatrix();
	}

#ifndef TARGET_OPENGLES
	//set the drawing mode back to FILL if its drawn the model with a different mode.
	if( renderType != OF_MESH_FILL ){
		glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(OF_MESH_FILL));
	}
#endif
	
//	ofPushMatrix();
//	ofTranslate(-200, 0, 0);
	
//	ofPopMatrix();

//	ofPopMatrix();
//	restoreTransformGL();
	ofPopStyle();
}

//-------------------------------------------
ofx::assimp::Bounds Model::getSceneBounds() {
	if( mBSceneBoundsDirty ) {
		mBSceneBoundsDirty=false;
		mSceneBoundsGlobal.clear();
//		mSceneBoundsGlobal.include( getGlobalTransformMatrix(), mSceneBoundsLocal.getBoundingVerts() );
//		mSceneBoundsGlobal.include( getGlobalTransformMatrix(), {mSceneBoundsLocal.min, mSceneBoundsLocal.max} );
		for( auto mesh : mMeshes ) {
			mSceneBoundsGlobal.include( mesh->getGlobalBounds().getBoundingVerts() );
		}
	}
	return mSceneBoundsGlobal;
}

//-------------------------------------------
ofx::assimp::Bounds Model::getSceneBoundsLocal() {
	return mSceneBoundsLocal;
}

//-------------------------------------------
std::shared_ptr<ofx::assimp::SrcScene> Model::getSrcScene() {
	return mSrcScene;
}

//-------------------------------------------
float Model::getNormalizedScale(){
	return normalizedScale;
}

////-------------------------------------------
//const aiScene* Model::getAssimpScene(){
//	return scene.get();
//}

//--------------------------------------------------------------
void Model::enableTextures(){
	bUsingTextures = true;
}

//--------------------------------------------------------------
void Model::enableNormals(){
	bUsingNormals = true;
}

//--------------------------------------------------------------
void Model::enableColors(){
	bUsingColors = true;
}

//--------------------------------------------------------------
void Model::enableMaterials(){
	bUsingMaterials = true;
}

//--------------------------------------------------------------
void Model::disableTextures(){
	bUsingTextures = false;
}

//--------------------------------------------------------------
void Model::disableNormals(){
	bUsingNormals = false;
}

//--------------------------------------------------------------
void Model::disableColors(){
	bUsingColors = false;
}

//--------------------------------------------------------------
void Model::disableMaterials(){
	bUsingMaterials = false;
}
