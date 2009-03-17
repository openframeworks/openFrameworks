/**
*  ofxPoint4f.h
*  by stefanix
*/


#ifndef _OFX_POINT4f
#define _OFX_POINT4f

#include "ofxVec4f.h"
#include "ofTypes.h"


class ofxPoint4f : public ofPoint {


  public:

    float w;


    ofxPoint4f( const float _x=0.0f,
                const float _y=0.0f,
                const float _z=0.0f,
                const float _w=0.0f );




    // Getters and Setters.
    //
    //
    void set( float _x, float _y, float _z, float _w );

    void set( const ofxPoint4f& pnt );



    float& operator[]( const int& i );



    // Check similarity/equality.
    //
    //
    bool operator==( const ofxPoint4f& pnt );

    bool operator!=( const ofxPoint4f& pnt );

    bool match( const ofxPoint4f& pnt, float tollerance=0.0001);



    // Translate point by vector.
    //
    //
    ofxPoint4f 	operator+( const ofxVec4f& vec ) const;

    ofxPoint4f& operator+=( const ofxVec4f& vec );

	ofxPoint4f 	operator+( const float f ) const;

    ofxPoint4f& operator+=( const float f );

    ofxPoint4f 	operator-( const ofxVec4f& vec ) const;

    ofxPoint4f& operator-=( const ofxVec4f& vec );

	ofxPoint4f 	operator-( const float f ) const;

    ofxPoint4f& operator-=( const float f );

    ofxPoint4f 	operator-() const;



    // Scale point (by scalar or vector).
    //
    //
    ofxPoint4f 	operator*( const float f ) const;

    ofxPoint4f& operator*=( const float f );

    ofxPoint4f 	operator*( const ofxVec4f& vec ) const;

    ofxPoint4f& operator*=( const ofxVec4f& vec );

    ofxPoint4f 	operator/( const float f ) const;

    ofxPoint4f& operator/=( const float f );

    ofxPoint4f 	operator/( const ofxVec4f& vec ) const;

    ofxPoint4f& operator/=( const ofxVec4f& vec );


    /**
    * Return the difference vector between two points.
    */
    ofxVec4f 	operator-( const ofxPoint4f& pnt ) const;



    // Distance between two points.
    //
    //
    float distance( const ofxPoint4f& pnt) const;


    float squareDistance( const ofxPoint4f& pnt ) const;



    // Linear interpolation.
    //
    //
    /**
    * p==0.0 results in this point, p==0.5 results in the
    * midpoint, and p==1.0 results in pnt being returned.
    */

    ofxPoint4f 	getInterpolated( const ofxPoint4f& pnt, float p ) const;

    ofxPoint4f& interpolate( const ofxPoint4f& pnt, float p );



    ofxPoint4f 	getMiddle( const ofxPoint4f& pnt ) const;

    ofxPoint4f& middle( const ofxPoint4f& pnt );


    // Average (centroid) among points.
    // (Addition is sometimes useful for calculating averages too)
    //
    //
    ofxPoint4f& average( const ofxPoint4f* points, int num );

    ofxPoint4f 	operator+( const ofxPoint4f& pnt ) const;

    ofxPoint4f& operator+=( const ofxPoint4f& pnt );


    //----------------------------------------
    // this methods are deprecated in 006 please use:

    // use squareDistance
    float 		distanceSquared( const ofxPoint4f& pnt ) const;

    // use getInterpolated
    ofxPoint4f 	interpolated( const ofxPoint4f& pnt, float p ) const;

    // use getMiddle
    ofxPoint4f 	middled( const ofxPoint4f& pnt ) const;

};





// Non-Member operators
//
//
ofxPoint4f operator+( float f, const ofxPoint4f& pnt );

ofxPoint4f operator-( float f, const ofxPoint4f& pnt );

ofxPoint4f operator*( float f, const ofxPoint4f& pnt );

ofxPoint4f operator/( float f, const ofxPoint4f& pnt );


#endif
