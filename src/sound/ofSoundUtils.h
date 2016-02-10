/*
 * ofSoundUtils.h
 *
 *  Created on: 30/07/2012
 *      Author: arturo
 */

#ifndef OFSOUNDUTILS_H_
#define OFSOUNDUTILS_H_

#include "ofMath.h"
#include "ofSoundBuffer.h"

inline void ofStereoVolumes(float volume, float pan, float & left, float & right){
	pan = ofClamp(pan, -1, 1);
	// calculates left/right volumes from pan-value (constant panning law)
	// see: Curtis Roads: Computer Music Tutorial p 460
	// thanks to jasch
	float angle = pan * 0.7853981633974483f; // in radians from -45. to +45.
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	left  = (cosAngle - sinAngle) * 0.7071067811865475 * volume; // multiplied by sqrt(2)/2
	right = (cosAngle + sinAngle) * 0.7071067811865475 * volume; // multiplied by sqrt(2)/2
}

#endif /* OFSOUNDUTILS_H_ */
