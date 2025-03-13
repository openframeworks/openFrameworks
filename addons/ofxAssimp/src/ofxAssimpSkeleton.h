//
//  ofxAssimpSkeleton.h
//
//  Created by Nick Hardeman on 10/25/23.
//

#pragma once
#include "ofxAssimpBone.h"

namespace ofxAssimp {
class Skeleton : public ofxAssimp::Bone {
public:
	virtual NodeType getType() override { return OFX_ASSIMP_SKELETON; }
	
protected:
	
};
}
