#pragma once

#include "ofConstants.h"
#include "ofRectangle.h"

class ofMesh;
class ofTexture;


/*
 
 note, the data in this code is taken from freeglut, and included in OF for compatability 
 with non glut windowing toolkits.  see .cpp for license info
 
 also, note that while this is used internally in ofGraphics, it's not really useful for end user usage.
 
 */

void ofDrawBitmapCharacterStart(int stringLength);
void ofDrawBitmapCharacter(int character, int x , int y );
void ofDrawBitmapCharacterEnd();
ofMesh & ofBitmapStringGetMesh(const string & text, int x, int y);
ofTexture & ofBitmapStringGetTextureRef();
ofRectangle ofBitmapStringGetBoundingBox(const string & text, int x, int y);

