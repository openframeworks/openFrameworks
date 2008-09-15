#ifndef _OFX_VEC2f
#define _OFX_VEC2f

#include "ofConstants.h"




class ofxVec2f : public ofPoint {


  public:

    ofxVec2f( float _x=0.0f, float _y=0.0f ) {
        x = _x;
        y = _y;
    }

    // Getters and Setters.
    //
    //
    void set( float _x, float _y ) {
        x = _x;
        y = _y;
    }

    void set( const ofPoint& vec ) {
        x = vec.x;
        y = vec.y;
    }

    float &operator[]( const int& i ) {
        switch(i) {
            case 0:  return x;
            case 1:  return y;
            default: return x;
        }
    }



    // Check similarity/equality.
    //
    //
    bool operator==( const ofPoint& vec ) {
        return (x == vec.x) && (y == vec.y);
    }

    bool operator!=( const ofPoint& vec ) {
        return (x != vec.x) || (y != vec.y);
    }

    bool match( const ofPoint& vec, float tollerance=0.0001 ) {
        return (fabs(x - vec.x) < tollerance)
            && (fabs(y - vec.y) < tollerance);
    }

    /**
    * Checks if vectors look in the same direction.
    * Tollerance is specified in degree.
    */
    bool align( const ofxVec2f& vec, float tollerance=0.0001 ) const {
        return  fabs( this->angle( vec ) ) < tollerance;
    }



    // Overloading for any type to any type
    //
    //
    void operator=( const ofPoint& vec ){
        x = vec.x;
        y = vec.y;
    }

    ofxVec2f operator+( const ofPoint& vec ) const {
        return ofxVec2f( x+vec.x, y+vec.y);
    }

    ofxVec2f& operator+=( const ofPoint& vec ) {
        x += vec.x;
        y += vec.y;
        return *this;
    }

    ofxVec2f operator-( const ofPoint& vec ) const {
        return ofxVec2f(x-vec.x, y-vec.y);
    }

    ofxVec2f& operator-=( const ofPoint& vec ) {
        x -= vec.x;
        y -= vec.y;
        return *this;
    }

    ofxVec2f operator*( const ofPoint& vec ) const {
        return ofxVec2f(x*vec.x, y*vec.y);
    }

    ofxVec2f& operator*=( const ofPoint& vec ) {
        x*=vec.x;
        y*=vec.y;
        return *this;
    }

    ofxVec2f operator/( const ofPoint& vec ) const {
        return ofxVec2f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y);
    }

    ofxVec2f& operator/=( const ofPoint& vec ) {
        vec.x!=0 ? x/vec.x : x;
        vec.y!=0 ? y/vec.y : y;
        return *this;
    }


    //operator overloading for float
    //
    //
    void operator=( const float f){
        x = f;
        y = f;
    }

    ofxVec2f operator+( const float f ) const {
        return ofxVec2f( x+f, y+f);
    }

    ofxVec2f& operator+=( const float f ) {
        x += f;
        y += f;
        return *this;
    }

    ofxVec2f operator-( const float f ) const {
        return ofxVec2f( x-f, y-f);
    }

    ofxVec2f& operator-=( const float f ) {
        x -= f;
        y -= f;
        return *this;
    }

    ofxVec2f operator-() const {
        return ofxVec2f(-x, -y);
    }

    ofxVec2f operator*( const float f ) const {
        return ofxVec2f(x*f, y*f);
    }

    ofxVec2f& operator*=( const float f ) {
        x*=f;
        y*=f;
        return *this;
    }

    ofxVec2f operator/( const float f ) const {
        if(f == 0) return ofxVec2f(x, y);

        return ofxVec2f(x/f, y/f);
    }

    ofxVec2f& operator/=( const float f ) {
        if(f == 0) return *this;

        x/=f;
        y/=f;
        return *this;
    }


    ofxVec2f rescaled( const float length ) const {
        float l = (float)sqrt(x*x + y*y);
        if( l > 0 )
            return ofxVec2f( (x/l)*length, (y/l)*length );
        else
            return ofxVec2f();
    }

    ofxVec2f& rescale( const float length ) {
        float l = (float)sqrt(x*x + y*y);
        if (l > 0) {
            x = (x/l)*length;
            y = (y/l)*length;
        }
        return *this;
    }



    // Rotation
    //
    //
    ofxVec2f rotated( float angle ) const {
        float a = angle * DEG_TO_RAD;
        return ofxVec2f( x*cos(a) - y*sin(a),
                        x*sin(a) + y*cos(a) );
    }

    ofxVec2f& rotate( float angle ) {
        float a = angle * DEG_TO_RAD;
        float xrot = x*cos(a) - y*sin(a);
        y = x*sin(a) + y*cos(a);
        x = xrot;
        return *this;
    }



    // Normalization
    //
    //
    ofxVec2f normalized() const {
        float length = (float)sqrt(x*x + y*y);
        if( length > 0 ) {
            return ofxVec2f( x/length, y/length );
        } else {
            return ofxVec2f();
        }
    }

    ofxVec2f& normalize() {
        float length = (float)sqrt(x*x + y*y);
        if( length > 0 ) {
            x /= length;
            y /= length;
        }
        return *this;
	}



    // Limit length.
    //
    //
	 ofxVec2f limited(float max) const {
        float length = (float)sqrt(x*x + y*y);
		if( length > max && length > 0 ) {
            return ofxVec2f( (x/length)*max, (y/length)*max );
		} else {
            return ofxVec2f( x, y );
        }
	}

    ofxVec2f& limit(float max) {
        float length = (float)sqrt(x*x + y*y);
		if( length > max && length > 0 ) {
            x = (x/length)*max;
            y = (y/length)*max;
        }
        return *this;
    }



    // Perpendicular normalized vector.
    //
    //
    ofxVec2f perpendiculared() const {
        float length = (float)sqrt( x*x + y*y );
        if( length > 0 )
            return ofxVec2f( -(y/length), x/length );
        else
            return ofxVec2f();
    }

    ofxVec2f& perpendicular() {
        float length = (float)sqrt( x*x + y*y );
        if( length > 0 ) {
            x = -(y/length);
            y = x/length;
        }
        return *this;
    }


    // Length
    //
    //
    float length() const {
        return (float)sqrt( x*x + y*y );
    }

    float lengthSquared() const {
        return (float)(x*x + y*y);
    }



    /**
    * Angle (deg) between two vectors.
    * This is a signed relative angle between -180 and 180.
    */
    float angle( const ofxVec2f& vec ) const {
        return atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y )*RAD_TO_DEG;
    }



    /**
    * Dot Product.
    */
    float dot( const ofxVec2f& vec ) const {
        return x*vec.x + y*vec.y;
    }

};





// Non-Member operators
//
//
static inline ofxVec2f operator+( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f+vec.x, f+vec.y);
}

static inline ofxVec2f operator-( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f-vec.x, f-vec.y);
}

static inline ofxVec2f operator*( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f*vec.x, f*vec.y);
}

static inline ofxVec2f operator/( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f/vec.x, f/vec.y);
}



#endif
