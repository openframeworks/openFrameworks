
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


#endif
