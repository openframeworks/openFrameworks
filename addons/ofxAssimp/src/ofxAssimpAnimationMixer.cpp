//
//  ofxAssimpAnimationMixer.cpp
//  Created by Nick Hardeman on 11/3/23.
//

#include "ofxAssimpAnimationMixer.h"
#include "ofLog.h"

using namespace ofx::assimp;

//--------------------------------------------------------------
bool AnimationClip::shouldRemove( const AnimationClip& ac ) {
	return ac.bRemove;
}

//--------------------------------------------------------------
size_t AnimationMixer::getNumAnimationClips() {
	return mAnimationClips.size();
}

//--------------------------------------------------------------
AnimationClip& AnimationMixer::getAnimationClip(int aindex) {
	if( getNumAnimationClips() < 1 ) {
		ofLogWarning("ofx::assimp::AnimationMixer::getAnimationClip") << "there are no animation clips.";
		return dummy;
	}
	aindex = ofClamp( aindex, 0, getNumAnimationClips()-1);
	return mAnimationClips[aindex];
}

//--------------------------------------------------------------
AnimationClip& AnimationMixer::getAnimationClip(const std::string& aname) {
	if( getNumAnimationClips() < 1 ) {
		ofLogWarning("ofx::assimp::AnimationMixer::getAnimationClip") << "there are no animation clips.";
		return dummy;
	}
	for( auto& anim : mAnimationClips ) {
		if( anim.animation.getName() == aname ) {
			return anim;
		}
	}
	ofLogWarning("ofx::assimp::AnimationMixer::getAnimationClip") << " could not find clip " << aname;
	return dummy;
}

//--------------------------------------------------------------
void AnimationMixer::add( const AnimationClip& aclip ) {
	mAnimationClips.push_back(aclip);
}

//--------------------------------------------------------------
bool AnimationMixer::remove( int aindex ) {
	if( aindex < getNumAnimationClips() && aindex > -1 && getNumAnimationClips() > 0) {
		mAnimationClips.erase( mAnimationClips.begin() + aindex );
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool AnimationMixer::remove( const std::string& aname ) {
	int tindex = -1;
	for(int i = 0; i < (int)mAnimationClips.size(); i++) {
		if( mAnimationClips[i].animation.getName() == aname ) {
			tindex = i;
			break;
		}
	}
	if( tindex > -1 ) {
		return remove(tindex);
	}
	return false;
}

//--------------------------------------------------------------
void AnimationMixer::removeAll() {
	mAnimationClips.clear();
}

//--------------------------------------------------------------
void AnimationMixer::playAll() {
	for( auto& anim : mAnimationClips ) {
		anim.animation.play();
	}
}

//--------------------------------------------------------------
void AnimationMixer::stopAll() {
	for( auto& anim : mAnimationClips ) {
		anim.animation.stop();
	}
}

//--------------------------------------------------------------
void AnimationMixer::update(float aElapsedTimef) {
	for(auto& anim : mAnimationClips ) {
//		std::cout << "AnimationMixer :: update : " << anim.animation.getName() << " position: " << anim.animation.getPositionInSeconds() << std::endl;
		anim.animation.update();
	}
	
	float tdelta = std::max( std::min(aElapsedTimef-mLastUpdateTime, 10.f), ai_epsilon);
	
	for( int i = 0; i < (int)mAnimationClips.size(); i++ ) {
		auto& anim = mAnimationClips[i];
		if(isTransitioning()) {
//		ofLogNotice("AnimationMixer::update") << i << " - " << anim.animation.getName() << " weights: " << anim.weight << " / " << anim.mTargetWeight << " seekDiff: " << anim.mSeekWeightDiff << " | " << ofGetFrameNum();
		}
		
		if( anim.mTargetWeight > -1.f ) {
			if( anim.weight != anim.mTargetWeight ) {
				if( anim.mSeekWeightDirection > 0.f ) {
					anim.weight += anim.mSeekWeightDiff * (tdelta / mTransitionDuration);
					if( anim.weight >= anim.mTargetWeight ) {
						anim.weight = anim.mTargetWeight;
					}
				} else {
					anim.weight -= anim.mSeekWeightDiff * (tdelta / mTransitionDuration);
					if( anim.weight <= anim.mTargetWeight ) {
						anim.weight = anim.mTargetWeight;
					}
				}
			}
		}
	}
	
	bool bFinishedTransition = false;
	if(isTransitioning()) {
//		ofLogNotice("AnimationMixer::update") << " total weights: " << getTotalClipWeights() << " | " << ofGetFrameNum();
		mTransitionPercent += (tdelta / mTransitionDuration);
		
		if(mTransitionPercent >= 1.0f ) {
			mBTransitioning = false;
			mTransitionPercent = 1.0f;
			if(mBRemoveOtherClipsAfterTransition && mTargetTransitionName != "") {
				for( int i = 0; i < (int)mAnimationClips.size(); i++ ) {
					if( mAnimationClips[i].mTargetWeight <= 0.f ) {
						mAnimationClips[i].tagForRemoval();
					}
				}
			}
			bFinishedTransition = true;
		}
	}
	if(mAnimationClips.size() ) {
		ofRemove(mAnimationClips, AnimationClip::shouldRemove );
	}
//	if(bFinishedTransition) {
//		ofLogNotice("AnimationMixer::update") << "transitioning number of clips: " << mAnimationClips.size() << " | " << ofGetFrameNum();
//	}
	
	mLastUpdateTime = aElapsedTimef;
}

//--------------------------------------------------------------
float AnimationMixer::getTotalClipWeights() {
	float totalW = 0.f;
	for( auto& anim : mAnimationClips ) {
		totalW += anim.weight;
	}
	return totalW;
}

//--------------------------------------------------------------
void AnimationMixer::transition( const std::string& aname, float aduration ) {
	transition( aname, aduration, true );
}

//--------------------------------------------------------------
void AnimationMixer::transition( const std::string& aname, float aduration, bool bRemoveOldClips ) {
	// first get the targeted animation //
	int animIndex = -1;
	for( int i = 0; i < (int)mAnimationClips.size(); i++ ) {
		if( mAnimationClips[i].animation.getName() == aname ) {
			animIndex = i;
			break;
		}
	}
	if( animIndex < 0 ) {
		ofLogWarning("AnimationMixer::transition") << "did not find animation with name " << aname;
		return;
	}
	
	for( int i = 0; i < (int)mAnimationClips.size(); i++ ) {
		if( i != animIndex ) {
			mAnimationClips[i].seekWeight(0.f);
		} else {
			mAnimationClips[i].seekWeight(1.f);
		}
	}
	
	mBTransitioning = true;
	mTransitionPercent = 0.f;
	mTransitionDuration = aduration;
	mTargetTransitionName = aname;
	mBRemoveOtherClipsAfterTransition = bRemoveOldClips;
}

//--------------------------------------------------------------
bool AnimationMixer::isTransitioning() {
	return mBTransitioning;
}

//--------------------------------------------------------------
float AnimationMixer::getTransitionPercent() {
	return mTransitionPercent;
}
