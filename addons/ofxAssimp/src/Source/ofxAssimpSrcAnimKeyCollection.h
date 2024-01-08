//
//  ofxAssimpSrcAnimKeyCollection.h
//  Created by Nick Hardeman on 11/1/23.
//

#pragma once
#include <assimp/scene.h>
#include "ofNode.h"


namespace ofx::assimp {

struct AnimVectorKey {
	float time = 0.0;
	glm::vec3 value;
	aiVector3D valueAi;
};

struct AnimRotationKey {
	float time = 0.0;
	glm::quat value;
	aiQuaternion valueAi;
};

class SrcAnimKeyCollection {
public:
	unsigned int uId = 0;
	std::string name = "";
	std::vector<ofx::assimp::AnimVectorKey> positionKeys;
	std::vector<ofx::assimp::AnimRotationKey> rotationKeys;
	std::vector<ofx::assimp::AnimVectorKey> scaleKeys;
	
	void clear() {
		positionKeys.clear();
		rotationKeys.clear();
		scaleKeys.clear();
	}
	
	void setup( aiNodeAnim* aNodeAnim, float aDurationInTicks );
	bool hasKeys();
	
	glm::vec3 getVec3ForTime( const float& atime, const std::vector<ofx::assimp::AnimVectorKey>& akeys );
	
	glm::vec3 getPosition( const float& atime );
	glm::vec3 getScale( const float& atime );
	glm::quat getRotation( const float& atime );
		
	std::vector<AnimVectorKey> getAnimVectorKeysForTime(const float& aStartTime, const float& aEndTime, unsigned int aNumKeys, aiVectorKey* aAiKeys);
	std::vector<AnimRotationKey> getAnimRotationKeysForTime(const float& aStartTime, const float& aEndTime, unsigned int aNumKeys, aiQuatKey* aAiKeys);
	
	
	aiNodeAnim* mNodeAnim = nullptr;
	float mDurationInTicks = 1;
	
};
}
