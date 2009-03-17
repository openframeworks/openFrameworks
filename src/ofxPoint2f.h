/**
*  ofxPoint2f.h
*  by stefanix
*/


#ifndef _OFX_POINT2f
#define _OFX_POINT2f

#include "ofxVec2f.h"


class ofxPoint2f : public ofPoint {


  public:

    ofxPoint2f( float _x=0.0f, float _y=0.0f );

    ofxPoint2f( const ofPoint& pnt );



    // Getters and Setters.
    //
    //
    void 	set( float _x, float _y );

    void 	set( const ofPoint& pnt );



    float& 	operator[]( const int& i );


    // Check similarity/equality.
    //
    //
    bool 	operator==( const ofPoint& pnt );

    bool 	operator!=( const ofPoint& pnt );

    bool 	match( const ofPoint& pnt, float tollerance=0.0001);



    // Overloading for any type to any type
    //
    //
    void 		operator=( const ofPoint& vec );

    ofxPoint2f 	operator+( const ofPoint& vec ) const;

    ofxPoint2f& operator+=( const ofPoint& vec );

    ofxPoint2f 	operator-( const ofPoint& vec ) const;

    ofxPoint2f& operator-=( const ofPoint& vec );

    ofxPoint2f 	operator*( const ofPoint& vec ) const;

    ofxPoint2f& operator*=( const ofPoint& vec );

    ofxPoint2f 	operator/( const ofPoint& vec ) const;

    ofxPoint2f& operator/=( const ofPoint& vec );


    //operator overloading for float
    //
    //
    void 		operator=( const float f);

    ofxPoint2f 	operator+( const float f ) const;

    ofxPoint2f& operator+=( const float f );

    ofxPoint2f 	operator-( const float f ) const;

    ofxPoint2f& operator-=( const float f );

    ofxPoint2f 	operator-() const;

    ofxPoint2f 	operator*( const float f ) const;

    ofxPoint2f& operator*=( const float f );

    ofxPoint2f 	operator/( const float f ) const;

    ofxPoint2f& operator/=( const float f );


    // Rotate point by angle (deg) around pivot point.
    //
    //



    ofxPoint2f 	getRotated( float angle, const ofxPoint2f& pivot ) const;

    ofxPoint2f& rotate( float angle, const ofxPoint2f& pivot );

    ofxPoint2f 	getRotatedRad( float angle, const ofxPoint2f& pivot ) const;

    ofxPoint2f& rotateRad( float angle, const ofxPoint2f& pivot );


    // Map point to coordinate system defined by origin, vx, and vy.
    //
    //



    ofxPoint2f 	getMapped( const ofxPoint2f& origin,
                      const ofxVec2f& vx,
                      const ofxVec2f& vy ) const;

    ofxPoint2f& map( const ofxPoint2f& origin,
                    const ofxVec2f& vx, const ofxVec2f& vy );


    // Distance between two points.
    //
    //
    float distance( const ofxPoint2f& pnt) const;

    float squareDistance( const ofxPoint2f& pnt ) const;



    // Linear interpolation.
    //
    //
    /**
    * p==0.0 results in this point, p==0.5 results in the
    * midpoint, and p==1.0 results in pnt being returned.
    */

    ofxPoint2f 	getInterpolated( const ofxPoint2f& pnt, float p ) const;

    ofxPoint2f& interpolate( const ofxPoint2f& pnt, float p );



    ofxPoint2f 	getMiddle( const ofxPoint2f& pnt ) const;

    ofxPoint2f& middle( const ofxPoint2f& pnt );



    // Average (centroid) among points.
    // Addition is sometimes useful for calculating averages too.
    //
    //
    ofxPoint2f& average( const ofxPoint2f* points, int num );


    // Perpendicular normalized vector from two points.
    //
    //
    ofxVec2f getPerpendicular( const ofxPoint2f& pnt ) const;




    //---------------------------------------------------
    // This methods are deprecated in 006 please use:

    // use getRotated
    ofxPoint2f rotated( float angle, const ofxPoint2f& pivot ) const;

    // use getMapped
    ofxPoint2f mapped( const ofxPoint2f& origin,
                          const ofxVec2f& vx,
                          const ofxVec2f& vy ) const;

    // use squareDistance
    float distanceSquared( const ofxPoint2f& pnt ) const;

    // use getInterpolated
    ofxPoint2f interpolated( const ofxPoint2f& pnt, float p ) const;

    // use getMiddle
    ofxPoint2f middled( const ofxPoint2f& pnt ) const;

    // use getPerpendicular
    ofxVec2f perpendicular( const ofxPoint2f& pnt ) const;
};


// Non-Member operators
//
//
ofxPoint2f operator+( float f, const ofxPoint2f& pnt );

ofxPoint2f operator-( float f, const ofxPoint2f& pnt );

ofxPoint2f operator*( float f, const ofxPoint2f& pnt );

ofxPoint2f operator/( float f, const ofxPoint2f& pnt );


#endif
