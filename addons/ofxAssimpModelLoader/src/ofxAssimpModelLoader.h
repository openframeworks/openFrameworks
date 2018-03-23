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
#include "ofPoint.h"
#include "ofMain.h"

struct aiScene;
struct aiNode;

class ofxAssimpModelLoader{

    public:
        ~ofxAssimpModelLoader();
        ofxAssimpModelLoader();

        // Pre-loads model from file or url into a buffer but does not initialize OpenGL resources (for load in background thread)
        bool preLoadModel(string modelName, bool optimize=false);
        // Pre-loads all model's textures without creating any GL resource
        void preloadTextures();
        // Pre-loads texture from texURL (file or URL) with texPath identifier inside the model.
        void preloadTexture(string texURL, string texPath);

        // Loads model's OpenGL resoruces
        bool loadModelGLResources();
        // Reloads model's OpenGL Resources (Textures)
        bool reloadModelGLResources();

		bool loadModel(std::string modelName, bool optimize=false);
        bool loadModel(ofBuffer & buffer, bool optimize=false, const char * extension="");
		bool loadTexturesForMaterial(aiMaterial* mtl, ofxAssimpMeshHelper &meshHelper);
        void createEmptyModel();
        void createLightsFromAiModel();
        void optimizeScene();

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
		
		ofPoint getPosition();
		ofPoint getSceneCenter();
		float getNormalizedScale();
		ofPoint getScale();
        ofMatrix4x4 getModelMatrix();

		ofPoint getSceneMin(bool bScaled = false);
		ofPoint	getSceneMax(bool bScaled = false);
						
		int	getNumRotations();	// returns the no. of applied rotations
		ofPoint	getRotationAxis(int which); // gets each rotation axis
		float getRotationAngle(int which); //gets each rotation angle

        void calculateDimensions();

		const aiScene * getAssimpScene();
         
    protected:
        void onAppExit(ofEventArgs & args);
        void updateAnimations();
        void updateMeshes(aiNode * node, ofMatrix4x4 parentMatrix);
        void updateBones();
        void updateModelMatrix();

        bool reloadModelTextures();
        bool reloadModelVBOs();

        // ai scene setup
        unsigned int initImportProperties(bool optimize);
        bool processScene();
		void resetScene();

        // Initial VBO creation, etc
        void loadGLResources();
    
        // updates the *actual GL resources* for the current animation
        void updateGLResources();

		// Imports the model from a file path
		void importModelFromAssimp(const char * filename, bool optimize);
		// Imports the model from a buffer
		void importModelFromAssimp(ofBuffer & buffer, bool optimize, const char * extension);

        // Load VBOs
        void loadVBOs(aiMesh* mesh, ofxAssimpMeshHelper& meshHelper);

        void getBoundingBoxWithMinVector( aiVector3D* min, aiVector3D* max);
        void getBoundingBoxForNode(const ofxAssimpMeshHelper & mesh,  aiVector3D* min, aiVector3D* max);

        ofFile file;

        aiVector3D scene_min, scene_max, scene_center;

        bool normalizeScale;
        double normalizedScale;

		std::vector<float> rotAngle;
		std::vector<ofPoint> rotAxis;
        ofPoint scale;
        ofPoint pos;
        ofMatrix4x4 modelMatrix;

		std::vector<ofLight> lights;
		std::vector<ofxAssimpTexture> textures;
		std::vector<ofxAssimpMeshHelper> modelMeshes;
		std::vector<ofxAssimpAnimation> animations;
        int currentAnimation; // DEPRECATED - to be removed with deprecated animation functions.

        bool bUsingTextures;
        bool bUsingNormals;
        bool bUsingColors;
        bool bUsingMaterials;
        float normalizeFactor;

        // the main Asset Import scene that does the magic.
		std::shared_ptr<const aiScene> scene;
		std::shared_ptr<aiPropertyStore> store;

        std::string modelURI; // url or path of the model file
        std::string modelDirectory;
};
