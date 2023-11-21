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
//	Animation(std::shared_ptr<const aiScene> scene, aiAnimation * animation);
	Animation(unsigned int aUid, aiAnimation * animation);
	~Animation();
	
//	aiAnimation * getAnimation();
	void setName( const std::string& aname ) { mName=aname; }
	std::string& getName() { return mName; }
	void setup( float aStartTick, float aEndTick );
	
	void update();
	
	void play();
	void stop();
	void reset();
	
	bool isFrameNew();
	bool isPaused();
	bool isPlaying();
	/// \brief Check to see if the animation is at the end and is not playing
	/// \return if the animation is finished playing and will not loop.
	bool isFinished();
	/// \brief Check to see if the animation is at the end.
	/// \return if the animation reached the end, regardless of loop.
	bool isDone();
	
	float getPosition();
	float getPositionInSeconds();
	int getPositionInMilliSeconds();
	float getPositionInTicks();
	
	float getStartTick();
	float getEndTick();
	
	float getSpeed();
	
	float getDuration() const;
	unsigned int getDurationMS() const;
	float getDurationInTicks();
	
	void setPaused(bool paused);
	void setPosition(float position);
	void setLoopType(ofLoopType state);
	void setSpeed(float s);
	
	// referred to ticks in Assimp
	unsigned int getCurrentFrame() const;
	unsigned int getTotalNumFrames() const;
	void setFrame( unsigned int aframe );
	
	unsigned int& getUid() {return mUid;}
	
protected:
	
	std::string mName = "";
	unsigned int mUid = 0;
		
	float mSrcDurationInTicks = 0.f;
//	std::shared_ptr<const aiScene> scene;
//	aiAnimation * animation;
	float animationCurrTime = 0.f;
	float animationPrevTime = 0.f;
	bool bPlay = false;
	bool bPause = false;
	ofLoopType loopType;
	float progress = 0.f;
	float progressInSeconds = 0.f;
	int progressInMilliSeconds = 0.f;
	float durationInSeconds = 0.f;
	unsigned int durationInMilliSeconds = 0.f;
	float speed = 1.0f;
	float speedFactor = 1.0f;
	
	// for creating animations that don't start at 0 and end at the last tick
	float mStartOffsetTick = 0.;
	float mEndOffsetTick = 1.;
	
	float progressInTicks = 0.0;
	float durationInTicks = 1.0;
	float mTicksPerSecond = 25.f;
	
	bool mBDone = false;
};

}
