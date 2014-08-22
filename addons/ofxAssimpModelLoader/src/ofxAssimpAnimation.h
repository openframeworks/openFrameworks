//
//  ofxAssimpAnimation.h
//  Created by Lukasz Karluk on 4/12/12.
//

#pragma once

#include "ofMain.h"
#include "assimp.h"
#include "aiScene.h"

class ofxAssimpAnimation {

public:
    
    ofxAssimpAnimation(shared_ptr<const aiScene> scene, aiAnimation * animation);
    ~ofxAssimpAnimation();
    
    aiAnimation * getAnimation();
    
    void update();
    
    void play();
    void stop();
    void reset();
    
    bool isFrameNew();
    bool isPaused();
    bool isPlaying();
    bool isFinished();
    
    float getPosition();
    float getPositionInSeconds();
    int getPositionInMilliSeconds();
    float getSpeed();
    float getDurationInSeconds();
    int getDurationInMilliSeconds();
    
    void setPaused(bool paused);
    void setPosition(float position);
    void setLoopState(ofLoopType state);
    void setSpeed(float speed);
    
protected:
    
    void updateAnimationNodes();
    
    shared_ptr<const aiScene> scene;
    aiAnimation * animation;
    float animationCurrTime;
    float animationPrevTime;
    bool bPlay;
    bool bPause;
    ofLoopType loopType;
    float progress;
    float progressInSeconds;
    int progressInMilliSeconds;
    float durationInSeconds;
    int durationInMilliSeconds;
    float speed;
};
