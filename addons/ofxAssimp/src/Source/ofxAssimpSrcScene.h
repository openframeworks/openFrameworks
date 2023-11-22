//
//  ofxAssimpSrcScene.h
//  Created by Nick Hardeman on 10/24/23.
//

#pragma once

#include "ofxAssimpSrcMesh.h"
#include "ofxAssimpAnimation.h"
#include "ofxAssimpTexture.h"
#include "ofxAssimpSrcBone.h"
#include "ofMath.h"
#include "ofConstants.h"
#include <assimp/Importer.hpp>
#include <unordered_map>
#include <map>
#include "ofFileUtils.h"

struct aiScene;
struct aiNode;

// This class loads and stores all of the resources used by a ofxAssimpModel
// It is not intended for rendering

namespace ofx::assimp {
//to pass into the load function use this syntax: ofxAssimpModelLoader::OPTIMIZE_DEFAULT
//Note these are negative as we want to let users pass in assimp flags directly if they want to
enum ImportFlags{
	OFX_ASSIMP_OPTIMIZE_NONE =-3,
	OFX_ASSIMP_OPTIMIZE_DEFAULT =-2,
	OFX_ASSIMP_OPTIMIZE_HIGH =-1
};

struct ImportSettings {
	of::filesystem::path filePath;
	int assimpOptimizeFlags=OFX_ASSIMP_OPTIMIZE_DEFAULT;
	bool importAsTex2d = true;
	bool importTextures = true;
	bool importAnimations = true;
	bool importMaterials = true;
	bool fixInfacingNormals = false; // aiProcess_FixInfacingNormals
//	bool importLights = false;
	bool convertToLeftHanded = true; // aiProcess_ConvertToLeftHanded
	
	std::vector<std::string> excludeNodesContainingStrings;
	unsigned int aiFlags = 0; // ai process flags, ie. aiProcess_FixInfacingNormals
};

class SrcScene {
public:
	
	SrcScene();
	~SrcScene();
	
	//use the default OF selected flags ( from the options above ) or pass in the exact assimp flags you want
	//note: you will probably want to |= aiProcess_ConvertToLeftHanded to anything you pass in
	bool load(std::string aPathToFile, int assimpOptimizeFlags=OFX_ASSIMP_OPTIMIZE_DEFAULT);
	bool load(ofBuffer & buffer, int assimpOptimizeFlags=OFX_ASSIMP_OPTIMIZE_DEFAULT, const char * extension="");
	bool load( const ImportSettings& asettings );
	void optimizeScene();
	void clear();
	
	std::vector< std::shared_ptr<ofx::assimp::SrcNode> > getRootNodes() { return mSrcNodes; }
	
	std::vector< ofx::assimp::Animation >& getAnimations() {return mAnimations;}
	
	ImportSettings getImportSettings() { return mSettings; }
	
	const aiScene* getAiScenePtr() { return scene.get(); }
	std::shared_ptr<const aiScene> getAiScene() { return scene; }
	
protected:
	
	// ai scene setup
	unsigned int initImportProperties(int assimpOptimizeFlags, const ImportSettings& asettings );
	bool processScene(const ImportSettings& asettings);
	void printAllNodeNames( aiNode* anode, int alevel );
	void processNodes();
	void processNodesRecursive(aiNode* anode, std::shared_ptr<SrcNode> aParentNode);
	void processMeshes(aiNode* anode, std::shared_ptr<SrcNode> aSrcNode);
	
	void processBones();
	bool isBone( aiNode* aAiNode );
	bool isArmature( aiNode* aAiNode );
	bool isRootBone( aiNode* aAiNode );
	aiBone* getAiBoneForAiNode( aiNode* aAiNode );
	void recursiveAddSrcBones( std::shared_ptr<ofx::assimp::SrcBone> abone );
	
	std::shared_ptr<ofx::assimp::SrcNode> getSrcNodeForAiNodeName( const std::string& aAiNodeName );
	
	void processLights();
	
	void processAnimations();
	void processKeyframes( std::shared_ptr<ofx::assimp::SrcNode> aSrcNode, aiNodeAnim* aNodeAnim, unsigned int aAnimIndex );
	
	// Initial VBO creation, etc
	void loadGLResources(std::shared_ptr<ofx::assimp::SrcMesh> aSrcMesh, aiMesh* amesh);
	
	ofFile mFile;
	
	std::vector<ofLight> mLights;
	std::map<of::filesystem::path, std::shared_ptr<ofx::assimp::Texture> > mAssimpTextures;
	std::vector< ofx::assimp::Animation > mAnimations;
	
	std::vector< std::shared_ptr<ofx::assimp::SrcMesh> > mSrcMeshes;
	std::vector< std::shared_ptr<ofx::assimp::SrcNode> > mSrcNodes;
	
	ImportSettings mSettings;
	
	//new C++ api
	Assimp::Importer importer;
	
	// the main Asset Import scene that does the magic.
	std::shared_ptr<const aiScene> scene;
	std::shared_ptr<aiPropertyStore> store;
	
//	ofx::assimp::Bounds mSceneBoundsLocal;
	
};
}
