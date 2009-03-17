#ifndef _OFX_VEC2f
#define _OFX_VEC2f

#include "ofConstants.h"
#include "ofTypes.h"



class ofxVec2f : public ofPoint {


  public:

    ofxVec2f( float _x=0.0f, float _y=0.0f );

    ofxVec2f( const ofPoint& pnt );



    // Getters and Setters.
    //
    //
    void set( float _x, float _y );

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
    * Tollerance is specified in degree.
    */
    bool align( const ofxVec2f& vec, float tollerance=0.0001 ) const;
    bool alignRad( const ofxVec2f& vec, float tollerance=0.0001 ) const;



    // Overloading for any type to any type
    //
    //
    void 	  operator=( const ofPoint& vec );

    ofxVec2f  operator+( const ofPoint& vec ) const;

    ofxVec2f& operator+=( const ofPoint& vec );

    ofxVec2f  operator-( const ofPoint& vec ) const;

    ofxVec2f& operator-=( const ofPoint& vec );

    ofxVec2f  operator*( const ofPoint& vec ) const;

    ofxVec2f& operator*=( const ofPoint& vec );

    ofxVec2f  operator/( const ofPoint& vec ) const;

    ofxVec2f& operator/=( const ofPoint& vec );


    //operator overloading for float
    //
    //
    void 	  operator=( const float f);

    ofxVec2f  operator+( const float f ) const;

    ofxVec2f& operator+=( const float f );

    ofxVec2f  operator-( const float f ) const;

    ofxVec2f& operator-=( const float f );

    ofxVec2f  operator-() const;

    ofxVec2f  operator*( const float f ) const;

    ofxVec2f& operator*=( const float f );

    ofxVec2f  operator/( const float f ) const;

    ofxVec2f& operator/=( const float f );


    // Scaling
    //
    //


    ofxVec2f  getScaled( const float length ) const;

    ofxVec2f& scale( const float length );



    // Rotation
    //
    //
    ofxVec2f  getRotated( float angle ) const;
    ofxVec2f  getRotatedRad( float angle ) const;

    ofxVec2f& rotate( float angle );
    ofxVec2f& rotateRad( float angle );



    // Normalization
    //
    //
    ofxVec2f  getNormalized() const;

    ofxVec2f& normalize();



    // Limit length.
    //
    //
	ofxVec2f  getLimited(float max) const;

    ofxVec2f& limit(float max);



    // Perpendicular normalized vector.
    //
    //
    ofxVec2f  getPerpendicular() const;

    ofxVec2f& perpendicular();


    // Length
    //
    //
    float length() const;

    float squareLength() const;


    /**
    * Angle (deg) between two vectors.
    * This is a signed relative angle between -180 and 180.
    */
    float angle( const ofxVec2f& vec ) const;
    float angleRad( const ofxVec2f& vec ) const;


    /**
    * Dot Product.
    */
    float dot( const ofxVec2f& vec ) const;



    //---------------------------------------------------
    // this methods are deprecated in 006 please use:

    // getScaled
    ofxVec2f rescaled( const float length ) const;

    // scale
    ofxVec2f& rescale( const float length );

    // getRotated
    ofxVec2f rotated( float angle ) const;

    // getNormalized
    ofxVec2f normalized() const;

    // getLimited
    ofxVec2f limited(float max) const;

    // getPerpendicular
    ofxVec2f perpendiculared() const;

    // squareLength
    float lengthSquared() const;

};





// Non-Member operators
//
//
ofxVec2f operator+( float f, const ofxVec2f& vec );

ofxVec2f operator-( float f, const ofxVec2f& vec );

ofxVec2f operator*( float f, const ofxVec2f& vec );

ofxVec2f operator/( float f, const ofxVec2f& vec );




#endif
