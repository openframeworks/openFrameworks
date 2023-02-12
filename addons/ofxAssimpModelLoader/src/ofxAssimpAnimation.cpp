//
//  ofxAssimpAnimation.cpp
//  Created by Lukasz Karluk on 4/12/12.
//

#include "ofxAssimpAnimation.h"
#include "ofAppRunner.h"
#include "ofMath.h"

ofxAssimpAnimation::ofxAssimpAnimation(std::shared_ptr<const aiScene> scene, aiAnimation * animation) {
	this->scene = scene;
	this->animation = animation;
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
		durationInSeconds = animation->mDuration;
		durationInMilliSeconds = durationInSeconds * 1000;
	}
}

ofxAssimpAnimation::~ofxAssimpAnimation() {
	//
}

aiAnimation * ofxAssimpAnimation::getAnimation() {
	return animation;
}

void ofxAssimpAnimation::update() {
	animationPrevTime = animationCurrTime;
	animationCurrTime = ofGetElapsedTimef();
	double tps = animation->mTicksPerSecond ? animation->mTicksPerSecond : 25.f;
	animationCurrTime *= tps;

	if(!bPlay || bPause) {
		return;
	}

	float duration = getDurationInSeconds();
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

void ofxAssimpAnimation::updateAnimationNodes() {
	for(unsigned int i=0; i<animation->mNumChannels; i++) {
		const aiNodeAnim * channel = animation->mChannels[i];
		aiNode * targetNode = scene->mRootNode->FindNode(channel->mNodeName);

		aiVector3D presentPosition(0, 0, 0);
		if(channel->mNumPositionKeys > 0) {
			unsigned int frame = 0;
			while(frame < channel->mNumPositionKeys - 1) {
				if(progressInSeconds < channel->mPositionKeys[frame+1].mTime) {
					break;
				}
				frame++;
			}

			unsigned int nextFrame = (frame + 1) % channel->mNumPositionKeys;
			const aiVectorKey & key = channel->mPositionKeys[frame];
			const aiVectorKey & nextKey = channel->mPositionKeys[nextFrame];
			double diffTime = nextKey.mTime - key.mTime;
			if(diffTime < 0.0) {
				diffTime += getDurationInSeconds();
			}
			if(diffTime > 0) {
				float factor = float((progressInSeconds - key.mTime) / diffTime);
				presentPosition = key.mValue + (nextKey.mValue - key.mValue) * factor;
			} else {
				presentPosition = key.mValue;
			}
		}

		aiQuaternion presentRotation(1, 0, 0, 0);
		if(channel->mNumRotationKeys > 0) {
			unsigned int frame = 0;
			while(frame < channel->mNumRotationKeys - 1) {
				if(progressInSeconds < channel->mRotationKeys[frame+1].mTime) {
					break;
				}
				frame++;
			}

			unsigned int nextFrame = (frame + 1) % channel->mNumRotationKeys;
			const aiQuatKey& key = channel->mRotationKeys[frame];
			const aiQuatKey& nextKey = channel->mRotationKeys[nextFrame];
			double diffTime = nextKey.mTime - key.mTime;
			if(diffTime < 0.0) {
				diffTime += getDurationInSeconds();
			}
			if(diffTime > 0) {
				float factor = float((progressInSeconds - key.mTime) / diffTime);
				aiQuaternion::Interpolate(presentRotation, key.mValue, nextKey.mValue, factor);
			} else {
				presentRotation = key.mValue;
			}
		}

		aiVector3D presentScaling(1, 1, 1);
		if(channel->mNumScalingKeys > 0) {
			unsigned int frame = 0;
			while(frame < channel->mNumScalingKeys - 1){
				if(progressInSeconds < channel->mScalingKeys[frame+1].mTime) {
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

void ofxAssimpAnimation::play() {
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

void ofxAssimpAnimation::stop() {
	speedFactor = 1.0;
	if(!bPlay) {
		return;
	}
	bPlay = false;
	bPause = false;
}

void ofxAssimpAnimation::reset() {
	speedFactor = 1.0;
	setPosition(0);
}

bool ofxAssimpAnimation::isFrameNew() {
	return (bPlay && !bPause); // assume its always a new frame when playing and not paused.
}

bool ofxAssimpAnimation::isPaused() {
	return bPause;
}

bool ofxAssimpAnimation::isPlaying() {
	return bPlay;
}

bool ofxAssimpAnimation::isFinished() {
	return !bPlay && (getPosition() == 1.0);
}

float ofxAssimpAnimation::getPosition() {
	return progress;
}

float ofxAssimpAnimation::getPositionInSeconds() {
	return progressInSeconds;
}

int ofxAssimpAnimation::getPositionInMilliSeconds() {
	return progressInMilliSeconds;
}

float ofxAssimpAnimation::getSpeed() {
	return speed;
}

float ofxAssimpAnimation::getDurationInSeconds() {
	return durationInSeconds;
}

int ofxAssimpAnimation::getDurationInMilliSeconds() {
	return durationInMilliSeconds;
}

void ofxAssimpAnimation::setPaused(bool paused) {
	bPause = paused;
}

void ofxAssimpAnimation::setPosition(float position) {
	position = ofClamp(position, 0.0f, 1.0f);
	if(progress == position) {
		return;
	}
	progress = position;
	progressInSeconds = progress * getDurationInSeconds();
	progressInMilliSeconds = progress * getDurationInMilliSeconds();

	updateAnimationNodes();
}

void ofxAssimpAnimation::setLoopState(ofLoopType state) {
	loopType = state;
}

void ofxAssimpAnimation::setSpeed(float s) {
	speed = s;
}
