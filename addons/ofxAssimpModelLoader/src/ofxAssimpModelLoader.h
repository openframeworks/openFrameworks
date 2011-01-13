
// assimp include files. These three are usually needed.
#import "assimp.h"
#import "aiPostProcess.h"
#import "aiScene.h"

#pragma once

#include "ofMain.h"

// ofDevCon 
// Written by Anton Marini (http://vade.info)
// With massive help from Memo Akten (http://www.memo.tv/) 

// TODO:
// 1) Keyframe interpolation for animations. This is currently done in the AssimpView Direct X project.
// 2) Path issues (?)
// 3) convert to ofMesh (?) in OF 007 ?
// 4) Ability to ease *between* two animations. Maybe later folks.
// 5) write deconstructor, bwahaha.

#import "ofxAssimpMeshHelper.h"

class ofxAssimpModelLoader{

    public:
        ~ofxAssimpModelLoader();
        ofxAssimpModelLoader();

        void loadModel(string modelName);
    
        // Call this to update the animation. This is a no-op if there is no animation
        // get rid of this and make lazy only when you update the animation time.
        //void update();  
    
        void setScale(float x, float y, float z);
        void setPosition(float x, float y, float z);
        void setRotation(int which, float angle, float rot_x, float rot_y, float r_z);

        // Scale the model to the screen automatically.
        void setScaleNomalization(bool normalize);

        // This changes when you load a different model, may be 0.
        unsigned int getAnimationCount();
    
        void setAnimation(int anim); // 0 to 1 - getNumAnimations()
        void setNormalizedTime(float time); // 0 - 1
        void setTime(float time); // 0 - duration
        float getDuration(int animation);

        void draw();
            
        // Our array of textures we load from the models path.
        vector <ofImage> textures;
        //list <ofImage> textures;
    
        // TODO: convert to ofMesh or ofVBOMesh
        vector <ofxAssimpMeshHelper> modelMeshes;  
         
    private:
        // the main Asset Import scene that does the magic.
        aiScene* scene;

        // Initial VBO creation, etc
        void loadGLResources();
        void deleteGLResources();
    
        // Updates the internal animation transforms for the selected animation index
        void updateAnimation(unsigned int animationIndex, float time);
    
        // updates the *actual GL resources* for the current animation
        void updateGLResources();
    
        void getBoundingBoxWithMinVector(struct aiVector3D* min, struct aiVector3D* max);
        void getBoundingBoxForNode(const struct aiNode* nd,  struct aiVector3D* min, struct aiVector3D* max, struct aiMatrix4x4* trafo);
        
        bool hasAnimations;
        float normalizedAnimationTime;
        int currentAnimation;
        
        bool normalizeScale;
    
        // TODO: make this return an of
        aiVector3D scene_min, scene_max, scene_center;
        double normalizedScale;    
        
        vector <float> rotAngle;
        vector <ofPoint> rotAxis;
        ofPoint scale;
        ofPoint pos;
        int numRotations;
};
