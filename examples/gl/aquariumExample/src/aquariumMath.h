#pragma once
// Matrix helpers copied from tdl.fast (BSD-3-Clause, Copyright 2009, Google Inc.).
// tdl installs the row-major multiply. The float[16] layout is what WebGL uploads.

#include <cmath>
#include <cstring>

namespace aq {

struct Mat4 {
	float m[16];
};

inline void identity(Mat4 & d) {
	std::memset(d.m, 0, sizeof(d.m));
	d.m[0] = d.m[5] = d.m[10] = d.m[15] = 1.f;
}

inline void copy(Mat4 & d, const Mat4 & a) {
	std::memcpy(d.m, a.m, sizeof(d.m));
}

// tdl.fast.rowMajor.mulMatrixMatrix4. Locals first so dst may alias a or b.
inline void mul(Mat4 & dst, const Mat4 & aIn, const Mat4 & bIn) {
	const float * a = aIn.m;
	const float * b = bIn.m;
	const float a00 = a[0], a01 = a[1], a02 = a[2], a03 = a[3];
	const float a10 = a[4], a11 = a[5], a12 = a[6], a13 = a[7];
	const float a20 = a[8], a21 = a[9], a22 = a[10], a23 = a[11];
	const float a30 = a[12], a31 = a[13], a32 = a[14], a33 = a[15];
	const float b00 = b[0], b01 = b[1], b02 = b[2], b03 = b[3];
	const float b10 = b[4], b11 = b[5], b12 = b[6], b13 = b[7];
	const float b20 = b[8], b21 = b[9], b22 = b[10], b23 = b[11];
	const float b30 = b[12], b31 = b[13], b32 = b[14], b33 = b[15];
	dst.m[0] = a00 * b00 + a01 * b10 + a02 * b20 + a03 * b30;
	dst.m[1] = a00 * b01 + a01 * b11 + a02 * b21 + a03 * b31;
	dst.m[2] = a00 * b02 + a01 * b12 + a02 * b22 + a03 * b32;
	dst.m[3] = a00 * b03 + a01 * b13 + a02 * b23 + a03 * b33;
	dst.m[4] = a10 * b00 + a11 * b10 + a12 * b20 + a13 * b30;
	dst.m[5] = a10 * b01 + a11 * b11 + a12 * b21 + a13 * b31;
	dst.m[6] = a10 * b02 + a11 * b12 + a12 * b22 + a13 * b32;
	dst.m[7] = a10 * b03 + a11 * b13 + a12 * b23 + a13 * b33;
	dst.m[8] = a20 * b00 + a21 * b10 + a22 * b20 + a23 * b30;
	dst.m[9] = a20 * b01 + a21 * b11 + a22 * b21 + a23 * b31;
	dst.m[10] = a20 * b02 + a21 * b12 + a22 * b22 + a23 * b32;
	dst.m[11] = a20 * b03 + a21 * b13 + a22 * b23 + a23 * b33;
	dst.m[12] = a30 * b00 + a31 * b10 + a32 * b20 + a33 * b30;
	dst.m[13] = a30 * b01 + a31 * b11 + a32 * b21 + a33 * b31;
	dst.m[14] = a30 * b02 + a31 * b12 + a32 * b22 + a33 * b32;
	dst.m[15] = a30 * b03 + a31 * b13 + a32 * b23 + a33 * b33;
}

inline void transpose(Mat4 & dst, const Mat4 & src) {
	Mat4 t;
	const float * m = src.m;
	t.m[0] = m[0]; t.m[1] = m[4]; t.m[2] = m[8]; t.m[3] = m[12];
	t.m[4] = m[1]; t.m[5] = m[5]; t.m[6] = m[9]; t.m[7] = m[13];
	t.m[8] = m[2]; t.m[9] = m[6]; t.m[10] = m[10]; t.m[11] = m[14];
	t.m[12] = m[3]; t.m[13] = m[7]; t.m[14] = m[11]; t.m[15] = m[15];
	dst = t;
}

// tdl.fast.inverse4
inline void inverse(Mat4 & dst, const Mat4 & src) {
	const float * m = src.m;
	const float m00 = m[0], m01 = m[1], m02 = m[2], m03 = m[3];
	const float m10 = m[4], m11 = m[5], m12 = m[6], m13 = m[7];
	const float m20 = m[8], m21 = m[9], m22 = m[10], m23 = m[11];
	const float m30 = m[12], m31 = m[13], m32 = m[14], m33 = m[15];
	const float tmp0 = m22 * m33, tmp1 = m32 * m23, tmp2 = m12 * m33, tmp3 = m32 * m13;
	const float tmp4 = m12 * m23, tmp5 = m22 * m13, tmp6 = m02 * m33, tmp7 = m32 * m03;
	const float tmp8 = m02 * m23, tmp9 = m22 * m03, tmp10 = m02 * m13, tmp11 = m12 * m03;
	const float tmp12 = m20 * m31, tmp13 = m30 * m21, tmp14 = m10 * m31, tmp15 = m30 * m11;
	const float tmp16 = m10 * m21, tmp17 = m20 * m11, tmp18 = m00 * m31, tmp19 = m30 * m01;
	const float tmp20 = m00 * m21, tmp21 = m20 * m01, tmp22 = m00 * m11, tmp23 = m10 * m01;
	const float t0 = (tmp0 * m11 + tmp3 * m21 + tmp4 * m31) - (tmp1 * m11 + tmp2 * m21 + tmp5 * m31);
	const float t1 = (tmp1 * m01 + tmp6 * m21 + tmp9 * m31) - (tmp0 * m01 + tmp7 * m21 + tmp8 * m31);
	const float t2 = (tmp2 * m01 + tmp7 * m11 + tmp10 * m31) - (tmp3 * m01 + tmp6 * m11 + tmp11 * m31);
	const float t3 = (tmp5 * m01 + tmp8 * m11 + tmp11 * m21) - (tmp4 * m01 + tmp9 * m11 + tmp10 * m21);
	const float d = 1.f / (m00 * t0 + m10 * t1 + m20 * t2 + m30 * t3);
	dst.m[0] = d * t0;
	dst.m[1] = d * t1;
	dst.m[2] = d * t2;
	dst.m[3] = d * t3;
	dst.m[4] = d * ((tmp1 * m10 + tmp2 * m20 + tmp5 * m30) - (tmp0 * m10 + tmp3 * m20 + tmp4 * m30));
	dst.m[5] = d * ((tmp0 * m00 + tmp7 * m20 + tmp8 * m30) - (tmp1 * m00 + tmp6 * m20 + tmp9 * m30));
	dst.m[6] = d * ((tmp3 * m00 + tmp6 * m10 + tmp11 * m30) - (tmp2 * m00 + tmp7 * m10 + tmp10 * m30));
	dst.m[7] = d * ((tmp4 * m00 + tmp9 * m10 + tmp10 * m20) - (tmp5 * m00 + tmp8 * m10 + tmp11 * m20));
	dst.m[8] = d * ((tmp12 * m13 + tmp15 * m23 + tmp16 * m33) - (tmp13 * m13 + tmp14 * m23 + tmp17 * m33));
	dst.m[9] = d * ((tmp13 * m03 + tmp18 * m23 + tmp21 * m33) - (tmp12 * m03 + tmp19 * m23 + tmp20 * m33));
	dst.m[10] = d * ((tmp14 * m03 + tmp19 * m13 + tmp22 * m33) - (tmp15 * m03 + tmp18 * m13 + tmp23 * m33));
	dst.m[11] = d * ((tmp17 * m03 + tmp20 * m13 + tmp23 * m23) - (tmp16 * m03 + tmp21 * m13 + tmp22 * m23));
	dst.m[12] = d * ((tmp14 * m22 + tmp17 * m32 + tmp13 * m12) - (tmp16 * m32 + tmp12 * m12 + tmp15 * m22));
	dst.m[13] = d * ((tmp20 * m32 + tmp12 * m02 + tmp19 * m22) - (tmp18 * m22 + tmp21 * m32 + tmp13 * m02));
	dst.m[14] = d * ((tmp18 * m12 + tmp23 * m32 + tmp15 * m02) - (tmp22 * m32 + tmp14 * m02 + tmp19 * m12));
	dst.m[15] = d * ((tmp22 * m22 + tmp16 * m02 + tmp21 * m12) - (tmp20 * m12 + tmp23 * m22 + tmp17 * m02));
}

inline float dot3(const float * a, const float * b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline void cross3(float * d, const float * a, const float * b) {
	d[0] = a[1] * b[2] - a[2] * b[1];
	d[1] = a[2] * b[0] - a[0] * b[2];
	d[2] = a[0] * b[1] - a[1] * b[0];
}

inline void normalize3(float * d, const float * a) {
	float n = std::sqrt(dot3(a, a));
	if (n > 0.00001f) {
		d[0] = a[0] / n;
		d[1] = a[1] / n;
		d[2] = a[2] / n;
	} else {
		d[0] = d[1] = d[2] = 0.f;
	}
}

inline void cameraLookAt(Mat4 & dst, const float * eye, const float * target, const float * up) {
	float t0[3] = {eye[0] - target[0], eye[1] - target[1], eye[2] - target[2]};
	float vz[3], vx[3], vy[3], t1[3];
	normalize3(vz, t0);
	cross3(t1, up, vz);
	normalize3(vx, t1);
	cross3(vy, vz, vx);
	dst.m[0] = vx[0]; dst.m[1] = vx[1]; dst.m[2] = vx[2]; dst.m[3] = 0.f;
	dst.m[4] = vy[0]; dst.m[5] = vy[1]; dst.m[6] = vy[2]; dst.m[7] = 0.f;
	dst.m[8] = vz[0]; dst.m[9] = vz[1]; dst.m[10] = vz[2]; dst.m[11] = 0.f;
	dst.m[12] = eye[0]; dst.m[13] = eye[1]; dst.m[14] = eye[2]; dst.m[15] = 1.f;
}

inline void frustum(Mat4 & dst, float left, float right, float bottom, float top, float near, float far) {
	const float dx = right - left;
	const float dy = top - bottom;
	const float dz = near - far;
	std::memset(dst.m, 0, sizeof(dst.m));
	dst.m[0] = 2.f * near / dx;
	dst.m[5] = 2.f * near / dy;
	dst.m[8] = (left + right) / dx;
	dst.m[9] = (top + bottom) / dy;
	dst.m[10] = far / dz;
	dst.m[11] = -1.f;
	dst.m[14] = near * far / dz;
}

inline void translation(Mat4 & dst, float x, float y, float z) {
	identity(dst);
	dst.m[12] = x;
	dst.m[13] = y;
	dst.m[14] = z;
}

inline void scaling(Mat4 & dst, float x, float y, float z) {
	std::memset(dst.m, 0, sizeof(dst.m));
	dst.m[0] = x;
	dst.m[5] = y;
	dst.m[10] = z;
	dst.m[15] = 1.f;
}

inline void rotationY(Mat4 & dst, float angle) {
	const float c = std::cos(angle);
	const float s = std::sin(angle);
	std::memset(dst.m, 0, sizeof(dst.m));
	dst.m[0] = c;
	dst.m[2] = -s;
	dst.m[5] = 1.f;
	dst.m[8] = s;
	dst.m[10] = c;
	dst.m[15] = 1.f;
}

inline void rotationZ(Mat4 & dst, float angle) {
	const float c = std::cos(angle);
	const float s = std::sin(angle);
	std::memset(dst.m, 0, sizeof(dst.m));
	dst.m[0] = c;
	dst.m[1] = s;
	dst.m[4] = -s;
	dst.m[5] = c;
	dst.m[10] = 1.f;
	dst.m[15] = 1.f;
}

inline void axis(float * d, const Mat4 & m, int a) {
	d[0] = m.m[a * 4 + 0];
	d[1] = m.m[a * 4 + 1];
	d[2] = m.m[a * 4 + 2];
}

} // namespace aq
