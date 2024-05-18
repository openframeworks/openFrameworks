/*
 * ofSoundUtils.h
 *
 *  Created on: 30/07/2012
 *      Author: arturo
 */

#pragma once

#define GLM_FORCE_CTOR_INIT
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/constants.hpp>
#include <glm/ext/scalar_common.hpp>


inline void ofStereoVolumes(float volume, float pan, float & left, float & right){
	pan = glm::clamp(pan, -1.0f, 1.0f);
	// calculates left/right volumes from pan-value (constant panning law)
	// see: Curtis Roads: Computer Music Tutorial p 460
	// thanks to jasch
	
	float angle = pan * glm::quarter_pi<float>(); // in radians from -45. to +45.
	float cosAngle = std::cos(angle);
	float sinAngle = std::sin(angle);
	
	float v = glm::one_over_root_two<float>() * volume; // multiplied by sqrt(2)/2
	left  = (cosAngle - sinAngle) * v;
	right = (cosAngle + sinAngle) * v;
}
