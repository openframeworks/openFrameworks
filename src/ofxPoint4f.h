/**
*  ofxPoint4f.h
*  by stefanix
*/


#ifndef _OFX_POINT4f
#define _OFX_POINT4f

#include "ofxVec4f.h"



class ofxPoint4f : public ofPoint {


  public:

    float w;


    ofxPoint4f( const float _x=0.0f,
                const float _y=0.0f,
                const float _z=0.0f,
                const float _w=0.0f )
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }




    // Getters and Setters.
    //
    //
    void set( float _x, float _y, float _z, float _w ) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    void set( const ofxPoint4f& pnt ) {
        x = pnt.x;
        y = pnt.y;
        z = pnt.z;
        w = pnt.w;
    }

    float& operator[]( const int& i ) {
        switch(i) {
            case 0:  return x;
            case 1:  return y;
            case 2:  return z;
            case 3:  return w;
            default: return x;
        }
    }



    // Check similarity/equality.
    //
    //
    bool operator==( const ofxPoint4f& pnt ) {
        return (x == pnt.x) && (y == pnt.y) && (z == pnt.z) && (w == pnt.w);
    }

    bool operator!=( const ofxPoint4f& pnt ) {
        return (x != pnt.x) || (y != pnt.y) || (z != pnt.z) || (w != pnt.w);
    }

    bool match( const ofxPoint4f& pnt, float tollerance=0.0001) {
        return (fabs(x - pnt.x) < tollerance)
            && (fabs(y - pnt.y) < tollerance)
            && (fabs(z - pnt.z) < tollerance)
            && (fabs(w - pnt.w) < tollerance);
    }



    // Translate point by vector.
    //
    //
    ofxPoint4f operator+( const ofxVec4f& vec ) const {
        return ofxPoint4f( x+vec.x, y+vec.y, z+vec.z, w+vec.w);
    }

    ofxPoint4f& operator+=( const ofxVec4f& vec ) {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        w += vec.w;
        return *this;
    }

	ofxPoint4f operator+( const float f ) const {
        return ofxPoint4f( x+f, y+f, z+f, w+f );
    }

    ofxPoint4f& operator+=( const float f ) {
        x+=f;
        y+=f;
        z+=f;
        w+=f;
        return *this;
    }

    ofxPoint4f operator-( const ofxVec4f& vec ) const {
        return ofxPoint4f( x-vec.x, y-vec.y, z-vec.z, w-vec.w );
    }

    ofxPoint4f& operator-=( const ofxVec4f& vec ) {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        w -= vec.w;
        return *this;
    }

	ofxPoint4f operator-( const float f ) const {
        return ofxPoint4f( x-f, y-f, z-f, w-f );
    }

    ofxPoint4f& operator-=( const float f ) {
        x-=f;
        y-=f;
        z-=f;
        w-=f;
        return *this;
    }

    ofxPoint4f operator-() const {
        return ofxPoint4f( -x, -y, -z, -w );
    }



    // Scale point (by scalar or vector).
    //
    //
    ofxPoint4f operator*( const float f ) const {
        return ofxPoint4f( x*f, y*f, z*f, w*f );
    }

    ofxPoint4f& operator*=( const float f ) {
        x*=f;
        y*=f;
        z*=f;
        w*=f;
        return *this;
    }

    ofxPoint4f operator*( const ofxVec4f& vec ) const {
        return ofxPoint4f( x*vec.x, y*vec.y, z*vec.z, w*vec.w );
    }

    ofxPoint4f& operator*=( const ofxVec4f& vec ) {
        x*=vec.x;
        y*=vec.y;
        z*=vec.z;
        w*=vec.w;
        return *this;
    }

    ofxPoint4f operator/( const float f ) const {
        if(f == 0) return ofxPoint4f(x, y, z, w);

        return ofxPoint4f( x/f, y/f, z/f, w/f );
    }

    ofxPoint4f& operator/=( const float f ) {
        if(f == 0)return *this;

        x/=f;
        y/=f;
        z/=f;
        w/=f;
        return *this;
    }

    ofxPoint4f operator/( const ofxVec4f& vec ) const
    {
        return ofxPoint4f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z, vec.w!=0 ? w/vec.w : w );
    }

    ofxPoint4f& operator/=( const ofxVec4f& vec ) {
        vec.x!=0 ? x/vec.x : x;
        vec.y!=0 ? y/vec.y : y;
        vec.z!=0 ? z/vec.z : z;
        vec.w!=0 ? w/vec.w : w;
        return *this;
    }


    /**
    * Return the difference vector between two points.
    */
    ofxVec4f operator-( const ofxPoint4f& pnt ) const {
        return ofxVec4f( x-pnt.x, y-pnt.y, z-pnt.z, w-pnt.w );
    }



    // Distance between two points.
    //
    //
    float distance( const ofxPoint4f& pnt) const {
        float vx = x-pnt.x;
        float vy = y-pnt.y;
        float vz = z-pnt.z;
        float vw = w-pnt.w;
        return (float)sqrt( vx*vx + vy*vy + vz*vz + vw*vw );
    }

    float distanceSquared( const ofxPoint4f& pnt ) const {
        float vx = x-pnt.x;
        float vy = y-pnt.y;
        float vz = z-pnt.z;
        float vw = w-pnt.w;
        return vx*vx + vy*vy + vz*vz + vw*vw;
    }



    // Linear interpolation.
    //
    //
    /**
    * p==0.0 results in this point, p==0.5 results in the
    * midpoint, and p==1.0 results in pnt being returned.
    */
    ofxPoint4f interpolated( const ofxPoint4f& pnt, float p ) const {
        return ofxPoint4f( x*(1-p) + pnt.x*p,
                          y*(1-p) + pnt.y*p,
                          z*(1-p) + pnt.z*p,
                          w*(1-p) + pnt.w*p );
    }

    ofxPoint4f& interpolate( const ofxPoint4f& pnt, float p ) {
        x = x*(1-p) + pnt.x*p;
        y = y*(1-p) + pnt.y*p;
        z = z*(1-p) + pnt.z*p;
        w = w*(1-p) + pnt.w*p;
        return *this;
    }

    ofxPoint4f middled( const ofxPoint4f& pnt ) const {
        return ofxPoint4f( (x+pnt.x)/2.0, (y+pnt.y)/2.0,
                           (z+pnt.z)/2.0, (w+pnt.w)/2.0 );
    }

    ofxPoint4f& middle( const ofxPoint4f& pnt ) {
        x = (x+pnt.x)/2.0;
        y = (y+pnt.y)/2.0;
        z = (z+pnt.z)/2.0;
        w = (w+pnt.w)/2.0;
        return *this;
    }


    // Average (centroid) among points.
    // (Addition is sometimes useful for calculating averages too)
    //
    //
    ofxPoint4f& average( const ofxPoint4f* points, int num ) {
        x = 0.f;
        y = 0.f;
        z = 0.f;
        w = 0.f;
        for( int i=0; i<num; i++) {
            x += points[i].x;
            y += points[i].y;
            z += points[i].z;
            w += points[i].w;
        }
        x /= num;
        y /= num;
        z /= num;
        w /= num;
        return *this;
    }

    ofxPoint4f operator+( const ofxPoint4f& pnt ) const {
        return ofxPoint4f( x+pnt.x, y+pnt.y, z+pnt.z,  w+pnt.w );
    }

    ofxPoint4f& operator+=( const ofxPoint4f& pnt ) {
        x+=pnt.x;
        y+=pnt.y;
        z+=pnt.z;
        w+=pnt.w;
        return *this;
    }


};





// Non-Member operators
//
//
static inline ofxPoint4f operator+( float f, const ofxPoint4f& pnt ) {
    return ofxPoint4f( f+pnt.x, f+pnt.y, f+pnt.z, f+pnt.w );
}

static inline ofxPoint4f operator-( float f, const ofxPoint4f& pnt ) {
    return ofxPoint4f( f-pnt.x, f-pnt.y, f-pnt.z, f-pnt.w );
}

static inline ofxPoint4f operator*( float f, const ofxPoint4f& pnt ) {
    return ofxPoint4f( f*pnt.x, f*pnt.y, f*pnt.z, f*pnt.w );
}

static inline ofxPoint4f operator/( float f, const ofxPoint4f& pnt ) {
    return ofxPoint4f( f/pnt.x, f/pnt.y, f/pnt.z, f/pnt.w);
}


#endif
