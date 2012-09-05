/*
 *  ofQuaternion.h
 *  
 *  Created by Aaron Meyers on 6/22/09 -- modified by Arturo Castro, Zach Lieberman, Memo Akten
 *  based on code from OSG - 
 *  see OSG license for more details: 
 *  http://www.openscenegraph.org/projects/osg/wiki/Legal
 * 
 */

#pragma once
#include "ofConstants.h"
#include "ofVec3f.h"
#include "ofVec4f.h"
#include <cmath>

#if (_MSC_VER)       
// make microsoft visual studio complain less about double / float conversion.
#pragma warning(disable : 4244)
#endif


class ofMatrix4x4;

class ofQuaternion {
public:
    //    float _v[4];
    ofVec4f _v;
    
    inline ofQuaternion();
    inline ofQuaternion(float x, float y, float z, float w);
    inline ofQuaternion(const ofVec4f& v);
    inline ofQuaternion(float angle, const ofVec3f& axis);
    inline ofQuaternion(float angle1, const ofVec3f& axis1, float angle2, const ofVec3f& axis2, float angle3, const ofVec3f& axis3);
    
    inline ofQuaternion& operator =(const ofQuaternion& q);
    inline bool operator ==(const ofQuaternion& q) const;
    inline bool operator !=(const ofQuaternion& q) const;
    //    inline bool operator <(const ofQuaternion& q) const;  // why?
    
    inline ostream& operator<<(ostream& os);
    inline istream& operator>>(istream& is);
    
    inline ofVec4f asVec4() const;
    inline ofVec3f asVec3() const;

    
    inline void set(float x, float y, float z, float w);
    inline void set(const ofVec4f& v);
    
    void set(const ofMatrix4x4& matrix);
    void get(ofMatrix4x4& matrix) const;
    
    inline float& operator [](int i);
    inline float operator [](int i) const;
    
    inline float& x();
    inline float& y();
    inline float& z();
    inline float& w();
    
    inline float x() const;
    inline float y() const;
    inline float z() const;
    inline float w() const;
    
    // return true if the Quat represents a zero rotation, and therefore can be ignored in computations.
    inline bool zeroRotation() const;
    
    
    
    // BASIC ARITHMETIC METHODS
    // Implemented in terms of Vec4s. Some Vec4 operators, e.g.
    // operator* are not appropriate for quaternions (as
    // mathematical objects) so they are implemented differently.
    // Also define methods for conjugate and the multiplicative inverse.
    
    inline const ofQuaternion operator *(float rhs) const;                  // Multiply by scalar
    inline ofQuaternion& operator *=(float rhs);                            // Unary multiply by scalar
    inline const ofQuaternion operator*(const ofQuaternion& rhs) const;     // Binary multiply
    inline ofQuaternion& operator*=(const ofQuaternion& rhs);               // Unary multiply
    inline ofQuaternion operator /(float rhs) const;                        // Divide by scalar
    inline ofQuaternion& operator /=(float rhs);                            // Unary divide by scalar
    inline const ofQuaternion operator/(const ofQuaternion& denom) const;   // Binary divide
    inline ofQuaternion& operator/=(const ofQuaternion& denom);             // Unary divide
    inline const ofQuaternion operator +(const ofQuaternion& rhs) const;    // Binary addition
    inline ofQuaternion& operator +=(const ofQuaternion& rhs);              // Unary addition
    inline const ofQuaternion operator -(const ofQuaternion& rhs) const;    // Binary subtraction
    inline ofQuaternion& operator -=(const ofQuaternion& rhs);              // Unary subtraction
    inline const ofQuaternion operator -() const;                           // returns the negative of the quaternion. calls operator -() on the Vec4 */
    inline ofVec3f operator*(const ofVec3f& v) const;                       // Rotate a vector by this quaternion.
    
    
    // Length of the quaternion = sqrt(vec . vec)
    inline float length() const;
    
    // Length of the quaternion = vec . vec
    inline float length2() const;
    
    // Conjugate
    inline ofQuaternion conj() const;
    
    // Multiplicative inverse method: q^(-1) = q^*/(q.q^*)
    inline const ofQuaternion inverse() const;
    
    
    
    // METHODS RELATED TO ROTATIONS
    // Set a quaternion which will perform a rotation of an
    // angle around the axis given by the vector(x,y,z).
    // Should be written to also accept an angle and a Vec3?
    
    // Define Spherical Linear interpolation method also
    void makeRotate(float angle, float x, float y, float z);
    void makeRotate(float angle, const ofVec3f& vec);
    void makeRotate(float angle1, const ofVec3f& axis1, float angle2, const ofVec3f& axis2, float angle3, const ofVec3f& axis3);
    
    
    // Make a rotation Quat which will rotate vec1 to vec2.
    // Generally take a dot product to get the angle between these
    // and then use a cross product to get the rotation axis
    // Watch out for the two special cases when the vectors
    // are co-incident or opposite in direction.
    void makeRotate(const ofVec3f& vec1, const ofVec3f& vec2);
    
    void makeRotate_original(const ofVec3f& vec1, const ofVec3f& vec2);
    
    // Return the angle and vector components represented by the quaternion.
    void getRotate(float&angle, float& x, float& y, float& z) const;
    void getRotate(float& angle, ofVec3f& vec) const;
    
    // calculate and return the rotation as euler angles
    ofVec3f getEuler() const;
    
    
    // Spherical Linear Interpolation.
    // As t goes from 0 to 1, the Quat object goes from "from" to "to".
    void slerp(float t, const ofQuaternion& from, const ofQuaternion& to);

    inline void normalize();
};


//----------------------------------------
ofQuaternion::ofQuaternion() {
    _v.set(0, 0, 0, 1);
}


//----------------------------------------
ostream& ofQuaternion::operator<<(ostream& os) {
    os << _v.x << ", " << _v.y << ", " << _v.z << ", " << _v.w;
    return os;
}


//----------------------------------------
istream& ofQuaternion::operator>>(istream& is) {
    is >> _v.x;
    is.ignore(2);
    is >> _v.y;
    is.ignore(2);
    is >> _v.z;
    is.ignore(2);
    is >> _v.w;
    return is;
}


//----------------------------------------
ofQuaternion::ofQuaternion(float x, float y, float z, float w) {
    _v.set(x, y, z, w);
}


//----------------------------------------
ofQuaternion::ofQuaternion(const ofVec4f& v) {
    _v = v;
}


//----------------------------------------
ofQuaternion::ofQuaternion(float angle, const ofVec3f& axis) {
    makeRotate(angle, axis);
}


//----------------------------------------
ofQuaternion::ofQuaternion(float angle1, const ofVec3f& axis1, float angle2, const ofVec3f& axis2, float angle3, const ofVec3f& axis3) {
    makeRotate(angle1, axis1, angle2, axis2, angle3, axis3);
}


//----------------------------------------
ofQuaternion& ofQuaternion::operator =(const ofQuaternion& q) {
    _v = q._v;
    return *this;
}


//----------------------------------------
bool ofQuaternion::operator ==(const ofQuaternion& q) const {
    return _v == q._v;
}


//----------------------------------------
bool ofQuaternion::operator !=(const ofQuaternion& q) const {
    return _v != q._v;
}


//----------------------------------------
//bool ofQuaternion::operator <(const ofQuaternion& q) const {
//    if(_v.x < v._v.x) return true;
//    else if(_v.x > v._v.x) return false;
//    else if(_v.y < v._v.y) return true;
//    else if(_v.y > v._v.y) return false;
//    else if(_v.z < v._v.z) return true;
//    else if(_v.z > v._v.z) return false;
//    else return (_v.w < v._v.w);
//}



//----------------------------------------
ofVec4f ofQuaternion::asVec4() const {
    return _v;
}


//----------------------------------------
ofVec3f ofQuaternion::asVec3() const {
    return ofVec3f(_v);
}


//----------------------------------------
void ofQuaternion::set(float x, float y, float z, float w) {
    _v.set(x, y, z, w);
}


//----------------------------------------
void ofQuaternion::set(const ofVec4f& v) {
    _v = v;
}


//----------------------------------------
float& ofQuaternion::operator [](int i) {
    return _v[i];
}



//----------------------------------------
float ofQuaternion::operator [](int i) const {
    return _v[i];
}


//----------------------------------------
float& ofQuaternion::x() {
    return _v.x;
}


//----------------------------------------
float& ofQuaternion::y() {
    return _v.y;
}


//----------------------------------------
float& ofQuaternion::z() {
    return _v.z;
}


//----------------------------------------
float& ofQuaternion::w() {
    return _v.w;
}


//----------------------------------------
float ofQuaternion::x() const {
    return _v.x;
}


//----------------------------------------
float ofQuaternion::y() const {
    return _v.y;
}


//----------------------------------------
float ofQuaternion::z() const {
    return _v.z;
}


//----------------------------------------
float ofQuaternion::w() const {
    return _v.w;
}


//----------------------------------------
bool ofQuaternion::zeroRotation() const {
    return _v.x == 0.0 && _v.y == 0.0 && _v.z == 0.0 && _v.w == 1.0;
}



//----------------------------------------
const ofQuaternion ofQuaternion::operator *(float rhs) const {
    return ofQuaternion(_v.x*rhs, _v.y*rhs, _v.z*rhs, _v.w*rhs);
}


//----------------------------------------
ofQuaternion& ofQuaternion::operator *=(float rhs) {
    _v.x *= rhs;
    _v.y *= rhs;
    _v.z *= rhs;
    _v.w *= rhs;
    return *this; // enable nesting
}


//----------------------------------------
const ofQuaternion ofQuaternion::operator*(const ofQuaternion& rhs) const {
    return ofQuaternion(rhs._v.w*_v.x + rhs._v.x*_v.w + rhs._v.y*_v.z - rhs._v.z*_v.y,
                        rhs._v.w*_v.y - rhs._v.x*_v.z + rhs._v.y*_v.w + rhs._v.z*_v.x,
                        rhs._v.w*_v.z + rhs._v.x*_v.y - rhs._v.y*_v.x + rhs._v.z*_v.w,
                        rhs._v.w*_v.w - rhs._v.x*_v.x - rhs._v.y*_v.y - rhs._v.z*_v.z);
}


//----------------------------------------
ofQuaternion& ofQuaternion::operator*=(const ofQuaternion& rhs) {
    float x = rhs._v.w * _v.x + rhs._v.x * _v.w + rhs._v.y * _v.z - rhs._v.z * _v.y;
    float y = rhs._v.w * _v.y - rhs._v.x * _v.z + rhs._v.y * _v.w + rhs._v.z * _v.x;
    float z = rhs._v.w * _v.z + rhs._v.x * _v.y - rhs._v.y * _v.x + rhs._v.z * _v.w;
    _v.w = rhs._v.w * _v.w - rhs._v.x * _v.x - rhs._v.y * _v.y - rhs._v.z * _v.z;
    
    _v.z = z;
    _v.y = y;
    _v.x = x;
    
    return (*this); // enable nesting
}


//----------------------------------------
ofQuaternion ofQuaternion::operator /(float rhs) const {
    float div = 1.0 / rhs;
    return ofQuaternion(_v.x*div, _v.y*div, _v.z*div, _v.w*div);
}


//----------------------------------------
ofQuaternion& ofQuaternion::operator /=(float rhs) {
    float div = 1.0 / rhs;
    _v.x *= div;
    _v.y *= div;
    _v.z *= div;
    _v.w *= div;
    return *this;
}


//----------------------------------------
const ofQuaternion ofQuaternion::operator/(const ofQuaternion& denom) const {
    return ((*this) * denom.inverse());
}


//----------------------------------------
ofQuaternion& ofQuaternion::operator/=(const ofQuaternion& denom) {
    (*this) = (*this) * denom.inverse();
    return (*this); // enable nesting
}


//----------------------------------------
const ofQuaternion ofQuaternion::operator +(const ofQuaternion& rhs) const {
    return ofQuaternion(_v.x + rhs._v.x, _v.y + rhs._v.y,
                        _v.z + rhs._v.z, _v.w + rhs._v.w);
}


//----------------------------------------
ofQuaternion& ofQuaternion::operator +=(const ofQuaternion& rhs) {
    _v.x += rhs._v.x;
    _v.y += rhs._v.y;
    _v.z += rhs._v.z;
    _v.w += rhs._v.w;
    return *this; // enable nesting
}


//----------------------------------------
const ofQuaternion ofQuaternion::operator -(const ofQuaternion& rhs) const {
    return ofQuaternion(_v.x - rhs._v.x, _v.y - rhs._v.y,
                        _v.z - rhs._v.z, _v.w - rhs._v.w);
}


//----------------------------------------
ofQuaternion& ofQuaternion::operator -=(const ofQuaternion& rhs) {
    _v.x -= rhs._v.x;
    _v.y -= rhs._v.y;
    _v.z -= rhs._v.z;
    _v.w -= rhs._v.w;
    return *this; // enable nesting
}


//----------------------------------------
const ofQuaternion ofQuaternion::operator -() const {
    return ofQuaternion(-_v.x, -_v.y, -_v.z, -_v.w);
}


//----------------------------------------
float ofQuaternion::length() const {
    return sqrt(_v.x*_v.x + _v.y*_v.y + _v.z*_v.z + _v.w*_v.w);
}


//----------------------------------------
float ofQuaternion::length2() const {
    return _v.x*_v.x + _v.y*_v.y + _v.z*_v.z + _v.w*_v.w;
}


//----------------------------------------
ofQuaternion ofQuaternion::conj() const {
    return ofQuaternion(-_v.x, -_v.y, -_v.z, _v.w);
}


//----------------------------------------
const ofQuaternion ofQuaternion::inverse() const {
    return conj() / length2();
}



//----------------------------------------
ofVec3f ofQuaternion::operator*(const ofVec3f& v) const {
    // nVidia SDK implementation
    ofVec3f uv, uuv;
    ofVec3f qvec(_v.x, _v.y, _v.z);
    uv = qvec.getCrossed(v);    //uv = qvec ^ v;
    uuv = qvec.getCrossed(uv);    //uuv = qvec ^ uv;
    uv *= (2.0f * _v.w);
    uuv *= 2.0f;
    return v + uv + uuv;
}

void ofQuaternion::normalize(){
	float len = _v.w*_v.w + _v.x*_v.x + _v.y*_v.y + _v.z*_v.z;
	float factor = 1.0f / sqrt(len);
	_v.x *= factor;
	_v.y *= factor;
	_v.z *= factor;
	_v.w *= factor;
}
