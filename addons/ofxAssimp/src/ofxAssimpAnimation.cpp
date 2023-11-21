//
//  ofxAssimpAnimation.cpp
//  Created by Lukasz Karluk on 4/12/12.
//

#include "ofxAssimpAnimation.h"
#include "ofAppRunner.h"
#include "ofMath.h"

using namespace ofx::assimp;

//-------------------------------------------
Animation::Animation() {
//	this->scene = nullptr;
//	this->animation = nullptr;
}

//-------------------------------------------
Animation::Animation(unsigned int aUid, aiAnimation * animation) {
//	this->scene = scene;
	mUid = aUid;
//	this->animation = animation;
	mName = animation->mName.data;
	animationCurrTime = 0;
	animationPrevTime = 0;
	bPlay = false;
	bPause = false;
	loopType = OF_LOOP_NONE;
	progress = 0;
	progressInSeconds = 0;
	progressInMilliSeconds = 0;
	durationInSeconds = 0;
	durationInMilliSeconds = 0;
	speed = 1;
	speedFactor = 1;
	mTicksPerSecond = 25.0;

	if(animation != NULL) {
		mSrcDurationInTicks = animation->mDuration;
		if( animation->mTicksPerSecond > 0.0 ) {
			mTicksPerSecond = animation->mTicksPerSecond;
		}
		
		setup(0, mSrcDurationInTicks);
//		durationInSeconds = animation->mDuration / mTicksPerSecond;
//		durationInMilliSeconds = durationInSeconds * 1000;
//		mStartOffsetTick = 0;
//		mEndOffsetTick = durationInTicks;
//		std::cout << "ofxAssimpAnimation : durationInSeconds: " << ceil(durationInSeconds*30.0) << " " << std::endl;
	}
}

//-------------------------------------------
Animation::~Animation() {
	
}

////-------------------------------------------
//aiAnimation * Animation::getAnimation() {
//	return animation;
//}

//-------------------------------------------
void Animation::setup(float aStartTick, float aEndTick) {
	mStartOffsetTick = ofClamp( aStartTick, 0, mSrcDurationInTicks );
	mEndOffsetTick = ofClamp( aEndTick, 0, mSrcDurationInTicks );
	durationInTicks = mEndOffsetTick - mStartOffsetTick;
	durationInSeconds = durationInTicks / mTicksPerSecond;
	durationInMilliSeconds = durationInSeconds * 1000;
}

//-------------------------------------------
void Animation::update() {
	animationPrevTime = animationCurrTime;
	animationCurrTime = ofGetElapsedTimef();
	double tps = mTicksPerSecond;//animation->mTicksPerSecond ? animation->mTicksPerSecond : 25.f;
	
//	animationCurrTime *= tps;

	if(!bPlay || bPause) {
		return;
	}
	
	mBDone = false;

	float duration = durationInTicks;
	
	if( duration < ai_epsilon ) {
		setPosition(0.0f);
		return;
	}
	
	float timeStep = (animationCurrTime - animationPrevTime) * tps;
	float positionStep = timeStep / (float)duration;
	float position = getPosition() + positionStep * speed * speedFactor;
	
//	std::cout << "Animation: " << getName() << " timeStep: " << timeStep << " position: " << position << " | " << ofGetFrameNum() << std::endl;

	if(position > 1.0 && loopType == OF_LOOP_NONE) {
		position = 1.0;
		stop();
	} else if(position > 1.0 && loopType == OF_LOOP_NORMAL) {
		mBDone = true;
		position = fmod(position, 1.0f);
	} else if(position > 1.0 && loopType == OF_LOOP_PALINDROME) {
		mBDone = true;
		speedFactor *= -1;
	} else if(position < 0.0 && loopType == OF_LOOP_PALINDROME) {
		mBDone = true;
		speedFactor *= -1;
	}

	setPosition(position);
}

//-------------------------------------------
void Animation::play() {
//	if(animation == NULL) {
//		return;
//	}
	if(bPlay) { // if already playing, ignore.
		bPause = false; // if paused, then unpause.
		return;
	}
	
	if( isFinished() ) {
		setPosition(0);
	}
	
	bPlay = true;
	bPause = false;

//	setPosition(0);
}

//-------------------------------------------
void Animation::stop() {
	speedFactor = 1.0;
	if(!bPlay) {
		return;
	}
	bPlay = false;
	bPause = false;
}

//-------------------------------------------
void Animation::reset() {
	speedFactor = 1.0;
	mBDone = false;
	setPosition(0);
	animationCurrTime = ofGetElapsedTimef();
	animationPrevTime = animationCurrTime;
}

//-------------------------------------------
bool Animation::isFrameNew() {
	return (bPlay && !bPause); // assume its always a new frame when playing and not paused.
}

//-------------------------------------------
bool Animation::isPaused() {
	return bPause;
}

//-------------------------------------------
bool Animation::isPlaying() {
	return bPlay;
}

//-------------------------------------------
bool Animation::isFinished() {
	return !bPlay && (getPosition() == 1.0);
}

//-------------------------------------------
bool Animation::isDone() {
	return isFinished() || mBDone;
}

//-------------------------------------------
float Animation::getPosition() {
	return progress;
}

//-------------------------------------------
float Animation::getPositionInSeconds() {
	return progressInSeconds;
}

//-------------------------------------------
int Animation::getPositionInMilliSeconds() {
	return progressInMilliSeconds;
}

//-------------------------------------------
float Animation::getPositionInTicks() {
	return progressInTicks;
}

//-------------------------------------------
float Animation::getStartTick() {
	return mStartOffsetTick;
}

//-------------------------------------------
float Animation::getEndTick() {
	return mEndOffsetTick;
}

//-------------------------------------------
float Animation::getSpeed() {
	return speed;
}

//-------------------------------------------
float Animation::getDuration() const {
	return durationInSeconds;
}

//-------------------------------------------
unsigned int Animation::getDurationMS() const {
	return durationInMilliSeconds;
}

//-------------------------------------------
float Animation::getDurationInTicks() {
	return durationInTicks;
}

//-------------------------------------------
void Animation::setPaused(bool paused) {
	bPause = paused;
}

//-------------------------------------------
void Animation::setPosition(float position) {
	position = ofClamp(position, 0.0f, 1.0f);
//	if(progress == position) {
//		return;
//	}
	progress = position;
	progressInTicks = progress * durationInTicks;
	progressInSeconds = progress * getDuration();
	progressInMilliSeconds = progress * getDurationMS();

//	updateAnimationNodes();
}

//-------------------------------------------
void Animation::setLoopType(ofLoopType state) {
	loopType = state;
}

//-------------------------------------------
void Animation::setSpeed(float s) {
	speed = s;
}

//-------------------------------------------
unsigned int Animation::getCurrentFrame() const {
	return progressInTicks;
}

//-------------------------------------------
unsigned int Animation::getTotalNumFrames() const {
	return durationInTicks;
}

//-------------------------------------------
void Animation::setFrame( unsigned int aframe ) {
	if( durationInTicks < 2 ) {
		setPosition(0.f);
		return;
	}
	setPosition( (float)aframe / ((float)getTotalNumFrames()-1.f) );
}
