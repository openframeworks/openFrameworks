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

namespace ofxAssimp {
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
	bool transformRootNode = true; // orient based on src scene root node, helps with correct orientation
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
	
	std::vector< std::shared_ptr<ofxAssimp::SrcNode> > getRootNodes() { return mSrcNodes; }
	
	std::vector< ofxAssimp::Animation >& getAnimations() {return mAnimations;}
	
	ImportSettings getImportSettings() { return mSettings; }
	
	const aiScene* getAiScenePtr() { return scene.get(); }
	std::shared_ptr<const aiScene> getAiScene() { return scene; }
	
protected:
	
	// ai scene setup
	unsigned int initImportProperties(int assimpOptimizeFlags, const ImportSettings& asettings );
	bool processScene(const ImportSettings& asettings);
	void printAllNodeNames( aiNode* anode, int alevel );
	void processNodes();
	void processNodesRecursive(aiNode* anode, std::shared_ptr<SrcNode> aParentNode, std::unordered_map<std::string, aiBone*>& aBoneMap);
	void processMeshes(aiNode* anode, std::shared_ptr<SrcNode> aSrcNode);
	
	void processBones();
	bool isBone( aiNode* aAiNode, const std::unordered_map<std::string, aiBone*>& aBoneMap );
	bool isArmature( aiNode* aAiNode, const std::unordered_map<std::string, aiBone*>& aBoneMap );
	bool isRootBone( aiNode* aAiNode, std::unordered_map<std::string, aiBone*>& aBoneMap );
	aiBone* getAiBoneForAiNode( aiNode* aAiNode, std::unordered_map<std::string, aiBone*>& aBoneMap );
	
	std::shared_ptr<ofxAssimp::SrcNode> getSrcNodeForAiNodeName( const std::string& aAiNodeName );
	
	void processLights();
	
	void processAnimations();
	void processKeyframes( std::shared_ptr<ofxAssimp::SrcNode> aSrcNode, aiNodeAnim* aNodeAnim, unsigned int aAnimIndex );
	
	// Initial VBO creation, etc
	void loadGLResources(std::shared_ptr<ofxAssimp::SrcMesh> aSrcMesh, aiMesh* amesh);
	
	ofFile mFile;
	
	std::vector<ofLight> mLights;
//	std::map<of::filesystem::path, std::shared_ptr<ofxAssimp::Texture> > mAssimpTextures;
	std::map<std::string, std::shared_ptr<ofxAssimp::Texture> > mAssimpTextures;
	std::map<of::filesystem::path, std::shared_ptr<ofTexture> > mTextureCacheMap;
	
	std::vector< ofxAssimp::Animation > mAnimations;
	
	std::vector< std::shared_ptr<ofxAssimp::SrcMesh> > mSrcMeshes;
	std::vector< std::shared_ptr<ofxAssimp::SrcNode> > mSrcNodes;
	
	ImportSettings mSettings;
	
	//new C++ api
	Assimp::Importer importer;
	
	// the main Asset Import scene that does the magic.
	std::shared_ptr<const aiScene> scene;
//	std::shared_ptr<aiPropertyStore> store;
	
};
}
