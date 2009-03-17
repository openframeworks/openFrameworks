#ifndef _OFX_VEC3f
#define _OFX_VEC3f

#include "ofConstants.h"
#include "ofTypes.h"



class ofxVec3f : public ofPoint {


  public:

    ofxVec3f( float _x=0.0f,
              float _y=0.0f,
              float _z=0.0f );

    ofxVec3f( const ofPoint& pnt );



    // Getters and Setters.
    //
    //
    void set( float _x, float _y, float _z );

    void set( const ofPoint& vec );



    float &operator[]( const int& i );



    // Check similarity/equality.
    //
    //
    bool operator==( const ofPoint& vec );

    bool operator!=( const ofPoint& vec );

    bool match( const ofPoint& vec, float tollerance=0.0001 );

    /**
    * Checks if vectors look in the same direction.
    */
    bool align( const ofxVec3f& vec, float tollerance=0.0001 ) const;
    bool alignRad( const ofxVec3f& vec, float tollerance=0.0001 ) const;


    // Operator overloading for ofPoint
    //
    //

    void 	  operator=( const ofPoint& vec );

    ofxVec3f  operator+( const ofPoint& pnt ) const;

    ofxVec3f& operator+=( const ofPoint& pnt );

    ofxVec3f  operator-( const ofPoint& vec ) const;

    ofxVec3f& operator-=( const ofPoint& vec );

    ofxVec3f  operator*( const ofPoint& vec ) const;

    ofxVec3f& operator*=( const ofPoint& vec );

    ofxVec3f  operator/( const ofPoint& vec ) const;

    ofxVec3f& operator/=( const ofPoint& vec );

    ofxVec3f  operator-() const;


    //operator overloading for float
    //
    //
    void 	  operator=( const float f);

    ofxVec3f  operator+( const float f ) const;

    ofxVec3f& operator+=( const float f );

 	ofxVec3f  operator-( const float f ) const;

    ofxVec3f& operator-=( const float f );

    ofxVec3f  operator*( const float f ) const;

    ofxVec3f& operator*=( const float f );

    ofxVec3f  operator/( const float f ) const;

    ofxVec3f& operator/=( const float f );


    //Scale
    //
    //
    ofxVec3f  getScaled( const float length ) const;

    ofxVec3f& scale( const float length );



    // Rotation
    //
    //
    ofxVec3f  getRotated( float angle, const ofxVec3f& axis ) const;
    ofxVec3f  getRotatedRad( float angle, const ofxVec3f& axis ) const;

    ofxVec3f& rotate( float angle, const ofxVec3f& axis );
    ofxVec3f& rotateRad( float angle, const ofxVec3f& axis );


    ofxVec3f  getRotated(float ax, float ay, float az) const;
    ofxVec3f  getRotatedRad(float ax, float ay, float az) const;

    ofxVec3f& rotate(float ax, float ay, float az);
    ofxVec3f& rotateRad(float ax, float ay, float az);



    // Normalization
    //
    //
    ofxVec3f  getNormalized() const;

    ofxVec3f& normalize();



    // Limit length.
    //
    //


    ofxVec3f  getLimited(float max) const;

    ofxVec3f& limit(float max);


    // Perpendicular vector.
    //
    //
    ofxVec3f  getCrossed( const ofxVec3f& vec ) const;

    ofxVec3f& cross( const ofxVec3f& vec );

    /**
    * Normalized perpendicular.
    */
    ofxVec3f  getPerpendicular( const ofxVec3f& vec ) const;

    ofxVec3f& perpendicular( const ofxVec3f& vec );


    // Length
    //
    //
    float length() const;

    float squareLength() const;



    /**
    * Angle (deg) between two vectors.
    * This is an unsigned relative angle from 0 to 180.
    * http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
    */
    float angle( const ofxVec3f& vec ) const;
    float angleRad( const ofxVec3f& vec ) const;



    /**
    * Dot Product.
    */
    float dot( const ofxVec3f& vec ) const;




    //-----------------------------------------------
    // this methods are deprecated in 006 please use:

    // getScaled
    ofxVec3f rescaled( const float length ) const;

    // scale
    ofxVec3f& rescale( const float length );

    // getRotated
    ofxVec3f rotated( float angle, const ofxVec3f& axis ) const;

    // getRotated should this be const???
    ofxVec3f rotated(float ax, float ay, float az);

    // getNormalized
    ofxVec3f normalized() const;

    // getLimited
    ofxVec3f limited(float max) const;

    // getCrossed
    ofxVec3f crossed( const ofxVec3f& vec ) const;

    // getPerpendicular
    ofxVec3f perpendiculared( const ofxVec3f& vec ) const;

    // squareLength
    float lengthSquared() const;
};




// Non-Member operators
//
//
ofxVec3f operator+( float f, const ofxVec3f& vec );

ofxVec3f operator-( float f, const ofxVec3f& vec );

ofxVec3f operator*( float f, const ofxVec3f& vec );

ofxVec3f operator/( float f, const ofxVec3f& vec );


#endif
