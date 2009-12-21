/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
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
//   * The name of Intel Corporation may not be used to endorse or promote products
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

/* The header is mostly for internal use and it is likely to change.
   It contains some macro definitions that are used in cxcore, cv, cvaux
   and, probably, other libraries. If you need some of this functionality,
   the safe way is to copy it into your code and rename the macros.
*/
#ifndef _CXCORE_MISC_H_
#define _CXCORE_MISC_H_

#ifdef HAVE_CONFIG_H
    #include "cvconfig.h"
#endif

#include <limits.h>
#ifdef _OPENMP
#include "omp.h"
#endif

/****************************************************************************************\
*                              Compile-time tuning parameters                            *
\****************************************************************************************/

/* maximal size of vector to run matrix operations on it inline (i.e. w/o ipp calls) */
#define  CV_MAX_INLINE_MAT_OP_SIZE  10

/* maximal linear size of matrix to allocate it on stack. */
#define  CV_MAX_LOCAL_MAT_SIZE  32

/* maximal size of local memory storage */
#define  CV_MAX_LOCAL_SIZE  \
    (CV_MAX_LOCAL_MAT_SIZE*CV_MAX_LOCAL_MAT_SIZE*(int)sizeof(double))

/* default image row align (in bytes) */
#define  CV_DEFAULT_IMAGE_ROW_ALIGN  4

/* matrices are continuous by default */
#define  CV_DEFAULT_MAT_ROW_ALIGN  1

/* maximum size of dynamic memory buffer.
   cvAlloc reports an error if a larger block is requested. */
#define  CV_MAX_ALLOC_SIZE    (((size_t)1 << (sizeof(size_t)*8-2)))

/* the alignment of all the allocated buffers */
#define  CV_MALLOC_ALIGN    32

/* default alignment for dynamic data strucutures, resided in storages. */
#define  CV_STRUCT_ALIGN    ((int)sizeof(double))

/* default storage block size */
#define  CV_STORAGE_BLOCK_SIZE   ((1<<16) - 128)

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK    (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

/* max length of strings */
#define  CV_MAX_STRLEN  1024

/* maximum possible number of threads in parallel implementations */
#ifdef _OPENMP
#define CV_MAX_THREADS 128
#else
#define CV_MAX_THREADS 1
#endif

#if 0 /*def  CV_CHECK_FOR_NANS*/
    #define CV_CHECK_NANS( arr ) cvCheckArray((arr))
#else
    #define CV_CHECK_NANS( arr )
#endif

/****************************************************************************************\
*                                  Common declarations                                   *
\****************************************************************************************/

/* get alloca declaration */
#ifdef __GNUC__
    #undef alloca
    #define alloca __builtin_alloca
#elif defined WIN32 || defined WIN64
    #if defined _MSC_VER || defined __BORLANDC__
        #include <malloc.h>
    #endif
#elif defined HAVE_ALLOCA_H
    #include <alloca.h>
#elif defined HAVE_ALLOCA
    #include <stdlib.h>
#elif
    #error
#endif

/* ! DO NOT make it an inline function */
#define cvStackAlloc(size) cvAlignPtr( alloca((size) + CV_MALLOC_ALIGN), CV_MALLOC_ALIGN )

#if defined _MSC_VER || defined __BORLANDC__
    #define CV_BIG_INT(n)   n##I64
    #define CV_BIG_UINT(n)  n##UI64
#else
    #define CV_BIG_INT(n)   n##LL
    #define CV_BIG_UINT(n)  n##ULL
#endif

#define CV_IMPL CV_EXTERN_C

#define CV_DBG_BREAK() { volatile int* crashMe = 0; *crashMe = 0; }

/* default step, set in case of continuous data
   to work around checks for valid step in some ipp functions */
#define  CV_STUB_STEP     (1 << 30)

#define  CV_SIZEOF_FLOAT ((int)sizeof(float))
#define  CV_SIZEOF_SHORT ((int)sizeof(short))

#define  CV_ORIGIN_TL  0
#define  CV_ORIGIN_BL  1

/* IEEE754 constants and macros */
#define  CV_POS_INF       0x7f800000
#define  CV_NEG_INF       0x807fffff /* CV_TOGGLE_FLT(0xff800000) */
#define  CV_1F            0x3f800000
#define  CV_TOGGLE_FLT(x) ((x)^((int)(x) < 0 ? 0x7fffffff : 0))
#define  CV_TOGGLE_DBL(x) \
    ((x)^((int64)(x) < 0 ? CV_BIG_INT(0x7fffffffffffffff) : 0))

#define  CV_NOP(a)      (a)
#define  CV_ADD(a, b)   ((a) + (b))
#define  CV_SUB(a, b)   ((a) - (b))
#define  CV_MUL(a, b)   ((a) * (b))
#define  CV_AND(a, b)   ((a) & (b))
#define  CV_OR(a, b)    ((a) | (b))
#define  CV_XOR(a, b)   ((a) ^ (b))
#define  CV_ANDN(a, b)  (~(a) & (b))
#define  CV_ORN(a, b)   (~(a) | (b))
#define  CV_SQR(a)      ((a) * (a))

#define  CV_LT(a, b)    ((a) < (b))
#define  CV_LE(a, b)    ((a) <= (b))
#define  CV_EQ(a, b)    ((a) == (b))
#define  CV_NE(a, b)    ((a) != (b))
#define  CV_GT(a, b)    ((a) > (b))
#define  CV_GE(a, b)    ((a) >= (b))

#define  CV_NONZERO(a)      ((a) != 0)
#define  CV_NONZERO_FLT(a)  (((a)+(a)) != 0)

/* general-purpose saturation macros */
#define  CV_CAST_8U(t)  (uchar)(!((t) & ~255) ? (t) : (t) > 0 ? 255 : 0)
#define  CV_CAST_8S(t)  (schar)(!(((t)+128) & ~255) ? (t) : (t) > 0 ? 127 : -128)
#define  CV_CAST_16U(t) (ushort)(!((t) & ~65535) ? (t) : (t) > 0 ? 65535 : 0)
#define  CV_CAST_16S(t) (short)(!(((t)+32768) & ~65535) ? (t) : (t) > 0 ? 32767 : -32768)
#define  CV_CAST_32S(t) (int)(t)
#define  CV_CAST_64S(t) (int64)(t)
#define  CV_CAST_32F(t) (float)(t)
#define  CV_CAST_64F(t) (double)(t)

#define  CV_PASTE2(a,b) a##b
#define  CV_PASTE(a,b)  CV_PASTE2(a,b)

#define  CV_EMPTY
#define  CV_MAKE_STR(a) #a

#define  CV_DEFINE_MASK         \
    float maskTab[2]; maskTab[0] = 0.f; maskTab[1] = 1.f;
#define  CV_ANDMASK( m, x )     ((x) & (((m) == 0) - 1))

/* (x) * ((m) == 1 ? 1.f : (m) == 0 ? 0.f : <ERR> */
#define  CV_MULMASK( m, x )       (maskTab[(m) != 0]*(x))

/* (x) * ((m) == -1 ? 1.f : (m) == 0 ? 0.f : <ERR> */
#define  CV_MULMASK1( m, x )      (maskTab[(m)+1]*(x))

#define  CV_ZERO_OBJ(x)  memset((x), 0, sizeof(*(x)))

#define  CV_DIM(static_array) ((int)(sizeof(static_array)/sizeof((static_array)[0])))

#define  CV_UN_ENTRY_C1(worktype)           \
    worktype s0 = scalar[0]

#define  CV_UN_ENTRY_C2(worktype)           \
    worktype s0 = scalar[0], s1 = scalar[1]

#define  CV_UN_ENTRY_C3(worktype)           \
    worktype s0 = scalar[0], s1 = scalar[1], s2 = scalar[2]

#define  CV_UN_ENTRY_C4(worktype)           \
    worktype s0 = scalar[0], s1 = scalar[1], s2 = scalar[2], s3 = scalar[3]

#define  cvUnsupportedFormat "Unsupported format"

CV_INLINE void* cvAlignPtr( const void* ptr, int align=32 )
{
    assert( (align & (align-1)) == 0 );
    return (void*)( ((size_t)ptr + align - 1) & ~(size_t)(align-1) );
}

CV_INLINE int cvAlign( int size, int align )
{
    assert( (align & (align-1)) == 0 && size < INT_MAX );
    return (size + align - 1) & -align;
}

CV_INLINE  CvSize  cvGetMatSize( const CvMat* mat )
{
    CvSize size = { mat->width, mat->height };
    return size;
}

#define  CV_DESCALE(x,n)     (((x) + (1 << ((n)-1))) >> (n))
#define  CV_FLT_TO_FIX(x,n)  cvRound((x)*(1<<(n)))

#if 0
/* This is a small engine for performing fast division of multiple numbers
   by the same constant. Most compilers do it too if they know the divisor value
   at compile-time. The algorithm was taken from Agner Fog's optimization guide
   at http://www.agner.org/assem */
typedef struct CvFastDiv
{
    unsigned delta, scale, divisor;
}
CvFastDiv;

#define CV_FAST_DIV_SHIFT 32

CV_INLINE CvFastDiv cvFastDiv( int divisor )
{
    CvFastDiv fastdiv;

    assert( divisor >= 1 );
    uint64 temp = ((uint64)1 << CV_FAST_DIV_SHIFT)/divisor;

    fastdiv.divisor = divisor;
    fastdiv.delta = (unsigned)(((temp & 1) ^ 1) + divisor - 1);
    fastdiv.scale = (unsigned)((temp + 1) >> 1);

    return fastdiv;
}

#define CV_FAST_DIV( x, fastdiv )  \
    ((int)(((int64)((x)*2 + (int)(fastdiv).delta))*(int)(fastdiv).scale>>CV_FAST_DIV_SHIFT))

#define CV_FAST_UDIV( x, fastdiv )  \
    ((int)(((uint64)((x)*2 + (fastdiv).delta))*(fastdiv).scale>>CV_FAST_DIV_SHIFT))
#endif

#define CV_MEMCPY_CHAR( dst, src, len )                                             \
{                                                                                   \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len);                                \
    char* _icv_memcpy_dst_ = (char*)(dst);                                          \
    const char* _icv_memcpy_src_ = (const char*)(src);                              \
                                                                                    \
    for( _icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_; _icv_memcpy_i_++ )  \
        _icv_memcpy_dst_[_icv_memcpy_i_] = _icv_memcpy_src_[_icv_memcpy_i_];        \
}


#define CV_MEMCPY_INT( dst, src, len )                                              \
{                                                                                   \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len);                                \
    int* _icv_memcpy_dst_ = (int*)(dst);                                            \
    const int* _icv_memcpy_src_ = (const int*)(src);                                \
    assert( ((size_t)_icv_memcpy_src_&(sizeof(int)-1)) == 0 &&                      \
            ((size_t)_icv_memcpy_dst_&(sizeof(int)-1)) == 0 );                      \
                                                                                    \
    for(_icv_memcpy_i_=0;_icv_memcpy_i_<_icv_memcpy_len_;_icv_memcpy_i_++)          \
        _icv_memcpy_dst_[_icv_memcpy_i_] = _icv_memcpy_src_[_icv_memcpy_i_];        \
}


#define CV_MEMCPY_AUTO( dst, src, len )                                             \
{                                                                                   \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len);                                \
    char* _icv_memcpy_dst_ = (char*)(dst);                                          \
    const char* _icv_memcpy_src_ = (const char*)(src);                              \
    if( (_icv_memcpy_len_ & (sizeof(int)-1)) == 0 )                                 \
    {                                                                               \
        assert( ((size_t)_icv_memcpy_src_&(sizeof(int)-1)) == 0 &&                  \
                ((size_t)_icv_memcpy_dst_&(sizeof(int)-1)) == 0 );                  \
        for( _icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_;                 \
            _icv_memcpy_i_+=sizeof(int) )                                           \
        {                                                                           \
            *(int*)(_icv_memcpy_dst_+_icv_memcpy_i_) =                              \
            *(const int*)(_icv_memcpy_src_+_icv_memcpy_i_);                         \
        }                                                                           \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        for(_icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_; _icv_memcpy_i_++)\
            _icv_memcpy_dst_[_icv_memcpy_i_] = _icv_memcpy_src_[_icv_memcpy_i_];    \
    }                                                                               \
}


#define CV_ZERO_CHAR( dst, len )                                                    \
{                                                                                   \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len);                                \
    char* _icv_memcpy_dst_ = (char*)(dst);                                          \
                                                                                    \
    for( _icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_; _icv_memcpy_i_++ )  \
        _icv_memcpy_dst_[_icv_memcpy_i_] = '\0';                                    \
}


#define CV_ZERO_INT( dst, len )                                                     \
{                                                                                   \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len);                                \
    int* _icv_memcpy_dst_ = (int*)(dst);                                            \
    assert( ((size_t)_icv_memcpy_dst_&(sizeof(int)-1)) == 0 );                      \
                                                                                    \
    for(_icv_memcpy_i_=0;_icv_memcpy_i_<_icv_memcpy_len_;_icv_memcpy_i_++)          \
        _icv_memcpy_dst_[_icv_memcpy_i_] = 0;                                       \
}


/****************************************************************************************\

  Generic implementation of QuickSort algorithm.
  ----------------------------------------------
  Using this macro user can declare customized sort function that can be much faster
  than built-in qsort function because of lower overhead on elements
  comparison and exchange. The macro takes less_than (or LT) argument - a macro or function
  that takes 2 arguments returns non-zero if the first argument should be before the second
  one in the sorted sequence and zero otherwise.

  Example:

    Suppose that the task is to sort points by ascending of y coordinates and if
    y's are equal x's should ascend.

    The code is:
    ------------------------------------------------------------------------------
           #define cmp_pts( pt1, pt2 ) \
               ((pt1).y < (pt2).y || ((pt1).y < (pt2).y && (pt1).x < (pt2).x))

           [static] CV_IMPLEMENT_QSORT( icvSortPoints, CvPoint, cmp_pts )
    ------------------------------------------------------------------------------

    After that the function "void icvSortPoints( CvPoint* array, size_t total, int aux );"
    is available to user.

  aux is an additional parameter, which can be used when comparing elements.
  The current implementation was derived from *BSD system qsort():

    * Copyright (c) 1992, 1993
    *  The Regents of the University of California.  All rights reserved.
    *
    * Redistribution and use in source and binary forms, with or without
    * modification, are permitted provided that the following conditions
    * are met:
    * 1. Redistributions of source code must retain the above copyright
    *    notice, this list of conditions and the following disclaimer.
    * 2. Redistributions in binary form must reproduce the above copyright
    *    notice, this list of conditions and the following disclaimer in the
    *    documentation and/or other materials provided with the distribution.
    * 3. All advertising materials mentioning features or use of this software
    *    must display the following acknowledgement:
    *  This product includes software developed by the University of
    *  California, Berkeley and its contributors.
    * 4. Neither the name of the University nor the names of its contributors
    *    may be used to endorse or promote products derived from this software
    *    without specific prior written permission.
    *
    * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
    * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
    * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    * SUCH DAMAGE.

\****************************************************************************************/

#define CV_IMPLEMENT_QSORT_EX( func_name, T, LT, user_data_type )                   \
void func_name( T *array, size_t total, user_data_type aux )                        \
{                                                                                   \
    int isort_thresh = 7;                                                           \
    T t;                                                                            \
    int sp = 0;                                                                     \
                                                                                    \
    struct                                                                          \
    {                                                                               \
        T *lb;                                                                      \
        T *ub;                                                                      \
    }                                                                               \
    stack[48];                                                                      \
                                                                                    \
    aux = aux;                                                                      \
                                                                                    \
    if( total <= 1 )                                                                \
        return;                                                                     \
                                                                                    \
    stack[0].lb = array;                                                            \
    stack[0].ub = array + (total - 1);                                              \
                                                                                    \
    while( sp >= 0 )                                                                \
    {                                                                               \
        T* left = stack[sp].lb;                                                     \
        T* right = stack[sp--].ub;                                                  \
                                                                                    \
        for(;;)                                                                     \
        {                                                                           \
            int i, n = (int)(right - left) + 1, m;                                  \
            T* ptr;                                                                 \
            T* ptr2;                                                                \
                                                                                    \
            if( n <= isort_thresh )                                                 \
            {                                                                       \
            insert_sort:                                                            \
                for( ptr = left + 1; ptr <= right; ptr++ )                          \
                {                                                                   \
                    for( ptr2 = ptr; ptr2 > left && LT(ptr2[0],ptr2[-1]); ptr2--)   \
                        CV_SWAP( ptr2[0], ptr2[-1], t );                            \
                }                                                                   \
                break;                                                              \
            }                                                                       \
            else                                                                    \
            {                                                                       \
                T* left0;                                                           \
                T* left1;                                                           \
                T* right0;                                                          \
                T* right1;                                                          \
                T* pivot;                                                           \
                T* a;                                                               \
                T* b;                                                               \
                T* c;                                                               \
                int swap_cnt = 0;                                                   \
                                                                                    \
                left0 = left;                                                       \
                right0 = right;                                                     \
                pivot = left + (n/2);                                               \
                                                                                    \
                if( n > 40 )                                                        \
                {                                                                   \
                    int d = n / 8;                                                  \
                    a = left, b = left + d, c = left + 2*d;                         \
                    left = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))     \
                                      : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));    \
                                                                                    \
                    a = pivot - d, b = pivot, c = pivot + d;                        \
                    pivot = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))    \
                                      : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));    \
                                                                                    \
                    a = right - 2*d, b = right - d, c = right;                      \
                    right = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))    \
                                      : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));    \
                }                                                                   \
                                                                                    \
                a = left, b = pivot, c = right;                                     \
                pivot = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))        \
                                   : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));       \
                if( pivot != left0 )                                                \
                {                                                                   \
                    CV_SWAP( *pivot, *left0, t );                                   \
                    pivot = left0;                                                  \
                }                                                                   \
                left = left1 = left0 + 1;                                           \
                right = right1 = right0;                                            \
                                                                                    \
                for(;;)                                                             \
                {                                                                   \
                    while( left <= right && !LT(*pivot, *left) )                    \
                    {                                                               \
                        if( !LT(*left, *pivot) )                                    \
                        {                                                           \
                            if( left > left1 )                                      \
                                CV_SWAP( *left1, *left, t );                        \
                            swap_cnt = 1;                                           \
                            left1++;                                                \
                        }                                                           \
                        left++;                                                     \
                    }                                                               \
                                                                                    \
                    while( left <= right && !LT(*right, *pivot) )                   \
                    {                                                               \
                        if( !LT(*pivot, *right) )                                   \
                        {                                                           \
                            if( right < right1 )                                    \
                                CV_SWAP( *right1, *right, t );                      \
                            swap_cnt = 1;                                           \
                            right1--;                                               \
                        }                                                           \
                        right--;                                                    \
                    }                                                               \
                                                                                    \
                    if( left > right )                                              \
                        break;                                                      \
                    CV_SWAP( *left, *right, t );                                    \
                    swap_cnt = 1;                                                   \
                    left++;                                                         \
                    right--;                                                        \
                }                                                                   \
                                                                                    \
                if( swap_cnt == 0 )                                                 \
                {                                                                   \
                    left = left0, right = right0;                                   \
                    goto insert_sort;                                               \
                }                                                                   \
                                                                                    \
                n = MIN( (int)(left1 - left0), (int)(left - left1) );               \
                for( i = 0; i < n; i++ )                                            \
                    CV_SWAP( left0[i], left[i-n], t );                              \
                                                                                    \
                n = MIN( (int)(right0 - right1), (int)(right1 - right) );           \
                for( i = 0; i < n; i++ )                                            \
                    CV_SWAP( left[i], right0[i-n+1], t );                           \
                n = (int)(left - left1);                                            \
                m = (int)(right1 - right);                                          \
                if( n > 1 )                                                         \
                {                                                                   \
                    if( m > 1 )                                                     \
                    {                                                               \
                        if( n > m )                                                 \
                        {                                                           \
                            stack[++sp].lb = left0;                                 \
                            stack[sp].ub = left0 + n - 1;                           \
                            left = right0 - m + 1, right = right0;                  \
                        }                                                           \
                        else                                                        \
                        {                                                           \
                            stack[++sp].lb = right0 - m + 1;                        \
                            stack[sp].ub = right0;                                  \
                            left = left0, right = left0 + n - 1;                    \
                        }                                                           \
                    }                                                               \
                    else                                                            \
                        left = left0, right = left0 + n - 1;                        \
                }                                                                   \
                else if( m > 1 )                                                    \
                    left = right0 - m + 1, right = right0;                          \
                else                                                                \
                    break;                                                          \
            }                                                                       \
        }                                                                           \
    }                                                                               \
}

#define CV_IMPLEMENT_QSORT( func_name, T, cmp )  \
    CV_IMPLEMENT_QSORT_EX( func_name, T, cmp, int )

/****************************************************************************************\
*                     Structures and macros for integration with IPP                     *
\****************************************************************************************/

/* IPP-compatible return codes */
typedef enum CvStatus
{
    CV_BADMEMBLOCK_ERR          = -113,
    CV_INPLACE_NOT_SUPPORTED_ERR= -112,
    CV_UNMATCHED_ROI_ERR        = -111,
    CV_NOTFOUND_ERR             = -110,
    CV_BADCONVERGENCE_ERR       = -109,

    CV_BADDEPTH_ERR             = -107,
    CV_BADROI_ERR               = -106,
    CV_BADHEADER_ERR            = -105,
    CV_UNMATCHED_FORMATS_ERR    = -104,
    CV_UNSUPPORTED_COI_ERR      = -103,
    CV_UNSUPPORTED_CHANNELS_ERR = -102,
    CV_UNSUPPORTED_DEPTH_ERR    = -101,
    CV_UNSUPPORTED_FORMAT_ERR   = -100,

    CV_BADARG_ERR      = -49,  //ipp comp
    CV_NOTDEFINED_ERR  = -48,  //ipp comp

    CV_BADCHANNELS_ERR = -47,  //ipp comp
    CV_BADRANGE_ERR    = -44,  //ipp comp
    CV_BADSTEP_ERR     = -29,  //ipp comp

    CV_BADFLAG_ERR     =  -12,
    CV_DIV_BY_ZERO_ERR =  -11, //ipp comp
    CV_BADCOEF_ERR     =  -10,

    CV_BADFACTOR_ERR   =  -7,
    CV_BADPOINT_ERR    =  -6,
    CV_BADSCALE_ERR    =  -4,
    CV_OUTOFMEM_ERR    =  -3,
    CV_NULLPTR_ERR     =  -2,
    CV_BADSIZE_ERR     =  -1,
    CV_NO_ERR          =   0,
    CV_OK              =   CV_NO_ERR
}
CvStatus;

#define CV_ERROR_FROM_STATUS( result )                \
    CV_ERROR( cvErrorFromIppStatus( result ), "OpenCV function failed" )

#define IPPI_CALL( Func )                                              \
{                                                                      \
      CvStatus  ippi_call_result;                                      \
      ippi_call_result = Func;                                         \
                                                                       \
      if( ippi_call_result < 0 )                                       \
            CV_ERROR_FROM_STATUS( (ippi_call_result));                 \
}

#define CV_PLUGIN_NONE      0
#define CV_PLUGIN_OPTCV     1 /* custom "emerged" ippopencv library */
#define CV_PLUGIN_IPPCV     2 /* IPP: computer vision */
#define CV_PLUGIN_IPPI      3 /* IPP: image processing */
#define CV_PLUGIN_IPPS      4 /* IPP: signal processing */
#define CV_PLUGIN_IPPVM     5 /* IPP: vector math functions */
#define CV_PLUGIN_IPPCC     6 /* IPP: color space conversion */
#define CV_PLUGIN_MKL       8 /* Intel Math Kernel Library */

#define CV_PLUGIN_MAX      16

#define CV_PLUGINS1(lib1) ((lib1)&15)
#define CV_PLUGINS2(lib1,lib2) (((lib1)&15)|(((lib2)&15)<<4))
#define CV_PLUGINS3(lib1,lib2,lib3) (((lib1)&15)|(((lib2)&15)<<4)|(((lib2)&15)<<8))

#define CV_NOTHROW throw()

#ifndef IPCVAPI
#define IPCVAPI(type,declspec,name,args)                        \
    /* function pointer */                                      \
    typedef type (declspec* name##_t) args;                     \
    extern name##_t name##_p;                                   \
    type declspec name args;
#endif

#define IPCVAPI_EX(type,name,ipp_name,ipp_search_modules,args)  \
    IPCVAPI(type,CV_STDCALL,name,args)

#define IPCVAPI_C_EX(type,name,ipp_name,ipp_search_modules,args)\
    IPCVAPI(type,CV_CDECL,name,args)

#ifndef IPCVAPI_IMPL
#define IPCVAPI_IMPL(type,name,args,arg_names)                  \
    static type CV_STDCALL name##_f args;                       \
    name##_t name##_p = name##_f;                               \
    type CV_STDCALL name args { return name##_p arg_names; }    \
    static type CV_STDCALL name##_f args
#endif

/* IPP types' enumeration */
typedef enum CvDataType {
    cv1u,
    cv8u, cv8s,
    cv16u, cv16s, cv16sc,
    cv32u, cv32s, cv32sc,
    cv32f, cv32fc,
    cv64u, cv64s, cv64sc,
    cv64f, cv64fc
} CvDataType;

typedef enum CvHintAlgorithm {
    cvAlgHintNone,
    cvAlgHintFast,
    cvAlgHintAccurate
} CvHintAlgorithm;

typedef enum CvCmpOp {
    cvCmpLess,
    cvCmpLessEq,
    cvCmpEq,
    cvCmpGreaterEq,
    cvCmpGreater
} CvCmpOp;

typedef struct CvFuncTable
{
    void*   fn_2d[CV_DEPTH_MAX];
}
CvFuncTable;

typedef struct CvBigFuncTable
{
    void*   fn_2d[CV_DEPTH_MAX*CV_CN_MAX];
}
CvBigFuncTable;


typedef struct CvBtFuncTable
{
    void*   fn_2d[33];
}
CvBtFuncTable;

typedef CvStatus (CV_STDCALL *CvFunc2D_1A)(void* arr, int step, CvSize size);

typedef CvStatus (CV_STDCALL *CvFunc2D_1A1P)(void* arr, int step, CvSize size, void* param);

typedef CvStatus (CV_STDCALL *CvFunc2D_1A1P1I)(void* arr, int step, CvSize size,
                                               void* param, int flag);

typedef CvStatus (CV_STDCALL *CvFunc2DnC_1A1P)( void* arr, int step, CvSize size,
                                                int cn, int coi, void* param );

typedef CvStatus (CV_STDCALL *CvFunc2DnC_1A1P)( void* arr, int step, CvSize size,
                                                int cn, int coi, void* param );

typedef CvStatus (CV_STDCALL *CvFunc2D_1A2P)( void* arr, int step, CvSize size,
                                              void* param1, void* param2 );

typedef CvStatus (CV_STDCALL *CvFunc2DnC_1A2P)( void* arr, int step,
                                                CvSize size, int cn, int coi,
                                                void* param1, void* param2 );

typedef CvStatus (CV_STDCALL *CvFunc2D_1A4P)( void* arr, int step, CvSize size,
                                              void* param1, void* param2,
                                              void* param3, void* param4 );

typedef CvStatus (CV_STDCALL *CvFunc2DnC_1A4P)( void* arr, int step,
                                                CvSize size, int cn, int coi,
                                                void* param1, void* param2,
                                                void* param3, void* param4 );

typedef CvStatus (CV_STDCALL *CvFunc2D_2A)( void* arr0, int step0,
                                            void* arr1, int step1, CvSize size );

typedef CvStatus (CV_STDCALL *CvFunc2D_2A1P)( void* arr0, int step0,
                                              void* arr1, int step1,
                                              CvSize size, void* param );

typedef CvStatus (CV_STDCALL *CvFunc2DnC_2A1P)( void* arr0, int step0,
                                                void* arr1, int step1,
                                                CvSize size, int cn,
                                                int coi, void* param );

typedef CvStatus (CV_STDCALL *CvFunc2DnC_2A1P)( void* arr0, int step0,
                                                void* arr1, int step1,
                                                CvSize size, int cn,
                                                int coi, void* param );

typedef CvStatus (CV_STDCALL *CvFunc2D_2A2P)( void* arr0, int step0,
                                              void* arr1, int step1, CvSize size,
                                              void* param1, void* param2 );

typedef CvStatus (CV_STDCALL *CvFunc2DnC_2A2P)( void* arr0, int step0,
                                                void* arr1, int step1,
                                                CvSize size, int cn, int coi,
                                                void* param1, void* param2 );

typedef CvStatus (CV_STDCALL *CvFunc2D_2A1P1I)( void* arr0, int step0,
                                                void* arr1, int step1, CvSize size,
                                                void* param, int flag );

typedef CvStatus (CV_STDCALL *CvFunc2D_2A4P)( void* arr0, int step0,
                                              void* arr1, int step1, CvSize size,
                                              void* param1, void* param2,
                                              void* param3, void* param4 );

typedef CvStatus (CV_STDCALL *CvFunc2DnC_2A4P)( void* arr0, int step0,
                                                void* arr1, int step1, CvSize size,
                                                int cn, int coi,
                                                void* param1, void* param2,
                                                void* param3, void* param4 );

typedef CvStatus (CV_STDCALL *CvFunc2D_3A)( void* arr0, int step0,
                                            void* arr1, int step1,
                                            void* arr2, int step2, CvSize size );

typedef CvStatus (CV_STDCALL *CvFunc2D_3A1P)( void* arr0, int step0,
                                              void* arr1, int step1,
                                              void* arr2, int step2,
                                              CvSize size, void* param );

typedef CvStatus (CV_STDCALL *CvFunc2D_3A1I)( void* arr0, int step0,
                                              void* arr1, int step1,
                                              void* arr2, int step2,
                                              CvSize size, int flag );

typedef CvStatus (CV_STDCALL *CvFunc2DnC_3A1P)( void* arr0, int step0,
                                                void* arr1, int step1,
                                                void* arr2, int step2,
                                                CvSize size, int cn,
                                                int coi, void* param );

typedef CvStatus (CV_STDCALL *CvFunc2D_4A)( void* arr0, int step0,
                                            void* arr1, int step1,
                                            void* arr2, int step2,
                                            void* arr3, int step3,
                                            CvSize size );

typedef CvStatus (CV_STDCALL *CvFunc0D)( const void* src, void* dst, int param );

#define CV_DEF_INIT_FUNC_TAB_2D( FUNCNAME, FLAG )                   \
static void  icvInit##FUNCNAME##FLAG##Table( CvFuncTable* tab )     \
{                                                                   \
    assert( tab );                                                  \
                                                                    \
    tab->fn_2d[CV_8U]  = (void*)icv##FUNCNAME##_8u_##FLAG;          \
    tab->fn_2d[CV_8S]  = (void*)icv##FUNCNAME##_8s_##FLAG;          \
    tab->fn_2d[CV_16U] = (void*)icv##FUNCNAME##_16u_##FLAG;         \
    tab->fn_2d[CV_16S] = (void*)icv##FUNCNAME##_16s_##FLAG;         \
    tab->fn_2d[CV_32S] = (void*)icv##FUNCNAME##_32s_##FLAG;         \
    tab->fn_2d[CV_32F] = (void*)icv##FUNCNAME##_32f_##FLAG;         \
    tab->fn_2d[CV_64F] = (void*)icv##FUNCNAME##_64f_##FLAG;         \
}


#define CV_DEF_INIT_BIG_FUNC_TAB_2D( FUNCNAME, FLAG )               \
static void  icvInit##FUNCNAME##FLAG##Table( CvBigFuncTable* tab )  \
{                                                                   \
    assert( tab );                                                  \
                                                                    \
    tab->fn_2d[CV_8UC1]  = (void*)icv##FUNCNAME##_8u_C1##FLAG;      \
    tab->fn_2d[CV_8UC2]  = (void*)icv##FUNCNAME##_8u_C2##FLAG;      \
    tab->fn_2d[CV_8UC3]  = (void*)icv##FUNCNAME##_8u_C3##FLAG;      \
    tab->fn_2d[CV_8UC4]  = (void*)icv##FUNCNAME##_8u_C4##FLAG;      \
                                                                    \
    tab->fn_2d[CV_8SC1]  = (void*)icv##FUNCNAME##_8s_C1##FLAG;      \
    tab->fn_2d[CV_8SC2]  = (void*)icv##FUNCNAME##_8s_C2##FLAG;      \
    tab->fn_2d[CV_8SC3]  = (void*)icv##FUNCNAME##_8s_C3##FLAG;      \
    tab->fn_2d[CV_8SC4]  = (void*)icv##FUNCNAME##_8s_C4##FLAG;      \
                                                                    \
    tab->fn_2d[CV_16UC1] = (void*)icv##FUNCNAME##_16u_C1##FLAG;     \
    tab->fn_2d[CV_16UC2] = (void*)icv##FUNCNAME##_16u_C2##FLAG;     \
    tab->fn_2d[CV_16UC3] = (void*)icv##FUNCNAME##_16u_C3##FLAG;     \
    tab->fn_2d[CV_16UC4] = (void*)icv##FUNCNAME##_16u_C4##FLAG;     \
                                                                    \
    tab->fn_2d[CV_16SC1] = (void*)icv##FUNCNAME##_16s_C1##FLAG;     \
    tab->fn_2d[CV_16SC2] = (void*)icv##FUNCNAME##_16s_C2##FLAG;     \
    tab->fn_2d[CV_16SC3] = (void*)icv##FUNCNAME##_16s_C3##FLAG;     \
    tab->fn_2d[CV_16SC4] = (void*)icv##FUNCNAME##_16s_C4##FLAG;     \
                                                                    \
    tab->fn_2d[CV_32SC1] = (void*)icv##FUNCNAME##_32s_C1##FLAG;     \
    tab->fn_2d[CV_32SC2] = (void*)icv##FUNCNAME##_32s_C2##FLAG;     \
    tab->fn_2d[CV_32SC3] = (void*)icv##FUNCNAME##_32s_C3##FLAG;     \
    tab->fn_2d[CV_32SC4] = (void*)icv##FUNCNAME##_32s_C4##FLAG;     \
                                                                    \
    tab->fn_2d[CV_32FC1] = (void*)icv##FUNCNAME##_32f_C1##FLAG;     \
    tab->fn_2d[CV_32FC2] = (void*)icv##FUNCNAME##_32f_C2##FLAG;     \
    tab->fn_2d[CV_32FC3] = (void*)icv##FUNCNAME##_32f_C3##FLAG;     \
    tab->fn_2d[CV_32FC4] = (void*)icv##FUNCNAME##_32f_C4##FLAG;     \
                                                                    \
    tab->fn_2d[CV_64FC1] = (void*)icv##FUNCNAME##_64f_C1##FLAG;     \
    tab->fn_2d[CV_64FC2] = (void*)icv##FUNCNAME##_64f_C2##FLAG;     \
    tab->fn_2d[CV_64FC3] = (void*)icv##FUNCNAME##_64f_C3##FLAG;     \
    tab->fn_2d[CV_64FC4] = (void*)icv##FUNCNAME##_64f_C4##FLAG;     \
}

#define CV_DEF_INIT_FUNC_TAB_0D( FUNCNAME )                         \
static void  icvInit##FUNCNAME##Table( CvFuncTable* tab )           \
{                                                                   \
    tab->fn_2d[CV_8U]  = (void*)icv##FUNCNAME##_8u;                 \
    tab->fn_2d[CV_8S]  = (void*)icv##FUNCNAME##_8s;                 \
    tab->fn_2d[CV_16U] = (void*)icv##FUNCNAME##_16u;                \
    tab->fn_2d[CV_16S] = (void*)icv##FUNCNAME##_16s;                \
    tab->fn_2d[CV_32S] = (void*)icv##FUNCNAME##_32s;                \
    tab->fn_2d[CV_32F] = (void*)icv##FUNCNAME##_32f;                \
    tab->fn_2d[CV_64F] = (void*)icv##FUNCNAME##_64f;                \
}

#define CV_DEF_INIT_FUNC_TAB_1D  CV_DEF_INIT_FUNC_TAB_0D


#define CV_DEF_INIT_PIXSIZE_TAB_2D( FUNCNAME, FLAG )                \
static void icvInit##FUNCNAME##FLAG##Table( CvBtFuncTable* table )  \
{                                                                   \
    table->fn_2d[1]  = (void*)icv##FUNCNAME##_8u_C1##FLAG;          \
    table->fn_2d[2]  = (void*)icv##FUNCNAME##_8u_C2##FLAG;          \
    table->fn_2d[3]  = (void*)icv##FUNCNAME##_8u_C3##FLAG;          \
    table->fn_2d[4]  = (void*)icv##FUNCNAME##_16u_C2##FLAG;         \
    table->fn_2d[6]  = (void*)icv##FUNCNAME##_16u_C3##FLAG;         \
    table->fn_2d[8]  = (void*)icv##FUNCNAME##_32s_C2##FLAG;         \
    table->fn_2d[12] = (void*)icv##FUNCNAME##_32s_C3##FLAG;         \
    table->fn_2d[16] = (void*)icv##FUNCNAME##_64s_C2##FLAG;         \
    table->fn_2d[24] = (void*)icv##FUNCNAME##_64s_C3##FLAG;         \
    table->fn_2d[32] = (void*)icv##FUNCNAME##_64s_C4##FLAG;         \
}

#define  CV_GET_FUNC_PTR( func, table_entry )  \
    func = (table_entry);                      \
                                               \
    if( !func )                                \
        CV_ERROR( CV_StsUnsupportedFormat, "" )


#endif /*_CXCORE_MISC_H_*/
