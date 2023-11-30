//
//  ofxAssimpAnimationMixer.h
//  Created by Nick Hardeman on 11/3/23.
//

#pragma once
#include "ofxAssimpAnimation.h"

namespace ofx::assimp {
class AnimationClip {
public:
	static bool shouldRemove( const AnimationClip& ac );
	
	AnimationClip() {}
	AnimationClip(Animation anim, float aweight) {
		animation = anim;
		weight = aweight;
	}
//	AnimationClip(const Animation& anim, float aweight, bool abOneShot) {
//		animation = anim;
//		weight = aweight;
//		bOneShot = abOneShot;
//	}
	
	~AnimationClip() {}
	
	void seekWeight( float aTargetWeight ) {
		mTargetWeight = aTargetWeight;
		if( mTargetWeight > -1.f ) {
			if( mTargetWeight > weight ) {
				mSeekWeightDirection = 1.f;
			} else {
				mSeekWeightDirection = -1.f;
			}
			
			mSeekWeightDiff = fabsf(mTargetWeight-weight);
		}
	}
	
	void tagForRemoval() {bRemove=true;}
	
	float weight = 1.0f;
	Animation animation;
	
//protected:
	float mTargetWeight = -1.f;
	float mSeekWeightDirection = 1.f;
	float mSeekWeightDiff = 0.f;
	
//	bool bOneShot = false;
	
protected:
	bool bRemove = false;
	
};

class AnimationMixer {
public:
	
	size_t getNumAnimationClips();
	AnimationClip& getAnimationClip(int aindex);
	AnimationClip& getAnimationClip(const std::string& aname);
	std::vector<AnimationClip>& getAnimationClips() { return mAnimationClips; }
	/// \brief Add an animation clip.
	/// \param aclip add a clip to the mixer.
	void add( const AnimationClip& aclip );
	bool remove( int aindex );
	bool remove( const std::string& aname );
	void removeAll();
	
	void playAll();
	void stopAll();
	void update(float aElapsedTimef);
	
	float getTotalClipWeights();
	/// \brief Transition to animation clip by name.
	/// \param aname name of animation clip to transition to.
	void transition( const std::string& aname, float aduration );
	void transition( const std::string& aname, float aduration, bool bRemoveOldClips );
	bool isTransitioning();
	float getTransitionPercent();
	
protected:
	std::vector<AnimationClip> mAnimationClips;
	AnimationClip dummy;
	
	float mLastUpdateTime = 0.f;
	
	bool mBTransitioning = false;
	float mTransitionPercent = 0.f;
	
	float mTransitionDuration = 1.f;
	std::string mTargetTransitionName = "";
	bool mBRemoveOtherClipsAfterTransition = false;
	
};
}
