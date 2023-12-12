//
//  ofxAssimpSrcAnimKeyCollection.cpp
//  Created by Nick Hardeman on 11/1/23.
//

#include "ofxAssimpSrcAnimKeyCollection.h"
#include "ofxAssimpUtils.h"

using namespace ofx::assimp;

//--------------------------------------------------------------
void SrcAnimKeyCollection::setup( aiNodeAnim* aNodeAnim, float aDurationInTicks ) {
	mNodeAnim = aNodeAnim;
	mDurationInTicks = aDurationInTicks;
}

//--------------------------------------------------------------
bool  SrcAnimKeyCollection::hasKeys() {
	return mNodeAnim != nullptr;
}


//--------------------------------------------------------------
glm::vec3 SrcAnimKeyCollection::getVec3ForTime( const float& atime, const std::vector<ofx::assimp::AnimVectorKey>& akeys ) {
	size_t numKeys = akeys.size();
	for( size_t i = 0; i < numKeys; i++ ) {
		if( akeys[i].time == atime ) {
			return akeys[i].value;
		} else if( akeys[i].time > atime ) {
			if( i > 0 ) {
				float keyDiff = akeys[i].time - akeys[i-1].time;
				return glm::mix( akeys[i-1].value, akeys[i].value, (atime-akeys[i-1].time) / keyDiff );
			} else {
				return akeys[i].value;
			}
		}
	}
	if( numKeys > 1 ) {
		return akeys.back().value;
	}
	return glm::vec3(0.f, 0.f, 0.f);
}

//--------------------------------------------------------------
glm::vec3 SrcAnimKeyCollection::getPosition( const float& atime ) {
	auto rpos = glm::vec3(0.f, 0.f, 0.f);
	if( positionKeys.size() < 1 ) {
		
	} else if( positionKeys.size() == 1 ) {
		rpos = positionKeys[0].value;
	} else {
		rpos = getVec3ForTime( atime, positionKeys );
	}
	return rpos;
}

//--------------------------------------------------------------
glm::vec3 SrcAnimKeyCollection::getScale( const float& atime ) {
	auto rscale = glm::vec3(1.f, 1.f, 1.f);
	if( scaleKeys.size() < 1 ) {
		
	} else if(scaleKeys.size() == 1 ) {
		rscale = scaleKeys[0].value;
	} else {
		rscale = getVec3ForTime( atime, scaleKeys );
	}
	return rscale;
}

//--------------------------------------------------------------
glm::quat SrcAnimKeyCollection::getRotation( const float& atime ) {
	size_t numKeys = rotationKeys.size();
	auto rq = glm::quat(1.f, 0.f, 0.f, 0.f);
	if(numKeys < 2) {
		if( numKeys == 1 ) {
			return rotationKeys[0].value;
		} else {
			return rq;
		}
	} else {
		for( size_t i = 0; i < numKeys; i++ ) {
			if( rotationKeys[i].time == atime ) {
				return rotationKeys[i].value;
			} else if( rotationKeys[i].time > atime ) {
				if( i > 0 ) {
					float keyDiff = rotationKeys[i].time - rotationKeys[i-1].time;
					return glm::slerp( rotationKeys[i-1].value, rotationKeys[i].value, ((atime-rotationKeys[i-1].time) / keyDiff) );
				} else {
					return rotationKeys[i].value;
				}
			}
		}
	}
	if( numKeys > 1 ) {
		return rotationKeys.back().value;
	}
	return rq;
}

//--------------------------------------------------------------
std::vector<AnimVectorKey> SrcAnimKeyCollection::getAnimVectorKeysForTime(const float& aStartTime, const float& aEndTime, unsigned int aNumKeys, aiVectorKey* aAiKeys) {
	
	std::vector<AnimVectorKey> rkeys;
	if( aNumKeys < 1 ) {
		return rkeys;
	}
	
	if(aNumKeys == 1) {
		AnimVectorKey vkey;
		vkey.time = aStartTime;
		vkey.value = aiVecToOfVec(aAiKeys[0].mValue);
		vkey.valueAi = aAiKeys[0].mValue;
		rkeys.push_back( vkey );
		return rkeys;
	}
	
	double currTime = aStartTime;
	for( unsigned int i = 0; i < aNumKeys; i++ ) {
		auto& key1 = aAiKeys[i];
		auto v1 = aiVecToOfVec(key1.mValue);
		AnimVectorKey vkey;
		vkey.time = key1.mTime;
		vkey.value = v1;
		vkey.valueAi = key1.mValue;
		rkeys.push_back( vkey );
	}
	
	return rkeys;
}

//--------------------------------------------------------------
std::vector<AnimRotationKey> SrcAnimKeyCollection::getAnimRotationKeysForTime(const float& aStartTime, const float& aEndTime, unsigned int aNumKeys, aiQuatKey* aAiKeys) {
	
	std::vector<AnimRotationKey> rkeys;
	if( aNumKeys < 1 ) {
		return rkeys;
	}
	if(aNumKeys == 1) {
		AnimRotationKey vkey;
		vkey.time = aStartTime;
		vkey.value = aiQuatToOfQuat(aAiKeys[0].mValue);
		vkey.valueAi = aAiKeys[0].mValue;
		rkeys.push_back( vkey );
		return rkeys;
	}
	double currTime = aStartTime;
	for( unsigned int i = 0; i < aNumKeys; i++ ) {
		auto& key1 = aAiKeys[i];
		auto v1 = aiQuatToOfQuat(key1.mValue);
		AnimRotationKey vkey;
		vkey.time = key1.mTime;
		vkey.value = v1;
		vkey.valueAi = key1.mValue;
		rkeys.push_back( vkey );
	}
	
	return rkeys;
}
