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

#include "ofMain.h"
#include "ofxAssimpMeshHelper.h"
#include "ofxAssimpAnimation.h"
#include "ofxAssimpTexture.h"

class aiScene;
class aiNode;

class ofxAssimpModelLoader{

    public:
        ~ofxAssimpModelLoader();
        ofxAssimpModelLoader();

        bool loadModel(string modelName, bool optimize=false);
        bool loadModel(ofBuffer & buffer, bool optimize=false, const char * extension="");
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
        void setScaleNomalization(bool normalize);
        void setNormalizationFactor(float factor);

        vector<string> getMeshNames();
        int getNumMeshes();

        ofMesh getMesh(string name);
        ofMesh getMesh(int num);

        ofMesh getCurrentAnimatedMesh(string name);
        ofMesh getCurrentAnimatedMesh(int num);

        ofMaterial getMaterialForMesh(string name);
        ofMaterial getMaterialForMesh(int num);

        ofTexture getTextureForMesh(string name);
        ofTexture getTextureForMesh(int num);


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

        // Initial VBO creation, etc
        void loadGLResources();
    
        // Updates the internal animation transforms for the selected animation index
        void updateAnimation(unsigned int animationIndex, float time);
    
        // updates the *actual GL resources* for the current animation
        void updateGLResources();
    
        void getBoundingBoxWithMinVector(struct aiVector3D* min, struct aiVector3D* max);
        void getBoundingBoxForNode(const struct aiNode* nd,  struct aiVector3D* min, struct aiVector3D* max, struct aiMatrix4x4* trafo);

        ofFile file;

        aiVector3D scene_min, scene_max, scene_center;

        bool normalizeScale;
        double normalizedScale;

        vector<float> rotAngle;
        vector<ofPoint> rotAxis;
        ofPoint scale;
        ofPoint pos;
        ofMatrix4x4 modelMatrix;

        vector<ofLight> lights;
        vector<ofxAssimpTexture> textures;
        vector<ofxAssimpMeshHelper> modelMeshes;
        vector<ofxAssimpAnimation> animations;
        int currentAnimation; // DEPRECATED - to be removed with deprecated animation functions.

        bool bUsingTextures;
        bool bUsingNormals;
        bool bUsingColors;
        bool bUsingMaterials;
        float normalizeFactor;

        // the main Asset Import scene that does the magic.
        shared_ptr<const aiScene> scene;
};
