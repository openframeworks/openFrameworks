#pragma once

#include "ofConstants.h"


/*
 
 note, the data in this code is taken from freeglut, and included in OF for compatability 
 with non glut windowing toolkits.  see .cpp for license info
 
 also, note that while this is used internally in ofGraphics, it's not really useful for end user usage.
 
 */

void ofDrawBitmapCharacterStart(int stringLength);
void ofDrawBitmapCharacter(int character, int x , int y );
void ofDrawBitmapCharacterEnd();


