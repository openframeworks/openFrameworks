#pragma once

// based on the original ofxAssimpModelLoader
// ofDevCon
// Written by Anton Marini (http://vade.info)
// With massive help from Memo Akten for GL optimizing and pushing this faster than I expected
// Kyle McDonald and Arturo Castro for C++ nuances
// Lukasz Karluk additions Dec 2012.


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
	
	/// \brief Load a model from disk.
	/// \param aPathToFile path to the file to load.
	/// \param assimpOptimizeFlags optimization flags, OFX_ASSIMP_OPTIMIZE_NONE, OFX_ASSIMP_OPTIMIZE_DEFAULT, OFX_ASSIMP_OPTIMIZE_HIGH.
	/// \return True if the scene is loaded and processed successfully.
	bool load(std::string aPathToFile, int assimpOptimizeFlags=OFX_ASSIMP_OPTIMIZE_DEFAULT);
	/// \brief Load a model from buffer.
	/// \param buffer path to the file to load.
	/// \param assimpOptimizeFlags optimization flags, OFX_ASSIMP_OPTIMIZE_NONE, OFX_ASSIMP_OPTIMIZE_DEFAULT, OFX_ASSIMP_OPTIMIZE_HIGH.
	/// \return True if the scene is loaded and processed successfully.
	bool load(ofBuffer & buffer, int assimpOptimizeFlags=OFX_ASSIMP_OPTIMIZE_DEFAULT, const char * extension="");
	/// \brief Load a model from settings.
	/// \param asettings Import settings.
	/// \return True if the scene is loaded and processed successfully.
	bool load( const ImportSettings& asettings );
	/// \brief Setup a model from a SrcScene.
	/// \param ascene shared ptr of a SrcScene setup model with.
	/// \return True if the scene is processed successfully.
	bool setup( std::shared_ptr<ofx::assimp::SrcScene> ascene );
	
	/// \brief Check to see if the scene is valid and loaded.
	/// \return If the scene has been loaded or setup successfully.
	bool isLoaded();
	
	void clear();
	/// \brief Helpful for debugging.
	/// \return The node hierararchy as string.
	std::string getHierarchyAsString();
	
//	void createLightsFromAiModel();
	
	
	// -- update ---------------------------------------
	/// \brief Update bones and meshes based on imported animation. Must call lateUpdate after.
	void earlyUpdate();
	/// \brief Internally calls both earlyUpdate() and then lateUpdate().
	void update();
	/// \brief Update meshes from bones.
	void lateUpdate();
	/// \brief Force updating of meshes. Helpful if animations are disabled and bones are manipulated externally.
	void flagSceneDirty();
	
	
	// -- animations ---------------------------------------
	/// \brief Does the scene have animations.
	/// \return true if the scene has at least 1 animation.
	bool hasAnimations();
	/// \brief The total number of animations.
	unsigned int getNumAnimations();
	/// \brief The current active animation that is influencing the bones / meshes.
	/// \return The vector index of the current animation.
	unsigned int getCurrentAnimationIndex();
	/// \brief The current active animation that is influencing the bones / meshes.
	/// \return The current animation.
	ofx::assimp::Animation& getCurrentAnimation();
	/// \brief Immediately sets the current active animation that is influencing the bones / meshes.
	/// \param aindex vector index of the desired animation.
	/// \return True if the current index is not the same as aindex and there is at least 1 animation.
	bool setCurrentAnimation( int aindex );
	/// \brief Immediately sets the current active animation that is influencing the bones / meshes.
	/// \param aname name of the desired animation.
	/// \return True if the current animation is not the same as the desired animation and there is at least 1 animation.
	bool setCurrentAnimation( const std::string& aname );
	/// \brief Transition from the current animation to another animation with set duration.
	/// \param aTargetAnimIndex vector index of the desired animation.
	/// \param aduration duration of the transition.
	/// \return True if there is at least 1 animation.
	bool transitionCurrentAnimation( int aTargetAnimIndex, float aduration );
	/// \brief Transition from the current animation to another animation with set duration.
	/// \param aTargetAnimName name of the desired animation.
	/// \param aduration duration of the transition.
	/// \return True if there is at least 1 animation and the animation is found by the desired name.
	bool transitionCurrentAnimation( const std::string& aTargetAnimName, float aduration );
	/// \brief Check if the animation exists.
	/// \param aname name of the animation to check.
	/// \return True if the animation is found by the desired name.
	bool hasAnimation( const std::string& aname );
	/// \brief Get the vector index of an animation by name.
	/// \param aname name of the animation to find.
	/// \return Vector index of the animation if found, -1 if not found.
	int getAnimationIndex( const std::string& aname );
	/// \brief Get an animation by vector index.
	/// \param aindex vector index of animation to retrieve.
	/// \return Animation if found, dummy animation if no animations.
	ofx::assimp::Animation& getAnimation(int aindex);
	/// \brief Get an animation by aname.
	/// \param aname name of the animation to retrieve.
	/// \return Animation if found, dummy animation if no animations.
	ofx::assimp::Animation& getAnimation(const std::string& aname);
	/// \brief Add a new animation based on an existing animation.
	/// \param aSrcAnimIndex name of the animation to copy.
	/// \param aNewAnimName name of the new animation.
	/// \param aStartTick frame/tick of the source animation to use as the first frame of the new animation.
	/// \param aEndTick frame/tick of the source animation to use as the last frame of the new animation.
	/// \return true if a valid source animation index is used.
	bool addAnimation( int aSrcAnimIndex, const std::string& aNewAnimName, float aStartTick, float aEndTick );
	/// \brief Add a new animation based on an existing animation.
	/// \param aSrcAnimIndex name of the animation to copy.
	/// \param aNewAnimName name of the new animation.
	/// \param aStartTick frame/tick of the source animation to use as the first frame of the new animation.
	/// \param aEndTick frame/tick of the source animation to use as the last frame of the new animation.
	/// \param aLoopType set the loop type of the new animation.
	/// \return true if a valid source animation index is used.
	bool addAnimation( int aSrcAnimIndex, const std::string& aNewAnimName, float aStartTick, float aEndTick, ofLoopType aLoopType );
	
//	void playAllAnimations();
//	void stopAllAnimations();
//	void resetAllAnimations();
//	void setPausedForAllAnimations(bool pause);
//	void setLoopStateForAllAnimations(ofLoopType state);
//	void setPositionForAllAnimations(float position);
	
	
	// -- meshes ---------------------------------------
	/// \brief The number of vertices in the model.
	/// \return The total number of vertices from all meshes in the model.
	size_t getNumVertices();
	/// \brief Does the model have meshes.
	/// \return true If the model has at least one mesh.
	bool hasMeshes();
	/// \brief Total number of meshes.
	/// \return The number of meshes in the model.
	size_t getNumMeshes();
	/// \brief The names of the meshes in the model.
	/// \return A vector<string> populated with the names of the meshes in the model.
	std::vector<std::string> getMeshNames();
	/// \brief Get a mesh based on index. Clamps to valid index range.
	/// \param meshIndex vector index of the desired mesh.
	/// \return ofx::assimp::Mesh as a shared_ptr. Valid ptr if at least one mesh.
	std::shared_ptr<ofx::assimp::Mesh> getMesh(int meshIndex);
	/// \brief Get a mesh based on name.
	/// \param aname name of the desired mesh.
	/// \return ofx::assimp::Mesh as a shared_ptr. Valid ptr if mesh was found by name.
	std::shared_ptr<ofx::assimp::Mesh> getMesh(const std::string& aname);
	/// \brief Get all the meshes in the model.
	/// \return vector of ofx::assimp::Mesh as shared_ptr.
	std::vector< std::shared_ptr<ofx::assimp::Mesh> > getMeshes() {return mMeshes;}
	/// \brief Get an OF mesh based on name of assimp mesh.
	/// \param aname name of the desired assimp mesh.
	/// \return ofMesh that is not transformed.
	ofMesh& getOFMeshForMesh(const std::string& aname);
	/// \brief Get an OF mesh based on the vector index of the assimp mesh.
	/// \param ameshIndex vector index of the assimp mesh.
	/// \return ofMesh that is not transformed.
	ofMesh& getOFMeshForMesh(unsigned int ameshIndex);
	/// \brief Get a transformed OF mesh based on name of assimp mesh.
	/// \param aname name of the desired assimp mesh.
	/// \return Transformed ofMesh.
	ofMesh getTransformedOFMeshForMesh(const std::string& aname);
	/// \brief Get a transformed OF mesh based on the vector index of the assimp mesh.
	/// \param ameshIndex vector index of the assimp mesh.
	/// \return Transformed ofMesh.
	ofMesh getTransformedOFMeshForMesh(unsigned int ameshIndex);
	/// \brief Get an ofMaterial from an assimp mesh.
	/// \param aname name of the desired assimp mesh.
	/// \return ofMaterial that is applied to the assimp mesh.
	std::shared_ptr<ofMaterial> getMaterialForMesh(const std::string& aname);
	/// \brief Get an ofMaterial from an assimp mesh.
	/// \param ameshIndex vector index of the assimp mesh.
	/// \return ofMaterial that is applied to the assimp mesh.
	std::shared_ptr<ofMaterial> getMaterialForMesh(unsigned int ameshIndex);
	/// \brief Get the first valid ofTexture from an assimp mesh (starts with diffuse).\nWill return dummy texture if there is no texture found on the mesh.
	/// \param aname name of the desired assimp mesh.
	/// \return First valid ofTexture on the assimp mesh, if found.
	ofTexture& getTextureForMesh(const std::string& aname);
	/// \brief Get the first valid ofTexture from an assimp mesh (starts with diffuse).\nWill return dummy texture if there is no texture found on the mesh.
	/// \param ameshIndex vector index of the assimp mesh.
	/// \return First valid ofTexture on the assimp mesh, if found.
	ofTexture& getTextureForMesh(unsigned int ameshIndex);
	
	
	// -- skeletons / bones ---------------------------------------
	/// \brief Does the model have skeletons.
	/// \return true If the model has at least one skeleton.
	bool hasSkeletons();
	/// \brief Total number of skeletons.
	/// \return The number of skeletons in the model.
	size_t getNumSkeletons();
	/// \brief Get a skeleton based on index. Clamps to valid index range.
	/// \param aindex vector index of the desired skeleton.
	/// \return ofx::assimp::Skeleton as a shared_ptr. Valid ptr if at least one skeleton.
	std::shared_ptr<ofx::assimp::Skeleton> getSkeleton( const unsigned int& aindex );
	/// \brief Get a skeleton based on aname.
	/// \param aname name of the desired skeleton.
	/// \return ofx::assimp::Skeleton as a shared_ptr. Valid ptr if skeleton was found by name.
	std::shared_ptr<ofx::assimp::Skeleton> getSkeleton( const std::string& aname );
	/// \brief Get all the skeletons in the model.
	/// \return vector of ofx::assimp::Skeleton as shared_ptr.
	std::vector< std::shared_ptr<ofx::assimp::Skeleton> > getSkeletons();
	/// \brief Total number of bones in the model.
	/// \return The number of bones.
	unsigned int getNumBones();
	
	
	// -- draw ---------------------------------------
	void draw();
	void draw(ofPolyRenderMode renderType);
	void drawWireframe();
	void drawFaces();
	void drawVertices();
	
	void _drawMesh(const std::shared_ptr<ofx::assimp::Mesh>& amesh, ofPolyRenderMode aRenderType, bool bSetRenderType );
	void drawMesh(const std::shared_ptr<ofx::assimp::Mesh>& amesh, ofPolyRenderMode aRenderType=OF_MESH_FILL );
	void drawMesh(int aMeshIndex, ofPolyRenderMode aRenderType=OF_MESH_FILL );
	void drawMesh(const std::string& aMeshName, ofPolyRenderMode aRenderType=OF_MESH_FILL );
	
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
