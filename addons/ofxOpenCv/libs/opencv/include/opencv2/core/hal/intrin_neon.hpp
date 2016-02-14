/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Copyright (C) 2015, Itseez Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_HAL_INTRIN_NEON_HPP__
#define __OPENCV_HAL_INTRIN_NEON_HPP__

#include <algorithm>

namespace cv
{

//! @cond IGNORED

#define CV_SIMD128 1

struct v_uint8x16
{
    typedef uchar lane_type;
    enum { nlanes = 16 };

    v_uint8x16() {}
    explicit v_uint8x16(uint8x16_t v) : val(v) {}
    v_uint8x16(uchar v0, uchar v1, uchar v2, uchar v3, uchar v4, uchar v5, uchar v6, uchar v7,
               uchar v8, uchar v9, uchar v10, uchar v11, uchar v12, uchar v13, uchar v14, uchar v15)
    {
        uchar v[] = {v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15};
        val = vld1q_u8(v);
    }
    uchar get0() const
    {
        return vgetq_lane_u8(val, 0);
    }

    uint8x16_t val;
};

struct v_int8x16
{
    typedef schar lane_type;
    enum { nlanes = 16 };

    v_int8x16() {}
    explicit v_int8x16(int8x16_t v) : val(v) {}
    v_int8x16(schar v0, schar v1, schar v2, schar v3, schar v4, schar v5, schar v6, schar v7,
               schar v8, schar v9, schar v10, schar v11, schar v12, schar v13, schar v14, schar v15)
    {
        schar v[] = {v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15};
        val = vld1q_s8(v);
    }
    schar get0() const
    {
        return vgetq_lane_s8(val, 0);
    }

    int8x16_t val;
};

struct v_uint16x8
{
    typedef ushort lane_type;
    enum { nlanes = 8 };

    v_uint16x8() {}
    explicit v_uint16x8(uint16x8_t v) : val(v) {}
    v_uint16x8(ushort v0, ushort v1, ushort v2, ushort v3, ushort v4, ushort v5, ushort v6, ushort v7)
    {
        ushort v[] = {v0, v1, v2, v3, v4, v5, v6, v7};
        val = vld1q_u16(v);
    }
    ushort get0() const
    {
        return vgetq_lane_u16(val, 0);
    }

    uint16x8_t val;
};

struct v_int16x8
{
    typedef short lane_type;
    enum { nlanes = 8 };

    v_int16x8() {}
    explicit v_int16x8(int16x8_t v) : val(v) {}
    v_int16x8(short v0, short v1, short v2, short v3, short v4, short v5, short v6, short v7)
    {
        short v[] = {v0, v1, v2, v3, v4, v5, v6, v7};
        val = vld1q_s16(v);
    }
    short get0() const
    {
        return vgetq_lane_s16(val, 0);
    }

    int16x8_t val;
};

struct v_uint32x4
{
    typedef unsigned lane_type;
    enum { nlanes = 4 };

    v_uint32x4() {}
    explicit v_uint32x4(uint32x4_t v) : val(v) {}
    v_uint32x4(unsigned v0, unsigned v1, unsigned v2, unsigned v3)
    {
        unsigned v[] = {v0, v1, v2, v3};
        val = vld1q_u32(v);
    }
    unsigned get0() const
    {
        return vgetq_lane_u32(val, 0);
    }

    uint32x4_t val;
};

struct v_int32x4
{
    typedef int lane_type;
    enum { nlanes = 4 };

    v_int32x4() {}
    explicit v_int32x4(int32x4_t v) : val(v) {}
    v_int32x4(int v0, int v1, int v2, int v3)
    {
        int v[] = {v0, v1, v2, v3};
        val = vld1q_s32(v);
    }
    int get0() const
    {
        return vgetq_lane_s32(val, 0);
    }
    int32x4_t val;
};

struct v_float32x4
{
    typedef float lane_type;
    enum { nlanes = 4 };

    v_float32x4() {}
    explicit v_float32x4(float32x4_t v) : val(v) {}
    v_float32x4(float v0, float v1, float v2, float v3)
    {
        float v[] = {v0, v1, v2, v3};
        val = vld1q_f32(v);
    }
    float get0() const
    {
        return vgetq_lane_f32(val, 0);
    }
    float32x4_t val;
};

struct v_uint64x2
{
    typedef uint64 lane_type;
    enum { nlanes = 2 };

    v_uint64x2() {}
    explicit v_uint64x2(uint64x2_t v) : val(v) {}
    v_uint64x2(unsigned v0, unsigned v1)
    {
        uint64 v[] = {v0, v1};
        val = vld1q_u64(v);
    }
    uint64 get0() const
    {
        return vgetq_lane_u64(val, 0);
    }
    uint64x2_t val;
};

struct v_int64x2
{
    typedef int64 lane_type;
    enum { nlanes = 2 };

    v_int64x2() {}
    explicit v_int64x2(int64x2_t v) : val(v) {}
    v_int64x2(int v0, int v1)
    {
        int64 v[] = {v0, v1};
        val = vld1q_s64(v);
    }
    int64 get0() const
    {
        return vgetq_lane_s64(val, 0);
    }
    int64x2_t val;
};

#define OPENCV_HAL_IMPL_NEON_INIT(_Tpv, _Tp, suffix) \
inline v_##_Tpv v_setzero_##suffix() { return v_##_Tpv(vdupq_n_##suffix((_Tp)0)); } \
inline v_##_Tpv v_setall_##suffix(_Tp v) { return v_##_Tpv(vdupq_n_##suffix(v)); } \
inline _Tpv##_t vreinterpretq_##suffix##_##suffix(_Tpv##_t v) { return v; } \
inline v_uint8x16 v_reinterpret_as_u8(const v_##_Tpv& v) { return v_uint8x16(vreinterpretq_u8_##suffix(v.val)); } \
inline v_int8x16 v_reinterpret_as_s8(const v_##_Tpv& v) { return v_int8x16(vreinterpretq_s8_##suffix(v.val)); } \
inline v_uint16x8 v_reinterpret_as_u16(const v_##_Tpv& v) { return v_uint16x8(vreinterpretq_u16_##suffix(v.val)); } \
inline v_int16x8 v_reinterpret_as_s16(const v_##_Tpv& v) { return v_int16x8(vreinterpretq_s16_##suffix(v.val)); } \
inline v_uint32x4 v_reinterpret_as_u32(const v_##_Tpv& v) { return v_uint32x4(vreinterpretq_u32_##suffix(v.val)); } \
inline v_int32x4 v_reinterpret_as_s32(const v_##_Tpv& v) { return v_int32x4(vreinterpretq_s32_##suffix(v.val)); } \
inline v_uint64x2 v_reinterpret_as_u64(const v_##_Tpv& v) { return v_uint64x2(vreinterpretq_u64_##suffix(v.val)); } \
inline v_int64x2 v_reinterpret_as_s64(const v_##_Tpv& v) { return v_int64x2(vreinterpretq_s64_##suffix(v.val)); } \
inline v_float32x4 v_reinterpret_as_f32(const v_##_Tpv& v) { return v_float32x4(vreinterpretq_f32_##suffix(v.val)); }

OPENCV_HAL_IMPL_NEON_INIT(uint8x16, uchar, u8)
OPENCV_HAL_IMPL_NEON_INIT(int8x16, schar, s8)
OPENCV_HAL_IMPL_NEON_INIT(uint16x8, ushort, u16)
OPENCV_HAL_IMPL_NEON_INIT(int16x8, short, s16)
OPENCV_HAL_IMPL_NEON_INIT(uint32x4, unsigned, u32)
OPENCV_HAL_IMPL_NEON_INIT(int32x4, int, s32)
OPENCV_HAL_IMPL_NEON_INIT(uint64x2, uint64, u64)
OPENCV_HAL_IMPL_NEON_INIT(int64x2, int64, s64)
OPENCV_HAL_IMPL_NEON_INIT(float32x4, float, f32)

#define OPENCV_HAL_IMPL_NEON_PACK(_Tpvec, _Tp, hreg, suffix, _Tpwvec, wsuffix, pack, op) \
inline _Tpvec v_##pack(const _Tpwvec& a, const _Tpwvec& b) \
{ \
    hreg a1 = vqmov##op##_##wsuffix(a.val), b1 = vqmov##op##_##wsuffix(b.val); \
    return _Tpvec(vcombine_##suffix(a1, b1)); \
} \
inline void v_##pack##_store(_Tp* ptr, const _Tpwvec& a) \
{ \
    hreg a1 = vqmov##op##_##wsuffix(a.val); \
    vst1_##suffix(ptr, a1); \
} \
template<int n> inline \
_Tpvec v_rshr_##pack(const _Tpwvec& a, const _Tpwvec& b) \
{ \
    hreg a1 = vqrshr##op##_n_##wsuffix(a.val, n); \
    hreg b1 = vqrshr##op##_n_##wsuffix(b.val, n); \
    return _Tpvec(vcombine_##suffix(a1, b1)); \
} \
template<int n> inline \
void v_rshr_##pack##_store(_Tp* ptr, const _Tpwvec& a) \
{ \
    hreg a1 = vqrshr##op##_n_##wsuffix(a.val, n); \
    vst1_##suffix(ptr, a1); \
}

OPENCV_HAL_IMPL_NEON_PACK(v_uint8x16, uchar, uint8x8_t, u8, v_uint16x8, u16, pack, n)
OPENCV_HAL_IMPL_NEON_PACK(v_int8x16, schar, int8x8_t, s8, v_int16x8, s16, pack, n)
OPENCV_HAL_IMPL_NEON_PACK(v_uint16x8, ushort, uint16x4_t, u16, v_uint32x4, u32, pack, n)
OPENCV_HAL_IMPL_NEON_PACK(v_int16x8, short, int16x4_t, s16, v_int32x4, s32, pack, n)
OPENCV_HAL_IMPL_NEON_PACK(v_uint32x4, unsigned, uint32x2_t, u32, v_uint64x2, u64, pack, n)
OPENCV_HAL_IMPL_NEON_PACK(v_int32x4, int, int32x2_t, s32, v_int64x2, s64, pack, n)

OPENCV_HAL_IMPL_NEON_PACK(v_uint8x16, uchar, uint8x8_t, u8, v_int16x8, s16, pack_u, un)
OPENCV_HAL_IMPL_NEON_PACK(v_uint16x8, ushort, uint16x4_t, u16, v_int32x4, s32, pack_u, un)

inline v_float32x4 v_matmul(const v_float32x4& v, const v_float32x4& m0,
                            const v_float32x4& m1, const v_float32x4& m2,
                            const v_float32x4& m3)
{
    float32x2_t vl = vget_low_f32(v.val), vh = vget_high_f32(v.val);
    float32x4_t res = vmulq_lane_f32(m0.val, vl, 0);
    res = vmlaq_lane_f32(res, m1.val, vl, 1);
    res = vmlaq_lane_f32(res, m2.val, vh, 0);
    res = vmlaq_lane_f32(res, m3.val, vh, 1);
    return v_float32x4(res);
}

#define OPENCV_HAL_IMPL_NEON_BIN_OP(bin_op, _Tpvec, intrin) \
inline _Tpvec operator bin_op (const _Tpvec& a, const _Tpvec& b) \
{ \
    return _Tpvec(intrin(a.val, b.val)); \
} \
inline _Tpvec& operator bin_op##= (_Tpvec& a, const _Tpvec& b) \
{ \
    a.val = intrin(a.val, b.val); \
    return a; \
}

OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_uint8x16, vqaddq_u8)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_uint8x16, vqsubq_u8)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_int8x16, vqaddq_s8)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_int8x16, vqsubq_s8)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_uint16x8, vqaddq_u16)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_uint16x8, vqsubq_u16)
OPENCV_HAL_IMPL_NEON_BIN_OP(*, v_uint16x8, vmulq_u16)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_int16x8, vqaddq_s16)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_int16x8, vqsubq_s16)
OPENCV_HAL_IMPL_NEON_BIN_OP(*, v_int16x8, vmulq_s16)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_int32x4, vaddq_s32)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_int32x4, vsubq_s32)
OPENCV_HAL_IMPL_NEON_BIN_OP(*, v_int32x4, vmulq_s32)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_uint32x4, vaddq_u32)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_uint32x4, vsubq_u32)
OPENCV_HAL_IMPL_NEON_BIN_OP(*, v_uint32x4, vmulq_u32)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_float32x4, vaddq_f32)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_float32x4, vsubq_f32)
OPENCV_HAL_IMPL_NEON_BIN_OP(*, v_float32x4, vmulq_f32)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_int64x2, vaddq_s64)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_int64x2, vsubq_s64)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_uint64x2, vaddq_u64)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_uint64x2, vsubq_u64)

inline v_float32x4 operator / (const v_float32x4& a, const v_float32x4& b)
{
    float32x4_t reciprocal = vrecpeq_f32(b.val);
    reciprocal = vmulq_f32(vrecpsq_f32(b.val, reciprocal), reciprocal);
    reciprocal = vmulq_f32(vrecpsq_f32(b.val, reciprocal), reciprocal);
    return v_float32x4(vmulq_f32(a.val, reciprocal));
}
inline v_float32x4& operator /= (v_float32x4& a, const v_float32x4& b)
{
    float32x4_t reciprocal = vrecpeq_f32(b.val);
    reciprocal = vmulq_f32(vrecpsq_f32(b.val, reciprocal), reciprocal);
    reciprocal = vmulq_f32(vrecpsq_f32(b.val, reciprocal), reciprocal);
    a.val = vmulq_f32(a.val, reciprocal);
    return a;
}

inline void v_mul_expand(const v_int16x8& a, const v_int16x8& b,
                         v_int32x4& c, v_int32x4& d)
{
    c.val = vmull_s16(vget_low_s16(a.val), vget_low_s16(b.val));
    d.val = vmull_s16(vget_high_s16(a.val), vget_high_s16(b.val));
}

inline void v_mul_expand(const v_uint16x8& a, const v_uint16x8& b,
                         v_uint32x4& c, v_uint32x4& d)
{
    c.val = vmull_u16(vget_low_u16(a.val), vget_low_u16(b.val));
    d.val = vmull_u16(vget_high_u16(a.val), vget_high_u16(b.val));
}

inline void v_mul_expand(const v_uint32x4& a, const v_uint32x4& b,
                         v_uint64x2& c, v_uint64x2& d)
{
    c.val = vmull_u32(vget_low_u32(a.val), vget_low_u32(b.val));
    d.val = vmull_u32(vget_high_u32(a.val), vget_high_u32(b.val));
}

inline v_int32x4 v_dotprod(const v_int16x8& a, const v_int16x8& b)
{
    int32x4_t c = vmull_s16(vget_low_s16(a.val), vget_low_s16(b.val));
    int32x4_t d = vmull_s16(vget_high_s16(a.val), vget_high_s16(b.val));
    int32x4x2_t cd = vuzpq_s32(c, d);
    return v_int32x4(vaddq_s32(cd.val[0], cd.val[1]));
}

#define OPENCV_HAL_IMPL_NEON_LOGIC_OP(_Tpvec, suffix) \
    OPENCV_HAL_IMPL_NEON_BIN_OP(&, _Tpvec, vandq_##suffix) \
    OPENCV_HAL_IMPL_NEON_BIN_OP(|, _Tpvec, vorrq_##suffix) \
    OPENCV_HAL_IMPL_NEON_BIN_OP(^, _Tpvec, veorq_##suffix) \
    inline _Tpvec operator ~ (const _Tpvec& a) \
    { \
        return _Tpvec(vreinterpretq_##suffix##_u8(vmvnq_u8(vreinterpretq_u8_##suffix(a.val)))); \
    }

OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_uint8x16, u8)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_int8x16, s8)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_uint16x8, u16)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_int16x8, s16)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_uint32x4, u32)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_int32x4, s32)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_uint64x2, u64)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_int64x2, s64)

#define OPENCV_HAL_IMPL_NEON_FLT_BIT_OP(bin_op, intrin) \
inline v_float32x4 operator bin_op (const v_float32x4& a, const v_float32x4& b) \
{ \
    return v_float32x4(vreinterpretq_f32_s32(intrin(vreinterpretq_s32_f32(a.val), vreinterpretq_s32_f32(b.val)))); \
} \
inline v_float32x4& operator bin_op##= (v_float32x4& a, const v_float32x4& b) \
{ \
    a.val = vreinterpretq_f32_s32(intrin(vreinterpretq_s32_f32(a.val), vreinterpretq_s32_f32(b.val))); \
    return a; \
}

OPENCV_HAL_IMPL_NEON_FLT_BIT_OP(&, vandq_s32)
OPENCV_HAL_IMPL_NEON_FLT_BIT_OP(|, vorrq_s32)
OPENCV_HAL_IMPL_NEON_FLT_BIT_OP(^, veorq_s32)

inline v_float32x4 operator ~ (const v_float32x4& a)
{
    return v_float32x4(vreinterpretq_f32_s32(vmvnq_s32(vreinterpretq_s32_f32(a.val))));
}

inline v_float32x4 v_sqrt(const v_float32x4& x)
{
    float32x4_t x1 = vmaxq_f32(x.val, vdupq_n_f32(FLT_MIN));
    float32x4_t e = vrsqrteq_f32(x1);
    e = vmulq_f32(vrsqrtsq_f32(vmulq_f32(x1, e), e), e);
    e = vmulq_f32(vrsqrtsq_f32(vmulq_f32(x1, e), e), e);
    return v_float32x4(vmulq_f32(x.val, e));
}

inline v_float32x4 v_invsqrt(const v_float32x4& x)
{
    float32x4_t e = vrsqrteq_f32(x.val);
    e = vmulq_f32(vrsqrtsq_f32(vmulq_f32(x.val, e), e), e);
    e = vmulq_f32(vrsqrtsq_f32(vmulq_f32(x.val, e), e), e);
    return v_float32x4(e);
}

inline v_float32x4 v_abs(v_float32x4 x)
{ return v_float32x4(vabsq_f32(x.val)); }

// TODO: exp, log, sin, cos

#define OPENCV_HAL_IMPL_NEON_BIN_FUNC(_Tpvec, func, intrin) \
inline _Tpvec func(const _Tpvec& a, const _Tpvec& b) \
{ \
    return _Tpvec(intrin(a.val, b.val)); \
}

OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint8x16, v_min, vminq_u8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint8x16, v_max, vmaxq_u8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int8x16, v_min, vminq_s8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int8x16, v_max, vmaxq_s8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint16x8, v_min, vminq_u16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint16x8, v_max, vmaxq_u16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int16x8, v_min, vminq_s16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int16x8, v_max, vmaxq_s16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint32x4, v_min, vminq_u32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint32x4, v_max, vmaxq_u32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int32x4, v_min, vminq_s32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int32x4, v_max, vmaxq_s32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_float32x4, v_min, vminq_f32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_float32x4, v_max, vmaxq_f32)


#define OPENCV_HAL_IMPL_NEON_INT_CMP_OP(_Tpvec, cast, suffix, not_suffix) \
inline _Tpvec operator == (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vceqq_##suffix(a.val, b.val))); } \
inline _Tpvec operator != (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vmvnq_##not_suffix(vceqq_##suffix(a.val, b.val)))); } \
inline _Tpvec operator < (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vcltq_##suffix(a.val, b.val))); } \
inline _Tpvec operator > (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vcgtq_##suffix(a.val, b.val))); } \
inline _Tpvec operator <= (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vcleq_##suffix(a.val, b.val))); } \
inline _Tpvec operator >= (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vcgeq_##suffix(a.val, b.val))); }

OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_uint8x16, OPENCV_HAL_NOP, u8, u8)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_int8x16, vreinterpretq_s8_u8, s8, u8)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_uint16x8, OPENCV_HAL_NOP, u16, u16)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_int16x8, vreinterpretq_s16_u16, s16, u16)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_uint32x4, OPENCV_HAL_NOP, u32, u32)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_int32x4, vreinterpretq_s32_u32, s32, u32)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_float32x4, vreinterpretq_f32_u32, f32, u32)

OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint8x16, v_add_wrap, vaddq_u8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int8x16, v_add_wrap, vaddq_s8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint16x8, v_add_wrap, vaddq_u16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int16x8, v_add_wrap, vaddq_s16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint8x16, v_sub_wrap, vsubq_u8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int8x16, v_sub_wrap, vsubq_s8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint16x8, v_sub_wrap, vsubq_u16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int16x8, v_sub_wrap, vsubq_s16)

// TODO: absdiff for signed integers
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint8x16, v_absdiff, vabdq_u8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint16x8, v_absdiff, vabdq_u16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint32x4, v_absdiff, vabdq_u32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_float32x4, v_absdiff, vabdq_f32)

#define OPENCV_HAL_IMPL_NEON_BIN_FUNC2(_Tpvec, _Tpvec2, cast, func, intrin) \
inline _Tpvec2 func(const _Tpvec& a, const _Tpvec& b) \
{ \
    return _Tpvec2(cast(intrin(a.val, b.val))); \
}

OPENCV_HAL_IMPL_NEON_BIN_FUNC2(v_int8x16, v_uint8x16, vreinterpretq_u8_s8, v_absdiff, vabdq_s8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC2(v_int16x8, v_uint16x8, vreinterpretq_u16_s16, v_absdiff, vabdq_s16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC2(v_int32x4, v_uint32x4, vreinterpretq_u32_s32, v_absdiff, vabdq_s32)

inline v_float32x4 v_magnitude(const v_float32x4& a, const v_float32x4& b)
{
    v_float32x4 x(vmlaq_f32(vmulq_f32(a.val, a.val), b.val, b.val));
    return v_sqrt(x);
}

inline v_float32x4 v_sqr_magnitude(const v_float32x4& a, const v_float32x4& b)
{
    return v_float32x4(vmlaq_f32(vmulq_f32(a.val, a.val), b.val, b.val));
}

inline v_float32x4 v_muladd(const v_float32x4& a, const v_float32x4& b, const v_float32x4& c)
{
    return v_float32x4(vmlaq_f32(c.val, a.val, b.val));
}

// trade efficiency for convenience
#define OPENCV_HAL_IMPL_NEON_SHIFT_OP(_Tpvec, suffix, _Tps, ssuffix) \
inline _Tpvec operator << (const _Tpvec& a, int n) \
{ return _Tpvec(vshlq_##suffix(a.val, vdupq_n_##ssuffix((_Tps)n))); } \
inline _Tpvec operator >> (const _Tpvec& a, int n) \
{ return _Tpvec(vshlq_##suffix(a.val, vdupq_n_##ssuffix((_Tps)-n))); } \
template<int n> inline _Tpvec v_shl(const _Tpvec& a) \
{ return _Tpvec(vshlq_n_##suffix(a.val, n)); } \
template<int n> inline _Tpvec v_shr(const _Tpvec& a) \
{ return _Tpvec(vshrq_n_##suffix(a.val, n)); } \
template<int n> inline _Tpvec v_rshr(const _Tpvec& a) \
{ return _Tpvec(vrshrq_n_##suffix(a.val, n)); }

OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_uint8x16, u8, schar, s8)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_int8x16, s8, schar, s8)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_uint16x8, u16, short, s16)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_int16x8, s16, short, s16)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_uint32x4, u32, int, s32)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_int32x4, s32, int, s32)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_uint64x2, u64, int64, s64)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_int64x2, s64, int64, s64)

#define OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(_Tpvec, _Tp, suffix) \
inline _Tpvec v_load(const _Tp* ptr) \
{ return _Tpvec(vld1q_##suffix(ptr)); } \
inline _Tpvec v_load_aligned(const _Tp* ptr) \
{ return _Tpvec(vld1q_##suffix(ptr)); } \
inline _Tpvec v_load_halves(const _Tp* ptr0, const _Tp* ptr1) \
{ return _Tpvec(vcombine_##suffix(vld1_##suffix(ptr0), vld1_##suffix(ptr1))); } \
inline void v_store(_Tp* ptr, const _Tpvec& a) \
{ vst1q_##suffix(ptr, a.val); } \
inline void v_store_aligned(_Tp* ptr, const _Tpvec& a) \
{ vst1q_##suffix(ptr, a.val); } \
inline void v_store_low(_Tp* ptr, const _Tpvec& a) \
{ vst1_##suffix(ptr, vget_low_##suffix(a.val)); } \
inline void v_store_high(_Tp* ptr, const _Tpvec& a) \
{ vst1_##suffix(ptr, vget_high_##suffix(a.val)); }

OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_uint8x16, uchar, u8)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_int8x16, schar, s8)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_uint16x8, ushort, u16)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_int16x8, short, s16)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_uint32x4, unsigned, u32)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_int32x4, int, s32)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_uint64x2, uint64, u64)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_int64x2, int64, s64)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_float32x4, float, f32)

#define OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(_Tpvec, scalartype, func, scalar_func) \
inline scalartype v_reduce_##func(const _Tpvec& a) \
{ \
    scalartype CV_DECL_ALIGNED(16) buf[4]; \
    v_store_aligned(buf, a); \
    scalartype s0 = scalar_func(buf[0], buf[1]); \
    scalartype s1 = scalar_func(buf[2], buf[3]); \
    return scalar_func(s0, s1); \
}

OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_uint32x4, unsigned, sum, OPENCV_HAL_ADD)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_uint32x4, unsigned, max, std::max)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_uint32x4, unsigned, min, std::min)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_int32x4, int, sum, OPENCV_HAL_ADD)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_int32x4, int, max, std::max)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_int32x4, int, min, std::min)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_float32x4, float, sum, OPENCV_HAL_ADD)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_float32x4, float, max, std::max)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_float32x4, float, min, std::min)

inline int v_signmask(const v_uint8x16& a)
{
    int8x8_t m0 = vcreate_s8(CV_BIG_UINT(0x0706050403020100));
    uint8x16_t v0 = vshlq_u8(vshrq_n_u8(a.val, 7), vcombine_s8(m0, m0));
    uint64x2_t v1 = vpaddlq_u32(vpaddlq_u16(vpaddlq_u8(v0)));
    return (int)vgetq_lane_u64(v1, 0) + ((int)vgetq_lane_u64(v1, 1) << 8);
}
inline int v_signmask(const v_int8x16& a)
{ return v_signmask(v_reinterpret_as_u8(a)); }

inline int v_signmask(const v_uint16x8& a)
{
    int16x4_t m0 = vcreate_s16(CV_BIG_UINT(0x0003000200010000));
    uint16x8_t v0 = vshlq_u16(vshrq_n_u16(a.val, 15), vcombine_s16(m0, m0));
    uint64x2_t v1 = vpaddlq_u32(vpaddlq_u16(v0));
    return (int)vgetq_lane_u64(v1, 0) + ((int)vgetq_lane_u64(v1, 1) << 4);
}
inline int v_signmask(const v_int16x8& a)
{ return v_signmask(v_reinterpret_as_u16(a)); }

inline int v_signmask(const v_uint32x4& a)
{
    int32x2_t m0 = vcreate_s32(CV_BIG_UINT(0x0000000100000000));
    uint32x4_t v0 = vshlq_u32(vshrq_n_u32(a.val, 31), vcombine_s32(m0, m0));
    uint64x2_t v1 = vpaddlq_u32(v0);
    return (int)vgetq_lane_u64(v1, 0) + ((int)vgetq_lane_u64(v1, 1) << 2);
}
inline int v_signmask(const v_int32x4& a)
{ return v_signmask(v_reinterpret_as_u32(a)); }
inline int v_signmask(const v_float32x4& a)
{ return v_signmask(v_reinterpret_as_u32(a)); }

#define OPENCV_HAL_IMPL_NEON_CHECK_ALLANY(_Tpvec, suffix, shift) \
inline bool v_check_all(const v_##_Tpvec& a) \
{ \
    _Tpvec##_t v0 = vshrq_n_##suffix(vmvnq_##suffix(a.val), shift); \
    uint64x2_t v1 = vreinterpretq_u64_##suffix(v0); \
    return (vgetq_lane_u64(v1, 0) | vgetq_lane_u64(v1, 1)) == 0; \
} \
inline bool v_check_any(const v_##_Tpvec& a) \
{ \
    _Tpvec##_t v0 = vshrq_n_##suffix(a.val, shift); \
    uint64x2_t v1 = vreinterpretq_u64_##suffix(v0); \
    return (vgetq_lane_u64(v1, 0) | vgetq_lane_u64(v1, 1)) != 0; \
}

OPENCV_HAL_IMPL_NEON_CHECK_ALLANY(uint8x16, u8, 7)
OPENCV_HAL_IMPL_NEON_CHECK_ALLANY(uint16x8, u16, 15)
OPENCV_HAL_IMPL_NEON_CHECK_ALLANY(uint32x4, u32, 31)

inline bool v_check_all(const v_int8x16& a)
{ return v_check_all(v_reinterpret_as_u8(a)); }
inline bool v_check_all(const v_int16x8& a)
{ return v_check_all(v_reinterpret_as_u16(a)); }
inline bool v_check_all(const v_int32x4& a)
{ return v_check_all(v_reinterpret_as_u32(a)); }
inline bool v_check_all(const v_float32x4& a)
{ return v_check_all(v_reinterpret_as_u32(a)); }

inline bool v_check_any(const v_int8x16& a)
{ return v_check_any(v_reinterpret_as_u8(a)); }
inline bool v_check_any(const v_int16x8& a)
{ return v_check_any(v_reinterpret_as_u16(a)); }
inline bool v_check_any(const v_int32x4& a)
{ return v_check_any(v_reinterpret_as_u32(a)); }
inline bool v_check_any(const v_float32x4& a)
{ return v_check_any(v_reinterpret_as_u32(a)); }

#define OPENCV_HAL_IMPL_NEON_SELECT(_Tpvec, suffix, usuffix) \
inline _Tpvec v_select(const _Tpvec& mask, const _Tpvec& a, const _Tpvec& b) \
{ \
    return _Tpvec(vbslq_##suffix(vreinterpretq_##usuffix##_##suffix(mask.val), a.val, b.val)); \
}

OPENCV_HAL_IMPL_NEON_SELECT(v_uint8x16, u8, u8)
OPENCV_HAL_IMPL_NEON_SELECT(v_int8x16, s8, u8)
OPENCV_HAL_IMPL_NEON_SELECT(v_uint16x8, u16, u16)
OPENCV_HAL_IMPL_NEON_SELECT(v_int16x8, s16, u16)
OPENCV_HAL_IMPL_NEON_SELECT(v_uint32x4, u32, u32)
OPENCV_HAL_IMPL_NEON_SELECT(v_int32x4, s32, u32)
OPENCV_HAL_IMPL_NEON_SELECT(v_float32x4, f32, u32)

#define OPENCV_HAL_IMPL_NEON_EXPAND(_Tpvec, _Tpwvec, _Tp, suffix) \
inline void v_expand(const _Tpvec& a, _Tpwvec& b0, _Tpwvec& b1) \
{ \
    b0.val = vmovl_##suffix(vget_low_##suffix(a.val)); \
    b1.val = vmovl_##suffix(vget_high_##suffix(a.val)); \
} \
inline _Tpwvec v_load_expand(const _Tp* ptr) \
{ \
    return _Tpwvec(vmovl_##suffix(vld1_##suffix(ptr))); \
}

OPENCV_HAL_IMPL_NEON_EXPAND(v_uint8x16, v_uint16x8, uchar, u8)
OPENCV_HAL_IMPL_NEON_EXPAND(v_int8x16, v_int16x8, schar, s8)
OPENCV_HAL_IMPL_NEON_EXPAND(v_uint16x8, v_uint32x4, ushort, u16)
OPENCV_HAL_IMPL_NEON_EXPAND(v_int16x8, v_int32x4, short, s16)
OPENCV_HAL_IMPL_NEON_EXPAND(v_uint32x4, v_uint64x2, uint, u32)
OPENCV_HAL_IMPL_NEON_EXPAND(v_int32x4, v_int64x2, int, s32)

inline v_uint32x4 v_load_expand_q(const uchar* ptr)
{
    uint8x8_t v0 = vcreate_u8(*(unsigned*)ptr);
    uint16x4_t v1 = vget_low_u16(vmovl_u8(v0));
    return v_uint32x4(vmovl_u16(v1));
}

inline v_int32x4 v_load_expand_q(const schar* ptr)
{
    int8x8_t v0 = vcreate_s8(*(unsigned*)ptr);
    int16x4_t v1 = vget_low_s16(vmovl_s8(v0));
    return v_int32x4(vmovl_s16(v1));
}

#define OPENCV_HAL_IMPL_NEON_UNPACKS(_Tpvec, suffix) \
inline void v_zip(const v_##_Tpvec& a0, const v_##_Tpvec& a1, v_##_Tpvec& b0, v_##_Tpvec& b1) \
{ \
    _Tpvec##x2_t p = vzipq_##suffix(a0.val, a1.val); \
    b0.val = p.val[0]; \
    b1.val = p.val[1]; \
} \
inline v_##_Tpvec v_combine_low(const v_##_Tpvec& a, const v_##_Tpvec& b) \
{ \
    return v_##_Tpvec(vcombine_##suffix(vget_low_##suffix(a.val), vget_low_##suffix(b.val))); \
} \
inline v_##_Tpvec v_combine_high(const v_##_Tpvec& a, const v_##_Tpvec& b) \
{ \
    return v_##_Tpvec(vcombine_##suffix(vget_high_##suffix(a.val), vget_high_##suffix(b.val))); \
} \
inline void v_recombine(const v_##_Tpvec& a, const v_##_Tpvec& b, v_##_Tpvec& c, v_##_Tpvec& d) \
{ \
    c.val = vcombine_##suffix(vget_low_##suffix(a.val), vget_low_##suffix(b.val)); \
    d.val = vcombine_##suffix(vget_high_##suffix(a.val), vget_high_##suffix(b.val)); \
}

OPENCV_HAL_IMPL_NEON_UNPACKS(uint8x16, u8)
OPENCV_HAL_IMPL_NEON_UNPACKS(int8x16, s8)
OPENCV_HAL_IMPL_NEON_UNPACKS(uint16x8, u16)
OPENCV_HAL_IMPL_NEON_UNPACKS(int16x8, s16)
OPENCV_HAL_IMPL_NEON_UNPACKS(uint32x4, u32)
OPENCV_HAL_IMPL_NEON_UNPACKS(int32x4, s32)
OPENCV_HAL_IMPL_NEON_UNPACKS(float32x4, f32)

#define OPENCV_HAL_IMPL_NEON_EXTRACT(_Tpvec, suffix) \
template <int s> \
inline v_##_Tpvec v_extract(const v_##_Tpvec& a, const v_##_Tpvec& b) \
{ \
    return v_##_Tpvec(vextq_##suffix(a.val, b.val, s)); \
}

OPENCV_HAL_IMPL_NEON_EXTRACT(uint8x16, u8)
OPENCV_HAL_IMPL_NEON_EXTRACT(int8x16, s8)
OPENCV_HAL_IMPL_NEON_EXTRACT(uint16x8, u16)
OPENCV_HAL_IMPL_NEON_EXTRACT(int16x8, s16)
OPENCV_HAL_IMPL_NEON_EXTRACT(uint32x4, u32)
OPENCV_HAL_IMPL_NEON_EXTRACT(int32x4, s32)
OPENCV_HAL_IMPL_NEON_EXTRACT(uint64x2, u64)
OPENCV_HAL_IMPL_NEON_EXTRACT(int64x2, s64)
OPENCV_HAL_IMPL_NEON_EXTRACT(float32x4, f32)

inline v_int32x4 v_round(const v_float32x4& a)
{
    static const int32x4_t v_sign = vdupq_n_s32(1 << 31),
        v_05 = vreinterpretq_s32_f32(vdupq_n_f32(0.5f));

    int32x4_t v_addition = vorrq_s32(v_05, vandq_s32(v_sign, vreinterpretq_s32_f32(a.val)));
    return v_int32x4(vcvtq_s32_f32(vaddq_f32(a.val, vreinterpretq_f32_s32(v_addition))));
}

inline v_int32x4 v_floor(const v_float32x4& a)
{
    int32x4_t a1 = vcvtq_s32_f32(a.val);
    uint32x4_t mask = vcgtq_f32(vcvtq_f32_s32(a1), a.val);
    return v_int32x4(vaddq_s32(a1, vreinterpretq_s32_u32(mask)));
}

inline v_int32x4 v_ceil(const v_float32x4& a)
{
    int32x4_t a1 = vcvtq_s32_f32(a.val);
    uint32x4_t mask = vcgtq_f32(a.val, vcvtq_f32_s32(a1));
    return v_int32x4(vsubq_s32(a1, vreinterpretq_s32_u32(mask)));
}

inline v_int32x4 v_trunc(const v_float32x4& a)
{ return v_int32x4(vcvtq_s32_f32(a.val)); }

#define OPENCV_HAL_IMPL_NEON_TRANSPOSE4x4(_Tpvec, suffix) \
inline void v_transpose4x4(const v_##_Tpvec& a0, const v_##_Tpvec& a1, \
                         const v_##_Tpvec& a2, const v_##_Tpvec& a3, \
                         v_##_Tpvec& b0, v_##_Tpvec& b1, \
                         v_##_Tpvec& b2, v_##_Tpvec& b3) \
{ \
    /* m00 m01 m02 m03 */ \
    /* m10 m11 m12 m13 */ \
    /* m20 m21 m22 m23 */ \
    /* m30 m31 m32 m33 */ \
    _Tpvec##x2_t t0 = vtrnq_##suffix(a0.val, a1.val); \
    _Tpvec##x2_t t1 = vtrnq_##suffix(a2.val, a3.val); \
    /* m00 m10 m02 m12 */ \
    /* m01 m11 m03 m13 */ \
    /* m20 m30 m22 m32 */ \
    /* m21 m31 m23 m33 */ \
    b0.val = vcombine_##suffix(vget_low_##suffix(t0.val[0]), vget_low_##suffix(t1.val[0])); \
    b1.val = vcombine_##suffix(vget_low_##suffix(t0.val[1]), vget_low_##suffix(t1.val[1])); \
    b2.val = vcombine_##suffix(vget_high_##suffix(t0.val[0]), vget_high_##suffix(t1.val[0])); \
    b3.val = vcombine_##suffix(vget_high_##suffix(t0.val[1]), vget_high_##suffix(t1.val[1])); \
}

OPENCV_HAL_IMPL_NEON_TRANSPOSE4x4(uint32x4, u32)
OPENCV_HAL_IMPL_NEON_TRANSPOSE4x4(int32x4, s32)
OPENCV_HAL_IMPL_NEON_TRANSPOSE4x4(float32x4, f32)

#define OPENCV_HAL_IMPL_NEON_INTERLEAVED(_Tpvec, _Tp, suffix) \
inline void v_load_deinterleave(const _Tp* ptr, v_##_Tpvec& a, v_##_Tpvec& b, v_##_Tpvec& c) \
{ \
    _Tpvec##x3_t v = vld3q_##suffix(ptr); \
    a.val = v.val[0]; \
    b.val = v.val[1]; \
    c.val = v.val[2]; \
} \
inline void v_load_deinterleave(const _Tp* ptr, v_##_Tpvec& a, v_##_Tpvec& b, \
                                v_##_Tpvec& c, v_##_Tpvec& d) \
{ \
    _Tpvec##x4_t v = vld4q_##suffix(ptr); \
    a.val = v.val[0]; \
    b.val = v.val[1]; \
    c.val = v.val[2]; \
    d.val = v.val[3]; \
} \
inline void v_store_interleave( _Tp* ptr, const v_##_Tpvec& a, const v_##_Tpvec& b, const v_##_Tpvec& c) \
{ \
    _Tpvec##x3_t v; \
    v.val[0] = a.val; \
    v.val[1] = b.val; \
    v.val[2] = c.val; \
    vst3q_##suffix(ptr, v); \
} \
inline void v_store_interleave( _Tp* ptr, const v_##_Tpvec& a, const v_##_Tpvec& b, \
                               const v_##_Tpvec& c, const v_##_Tpvec& d) \
{ \
    _Tpvec##x4_t v; \
    v.val[0] = a.val; \
    v.val[1] = b.val; \
    v.val[2] = c.val; \
    v.val[3] = d.val; \
    vst4q_##suffix(ptr, v); \
}

OPENCV_HAL_IMPL_NEON_INTERLEAVED(uint8x16, uchar, u8)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(int8x16, schar, s8)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(uint16x8, ushort, u16)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(int16x8, short, s16)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(uint32x4, unsigned, u32)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(int32x4, int, s32)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(float32x4, float, f32)

inline v_float32x4 v_cvt_f32(const v_int32x4& a)
{
    return v_float32x4(vcvtq_f32_s32(a.val));
}

//! @endcond

}

#endif
