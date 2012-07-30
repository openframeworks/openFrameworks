/*
 * ofSoundUtils.h
 *
 *  Created on: 30/07/2012
 *      Author: arturo
 */

#ifndef OFSOUNDUTILS_H_
#define OFSOUNDUTILS_H_

#include "ofMath.h"

void ofStereoVolumes(float volume, float pan, float & left, float & right);
float ofHermiteInterpolate(float pct, float xm1, float x0, float x1, float x2);


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

// from http://musicdsp.org/showArchiveComment.php?ArchiveID=93
// laurent de soras
inline float ofHermiteInterpolate(float pct, float xm1, float x0, float x1, float x2)
{
	const float c = (x1 - xm1) * 0.5f;
	const float v = x0 - x1;
	const float w = c + v;
	const float a = w + v + (x2 - x0) * 0.5f;
	const float b_neg = w + a;

	return ((((a * pct) - b_neg) * pct + c) * pct + x0);
}
#endif /* OFSOUNDUTILS_H_ */
