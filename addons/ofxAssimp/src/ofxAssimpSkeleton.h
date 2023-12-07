//
//  ofxAssimpSkeleton.h
//  ofxAssimpExample
//
//  Created by Nick Hardeman on 10/25/23.
//

#pragma once
#include "ofxAssimpBone.h"

namespace ofx::assimp {
class Skeleton : public ofx::assimp::Bone {
public:
	virtual NodeType getType() override { return OFX_ASSIMP_SKELETON; }
	
protected:
	
};
}
