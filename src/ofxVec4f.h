#ifndef _OFX_VEC4f
#define _OFX_VEC4f

#include "ofConstants.h"
#include "ofTypes.h"


class ofxVec4f : public ofPoint {


  public:

    float w;


    ofxVec4f( float _x=0.0f,
              float _y=0.0f,
              float _z=0.0f,
              float _w=0.0f );



    // Getters and Setters.
    //
    //
    void set( float _x, float _y, float _z, float _w );

    void set( const ofxVec4f& vec );



    float &operator[]( const int& i );



    // Check similarity/equality.
    //
    //
    bool operator==( const ofxVec4f& vec );

    bool operator!=( const ofxVec4f& vec );

    bool match( const ofxVec4f& vec, float tollerance=0.0001);




    // Additions and Subtractions.
    //
    //
    ofxVec4f  operator+( const ofxVec4f& vec ) const;

    ofxVec4f& operator+=( const ofxVec4f& vec );

    ofxVec4f  operator-( const float f ) const;

    ofxVec4f& operator-=( const float f );

    ofxVec4f  operator-( const ofxVec4f& vec ) const;

    ofxVec4f& operator-=( const ofxVec4f& vec );

    ofxVec4f  operator+( const float f ) const;

    ofxVec4f& operator+=( const float f );

    ofxVec4f  operator-() const;


    // Scalings
    //
    //
    ofxVec4f  operator*( const ofxVec4f& vec ) const;

    ofxVec4f& operator*=( const ofxVec4f& vec );

    ofxVec4f  operator*( const float f ) const;

    ofxVec4f& operator*=( const float f );

    ofxVec4f  operator/( const ofxVec4f& vec ) const;

    ofxVec4f& operator/=( const ofxVec4f& vec );

    ofxVec4f  operator/( const float f ) const;

    ofxVec4f& operator/=( const float f );




    ofxVec4f  getScaled( const float length ) const;

    ofxVec4f& scale( const float length );



    // Normalization
    //
    //

    ofxVec4f  getNormalized() const;

    ofxVec4f& normalize();



    // Limit length.
    //
    //

	ofxVec4f  getLimited(float max) const;

    ofxVec4f& limit(float max);



    // Length
    //
    //
    float length() const;

    float squareLength() const;



    /**
    * Dot Product.
    */
    float dot( const ofxVec4f& vec ) const;




    //---------------------------------------
    // this methods are deprecated in 006 please use:

    // getScaled
    ofxVec4f rescaled( const float length ) const;

    // scale
    ofxVec4f& rescale( const float length );

    // getNormalized
    ofxVec4f normalized() const;

    // getLimited
    ofxVec4f limited(float max) const;

    // squareLength
    float lengthSquared() const;
};




// Non-Member operators
//
//
ofxVec4f operator+( float f, const ofxVec4f& vec );

ofxVec4f operator-( float f, const ofxVec4f& vec );

ofxVec4f operator*( float f, const ofxVec4f& vec );

ofxVec4f operator/( float f, const ofxVec4f& vec );


#endif
