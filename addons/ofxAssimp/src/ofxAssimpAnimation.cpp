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
	this->scene = nullptr;
	this->animation = nullptr;
}

//-------------------------------------------
Animation::Animation(std::shared_ptr<const aiScene> scene, aiAnimation * animation) {
	this->scene = scene;
	this->animation = animation;
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

	if(animation != NULL) {
		durationInTicks = animation->mDuration;
		double ticksPerSecond = 25.0;
		if( animation->mTicksPerSecond > 0.0 ) {
			ticksPerSecond = animation->mTicksPerSecond;
		}
		durationInSeconds = animation->mDuration / ticksPerSecond;
		durationInMilliSeconds = durationInSeconds * 1000;
		std::cout << "ofxAssimpAnimation : durationInSeconds: " << ceil(durationInSeconds*30.0) << " " << std::endl;
	}
}

//-------------------------------------------
Animation::~Animation() {
	
}

//-------------------------------------------
aiAnimation * Animation::getAnimation() {
	return animation;
}

//-------------------------------------------
void Animation::update() {
	animationPrevTime = animationCurrTime;
	animationCurrTime = ofGetElapsedTimef();
	double tps = animation->mTicksPerSecond ? animation->mTicksPerSecond : 25.f;
	animationCurrTime *= tps;

	if(!bPlay || bPause) {
		return;
	}

	float duration = durationInTicks;
	
	if( duration < ai_epsilon ) {
		setPosition(0.0f);
		return;
	}
	
	float timeStep = animationCurrTime - animationPrevTime;
	float positionStep = timeStep / (float)duration;
	float position = getPosition() + positionStep * speed * speedFactor;

	if(position > 1.0 && loopType == OF_LOOP_NONE) {
		position = 1.0;
		stop();
	} else if(position > 1.0 && loopType == OF_LOOP_NORMAL) {
		position = fmod(position, 1.0f);
	} else if(position > 1.0 && loopType == OF_LOOP_PALINDROME) {
		speedFactor *= -1;
	} else if(position < 0.0 && loopType == OF_LOOP_PALINDROME) {
		speedFactor *= -1;
	}

	setPosition(position);
}

//-------------------------------------------
void Animation::updateAnimationNodes() {
	for(unsigned int i=0; i<animation->mNumChannels; i++) {
		const aiNodeAnim * channel = animation->mChannels[i];
		aiNode * targetNode = scene->mRootNode->FindNode(channel->mNodeName);

		aiVector3D presentPosition(0, 0, 0);
		if(channel->mNumPositionKeys > 0) {
			unsigned int frame = 0;
			while(frame < channel->mNumPositionKeys - 1) {
				if(progressInTicks < channel->mPositionKeys[frame+1].mTime) {
					break;
				}
				frame++;
			}

			unsigned int nextFrame = (frame + 1) % channel->mNumPositionKeys;
			const aiVectorKey & key = channel->mPositionKeys[frame];
			const aiVectorKey & nextKey = channel->mPositionKeys[nextFrame];
			double diffTime = nextKey.mTime - key.mTime;
			if(diffTime < 0.0) {
				diffTime += durationInTicks;//getDurationInSeconds();
			}
			if(diffTime > 0) {
				float factor = float((progressInTicks - key.mTime) / diffTime);
				presentPosition = key.mValue + (nextKey.mValue - key.mValue) * factor;
			} else {
				presentPosition = key.mValue;
			}
		}

		aiQuaternion presentRotation(1, 0, 0, 0);
		if(channel->mNumRotationKeys > 0) {
			unsigned int frame = 0;
			while(frame < channel->mNumRotationKeys - 1) {
				if(progressInTicks < channel->mRotationKeys[frame+1].mTime) {
					break;
				}
				frame++;
			}

			unsigned int nextFrame = (frame + 1) % channel->mNumRotationKeys;
			const aiQuatKey& key = channel->mRotationKeys[frame];
			const aiQuatKey& nextKey = channel->mRotationKeys[nextFrame];
			double diffTime = nextKey.mTime - key.mTime;
			if(diffTime < 0.0) {
				diffTime += durationInTicks;//getDurationInSeconds();
			}
			if(diffTime > 0) {
				float factor = float((progressInTicks - key.mTime) / diffTime);
				aiQuaternion::Interpolate(presentRotation, key.mValue, nextKey.mValue, factor);
			} else {
				presentRotation = key.mValue;
			}
		}

		aiVector3D presentScaling(1, 1, 1);
		if(channel->mNumScalingKeys > 0) {
			unsigned int frame = 0;
			while(frame < channel->mNumScalingKeys - 1){
				if(progressInTicks < channel->mScalingKeys[frame+1].mTime) {
					break;
				}
				frame++;
			}

			presentScaling = channel->mScalingKeys[frame].mValue;
		}

		aiMatrix4x4 mat = aiMatrix4x4(presentRotation.GetMatrix());
		mat.a1 *= presentScaling.x; mat.b1 *= presentScaling.x; mat.c1 *= presentScaling.x;
		mat.a2 *= presentScaling.y; mat.b2 *= presentScaling.y; mat.c2 *= presentScaling.y;
		mat.a3 *= presentScaling.z; mat.b3 *= presentScaling.z; mat.c3 *= presentScaling.z;
		mat.a4 = presentPosition.x; mat.b4 = presentPosition.y; mat.c4 = presentPosition.z;

		targetNode->mTransformation = mat;
	}
}

//-------------------------------------------
void Animation::play() {
	if(animation == NULL) {
		return;
	}
	if(bPlay) { // if already playing, ignore.
		bPause = false; // if paused, then unpause.
		return;
	}
	bPlay = true;
	bPause = false;

	setPosition(0);
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
	setPosition(0);
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
float Animation::getSpeed() {
	return speed;
}

//-------------------------------------------
float Animation::getDurationInSeconds() {
	return durationInSeconds;
}

//-------------------------------------------
int Animation::getDurationInMilliSeconds() {
	return durationInMilliSeconds;
}

//-------------------------------------------
void Animation::setPaused(bool paused) {
	bPause = paused;
}

//-------------------------------------------
void Animation::setPosition(float position) {
	position = ofClamp(position, 0.0f, 1.0f);
	if(progress == position) {
		return;
	}
	progress = position;
	progressInTicks = progress * durationInTicks;
	progressInSeconds = progress * getDurationInSeconds();
	progressInMilliSeconds = progress * getDurationInMilliSeconds();

	updateAnimationNodes();
}

//-------------------------------------------
void Animation::setLoopState(ofLoopType state) {
	loopType = state;
}

//-------------------------------------------
void Animation::setSpeed(float s) {
	speed = s;
}
