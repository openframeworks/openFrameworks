
// notes:
// -----------------------------------------------------------
// for fast things look here: http://musicdsp.org/archive.php?classid=5#115
// -----------------------------------------------------------
// the random () calls are based on misconceptions described here:
// http://www.azillionmonkeys.com/qed/random.html
// (Bad advice from C.L.C. FAQ)
// we should correct this -- 
// -----------------------------------------------------------


#ifndef _OF_MATH_H
#define _OF_MATH_H


#include "ofConstants.h"

int 		ofNextPow2 ( int a );
void 		ofSeedRandom();
void 		ofSeedRandom(int val);
float 		ofRandom(float val0, float val1);		// random (x - y)
float 		ofRandomf();							// random (-1 - 1)
float 		ofRandomuf();							// random (0 - 1) 

float		ofNormalize(float value, float min, float max);
float		ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);
float		ofClamp(float value, float min, float max);
float		ofLerp(float start, float stop, float amt);
float		ofDist(float x1, float y1, float x2, float y2);
float		ofDistSquared(float x1, float y1, float x2, float y2);
int			ofSign(float n);
bool		ofInRange(float t, float min, float max);
float		ofRadToDeg(float radians);
float		ofDegToRad(float degrees);
float		ofRandomWidth();
float		ofRandomHeight();

			//returns noise in 0.0 to 1.0 range
float		ofNoise(float x);
float		ofNoise(float x, float y);
float		ofNoise(float x, float y, float z);
float		ofNoise(float x, float y, float z, float w);

			//returns noise in -1.0 to 1.0 range
float		ofSignedNoise(float x);
float		ofSignedNoise(float x, float y);
float		ofSignedNoise(float x, float y, float z);
float		ofSignedNoise(float x, float y, float z, float w);

#endif
