#pragma once

#include "ofVec2f.h"
#include "ofVec3f.h"
#include "ofVec4f.h"
#include "ofMatrix3x3.h"
#include "ofMatrix4x4.h"
#include "ofQuaternion.h"


//--------------------------------------------------------------
inline const ofVec2f & toOf(const glm::vec2 & v){
	return *reinterpret_cast<const ofVec2f*>(&v);
}

//--------------------------------------------------------------
inline const ofVec3f & toOf(const glm::vec3 & v){
	return *reinterpret_cast<const ofVec3f*>(&v);
}

//--------------------------------------------------------------
inline const ofVec4f & toOf(const glm::vec4 & v){
	return *reinterpret_cast<const ofVec4f*>(&v);
}

//--------------------------------------------------------------
inline const ofMatrix4x4 & toOf(const glm::mat4 & v){
	return *reinterpret_cast<const ofMatrix4x4*>(&v);
}

//--------------------------------------------------------------
inline const ofMatrix3x3 & toOf(const glm::mat3 & v){
	return *reinterpret_cast<const ofMatrix3x3*>(&v);
}

//--------------------------------------------------------------
inline const glm::vec2 & toGlm(const ofVec2f & v){
	return *reinterpret_cast<const glm::vec2*>(&v);
}

//--------------------------------------------------------------
inline const glm::vec3 & toGlm(const ofVec3f & v){
	return *reinterpret_cast<const glm::vec3*>(&v);
}

//--------------------------------------------------------------
inline const glm::vec4 & toGlm(const ofVec4f & v){
	return *reinterpret_cast<const glm::vec4*>(&v);
}

//--------------------------------------------------------------
inline const glm::mat4 & toGlm(const ofMatrix4x4 & v){
	return *reinterpret_cast<const glm::mat4*>(&v);
}

//--------------------------------------------------------------
inline const glm::mat3 & toGlm(const ofMatrix3x3 & v){
	return *reinterpret_cast<const glm::mat3*>(&v);
}

//--------------------------------------------------------------
inline const glm::vec2 & toGlm(const glm::vec2 & v){
	return v;
}

//--------------------------------------------------------------
inline const glm::vec3 & toGlm(const glm::vec3 & v){
	return v;
}

//--------------------------------------------------------------
inline const glm::vec4 & toGlm(const glm::vec4 & v){
	return v;
}

//--------------------------------------------------------------
inline const glm::quat toGlm(const ofQuaternion & q){
	return glm::quat(q.w(), glm::vec3(q.x(), q.y(), q.z()));
}

//--------------------------------------------------------------
inline const glm::quat & toGlm(const glm::quat & q){
	return q;
}

namespace glm {
	//--------------------------------------------------------------
	inline ostream& operator<<(ostream& os, const glm::vec3& vec) {
		os << vec.x << ", " << vec.y << ", " << vec.z;
		return os;
	}

	//--------------------------------------------------------------
	inline istream& operator >> (istream& is, glm::vec3& vec) {
		is >> vec.x;
		is.ignore(2);
		is >> vec.y;
		is.ignore(2);
		is >> vec.z;
		return is;
	}

	//--------------------------------------------------------------
	inline ostream& operator<<(ostream& os, const glm::vec2& vec) {
		os << vec.x << ", " << vec.y;
		return os;
	}

	//--------------------------------------------------------------
	inline istream& operator >> (istream& is, glm::vec2& vec) {
		is >> vec.x;
		is.ignore(2);
		is >> vec.y;
		return is;
	}

	//--------------------------------------------------------------
	inline ostream& operator<<(ostream& os, const glm::vec4& vec) {
		os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
		return os;
	}

	//--------------------------------------------------------------
	inline istream& operator >> (istream& is, glm::vec4& vec) {
		is >> vec.x;
		is.ignore(2);
		is >> vec.y;
		is.ignore(2);
		is >> vec.z;
		is.ignore(2);
		is >> vec.w;
		return is;
	}
}

//--------------------------------------------------------------
inline glm::vec3 operator+(const glm::vec3 & v1, const ofVec3f & v2){
	return v1 + glm::vec3(v2);
}

//--------------------------------------------------------------
inline glm::vec3 operator-(const glm::vec3 & v1, const ofVec3f & v2){
	return v1 - glm::vec3(v2);
}

//--------------------------------------------------------------
inline glm::vec3 operator*(const glm::vec3 & v1, const ofVec3f & v2){
	return v1 * glm::vec3(v2);
}

//--------------------------------------------------------------
inline glm::vec3 operator/(const glm::vec3 & v1, const ofVec3f & v2){
	return v1 / glm::vec3(v2);
}

//--------------------------------------------------------------
inline glm::vec3 & operator+=(glm::vec3 & v1, const ofVec3f & v2){
	v1 += glm::vec3(v2);
	return v1;
}

//--------------------------------------------------------------
inline glm::vec3 & operator-=(glm::vec3 & v1, const ofVec3f & v2){
	v1 -= glm::vec3(v2);
	return v1;
}

//--------------------------------------------------------------
inline glm::vec3 & operator*=(glm::vec3 & v1, const ofVec3f & v2){
	v1 *= glm::vec3(v2);
	return v1;
}

//--------------------------------------------------------------
inline glm::vec3 & operator/=(glm::vec3 & v1, const ofVec3f & v2){
	v1 /= glm::vec3(v2);
	return v1;
}

//--------------------------------------------------------------
inline glm::vec2 operator+(const glm::vec2 & v1, const ofVec2f & v2){
	return v1 + glm::vec2(v2);
}

//--------------------------------------------------------------
inline glm::vec2 operator-(const glm::vec2 & v1, const ofVec2f & v2){
	return v1 - glm::vec2(v2);
}

//--------------------------------------------------------------
inline glm::vec2 operator*(const glm::vec2 & v1, const ofVec2f & v2){
	return v1 * glm::vec2(v2);
}

//--------------------------------------------------------------
inline glm::vec2 operator/(const glm::vec2 & v1, const ofVec2f & v2){
	return v1 / glm::vec2(v2);
}

//--------------------------------------------------------------
inline glm::vec2 & operator+=(glm::vec2 & v1, const ofVec2f & v2){
	v1 += glm::vec2(v2);
	return v1;
}

//--------------------------------------------------------------
inline glm::vec2 & operator-=(glm::vec2 & v1, const ofVec2f & v2){
	v1 -= glm::vec2(v2);
	return v1;
}

//--------------------------------------------------------------
inline glm::vec2 & operator*=(glm::vec2 & v1, const ofVec2f & v2){
	v1 *= glm::vec2(v2);
	return v1;
}

//--------------------------------------------------------------
inline glm::vec2 & operator/=(glm::vec2 & v1, const ofVec2f & v2){
	v1 /= glm::vec2(v2);
	return v1;
}
