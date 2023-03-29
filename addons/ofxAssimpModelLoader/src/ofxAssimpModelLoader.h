#pragma once

// ofDevCon
// Written by Anton Marini (http://vade.info)
// With massive help from Memo Akten for GL optimizing and pushing this faster than I expected
// Kyle McDonald and Arturo Castro for C++ nuances
// Lukasz Karluk additions Dec 2012.

// TODO:
// 1) Path issues - not all models:
//      a) reference images current working dir
//      b) properly describe sub-folders
//      c) reference absolute paths for images that dont exist.
// 2) convert to ofMesh (?) in OF 007 ?
// 3) Ability to ease *between* two animations. Maybe later folks.

#include "ofxAssimpMeshHelper.h"
#include "ofxAssimpAnimation.h"
#include "ofxAssimpTexture.h"
#include "ofMesh.h"
#include "ofMath.h"
#include "ofConstants.h"
#include <assimp/Importer.hpp>
#include <unordered_map>
#include <map>

struct aiScene;
struct aiNode;

class ofxAssimpModelLoader{

	public:

		//to pass into the load function use this syntax: ofxAssimpModelLoader::OPTIMIZE_DEFAULT
		//Note these are negative as we want to let users pass in assimp flags directly if they want to
		enum Flags{
			OPTIMIZE_NONE =-3,
			OPTIMIZE_DEFAULT =-2,
			OPTIMIZE_HIGH =-1
		};

		~ofxAssimpModelLoader();
		ofxAssimpModelLoader();

		//use the default OF selected flags ( from the options above ) or pass in the exact assimp flags you want
		//note: you will probably want to |= aiProcess_ConvertToLeftHanded to anything you pass in
		bool load(std::string modelName, int assimpOptimizeFlags=OPTIMIZE_DEFAULT);
		bool load(ofBuffer & buffer, int assimpOptimizeFlags=OPTIMIZE_DEFAULT, const char * extension="");

		OF_DEPRECATED_MSG("ofxAssimpModelLoader::load(std::string modelName, bool bOptimize) is deprecated, use load(std::string modelName, int assimpOptimizeFlags) instead.", bool load(std::string modelName, bool optimize));
		OF_DEPRECATED_MSG("ofxAssimpModelLoader::load(ofBuffer & buffer, bool optimize=false, const char * extension="") is deprecated, use load(std::string modelName, int assimpOptimizeFlags) instead.", bool load(ofBuffer & buffer, bool optimize, const char * extension));

		OF_DEPRECATED_MSG("ofxAssimpModelLoader::loadModel() is deprecated, use load() instead.", bool loadModel(std::string modelName, bool optimize=false));
		OF_DEPRECATED_MSG("ofxAssimpModelLoader::loadModel() is deprecated, use load() instead.", bool loadModel(ofBuffer & buffer, bool optimize=false, const char * extension=""));

		void createEmptyModel();
		void createLightsFromAiModel();
		void optimizeScene();

		// GL_CW, GL_CCW
		void enableCulling(int glCullType);
		void disableCulling();

		void update();

		bool hasAnimations();
		unsigned int getAnimationCount();
		ofxAssimpAnimation & getAnimation(int animationIndex);
		void playAllAnimations();
		void stopAllAnimations();
		void resetAllAnimations();
		void setPausedForAllAnimations(bool pause);
		void setLoopStateForAllAnimations(ofLoopType state);
		void setPositionForAllAnimations(float position);
		OF_DEPRECATED_MSG("Use ofxAssimpAnimation instead", void setAnimation(int animationIndex));
		OF_DEPRECATED_MSG("Use ofxAssimpAnimation instead", void setNormalizedTime(float time));
		OF_DEPRECATED_MSG("Use ofxAssimpAnimation instead", void setTime(float time));
		OF_DEPRECATED_MSG("Use ofxAssimpAnimation instead", float getDuration(int animationIndex));

		bool hasMeshes();
		unsigned int getMeshCount();
		ofxAssimpMeshHelper & getMeshHelper(int meshIndex);

		void clear();

		void setScale(float x, float y, float z);
		void setPosition(float x, float y, float z);
		void setRotation(int which, float angle, float rot_x, float rot_y, float r_z);

		// Scale the model to the screen automatically.
		void setScaleNormalization(bool normalize);
		void setNormalizationFactor(float factor);

		std::vector<std::string> getMeshNames();
		unsigned int getNumMeshes();

		ofMesh getMesh(std::string name);
		ofMesh getMesh(unsigned int num);

		ofMesh getCurrentAnimatedMesh(std::string name);
		ofMesh getCurrentAnimatedMesh(unsigned int num);

		ofMaterial getMaterialForMesh(std::string name);
		ofMaterial getMaterialForMesh(unsigned int num);

		ofTexture getTextureForMesh(std::string name);
		ofTexture getTextureForMesh(unsigned int num);


		void drawWireframe();
		void drawFaces();
		void drawVertices();

		void enableTextures();
		void disableTextures();
		void enableNormals();
		void enableMaterials();
		void disableNormals();
		void enableColors();
		void disableColors();
		void disableMaterials();

		void draw(ofPolyRenderMode renderType);

		glm::vec3 getPosition();
		glm::vec3 getSceneCenter();
		float getNormalizedScale();
		glm::vec3 getScale();
		glm::mat4 getModelMatrix();

		//these provide the raw scene information with scaling applied from the setScale command not the normalized scale or other transforms
		//not super useful but leaving as is for legacy usage
		glm::vec3 getSceneMin(bool bScaled = false);
		glm::vec3 getSceneMax(bool bScaled = false);

		//this should allow for drawing bounds that enclose the model being drawn with ofxAssimpModelLoader::drawFaces
		glm::vec3 getSceneMinModelSpace();
		glm::vec3 getSceneMaxModelSpace();
		glm::vec3 getSceneCenterModelSpace();

		int	getNumRotations();	// returns the no. of applied rotations
		glm::vec3 getRotationAxis(int which); // gets each rotation axis
		float getRotationAngle(int which); //gets each rotation angle

		void calculateDimensions();

		const aiScene * getAssimpScene();

	protected:
		void updateAnimations();
		void updateMeshes(aiNode * node, glm::mat4 parentMatrix);
		void updateBones();
		void updateModelMatrix();

		// ai scene setup
		unsigned int initImportProperties(int assimpOptimizeFlags);
		bool processScene();

		// Initial VBO creation, etc
		void loadGLResources();

		// updates the *actual GL resources* for the current animation
		void updateGLResources();

		void getBoundingBoxWithMinVector( aiVector3D* min, aiVector3D* max);
		void getBoundingBoxForNode(const ofxAssimpMeshHelper & mesh,  aiVector3D* min, aiVector3D* max);

		ofFile file;

		aiVector3D scene_min, scene_max, scene_center;

		bool normalizeScale;
		double normalizedScale;

		std::vector<float> rotAngle;
		std::vector<glm::vec3> rotAxis;
		glm::vec3 scale {1.0,1.0,1.0};
		glm::vec3 pos {0.0,0.0,0.0};
		glm::mat4 modelMatrix; // { glm::mat4() }

		std::vector<ofLight> lights;
		std::map<
			of::filesystem::path,
			std::shared_ptr<ofTexture>
		> textures;
		std::vector<ofxAssimpMeshHelper> modelMeshes;
		std::vector<ofxAssimpAnimation> animations;
		int currentAnimation; // DEPRECATED - to be removed with deprecated animation functions.

		int mCullType = -1;

		bool bUsingTextures;
		bool bUsingNormals;
		bool bUsingColors;
		bool bUsingMaterials;
		float normalizeFactor;

		//new C++ api
		Assimp::Importer importer;

		// the main Asset Import scene that does the magic.
		std::shared_ptr<const aiScene> scene;
		std::shared_ptr<aiPropertyStore> store;
};
