


#pragma once
#include "ofConstants.h"

//----------------------------------------------------------
// ofPoint
//----------------------------------------------------------

class ofPoint {
public:
	
	ofPoint ();
	virtual ~ ofPoint ();
	ofPoint (float _x, float _y, float _z = 0.0f);
	ofPoint (ofPoint const & pnt);
	
	void set (float _x, float _y, float _z = 0);
	void set (ofPoint const & pnt);
	
	ofPoint operator - () const;
	bool operator == (ofPoint const & pnt);
	bool operator != (ofPoint const & pnt);
	ofPoint & operator = (ofPoint const & pnt);
	ofPoint & operator = (float const & val);
	ofPoint operator + (ofPoint const & pnt) const;
	ofPoint operator + (float const & val) const;
	ofPoint & operator += (ofPoint const & pnt);
	ofPoint & operator += (float const & val);
	ofPoint operator - (ofPoint const & pnt) const;
	ofPoint operator - (float const & val) const;
	ofPoint & operator -= (ofPoint const & pnt);
	ofPoint & operator -= (float const & val);
	ofPoint operator * (ofPoint const & pnt) const;
	ofPoint operator * (float const & val) const;
	ofPoint & operator *= (ofPoint const & pnt);
	ofPoint & operator *= (float const & val);
	ofPoint operator / (ofPoint const & pnt) const;
	ofPoint operator / (float const & val) const;
	ofPoint & operator /= (ofPoint const & pnt);
	ofPoint & operator /= (float const & val);
	
	
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
	
	float& operator[]( int n ){
		return v[n];
	}
};
