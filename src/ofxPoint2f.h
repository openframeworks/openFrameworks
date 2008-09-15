/**
*  ofxPoint2f.h
*  by stefanix
*/


#ifndef _OFX_POINT2f
#define _OFX_POINT2f

#include "ofxVec2f.h"


class ofxPoint2f : public ofPoint {


  public:

    ofxPoint2f( float _x=0.0f, float _y=0.0f ) {
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

    void set( const ofPoint& pnt ) {
        x = pnt.x;
        y = pnt.y;
    }

    float& operator[]( const int& i ) {
        switch(i) {
            case 0:  return x;
            case 1:  return y;
            default: return x;
        }
    }



    // Check similarity/equality.
    //
    //
    bool operator==( const ofPoint& pnt ) {
        return (x == pnt.x) && (y == pnt.y);
    }

    bool operator!=( const ofPoint& pnt ) {
        return (x != pnt.x) || (y != pnt.y);
    }

    bool match( const ofPoint& pnt, float tollerance=0.0001) {
        return (fabs(x - pnt.x) < tollerance)
            && (fabs(y - pnt.y) < tollerance);
    }



    // Overloading for any type to any type
    //
    //
    void operator=( const ofPoint& vec ){
        x = vec.x;
        y = vec.y;
    }

    ofxPoint2f operator+( const ofPoint& vec ) const {
        return ofxPoint2f( x+vec.x, y+vec.y);
    }

    ofxPoint2f& operator+=( const ofPoint& vec ) {
        x += vec.x;
        y += vec.y;
        return *this;
    }

    ofxPoint2f operator-( const ofPoint& vec ) const {
        return ofxPoint2f(x-vec.x, y-vec.y);
    }

    ofxPoint2f& operator-=( const ofPoint& vec ) {
        x -= vec.x;
        y -= vec.y;
        return *this;
    }

    ofxPoint2f operator*( const ofPoint& vec ) const {
        return ofxPoint2f(x*vec.x, y*vec.y);
    }

    ofxPoint2f& operator*=( const ofPoint& vec ) {
        x*=vec.x;
        y*=vec.y;
        return *this;
    }

    ofxPoint2f operator/( const ofPoint& vec ) const {
        return ofxPoint2f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y);
    }

    ofxPoint2f& operator/=( const ofPoint& vec ) {
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

    ofxPoint2f operator+( const float f ) const {
        return ofxPoint2f( x+f, y+f);
    }

    ofxPoint2f& operator+=( const float f ) {
        x += f;
        y += f;
        return *this;
    }

    ofxPoint2f operator-( const float f ) const {
        return ofxPoint2f( x-f, y-f);
    }

    ofxPoint2f& operator-=( const float f ) {
        x -= f;
        y -= f;
        return *this;
    }

    ofxPoint2f operator-() const {
        return ofxPoint2f(-x, -y);
    }

    ofxPoint2f operator*( const float f ) const {
        return ofxPoint2f(x*f, y*f);
    }

    ofxPoint2f& operator*=( const float f ) {
        x*=f;
        y*=f;
        return *this;
    }

    ofxPoint2f operator/( const float f ) const {
        if(f == 0) return ofxPoint2f(x, y);

        return ofxPoint2f(x/f, y/f);
    }

    ofxPoint2f& operator/=( const float f ) {
        if(f == 0) return *this;

        x/=f;
        y/=f;
        return *this;
    }


    // Rotate point by angle (deg) around pivot point.
    //
    //
    ofxPoint2f rotated( float angle, const ofxPoint2f& pivot ) const {
        float a = angle * DEG_TO_RAD;
        return ofxPoint2f( ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x,
                          ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y );
    }

    ofxPoint2f& rotate( float angle, const ofxPoint2f& pivot ) {
        float a = angle * DEG_TO_RAD;
        float xrot = ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x;
        y = ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y;
        x = xrot;
        return *this;
    }



    // Map point to coordinate system defined by origin, vx, and vy.
    //
    //
    ofxPoint2f mapped( const ofxPoint2f& origin,
                      const ofxVec2f& vx,
                      const ofxVec2f& vy ) const
    {
        return ofxPoint2f( origin.x + x*vx.x + y*vy.x,
                          origin.y + x*vx.y + y*vy.y );
    }

    ofxPoint2f& map( const ofxPoint2f& origin,
                    const ofxVec2f& vx, const ofxVec2f& vy )
    {
        float xmap = origin.x + x*vx.x + y*vy.x;
        y = origin.y + x*vx.y + y*vy.y;
        x = xmap;
        return *this;
    }


    // Distance between two points.
    //
    //
    float distance( const ofxPoint2f& pnt) const {
        float vx = x-pnt.x;
        float vy = y-pnt.y;
        return (float)sqrt(vx*vx + vy*vy);
    }

    float distanceSquared( const ofxPoint2f& pnt ) const {
        float vx = x-pnt.x;
        float vy = y-pnt.y;
        return vx*vx + vy*vy;
    }



    // Linear interpolation.
    //
    //
    /**
    * p==0.0 results in this point, p==0.5 results in the
    * midpoint, and p==1.0 results in pnt being returned.
    */
    ofxPoint2f interpolated( const ofxPoint2f& pnt, float p ) const {
        return ofxPoint2f( x*(1-p) + pnt.x*p, y*(1-p) + pnt.y*p );
    }

    ofxPoint2f& interpolate( const ofxPoint2f& pnt, float p ) {
        x = x*(1-p) + pnt.x*p;
        y = y*(1-p) + pnt.y*p;
        return *this;
    }

    ofxPoint2f middled( const ofxPoint2f& pnt ) const {
        return ofxPoint2f( (x+pnt.x)/2.0, (y+pnt.y)/2.0 );
    }

    ofxPoint2f& middle( const ofxPoint2f& pnt ) {
        x = (x+pnt.x)/2.0;
        y = (y+pnt.y)/2.0;
        return *this;
    }



    // Average (centroid) among points.
    // Addition is sometimes useful for calculating averages too.
    //
    //
    ofxPoint2f& average( const ofxPoint2f* points, int num ) {
        x = 0.f;
        y = 0.f;
        for( int i=0; i<num; i++) {
            x += points[i].x;
            y += points[i].y;
        }
        x /= num;
        y /= num;
        return *this;
    }


    // Perpendicular normalized vector from two points.
    //
    //
    ofxVec2f perpendicular( const ofxPoint2f& pnt ) const {
        //float a = -atan2( pnt.y - y, pnt.x - x );
        //return ofxVec2f( sin(a), cos(a) );

        //the following seems simpler and about 3x faster

        float vy = -(x - pnt.x);
        float vx = y - pnt.y;
        float length = (float)sqrt(vx*vx + vy*vy);
        if( length > 0 )
            return ofxVec2f( vx/length, vy/length );
        else
            return ofxVec2f(x, y);
    }


};


// Non-Member operators
//
//
static inline ofxPoint2f operator+( float f, const ofxPoint2f& pnt ) {
    return ofxPoint2f( f+pnt.x, f+pnt.y);
}

static inline ofxPoint2f operator-( float f, const ofxPoint2f& pnt ) {
    return ofxPoint2f( f-pnt.x, f-pnt.y);
}

static inline ofxPoint2f operator*( float f, const ofxPoint2f& pnt ) {
    return ofxPoint2f( f*pnt.x, f*pnt.y);
}

static inline ofxPoint2f operator/( float f, const ofxPoint2f& pnt ) {
    return ofxPoint2f( f/pnt.x, f/pnt.y);
}


#endif
