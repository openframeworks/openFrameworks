#ifndef _OF_TYPES
#define _OF_TYPES

#include "ofConstants.h"

// preliminary core types: suggestion for v0.05
class ofPoint {
  public:
   
    // union allows us to access the coordinates through
    // both an array 'v' and 'x', 'y', 'z' member varibles
    union  {
        struct {   
            float x;
            float y;
            float z;
        };
        float v[3];
    };
   
    ofPoint( float _x=0.0f, float _y=0.0f, float _z=0.0f ) {
        x = _x;
        y = _y;
        z = _z;
    } 
    
    void set(float _x, float _y, float _z = 0){
        x = _x;
        y = _y;
        z = _z;
    }        
};

class ofRectangle {
  public:
   float x;
   float y;
   float width;
   float height;   
   ofRectangle(){ x = y = width = height = 0; };
   ofRectangle(float _x, float _y, float _w, float _h){
	x = _x;
	y = _y;
	width = _w;
	height = _h;
   }
};


class ofEventArgs{};


#endif
