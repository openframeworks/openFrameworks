/**
*  ofxPoint3f.h
*  by stefanix
*/


#ifndef _OFX_POINT3f
#define _OFX_POINT3f

#include "ofxVec3f.h"
#include "ofTypes.h"


class ofxPoint3f : public ofPoint {


  public:

    ofxPoint3f( float _x=0.0f, float _y=0.0f, float _z=0.0f );

    ofxPoint3f( const ofPoint& pnt );



    // Getters and Setters.
    //
    //
    void set( float _x, float _y, float _z );

    void set( const ofPoint& pnt );


    float& operator[]( const int& i );


    // Check similarity/equality.
    //
    //
    bool operator==( const ofPoint& pnt );

    bool operator!=( const ofPoint& pnt );

    bool match( const ofPoint& pnt, float tollerance=0.0001);



    // Operator overloading for ofPoint
    //
    //

   void 		operator=( const ofPoint& vec );

    ofxPoint3f 	operator+( const ofPoint& pnt ) const;

    ofxPoint3f& operator+=( const ofPoint& pnt );

    ofxPoint3f 	operator-( const ofPoint& vec ) const;

    ofxPoint3f& operator-=( const ofPoint& vec );

    ofxPoint3f 	operator*( const ofPoint& vec ) const;

    ofxPoint3f& operator*=( const ofPoint& vec );

    ofxPoint3f 	operator/( const ofPoint& vec ) const;

    ofxPoint3f& operator/=( const ofPoint& vec );

    ofxPoint3f 	operator-() const;


    //operator overloading for float
    //
    //
    void 		operator=( const float f);

    ofxPoint3f 	operator+( const float f ) const;

    ofxPoint3f& operator+=( const float f );

 	ofxPoint3f 	operator-( const float f ) const;

    ofxPoint3f& operator-=( const float f );

    ofxPoint3f 	operator*( const float f ) const;

    ofxPoint3f& operator*=( const float f );

    ofxPoint3f 	operator/( const float f ) const;

    ofxPoint3f& operator/=( const float f );



    // Rotate point by angle (deg) around line defined by pivot and axis.
    //
    //


    ofxPoint3f 	getRotated( float angle,
                        const ofxPoint3f& pivot,
                        const ofxVec3f& axis ) const;

    ofxPoint3f& rotate( float angle,
                        const ofxPoint3f& pivot,
                        const ofxVec3f& axis );

    ofxPoint3f 	getRotatedRad( float angle,
                        const ofxPoint3f& pivot,
                        const ofxVec3f& axis ) const;

    ofxPoint3f& rotateRad( float angle,
                        const ofxPoint3f& pivot,
                        const ofxVec3f& axis );


    // Map point to coordinate system defined by origin, vx, vy, and vz.
    //
    //


    ofxPoint3f 	getMapped( const ofxPoint3f& origin,
                       const ofxVec3f& vx,
                       const ofxVec3f& vy,
                       const ofxVec3f& vz ) const;

    ofxPoint3f& map( const ofxPoint3f& origin,
                     const ofxVec3f& vx,
                     const ofxVec3f& vy,
                     const ofxVec3f& vz );


    // Distance between two points.
    //
    //
    float distance( const ofxPoint3f& pnt) const;


    float squareDistance( const ofxPoint3f& pnt ) const;



    // Linear interpolation.
    //
    //
    /**
    * p==0.0 results in this point, p==0.5 results in the
    * midpoint, and p==1.0 results in pnt being returned.
    */


    ofxPoint3f 	getInterpolated( const ofxPoint3f& pnt, float p ) const;

    ofxPoint3f& interpolate( const ofxPoint3f& pnt, float p );



    ofxPoint3f 	getMiddle( const ofxPoint3f& pnt ) const;

    ofxPoint3f& middle( const ofxPoint3f& pnt );


    // Average (centroid) among points.
    // Addition is sometimes useful for calculating averages too.
    //
    //
    ofxPoint3f& average( const ofxPoint3f* points, int num );





    // Perpendicular normalized vector from three points.
    //
    //

    ofxVec3f 	getPerpendicular( const ofxPoint3f& pnt1,
                            const ofxPoint3f& pnt2 ) const;






    //------------------------------------------
    // this methods are deprecated in 006, please use:

    // use getRotated
    ofxPoint3f 	rotated( float angle,
                            const ofxPoint3f& pivot,
                            const ofxVec3f& axis ) const;

    // use getMapped
    ofxPoint3f 	mapped( const ofxPoint3f& origin,
                           const ofxVec3f& vx,
                           const ofxVec3f& vy,
                           const ofxVec3f& vz ) const;

    // use squareDistance
    float		distanceSquared( const ofxPoint3f& pnt ) const;

    // use getInterpolated
    ofxPoint3f 	interpolated( const ofxPoint3f& pnt, float p ) const;

    // use getMiddle
    ofxPoint3f 	middled( const ofxPoint3f& pnt ) const;

    // use getPerpendicular
    ofxVec3f 	perpendicular( const ofxPoint3f& pnt1,
                                const ofxPoint3f& pnt2 ) const;
};





// Non-Member operators
//
//
ofxPoint3f operator+( float f, const ofxPoint3f& pnt );

ofxPoint3f operator-( float f, const ofxPoint3f& pnt );

ofxPoint3f operator*( float f, const ofxPoint3f& pnt );

ofxPoint3f operator/( float f, const ofxPoint3f& pnt );


#endif
