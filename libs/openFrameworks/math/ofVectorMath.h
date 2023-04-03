#pragma once

#include "ofVec2f.h"
#include "ofVec3f.h"
#include "ofVec4f.h"
#include "ofMatrix3x3.h"
#include "ofMatrix4x4.h"
#include "ofQuaternion.h"

#include "ofMathConstants.h"
#include "ofConstants.h"
//#include "glm/glm.hpp"

//#include "glm/detail/setup.hpp"
//#include "glm/detail/_swizzle.hpp"
//#include "glm/detail/_swizzle_func.hpp"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/geometric.hpp"
#include "glm/common.hpp"
#include "glm/trigonometric.hpp"
#include "glm/exponential.hpp"
#include "glm/vector_relational.hpp"

#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/perpendicular.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/spline.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/scalar_multiplication.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>

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
	template <typename T, precision P>
	inline std::ostream& operator<<(std::ostream& os, const vec<3, T, P>& vec) {
		os << vec.x << ", " << vec.y << ", " << vec.z;
		return os;
	}

	//--------------------------------------------------------------
	template <typename T, precision P>
	inline std::istream& operator>>(std::istream& is, vec<3, T, P>& vec) {
		is >> vec.x;
		is.ignore(2);
		is >> vec.y;
		is.ignore(2);
		is >> vec.z;
		return is;
	}

	//--------------------------------------------------------------
	template <typename T, precision P>
	inline std::ostream& operator<<(std::ostream& os, const vec<2, T, P>& vec) {
		os << vec.x << ", " << vec.y;
		return os;
	}

	//--------------------------------------------------------------
	template <typename T, precision P>
	inline std::istream& operator>>(std::istream& is, vec<2, T, P>& vec) {
		is >> vec.x;
		is.ignore(2);
		is >> vec.y;
		return is;
	}

	//--------------------------------------------------------------
	template <typename T, precision P>
	inline std::ostream& operator<<(std::ostream& os, const vec<4, T, P>& vec) {
		os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
		return os;
	}

	//--------------------------------------------------------------
	template <typename T, precision P>
	inline std::istream& operator>>(std::istream& is, vec<4, T, P>& vec) {
		is >> vec.x;
		is.ignore(2);
		is >> vec.y;
		is.ignore(2);
		is >> vec.z;
		is.ignore(2);
		is >> vec.w;
		return is;
	}

	//--------------------------------------------------------------
	template <typename T, precision P>
	inline std::ostream& operator<<(std::ostream& os, const mat<3, 3, T, P>& mat) {
		int w = 8;
		os << std::setw(w)
			<< mat[0][0] << ", " << std::setw(w)
			<< mat[0][1] << ", " << std::setw(w)
			<< mat[0][2] << std::endl;

		os << std::setw(w)
			<< mat[1][0] << ", " << std::setw(w)
			<< mat[1][1] << ", " << std::setw(w)
			<< mat[1][2] << std::endl;

		os << std::setw(w)
			<< mat[2][0] << ", " << std::setw(w)
			<< mat[2][1] << ", " << std::setw(w)
			<< mat[2][2];
		return os;
	}

	//--------------------------------------------------------------
	template <typename T, precision P>
	inline std::istream& operator>>(std::istream& is, mat<3, 3, T, P>& mat) {
		is >> mat[0][0]; is.ignore(2);
		is >> mat[0][1]; is.ignore(2);
		is >> mat[0][2]; is.ignore(1);

		is >> mat[1][0]; is.ignore(2);
		is >> mat[1][1]; is.ignore(2);
		is >> mat[1][2]; is.ignore(1);
		
		is >> mat[2][0]; is.ignore(2);
		is >> mat[2][1]; is.ignore(2);
		is >> mat[2][2];
		return is;
	}

	//--------------------------------------------------------------
	template <typename T, precision P>
	inline std::ostream& operator<<(std::ostream& os, const mat<4, 4, T, P>& mat) {
		int w = 8;
		os << std::setw(w)
			<< mat[0][0] << ", " << std::setw(w)
			<< mat[0][1] << ", " << std::setw(w)
			<< mat[0][2] << ", " << std::setw(w)
			<< mat[0][3] << std::endl;

		os << std::setw(w)
			<< mat[1][0] << ", " << std::setw(w)
			<< mat[1][1] << ", " << std::setw(w)
			<< mat[1][2] << ", " << std::setw(w)
			<< mat[1][3] << std::endl;

		os << std::setw(w)
			<< mat[2][0] << ", " << std::setw(w)
			<< mat[2][1] << ", " << std::setw(w)
			<< mat[2][2] << ", " << std::setw(w)
			<< mat[2][3] << std::endl;

		os << std::setw(w)
			<< mat[3][0] << ", " << std::setw(w)
			<< mat[3][1] << ", " << std::setw(w)
			<< mat[3][2] << ", " << std::setw(w)
			<< mat[3][3];
		return os;
	}

	//--------------------------------------------------------------
	template <typename T, precision P>
	inline std::istream& operator>>(std::istream& is, mat<4, 4, T, P>& mat) {
		is >> mat[0][0]; is.ignore(2);
		is >> mat[0][1]; is.ignore(2);
		is >> mat[0][2]; is.ignore(2);
		is >> mat[0][3]; is.ignore(1);
			  
		is >> mat[1][0]; is.ignore(2);
		is >> mat[1][1]; is.ignore(2);
		is >> mat[1][2]; is.ignore(2);
		is >> mat[1][3]; is.ignore(1);
			  
		is >> mat[2][0]; is.ignore(2);
		is >> mat[2][1]; is.ignore(2);
		is >> mat[2][2]; is.ignore(2);
		is >> mat[2][3]; is.ignore(1);
			  
		is >> mat[3][0]; is.ignore(2);
		is >> mat[3][1]; is.ignore(2);
		is >> mat[3][2]; is.ignore(2);
		is >> mat[3][3];
		return is;
	}

	//----------------------------------------
	template <typename T, precision P>
	inline std::ostream& operator<<(std::ostream& os, const qua<T, P>& q) {
		os << q.w << ", " << q.x << ", " << q.y << ", " << q.z;
		return os;
	}


	//----------------------------------------
	template <typename T, precision P>
	inline std::istream& operator>> (std::istream& is, qua<T, P>& q) {
		is >> q.w;
		is.ignore(2);
		is >> q.x;
		is.ignore(2);
		is >> q.y;
		is.ignore(2);
		is >> q.z;
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


