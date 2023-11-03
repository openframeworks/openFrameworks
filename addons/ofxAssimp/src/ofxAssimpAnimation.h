//
//  ofxAssimpAnimation.h
//  Created by Lukasz Karluk on 4/12/12.
//

#pragma once

#include "ofVideoBaseTypes.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ofx::assimp {
class Animation {
public:
	Animation();
	Animation(std::shared_ptr<const aiScene> scene, aiAnimation * animation);
	~Animation();
	
	aiAnimation * getAnimation();
	std::string getName() { return mName; }
	
	// TODO: Add start time, end time
	
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
	void setSpeed(float s);
	
//	unsigned int getCurrentFrame() const;
//	unsigned int getTotalNumFrames() const;
//	void setFrame( unsigned int aframe );
	
protected:
	
	std::string mName = "";
	
	void updateAnimationNodes();
	
	std::shared_ptr<const aiScene> scene;
	aiAnimation * animation;
	float animationCurrTime = 0.f;
	float animationPrevTime = 0.f;
	bool bPlay = false;
	bool bPause = false;
	ofLoopType loopType;
	float progress = 0.f;
	float progressInSeconds = 0.f;
	int progressInMilliSeconds = 0.f;
	double durationInSeconds = 0.f;
	int durationInMilliSeconds = 0.f;
	float speed = 1.0f;
	float speedFactor = 1.0f;
	
	double progressInTicks = 0.0;
	double durationInTicks = 1.0;
};
}
