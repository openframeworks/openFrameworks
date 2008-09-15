/**
*  ofxPoint3f.h
*  by stefanix
*/


#ifndef _OFX_POINT3f
#define _OFX_POINT3f

#include "ofxVec3f.h"



class ofxPoint3f : public ofPoint {


  public:

    ofxPoint3f( float _x=0.0f, float _y=0.0f, float _z=0.0f ) {
        x = _x;
        y = _y;
        z = _z;
    }

    // Getters and Setters.
    //
    //
    void set( float _x, float _y, float _z ) {
        x = _x;
        y = _y;
        z = _z;
    }

    void set( const ofPoint& pnt ) {
        x = pnt.x;
        y = pnt.y;
        z = pnt.z;
    }

    float& operator[]( const int& i ) {
        switch(i) {
            case 0:  return x;
            case 1:  return y;
            case 2:  return z;
            default: return x;
        }
    }


    // Check similarity/equality.
    //
    //
    bool operator==( const ofPoint& pnt ) {
        return (x == pnt.x) && (y == pnt.y) && (z == pnt.z);
    }

    bool operator!=( const ofPoint& pnt ) {
        return (x != pnt.x) || (y != pnt.y) || (z != pnt.z);
    }

    bool match( const ofPoint& pnt, float tollerance=0.0001) {
        return (fabs(x - pnt.x) < tollerance)
            && (fabs(y - pnt.y) < tollerance)
            && (fabs(z - pnt.z) < tollerance);
    }



    // Operator overloading for ofPoint
    //
    //

   void operator=( const ofPoint& vec ){
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }

    ofxPoint3f operator+( const ofPoint& pnt ) const {
        return ofxPoint3f( x+pnt.x, y+pnt.y, z+pnt.z );
    }

    ofxPoint3f& operator+=( const ofPoint& pnt ) {
        x+=pnt.x;
        y+=pnt.y;
        z+=pnt.z;
        return *this;
    }

    ofxPoint3f operator-( const ofPoint& vec ) const {
        return ofxPoint3f( x-vec.x, y-vec.y, z-vec.z );
    }

    ofxPoint3f& operator-=( const ofPoint& vec ) {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        return *this;
    }

    ofxPoint3f operator*( const ofPoint& vec ) const {
        return ofxPoint3f( x*vec.x, y*vec.y, z*vec.z );
    }

    ofxPoint3f& operator*=( const ofPoint& vec ) {
        x*=vec.x;
        y*=vec.y;
        z*=vec.z;
        return *this;
    }

    ofxPoint3f operator/( const ofPoint& vec ) const {
        return ofxPoint3f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z );
    }

    ofxPoint3f& operator/=( const ofPoint& vec ) {
        vec.x!=0 ? x/vec.x : x;
        vec.y!=0 ? y/vec.y : y;
        vec.z!=0 ? z/vec.z : z;
        return *this;
    }

    ofxPoint3f operator-() const {
        return ofxPoint3f( -x, -y, -z );
    }


    //operator overloading for float
    //
    //
    void operator=( const float f){
        x = f;
        y = f;
        z = f;
    }

    ofxPoint3f operator+( const float f ) const {
        return ofxPoint3f( x+f, y+f, z+f);
    }

    ofxPoint3f& operator+=( const float f ) {
        x += f;
        y += f;
		z += f;
        return *this;
    }

 	ofxPoint3f operator-( const float f ) const {
        return ofxPoint3f( x-f, y-f, z-f);
    }

    ofxPoint3f& operator-=( const float f ) {
        x -= f;
        y -= f;
		z -= f;
        return *this;
    }

    ofxPoint3f operator*( const float f ) const {
        return ofxPoint3f( x*f, y*f, z*f );
    }

    ofxPoint3f& operator*=( const float f ) {
        x*=f;
        y*=f;
        z*=f;
        return *this;
    }

    ofxPoint3f operator/( const float f ) const {
         if(f == 0) return ofxPoint3f( x, y, z);

        return ofxPoint3f( x/f, y/f, z/f );
    }

    ofxPoint3f& operator/=( const float f ) {
        if(f == 0) return *this;

        x/=f;
        y/=f;
        z/=f;
        return *this;
    }



    // Rotate point by angle (deg) around line defined by pivot and axis.
    //
    //
    ofxPoint3f rotated( float angle,
                        const ofxPoint3f& pivot,
                        const ofxVec3f& axis ) const
    {
        ofxVec3f ax = axis.normalized();
        float tx = x - pivot.x;
        float ty = y - pivot.y;
        float tz = z - pivot.z;

        float a = angle*DEG_TO_RAD;
        float sina = sin( a );
        float cosa = cos( a );
        float cosb = 1.0 - cosa;

        float xrot = tx*(ax.x*ax.x*cosb + cosa)
                   + ty*(ax.x*ax.y*cosb - ax.z*sina)
                   + tz*(ax.x*ax.z*cosb + ax.y*sina);
        float yrot = tx*(ax.y*ax.x*cosb + ax.z*sina)
                   + ty*(ax.y*ax.y*cosb + cosa)
                   + tz*(ax.y*ax.z*cosb - ax.x*sina);
        float zrot = tx*(ax.z*ax.x*cosb - ax.y*sina)
                   + ty*(ax.z*ax.y*cosb + ax.x*sina)
                   + tz*(ax.z*ax.z*cosb + cosa);


        return ofxPoint3f( xrot+pivot.x, yrot+pivot.y, zrot+pivot.z );
    }

    ofxPoint3f& rotate( float angle,
                        const ofxPoint3f& pivot,
                        const ofxVec3f& axis )
    {
        ofxVec3f ax = axis.normalized();
        x -= pivot.x;
        y -= pivot.y;
        z -= pivot.z;

        float a = angle*DEG_TO_RAD;
        float sina = sin( a );
        float cosa = cos( a );
        float cosb = 1.0 - cosa;

        float xrot = x*(ax.x*ax.x*cosb + cosa)
                   + y*(ax.x*ax.y*cosb - ax.z*sina)
                   + z*(ax.x*ax.z*cosb + ax.y*sina);
        float yrot = x*(ax.y*ax.x*cosb + ax.z*sina)
                   + y*(ax.y*ax.y*cosb + cosa)
                   + z*(ax.y*ax.z*cosb - ax.x*sina);
        float zrot = x*(ax.z*ax.x*cosb - ax.y*sina)
                   + y*(ax.z*ax.y*cosb + ax.x*sina)
                   + z*(ax.z*ax.z*cosb + cosa);

        x = xrot + pivot.x;
        y = yrot + pivot.y;
        z = zrot + pivot.z;

        return *this;
    }



    // Map point to coordinate system defined by origin, vx, vy, and vz.
    //
    //
    ofxPoint3f mapped( const ofxPoint3f& origin,
                       const ofxVec3f& vx,
                       const ofxVec3f& vy,
                       const ofxVec3f& vz ) const
    {
        return ofxPoint3f( origin.x + x*vx.x + y*vy.x + z*vz.x,
                           origin.y + x*vx.y + y*vy.y + z*vz.y,
                           origin.z + x*vx.z + y*vy.z + z*vz.z );
    }

    ofxPoint3f& map( const ofxPoint3f& origin,
                     const ofxVec3f& vx,
                     const ofxVec3f& vy,
                     const ofxVec3f& vz )
    {
        float xmap = origin.x + x*vx.x + y*vy.x + z*vz.x;
        float ymap =  origin.y + x*vx.y + y*vy.y + z*vz.y;
        z = origin.z + x*vx.z + y*vy.z + z*vz.z;
        x = xmap;
        y = ymap;
        return *this;
    }


    // Distance between two points.
    //
    //
    float distance( const ofxPoint3f& pnt) const {
        float vx = x-pnt.x;
        float vy = y-pnt.y;
        float vz = z-pnt.z;
        return (float)sqrt(vx*vx + vy*vy + vz*vz);
    }

    float distanceSquared( const ofxPoint3f& pnt ) const {
        float vx = x-pnt.x;
        float vy = y-pnt.y;
        float vz = z-pnt.z;
        return vx*vx + vy*vy + vz*vz;
    }



    // Linear interpolation.
    //
    //
    /**
    * p==0.0 results in this point, p==0.5 results in the
    * midpoint, and p==1.0 results in pnt being returned.
    */
    ofxPoint3f interpolated( const ofxPoint3f& pnt, float p ) const {
        return ofxPoint3f( x*(1-p) + pnt.x*p,
                           y*(1-p) + pnt.y*p,
                           z*(1-p) + pnt.z*p );
    }

    ofxPoint3f& interpolate( const ofxPoint3f& pnt, float p ) {
        x = x*(1-p) + pnt.x*p;
        y = y*(1-p) + pnt.y*p;
        z = z*(1-p) + pnt.z*p;
        return *this;
    }

    ofxPoint3f middled( const ofxPoint3f& pnt ) const {
        return ofxPoint3f( (x+pnt.x)/2.0, (y+pnt.y)/2.0, (z+pnt.z)/2.0 );
    }

    ofxPoint3f& middle( const ofxPoint3f& pnt ) {
        x = (x+pnt.x)/2.0;
        y = (y+pnt.y)/2.0;
        z = (z+pnt.z)/2.0;
        return *this;
    }


    // Average (centroid) among points.
    // Addition is sometimes useful for calculating averages too.
    //
    //
    ofxPoint3f& average( const ofxPoint3f* points, int num ) {
        x = 0.f;
        y = 0.f;
        z = 0.f;
        for( int i=0; i<num; i++) {
            x += points[i].x;
            y += points[i].y;
            z += points[i].z;
        }
        x /= num;
        y /= num;
        z /= num;
        return *this;
    }





    // Perpendicular normalized vector from three points.
    //
    //
    ofxVec3f perpendicular( const ofxPoint3f& pnt1,
                            const ofxPoint3f& pnt2 ) const
    {
        float v1x = x - pnt1.x;
        float v1y = y - pnt1.y;
        float v1z = z - pnt1.z;
        float v2x = x - pnt2.x;
        float v2y = y - pnt2.y;
        float v2z = z - pnt2.z;

        float crossX = v1y*v2z - v1z*v2y;
        float crossY = v1z*v2x - v1x*v2z;
        float crossZ = v1x*v2y - v1y*v2x;

        float length = (float)sqrt(crossX*crossX +
                                   crossY*crossY +
                                   crossZ*crossZ);

        if( length > 0 )
            return ofxVec3f( crossX/length, crossY/length, crossZ/length );
        else
            return ofxVec3f();
    }


};





// Non-Member operators
//
//
static inline ofxPoint3f operator+( float f, const ofxPoint3f& pnt ) {
    return ofxPoint3f( f+pnt.x, f+pnt.y, f+pnt.z );
}

static inline ofxPoint3f operator-( float f, const ofxPoint3f& pnt ) {
    return ofxPoint3f( f-pnt.x, f-pnt.y, f-pnt.z );
}

static inline ofxPoint3f operator*( float f, const ofxPoint3f& pnt ) {
    return ofxPoint3f( f*pnt.x, f*pnt.y, f*pnt.z );
}

static inline ofxPoint3f operator/( float f, const ofxPoint3f& pnt ) {
    return ofxPoint3f( f/pnt.x, f/pnt.y, f/pnt.z);
}


#endif
