#pragma once

#include "ofMathConstants.h"

template<typename T>
class ofColor_;
typedef ofColor_<float> ofFloatColor;
using ofDefaultVertexType = ofDefaultVec3;
using ofDefaultNormalType = ofDefaultVec3;
using ofDefaultColorType = ofFloatColor;
using ofDefaultTexCoordType = ofDefaultVec2;

enum ofPolyRenderMode{
	OF_MESH_POINTS,
	OF_MESH_WIREFRAME,
	OF_MESH_FILL
};
