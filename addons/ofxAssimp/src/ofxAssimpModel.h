#pragma once

// ofDevCon
// Written by Anton Marini (http://vade.info)
// With massive help from Memo Akten for GL optimizing and pushing this faster than I expected
// Kyle McDonald and Arturo Castro for C++ nuances
// Lukasz Karluk additions Dec 2012.
// Nicholas Hardeman additions 2023.

// TODO:
// 1) Path issues - not all models:
//      a) reference images current working dir
//      b) properly describe sub-folders
//      c) reference absolute paths for images that dont exist.
// 2) convert to ofMesh (?) in OF 007 ?
// 3) Ability to ease *between* two animations. Maybe later folks.

#include "ofxAssimpMesh.h"
#include "ofxAssimpSrcScene.h"
#include "ofxAssimpSkeleton.h"

struct aiScene;
struct aiNode;

namespace ofx::assimp {

class Model : public ofx::assimp::Node {
public:
	
	~Model();
	Model();
	
	virtual NodeType getType() { return OFX_ASSIMP_MODEL; }
	
	//use the default OF selected flags ( from the options above ) or pass in the exact assimp flags you want
	//note: you will probably want to |= aiProcess_ConvertToLeftHanded to anything you pass in
	bool load(std::string aPathToFile, int assimpOptimizeFlags=OFX_ASSIMP_OPTIMIZE_DEFAULT);
	bool load(ofBuffer & buffer, int assimpOptimizeFlags=OFX_ASSIMP_OPTIMIZE_DEFAULT, const char * extension="");
	bool load( const ImportSettings& asettings );
	
	bool setup( std::shared_ptr<ofx::assimp::SrcScene> ascene );
	
	bool isLoaded();
	
	void clear();
	
	std::string getHierarchyAsString();
	
//	void createLightsFromAiModel();
	
	
	// -- update ---------------------------------------
	void earlyUpdate();
	void update();
	void lateUpdate();
	
	void flagSceneDirty();
	
	
	// -- animations ---------------------------------------
	bool hasAnimations();
	unsigned int getNumAnimations();
	unsigned int getCurrentAnimationIndex();
	ofx::assimp::Animation& getCurrentAnimation();
	bool setCurrentAnimation( int aindex );
	bool setCurrentAnimation( const std::string& aname );
	
	bool transitionCurrentAnimation( int aTargetAnimIndex, float aduration );
	bool transitionCurrentAnimation( const std::string& aTargetAnimName, float aduration );
	
	bool hasAnimation( const std::string& aname );
	int getAnimationIndex( const std::string& aname );
	ofx::assimp::Animation& getAnimation(int aindex);
	ofx::assimp::Animation& getAnimation(const std::string& aname);
	
	bool addAnimation( int aSrcAnimIndex, const std::string& aNewAnimName, float aStartTick, float aEndTick );
	bool addAnimation( int aSrcAnimIndex, const std::string& aNewAnimName, float aStartTick, float aEndTick, ofLoopType aLoopType );
	
//	void playAllAnimations();
//	void stopAllAnimations();
//	void resetAllAnimations();
//	void setPausedForAllAnimations(bool pause);
//	void setLoopStateForAllAnimations(ofLoopType state);
//	void setPositionForAllAnimations(float position);
	
	
	// -- meshes ---------------------------------------
	size_t getNumVertices();
	bool hasMeshes();
	size_t getNumMeshes();
	std::vector<std::string> getMeshNames();
	std::shared_ptr<ofx::assimp::Mesh> getMesh(int meshIndex);
	std::shared_ptr<ofx::assimp::Mesh> getMesh(const std::string& aname);
	std::vector< std::shared_ptr<ofx::assimp::Mesh> > getMeshes() {return mMeshes;}
	
	ofMesh& getMeshForMesh(const std::string& aname);
	ofMesh& getMeshForMesh(unsigned int anum);
	
	ofMesh getTransformedMeshForMesh(const std::string& aname);
	ofMesh getTransformedMeshForMesh(unsigned int anum);
	
	std::shared_ptr<ofMaterial> getMaterialForMesh(const std::string& aname);
	std::shared_ptr<ofMaterial> getMaterialForMesh(unsigned int anum);
	
	ofTexture& getTextureForMesh(const std::string& aname);
	ofTexture& getTextureForMesh(unsigned int anum);
	
	
	// -- skeletons / bones ---------------------------------------
	bool hasSkeletons();
	size_t getNumSkeletons();
	std::shared_ptr<ofx::assimp::Skeleton> getSkeleton( const unsigned int& aindex );
	std::shared_ptr<ofx::assimp::Skeleton> getSkeleton( const std::string& aname );
	std::vector< std::shared_ptr<ofx::assimp::Skeleton> > getSkeletons();
	unsigned int getNumBones();
	
	
	// -- draw ---------------------------------------
	void draw();
	void draw(ofPolyRenderMode renderType);
	void drawWireframe();
	void drawFaces();
	void drawVertices();
	void drawBones();
	void drawBones(float aAxisSize);
	
	
	// -- render settings ---------------------------------------
	void enableCulling(int glCullType); // GL_CW, GL_CCW
	void disableCulling();
	
	void enableTextures();
	void disableTextures();
	void enableNormals();
	void enableMaterials();
	void disableNormals();
	void enableColors();
	void disableColors();
	void disableMaterials();
	
	
	// -- scaling ---------------------------------------
	void centerAndScaleToWindow();
	void centerAndScaleToViewRect( const ofRectangle& arect );
	void centerAndScaleToViewRect( const ofRectangle& arect, float aNormalizeFactor);
	void centerAndScaleToViewRect( float awidth, float aheight );
	void centerAndScaleToViewRect( float ax, float ay, float awidth, float aheight );
	void centerAndScaleToViewRect( float ax, float ay, float awidth, float aheight, float aNormalizeFactor);
	
	float getNormalizedScale();
	float getNormalizedFactor(){ return normalizeFactor;}
	
	
	// -- bounds ---------------------------------------
	ofx::assimp::Bounds getSceneBounds();
	ofx::assimp::Bounds getSceneBoundsLocal();
	std::shared_ptr<ofx::assimp::SrcScene> getSrcScene();
	void calculateDimensions();
	
	
protected:
	void updateAnimations();
	void updateMeshesFromBones();
	
	bool processScene();
	void processSceneNodesRecursive( std::shared_ptr<ofx::assimp::SrcNode> aSrcNode, std::shared_ptr<ofx::assimp::Node> aParentNode );
	
	// updates the *actual GL resources* for the current animation
	void updateGLResources();
	
	double normalizedScale = 1.0;
	
	// stores all of the resources
	std::shared_ptr<ofx::assimp::SrcScene> mSrcScene;
	
	std::vector< std::shared_ptr<ofx::assimp::Mesh> > mMeshes;
	std::vector< std::shared_ptr<ofx::assimp::Bone> > mBones;
	// the skeletons are the root bones
	std::vector< std::shared_ptr<ofx::assimp::Skeleton> > mSkeletons;
	std::vector< std::shared_ptr<ofx::assimp::Node> > mNullNodes;
	std::vector<ofx::assimp::Animation> mAnimations;
	
	int mAnimationIndex = 0;
	
	int mCullType = -1;
	
	bool mBSceneDirty = false;
	
	bool bUsingTextures;
	bool bUsingNormals;
	bool bUsingColors;
	bool bUsingMaterials;
	float normalizeFactor;
	
	bool bLoadedSrcScene = false;
	bool bProcessedSceneSuccessfully = false;
	
	bool mBSceneBoundsDirty=true;
	ofx::assimp::Bounds mSceneBoundsGlobal;
	ofx::assimp::Bounds mSceneBoundsLocal;
	
	ofMesh dummyMesh;
	ofx::assimp::Animation dummyAnimation;
	ofTexture dummyTexture;
};
}
