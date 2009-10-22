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

#ifndef _CVMAT_HPP_
#define _CVMAT_HPP_

#if 0 && (defined __cplusplus && (_MSC_VER>=1200 || defined __BORLANDC__ || defined __GNUC__))

#if _MSC_VER >= 1200
#pragma warning( disable: 4710 ) /* suppress "function ... is not inlined" */
#endif

#include <string.h>
#include <stdio.h>

#undef min
#undef max

/****************************************************************************************\
*                            C++ - like operations on CvScalar                           *
\****************************************************************************************/

inline CvScalar& operator += ( CvScalar& a, const CvScalar& b )
{
    double t0 = a.val[0] + b.val[0];
    double t1 = a.val[1] + b.val[1];
    a.val[0] = t0;
    a.val[1] = t1;

    t0 = a.val[2] + b.val[2];
    t1 = a.val[3] + b.val[3];
    a.val[2] = t0;
    a.val[3] = t1;

    return a;
}


inline CvScalar& operator -= ( CvScalar& a, const CvScalar& b )
{
    double t0 = a.val[0] - b.val[0];
    double t1 = a.val[1] - b.val[1];
    a.val[0] = t0;
    a.val[1] = t1;

    t0 = a.val[2] - b.val[2];
    t1 = a.val[3] - b.val[3];
    a.val[2] = t0;
    a.val[3] = t1;

    return a;
}


inline CvScalar& operator *= ( CvScalar& a, double b )
{
    double t0 = a.val[0] * b;
    double t1 = a.val[1] * b;
    a.val[0] = t0;
    a.val[1] = t1;

    t0 = a.val[2] * b;
    t1 = a.val[3] * b;
    a.val[2] = t0;
    a.val[3] = t1;

    return a;
}


inline CvScalar& operator /= ( CvScalar& a, double b )
{
    double inv_b = 1./b;
    double t0 = a.val[0] * inv_b;
    double t1 = a.val[1] * inv_b;
    a.val[0] = t0;
    a.val[1] = t1;

    t0 = a.val[2] * inv_b;
    t1 = a.val[3] * inv_b;
    a.val[2] = t0;
    a.val[3] = t1;

    return a;
}


inline CvScalar& operator *= ( CvScalar& a, const CvScalar& b )
{
    double t0 = a.val[0]*b.val[0] - a.val[1]*b.val[1] -
                a.val[2]*b.val[2] - a.val[3]*b.val[3];

    double t1 = a.val[0]*b.val[1] + a.val[1]*b.val[0] +
                a.val[2]*b.val[3] - a.val[3]*b.val[2];

    double t2 = a.val[0]*b.val[2] - a.val[1]*b.val[3] +
                a.val[2]*b.val[0] + a.val[3]*b.val[1];

    double t3 = a.val[0]*b.val[3] + a.val[1]*b.val[2] -
                a.val[2]*b.val[1] + a.val[3]*b.val[0];

    a.val[0] = t0;
    a.val[1] = t1;
    a.val[2] = t2;
    a.val[3] = t3;

    return a;
}


inline CvScalar& operator /= ( CvScalar& a, const CvScalar& b )
{
    double inv_d = -1./(b.val[0]*b.val[0] + b.val[1]*b.val[1] +
                        b.val[2]*b.val[2] + b.val[3]*b.val[3]);
    return a *= cvScalar( b.val[0] * -inv_d, b.val[1] * inv_d,
                          b.val[2] * inv_d, b.val[3] * inv_d );
}


inline CvScalar& operator += ( CvScalar& a, double b )
{
    a.val[0] += b;
    return a;
}


inline CvScalar& operator -= ( CvScalar& a, double b )
{
    a.val[0] -= b;
    return a;
}


inline CvScalar operator + ( const CvScalar& a, const CvScalar& b )
{
    return cvScalar( a.val[0] + b.val[0], a.val[1] + b.val[1],
                     a.val[2] + b.val[2], a.val[3] + b.val[3] );
}


inline CvScalar operator - ( const CvScalar& a, const CvScalar& b )
{
    return cvScalar( a.val[0] - b.val[0], a.val[1] - b.val[1],
                     a.val[2] - b.val[2], a.val[3] - b.val[3] );
}


inline CvScalar operator + ( const CvScalar& a, double b )
{
    return cvScalar( a.val[0] + b, a.val[1], a.val[2], a.val[3] );
}


inline CvScalar operator - ( const CvScalar& a, double b )
{
    return cvScalar( a.val[0] - b, a.val[1], a.val[2], a.val[3] );
}


inline CvScalar operator + ( double a, const CvScalar& b )
{
    return cvScalar( a + b.val[0], b.val[1], b.val[2], b.val[3] );
}


inline CvScalar operator - ( double a, const CvScalar& b )
{
    return cvScalar( a - b.val[0], -b.val[1], -b.val[2], -b.val[3] );
}


inline CvScalar operator - ( const CvScalar& b )
{
    return cvScalar( -b.val[0], -b.val[1], -b.val[2], -b.val[3] );
}


inline CvScalar operator * ( const CvScalar& a, const CvScalar& b )
{
    CvScalar c = a;

    return (c *= b);
}


inline CvScalar operator * ( const CvScalar& a, double b )
{
    return cvScalar( a.val[0]*b, a.val[1]*b, a.val[2]*b, a.val[3]*b );
}


inline CvScalar operator * ( double a, const CvScalar& b )
{
    return cvScalar( b.val[0]*a, b.val[1]*a, b.val[2]*a, b.val[3]*a );
}


inline CvScalar operator / ( const CvScalar& a, const CvScalar& b )
{
    CvScalar c = a;
    return (c /= b);
}


inline CvScalar operator / ( const CvScalar& a, double b )
{
    double inv_b = 1./b;
    return cvScalar( a.val[0]*inv_b, a.val[1]*inv_b,
                     a.val[2]*inv_b, a.val[3]*inv_b );
}


inline CvScalar operator / ( double a, const CvScalar& b )
{
    double inv_d = -a/(b.val[0]*b.val[0] + b.val[1]*b.val[1] +
                       b.val[2]*b.val[2] + b.val[3]*b.val[3]);
    return cvScalar( b.val[0] * -inv_d, b.val[1] * inv_d,
                     b.val[2] * inv_d, b.val[3] * inv_d );
}


inline CvScalar& operator &= ( CvScalar& a, const CvScalar& b )
{
    int t0 = cvRound(a.val[0]) & cvRound(b.val[0]);
    int t1 = cvRound(a.val[1]) & cvRound(b.val[1]);
    a.val[0] = t0;
    a.val[1] = t1;

    t0 = cvRound(a.val[2]) & cvRound(b.val[2]);
    t1 = cvRound(a.val[3]) & cvRound(b.val[3]);
    a.val[2] = t0;
    a.val[3] = t1;

    return a;
}


inline CvScalar& operator |= ( CvScalar& a, const CvScalar& b )
{
    int t0 = cvRound(a.val[0]) | cvRound(b.val[0]);
    int t1 = cvRound(a.val[1]) | cvRound(b.val[1]);
    a.val[0] = t0;
    a.val[1] = t1;

    t0 = cvRound(a.val[2]) | cvRound(b.val[2]);
    t1 = cvRound(a.val[3]) | cvRound(b.val[3]);
    a.val[2] = t0;
    a.val[3] = t1;

    return a;
}


inline CvScalar& operator ^= ( CvScalar& a, const CvScalar& b )
{
    int t0 = cvRound(a.val[0]) ^ cvRound(b.val[0]);
    int t1 = cvRound(a.val[1]) ^ cvRound(b.val[1]);
    a.val[0] = t0;
    a.val[1] = t1;

    t0 = cvRound(a.val[2]) ^ cvRound(b.val[2]);
    t1 = cvRound(a.val[3]) ^ cvRound(b.val[3]);
    a.val[2] = t0;
    a.val[3] = t1;

    return a;
}


inline CvScalar operator & ( const CvScalar& a, const CvScalar& b )
{
    CvScalar c = a;
    return (c &= b);
}


inline CvScalar operator | ( const CvScalar& a, const CvScalar& b )
{
    CvScalar c = a;
    return (c |= b);
}


inline CvScalar operator ^ ( const CvScalar& a, const CvScalar& b )
{
    CvScalar c = a;
    return (c ^= b);
}


inline CvScalar operator ~ ( const CvScalar& a )
{
    return cvScalar( ~cvRound(a.val[0]), ~cvRound(a.val[1]),
                     ~cvRound(a.val[2]), ~cvRound(a.val[3])); 
}


/****************************************************************************************\
*                                   C++ Matrix Class                                     *
\****************************************************************************************/

struct  _CvMATConstElem_;
struct  _CvMATElem_;
struct  _CvMATElemCn_;

struct  _CvMAT_T_;
struct  _CvMAT_MUL_;
struct  _CvMAT_INV_;
struct  _CvMAT_SCALE_;
struct  _CvMAT_SCALE_SHIFT_;
struct  _CvMAT_ADD_;
struct  _CvMAT_ADD_EX_;
struct  _CvMAT_MUL_ADD_;
struct  _CvMAT_LOGIC_;
struct  _CvMAT_UN_LOGIC_;
struct  _CvMAT_NOT_;
struct  _CvMAT_CVT_;
struct  _CvMAT_COPY_;
struct  _CvMAT_DOT_OP_;
struct  _CvMAT_SOLVE_;
struct  _CvMAT_CMP_;

class CV_EXPORTS CvMAT : public CvMat
{
protected:

public:
    /* helper methods for retrieving/setting matrix elements */
    static double get( const uchar* ptr, int type, int coi = 0 );
    static void set( uchar* ptr, int type, int coi, double d );
    static void set( uchar* ptr, int type, int coi, int i );
    static void set( uchar* ptr, int type, double d );
    static void set( uchar* ptr, int type, int i );

    /******************* constructors ********************/
    /* empty */
    explicit CvMAT();

    /* creation */
    explicit CvMAT( int rows, int cols, int type, void* data, int step = CV_AUTOSTEP );
    explicit CvMAT( int rows, int type, void* data, int step = CV_AUTOSTEP );
    explicit CvMAT( int rows, int cols, int type );
    explicit CvMAT( int rows, int type );
    
    /* extracting part of an existing matrix */
    explicit CvMAT( const CvMat& mat, CvRect rect ); /* submatrix */
    explicit CvMAT( const CvMat& mat, int k, int i ); /* submatrix:
                                                    k == 0 - i-th row
                                                    k > 0 - i-th column
                                                    k < 0 - i-th diagonal */
    /* copying */
    CvMAT( const CvMat& mat );
    CvMAT( const CvMAT& mat );
    CvMAT( const IplImage& img );

    /* CvMAT b = op(a1,a2,...) */
    explicit CvMAT( const _CvMAT_T_& mat_t );
    explicit CvMAT( const _CvMAT_INV_& inv_mat );
    explicit CvMAT( const _CvMAT_ADD_& mat_add );
    explicit CvMAT( const _CvMAT_ADD_EX_& mat_add );
    explicit CvMAT( const _CvMAT_SCALE_& scale_mat );
    explicit CvMAT( const _CvMAT_SCALE_SHIFT_& scale_shift_mat );
    explicit CvMAT( const _CvMAT_MUL_& mmul );
    explicit CvMAT( const _CvMAT_MUL_ADD_& mmuladd );
    explicit CvMAT( const _CvMAT_LOGIC_& mat_logic );
    explicit CvMAT( const _CvMAT_UN_LOGIC_& mat_logic );
    explicit CvMAT( const _CvMAT_NOT_& not_mat );
    explicit CvMAT( const _CvMAT_COPY_& mat_copy );
    explicit CvMAT( const _CvMAT_CVT_& mat_copy );
    explicit CvMAT( const _CvMAT_DOT_OP_& dot_mul );
    explicit CvMAT( const _CvMAT_SOLVE_& solve_mat );
    explicit CvMAT( const _CvMAT_CMP_& cmp_mat );

    /* desctructor */
    ~CvMAT();

    /* copying and filling with a constant */
    CvMAT& operator = ( const CvMAT& mat );
    CvMAT& operator = ( const CvMat& mat );
    CvMAT& operator = ( const IplImage& img );
    CvMAT& operator = ( double fillval );
    CvMAT& operator = ( const CvScalar& fillval );
    
    /* b = op(a1, a2,...) */
    CvMAT& operator = ( const _CvMAT_T_& mat_t );
    CvMAT& operator = ( const _CvMAT_INV_& inv_mat );
    CvMAT& operator = ( const _CvMAT_ADD_& mat_add );
    CvMAT& operator = ( const _CvMAT_ADD_EX_& mat_add );
    CvMAT& operator = ( const _CvMAT_SCALE_& scale_mat );
    CvMAT& operator = ( const _CvMAT_SCALE_SHIFT_& scale_shift_mat );
    CvMAT& operator = ( const _CvMAT_MUL_& mmul );
    CvMAT& operator = ( const _CvMAT_MUL_ADD_& mmuladd );
    CvMAT& operator = ( const _CvMAT_LOGIC_& mat_logic );
    CvMAT& operator = ( const _CvMAT_UN_LOGIC_& mat_logic );
    CvMAT& operator = ( const _CvMAT_NOT_& not_mat );
    CvMAT& operator = ( const _CvMAT_DOT_OP_& dot_mul );
    CvMAT& operator = ( const _CvMAT_SOLVE_& solve_mat );
    CvMAT& operator = ( const _CvMAT_CMP_& cmp_mat );
    CvMAT& operator = ( const _CvMAT_CVT_& mat_cvt );

    /* copy matrix data, not only matrix header */
    CvMAT& operator = ( const _CvMAT_COPY_& mat_copy );

    /* augmented assignments */
    CvMAT& operator += ( const CvMat& mat );
    CvMAT& operator += ( double val );
    CvMAT& operator += ( const CvScalar& val );
    CvMAT& operator += ( const _CvMAT_SCALE_& scale_mat );
    CvMAT& operator += ( const _CvMAT_SCALE_SHIFT_& scale_mat );
    CvMAT& operator += ( const _CvMAT_MUL_& mmul );

    CvMAT& operator -= ( const CvMat& mat );
    CvMAT& operator -= ( double val );
    CvMAT& operator -= ( const CvScalar& val );
    CvMAT& operator -= ( const _CvMAT_SCALE_& scale_mat );
    CvMAT& operator -= ( const _CvMAT_SCALE_SHIFT_& scale_mat );
    CvMAT& operator -= ( const _CvMAT_MUL_& mmul );

    CvMAT& operator *= ( const CvMat& mat );
    CvMAT& operator *= ( double val );
    CvMAT& operator *= ( const CvScalar& val );
    CvMAT& operator *= ( const _CvMAT_SCALE_& scale_mat );
    CvMAT& operator *= ( const _CvMAT_SCALE_SHIFT_& scale_mat );

    CvMAT& operator &= ( const CvMat& mat );
    CvMAT& operator &= ( double val );
    CvMAT& operator &= ( const CvScalar& val );

    CvMAT& operator |= ( const CvMat& mat );
    CvMAT& operator |= ( double val );
    CvMAT& operator |= ( const CvScalar& val );

    CvMAT& operator ^= ( const CvMat& mat );
    CvMAT& operator ^= ( double val );
    CvMAT& operator ^= ( const CvScalar& val );

    /* various scalar charactertics */
    double norm( int norm_type = CV_L2 ) const;
    double norm( CvMat& mat, int norm_type = CV_L2 ) const;
    CvScalar sum() const;

    double det() const;
    double trace() const;

    _CvMAT_T_  t() const; /* transposition */
    _CvMAT_INV_ inv(int method = 0) const;
    /* inversion using one of the following methods:
          method = 0 - Gaussian elimination,
          method = 1 - SVD */

    _CvMAT_DOT_OP_  mul( const CvMAT& mat ) const;
    _CvMAT_DOT_OP_  mul( const _CvMAT_SCALE_& mat ) const;

    _CvMAT_DOT_OP_  div( const CvMAT& mat ) const;
    _CvMAT_DOT_OP_  div( const _CvMAT_SCALE_& mat ) const;

    _CvMAT_DOT_OP_  min( const CvMAT& mat ) const;
    _CvMAT_DOT_OP_  max( const CvMAT& mat ) const;
    _CvMAT_DOT_OP_  min( double value ) const;
    _CvMAT_DOT_OP_  max( double value ) const;
    double          min( CvPoint* minloc = 0 ) const;
    double          max( CvPoint* maxloc = 0 ) const;

    _CvMAT_DOT_OP_  abs() const;
    
    /* accessing matrix elements */
    _CvMATElem_ operator ()( int row );
    _CvMATConstElem_ operator ()( int row ) const;

    _CvMATElem_ operator ()( int row, int col );
    _CvMATConstElem_ operator ()( int row, int col ) const;

    _CvMATElem_ operator ()( CvPoint loc );
    _CvMATConstElem_ operator ()( CvPoint loc ) const;

    _CvMATElemCn_ operator()( int row, int col, int coi );
    double operator()( int row, int col, int coi ) const;

    _CvMATElemCn_ operator()( CvPoint pt, int coi );
    double operator()( CvPoint pt, int coi ) const;

    void* ptr( int row );
    const void* ptr( int row ) const;

    void* ptr( int row, int col );
    const void* ptr( int row, int col ) const;

    void* ptr( CvPoint pt );
    const void* ptr( CvPoint pt ) const;

    /* accessing matrix parts */
    CvMAT row( int row ) const;
    CvMAT rowrange( int row1, int row2 ) const;
    CvMAT col( int col ) const;
    CvMAT colrange( int col1, int col2 ) const;
    CvMAT rect( CvRect rect ) const;
    CvMAT diag( int diag = 0 ) const;

    _CvMAT_COPY_ clone() const;

    /* convert matrix */
    _CvMAT_CVT_ cvt( int newdepth = -1, double scale = 1,
                     double shift = 0 ) const;

    /* matrix transformation */
    void  reshape( int newcn, int newrows = 0 );
    void  flipX();
    void  flipY();
    void  flipXY();

    /* matrix I/O: use dynamically linked runtime libraries */
    void  write( const char* name = 0, FILE* f = 0, const char* fmt = 0 );
    void  read( char** name = 0, FILE* f = 0 );

    /* decrease matrix data reference counter and clear data pointer */
    void  release();
protected:
    
    void  create( int rows, int cols, int type );
};


/* !!! Internal Use Only !!! */
/* proxies for matrix elements */

/* const_A(i,j) */
struct  CV_EXPORTS _CvMATConstElem_
{
    explicit _CvMATConstElem_( const uchar* ptr, int type );
    operator CvScalar () const;
    double operator ()( int coi = 0 ) const;

    uchar* ptr;
    int type;
};


/* A(i,j,cn) or A(i,j)(cn) */
struct  CV_EXPORTS _CvMATElemCn_
{
    explicit _CvMATElemCn_( uchar* ptr, int type, int coi );
    operator double() const;
    
    _CvMATElemCn_& operator = ( const _CvMATConstElem_& elem );
    _CvMATElemCn_& operator = ( const _CvMATElemCn_& elem );
    _CvMATElemCn_& operator = ( const CvScalar& scalar );
    _CvMATElemCn_& operator = ( double d );
    _CvMATElemCn_& operator = ( float f );
    _CvMATElemCn_& operator = ( int i );

    uchar* ptr;
    int type;
};


/* A(i,j) */
struct  CV_EXPORTS _CvMATElem_ : public _CvMATConstElem_
{
    explicit _CvMATElem_( uchar* ptr, int type );
    _CvMATElemCn_ operator ()( int coi = 0 );

    _CvMATElem_& operator = ( const _CvMATConstElem_& elem );
    _CvMATElem_& operator = ( const _CvMATElem_& elem );
    _CvMATElem_& operator = ( const _CvMATElemCn_& elem );
    _CvMATElem_& operator = ( const CvScalar& val );
    _CvMATElem_& operator = ( double d );
    _CvMATElem_& operator = ( float f );
    _CvMATElem_& operator = ( int i );
};


struct  CV_EXPORTS _CvMAT_BASE_OP_
{
    _CvMAT_BASE_OP_() {};
    virtual operator CvMAT() const = 0;

    _CvMAT_DOT_OP_  mul( const CvMAT& a ) const;
    _CvMAT_DOT_OP_  mul( const _CvMAT_SCALE_& a ) const;

    _CvMAT_DOT_OP_  div( const CvMAT& a ) const;
    _CvMAT_DOT_OP_  div( const _CvMAT_SCALE_& a ) const;

    _CvMAT_DOT_OP_  max( const CvMAT& a ) const;
    _CvMAT_DOT_OP_  min( const CvMAT& a ) const;

    _CvMAT_DOT_OP_  max( double value ) const;
    _CvMAT_DOT_OP_  min( double value ) const;

    double          max( CvPoint* maxloc = 0 ) const;
    double          min( CvPoint* minloc = 0 ) const;

    _CvMAT_DOT_OP_  abs() const;

    _CvMAT_INV_     inv( int method = 0 ) const;
    _CvMAT_T_       t() const;

    CvMAT     row( int row ) const;
    CvMAT     rowrange( int row1, int row2 ) const;
    CvMAT     col( int col ) const;
    CvMAT     colrange( int col1, int col2 ) const;
    CvMAT     rect( CvRect rect ) const;
    CvMAT     diag( int diag = 0 ) const;
    _CvMAT_CVT_     cvt( int newdepth = -1, double scale = 1, double shift = 0 ) const;
    
    double          norm( int norm_type = CV_L2 ) const;
    double          det() const;
    double          trace() const;
    CvScalar        sum() const;
};


/* (A^t)*alpha */
struct  CV_EXPORTS _CvMAT_T_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_T_( const CvMAT* a );
    explicit _CvMAT_T_( const CvMAT* a, double alpha );
    
    double det() const;
    double norm( int normType = CV_L2 ) const;
    operator CvMAT() const;

    CvMAT  a;
    double alpha;
};


/* inv(A) */
struct  CV_EXPORTS _CvMAT_INV_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_INV_( const CvMAT* mat, int method );
    operator CvMAT() const;

    CvMAT a;
    int method;
};


/* (A^ta)*(B^tb)*alpha */
struct  CV_EXPORTS _CvMAT_MUL_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_MUL_( const CvMAT* a, const CvMAT* b, int t_ab );
    explicit _CvMAT_MUL_( const CvMAT* a, const CvMAT* b,
                          double alpha, int t_abc );
    operator CvMAT() const;

    double alpha;
    CvMAT* a;
    CvMAT* b;
    int t_ab; /* (t_ab & 1) = ta, (t_ab & 2) = tb */
};


/* (A^ta)*(B^tb)*alpha + (C^tc)*beta */
struct  CV_EXPORTS _CvMAT_MUL_ADD_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_MUL_ADD_( const CvMAT* a, const CvMAT* b,
                              const CvMAT* c, int t_abc );
    explicit _CvMAT_MUL_ADD_( const CvMAT* a, const CvMAT* b, double alpha,
                              const CvMAT* c, double beta, int t_abc );
    operator CvMAT() const;

    double alpha, beta;
    CvMAT* a;
    CvMAT* b;
    CvMAT* c;
    int t_abc; /* (t_abc & 1) = ta, (t_abc & 2) = tb, (t_abc & 4) = tc */
};


/* A + B*beta */
struct  CV_EXPORTS _CvMAT_ADD_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_ADD_( const CvMAT* a, const CvMAT* b, double beta = 1 );
    operator CvMAT() const;

    double   norm( int norm_type = CV_L2 ) const;
    _CvMAT_DOT_OP_ abs() const;

    double beta;
    CvMAT* a;
    CvMAT* b;
};


/* A*alpha + B*beta + gamma */
struct  CV_EXPORTS _CvMAT_ADD_EX_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_ADD_EX_( const CvMAT* a, double alpha,
                             const CvMAT* b, double beta, double gamma = 0 );
    operator CvMAT() const;

    double alpha, beta, gamma;
    CvMAT* a;
    CvMAT* b;
};


/* A*alpha */
struct  CV_EXPORTS _CvMAT_SCALE_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_SCALE_( const CvMAT* a, double alpha );
    operator CvMAT() const;

    _CvMAT_DOT_OP_  mul( const CvMAT& a ) const;
    _CvMAT_DOT_OP_  mul( const _CvMAT_SCALE_& a ) const;

    _CvMAT_DOT_OP_  div( const CvMAT& a ) const;
    _CvMAT_DOT_OP_  div( const _CvMAT_SCALE_& a ) const;

    double alpha;
    CvMAT* a;
};


/* A*alpha + beta */
struct  CV_EXPORTS _CvMAT_SCALE_SHIFT_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_SCALE_SHIFT_( const CvMAT* a, double alpha, double beta );
    operator CvMAT() const;

    _CvMAT_DOT_OP_  abs() const;

    double alpha, beta;
    CvMAT* a;
};


/* (A & B), (A | B) or (A ^ B) */
struct  CV_EXPORTS _CvMAT_LOGIC_ : public _CvMAT_BASE_OP_
{
    enum Op { AND = 0, OR = 1, XOR = 2 };
    explicit _CvMAT_LOGIC_( const CvMAT* a, const CvMAT* b, Op op, int flags = 0 );
    operator CvMAT() const;

    CvMAT* a;
    CvMAT* b;
    Op op;
    int flags;
};


/* (A & scalar), (A | scalar) or (A ^ scalar) */
struct  CV_EXPORTS _CvMAT_UN_LOGIC_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_UN_LOGIC_( const CvMAT* a, double alpha,
                               _CvMAT_LOGIC_::Op op, int flags = 0 );
    operator CvMAT() const;

    CvMAT* a;
    double alpha;
    _CvMAT_LOGIC_::Op op;
    int flags;
};


/* ~A */
struct  CV_EXPORTS _CvMAT_NOT_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_NOT_( const CvMAT* a );
    operator CvMAT() const;

    CvMAT* a;
};


/* conversion of data type */
struct  CV_EXPORTS _CvMAT_CVT_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_CVT_( const CvMAT* a, int newdepth = -1,
                          double scale = 1, double shift = 0 );
    operator CvMAT() const;

    CvMAT a;
    int newdepth;
    double scale, shift;
};


/* conversion of data type */
struct  CV_EXPORTS _CvMAT_COPY_
{
    explicit _CvMAT_COPY_( const CvMAT* a );
    operator CvMAT() const;
    CvMAT* a;
};


/* a.op(b), where op = mul, div, min, max ... */
struct  CV_EXPORTS _CvMAT_DOT_OP_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_DOT_OP_( const CvMAT* a, const CvMAT* b,
                             int op, double alpha = 1 );
    operator CvMAT() const;

    CvMAT a; /* keep the left operand copy */
    CvMAT* b;
    double alpha;
    int op;
};


/* A.inv()*B or A.pinv()*B */
struct  CV_EXPORTS _CvMAT_SOLVE_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_SOLVE_( const CvMAT* a, const CvMAT* b, int method );
    operator CvMAT() const;

    CvMAT* a;
    CvMAT* b;
    int method;
};


/* A <=> B */
struct  CV_EXPORTS _CvMAT_CMP_ : public _CvMAT_BASE_OP_
{
    explicit _CvMAT_CMP_( const CvMAT* a, const CvMAT* b, int cmp_op );
    explicit _CvMAT_CMP_( const CvMAT* a, double alpha, int cmp_op );
    operator CvMAT() const;

    CvMAT* a;
    CvMAT* b;
    double alpha;
    int cmp_op;
};


/************************* _CvMATConstElem_ inline methods ******************************/

inline _CvMATConstElem_::_CvMATConstElem_(const uchar* p, int t) : ptr((uchar*)p), type(t)
{}


inline _CvMATConstElem_::operator CvScalar() const
{
    CvScalar scalar;
    cvRawDataToScalar( ptr, type, &scalar );

    return scalar;
}


inline double _CvMATConstElem_::operator ()( int coi ) const
{   return CvMAT::get( ptr, type, coi );    }


inline _CvMATElemCn_::_CvMATElemCn_( uchar* p, int t, int coi ) :
    ptr(p), type(CV_MAT_DEPTH(t))
{
    if( coi )
    {
        assert( (unsigned)coi < (unsigned)CV_MAT_CN(t) );
        ptr += coi * CV_ELEM_SIZE(type);
    }
}


inline _CvMATElemCn_::operator double() const
{   return CvMAT::get( ptr, type ); }


inline _CvMATElemCn_& _CvMATElemCn_::operator = ( const _CvMATConstElem_& elem )
{
    if( type == elem.type )
        memcpy( ptr, elem.ptr, CV_ELEM_SIZE(type) );
    else
    {
        assert( CV_MAT_CN(elem.type) == 1 );
        CvMAT::set( ptr, type, 0, elem(0));
    }

    return *this;
}


inline _CvMATElemCn_& _CvMATElemCn_::operator = ( const _CvMATElemCn_& elem )
{
    if( type == elem.type )
        memcpy( ptr, elem.ptr, CV_ELEM_SIZE(type) );
    else
        CvMAT::set( ptr, type, 0, (double)elem );
    return *this;
}


inline _CvMATElemCn_& _CvMATElemCn_::operator = ( const CvScalar& scalar )
{   
    CvMAT::set( ptr, type, 0, scalar.val[0] );
    return *this;
}


inline _CvMATElemCn_& _CvMATElemCn_::operator = ( double d )
{   
    CvMAT::set( ptr, type, 0, d );
    return *this;
}


inline _CvMATElemCn_& _CvMATElemCn_::operator = ( float f )
{   
    CvMAT::set( ptr, type, 0, (double)f );
    return *this;
}


inline _CvMATElemCn_& _CvMATElemCn_::operator = ( int i )
{   
    CvMAT::set( ptr, type, 0, i );
    return *this;
}


inline _CvMATElem_::_CvMATElem_( uchar* p, int t ) : _CvMATConstElem_( (const uchar*)p, t )
{}


inline _CvMATElemCn_ _CvMATElem_::operator ()( int coi )
{   return _CvMATElemCn_( ptr, type, coi ); }


inline _CvMATElem_& _CvMATElem_::operator = ( const _CvMATConstElem_& elem )
{
    if( type == elem.type )
        memcpy( ptr, elem.ptr, CV_ELEM_SIZE(type) );
    else
    {
        assert( CV_MAT_CN( type ^ elem.type ) == 0 );
        CvScalar sc = (CvScalar)elem;
        cvScalarToRawData( &sc, ptr, type, 0 );
    }

    return *this;
}


inline _CvMATElem_& _CvMATElem_::operator = ( const _CvMATElem_& elem )
{
    *this = (const _CvMATConstElem_&)elem;
    return *this;
}


inline _CvMATElem_& _CvMATElem_::operator = ( const _CvMATElemCn_& elem )
{
    if( type == elem.type )
        memcpy( ptr, elem.ptr, CV_ELEM_SIZE(type) );
    else
        CvMAT::set( ptr, type, (double)elem );

    return *this;
}


inline _CvMATElem_& _CvMATElem_::operator = ( const CvScalar& scalar )
{
    cvScalarToRawData( &scalar, ptr, type, 0 );
    return *this;
}


inline _CvMATElem_& _CvMATElem_::operator = ( double d )
{
    CvMAT::set( ptr, type, d );
    return *this;
}


inline _CvMATElem_& _CvMATElem_::operator = ( float f )
{
    CvMAT::set( ptr, type, (double)f );
    return *this;
}


inline _CvMATElem_& _CvMATElem_::operator = ( int i )
{
    CvMAT::set( ptr, type, i );
    return *this;
}


/********************************** CvMAT inline methods ********************************/

inline CvMAT::CvMAT()
{
    memset( this, 0, sizeof(*this));
}


inline CvMAT::CvMAT( int rows, int cols, int type, void* data, int step )
{
    cvInitMatHeader( this, rows, cols, type, data, step );
}


inline CvMAT::CvMAT( int rows, int type, void* data, int step )
{
    cvInitMatHeader( this, rows, 1, type, data, step );
}


inline void CvMAT::create( int rows, int cols, int type )
{
    int step = cols*CV_ELEM_SIZE(type), total_size = step*rows;
    this->rows = rows;
    this->cols = cols;
    this->step = rows == 1 ? 0 : step;
    this->type = CV_MAT_MAGIC_VAL | (type & CV_MAT_TYPE_MASK) | CV_MAT_CONT_FLAG;
    refcount = (int*)cvAlloc((size_t)total_size + 8);
    data.ptr = (uchar*)(((size_t)(refcount + 1) + 7) & -8);
    *refcount = 1;
}


inline CvMAT::CvMAT( int rows, int cols, int type )
{
    create( rows, cols, type );
}


inline CvMAT::CvMAT( int rows, int type )
{
    create( rows, 1, type );
}


inline CvMAT::CvMAT( const CvMat& mat )
{
    memcpy( this, &mat, sizeof(mat));
    if( refcount )
        (*refcount)++;
}


inline CvMAT::CvMAT( const CvMAT& mat )
{
    memcpy( this, &mat, sizeof(mat));
    if( refcount )
        (*refcount)++;
}


inline CvMAT::CvMAT( const IplImage& img )
{
    cvGetMat( &img, this );
}


inline void CvMAT::release()
{
    data.ptr = NULL;
    if( refcount != NULL && --*refcount == 0 )
        cvFree( (void**)&refcount );
    refcount = 0;
}


inline CvMAT::~CvMAT()
{
    release();
}


inline CvMAT& CvMAT::operator = ( const CvMAT& mat )
{
    if( this != &mat )
    {
        release();
        memcpy( this, &mat, sizeof(mat));
        if( refcount )
            (*refcount)++;
    }
    return *this;
}


inline CvMAT& CvMAT::operator = ( const CvMat& mat )
{
    *this = (const CvMAT&)mat;
    return *this;
}


inline CvMAT& CvMAT::operator = ( const IplImage& img )
{
    release();
    cvGetMat( &img, this );

    return *this;
}


inline CvMAT& CvMAT::operator = ( double fillval )
{
    cvFillImage( this, fillval );
    return *this;
}


inline CvMAT& CvMAT::operator = ( const CvScalar& fillval )
{
    cvSet( this, fillval );
    return *this;
}


inline CvMAT& CvMAT::operator += ( const CvMat& mat )
{
    cvAdd( this, &mat, this );
    return *this;
}


inline CvMAT& CvMAT::operator += ( double val )
{
    cvAddS( this, cvScalar(val), this );
    return *this;
}


inline CvMAT& CvMAT::operator += ( const CvScalar& val )
{
    cvAddS( this, val, this );
    return *this;
}


inline CvMAT& CvMAT::operator -= ( const CvMat& mat )
{
    cvSub( this, &mat, this );
    return *this;
}


inline CvMAT& CvMAT::operator -= ( double val )
{
    cvSubS( this, cvScalar(val), this );
    return *this;
}


inline CvMAT& CvMAT::operator -= ( const CvScalar& val )
{
    cvSubS( this, val, this );
    return *this;
}


inline CvMAT& CvMAT::operator *= ( const CvMat& mat )
{
    cvMul( this, &mat, this );
    return *this;    
}


inline CvMAT& CvMAT::operator *= ( double val )
{
    cvScale( this, this, val, 0 );
    return *this;
}


inline CvMAT& CvMAT::operator *= ( const CvScalar& val )
{
    cvScaleAdd( this, val, 0, this );
    return *this;
}


inline CvMAT& CvMAT::operator &= ( const CvMat& mat )
{
    cvAnd( this, &mat, this );
    return *this;
}


inline CvMAT& CvMAT::operator &= ( double val )
{
    cvAndS( this, cvScalarAll(val), this );
    return *this;
}


inline CvMAT& CvMAT::operator &= ( const CvScalar& val )
{
    cvAndS( this, val, this );
    return *this;
}


inline CvMAT& CvMAT::operator |= ( const CvMat& mat )
{
    cvOr( this, &mat, this );
    return *this;
}


inline CvMAT& CvMAT::operator |= ( double val )
{
    cvOrS( this, cvScalarAll(val), this );
    return *this;
}


inline CvMAT& CvMAT::operator |= ( const CvScalar& val )
{
    cvOrS( this, val, this );
    return *this;
}


inline CvMAT& CvMAT::operator ^= ( const CvMat& mat )
{
    cvXor( this, &mat, this );
    return *this;
}


inline CvMAT& CvMAT::operator ^= ( double val )
{
    cvXorS( this, cvScalarAll(val), this );
    return *this;
}


inline CvMAT& CvMAT::operator ^= ( const CvScalar& val )
{
    cvXorS( this, val, this );
    return *this;
}


inline double CvMAT::norm( int normType ) const
{   return cvNorm( this, 0, normType ); }


inline double CvMAT::min( CvPoint* minloc ) const
{   
    double t;
    cvMinMaxLoc( this, &t, 0, minloc, 0, 0 );
    return t;
}

inline double CvMAT::max( CvPoint* maxloc ) const
{   
    double t;
    cvMinMaxLoc( this, 0, &t, 0, maxloc, 0 );
    return t;
}


inline double CvMAT::norm( CvMat& mat, int normType ) const
{   return cvNorm( this, &mat, normType );  }


inline CvScalar CvMAT::sum() const
{   return cvSum( this );   }


inline double CvMAT::det() const
{   return cvDet( this );   }


inline void CvMAT::reshape( int newcn, int newrows )
{   cvReshape( this, this, newcn, newrows );    }


inline void CvMAT::flipX()
{   cvFlip( this, this, 1 );    }


inline void CvMAT::flipY()
{   cvFlip( this, this, 0 );    }


inline void CvMAT::flipXY()
{   cvFlip( this, this, -1 );   }


inline _CvMATElem_ CvMAT::operator ()( int row )
{   return _CvMATElem_( CV_MAT_ELEM_PTR( *this, row, 0 ), type );   }


inline _CvMATConstElem_ CvMAT::operator ()( int row ) const
{   return _CvMATConstElem_( CV_MAT_ELEM_PTR( *this, row, 0 ), type ); }


inline _CvMATElem_ CvMAT::operator ()( int row, int col )
{   return _CvMATElem_( CV_MAT_ELEM_PTR( *this, row, col ), type ); }


inline _CvMATConstElem_ CvMAT::operator ()( int row, int col ) const
{   return _CvMATConstElem_( CV_MAT_ELEM_PTR( *this, row, col ), type ); }


inline _CvMATElemCn_ CvMAT::operator()( int row, int col, int coi )
{   return _CvMATElemCn_( CV_MAT_ELEM_PTR( *this, row, col ), type, coi );  }


inline _CvMATElemCn_ CvMAT::operator()( CvPoint pt, int coi )
{   return _CvMATElemCn_( CV_MAT_ELEM_PTR( *this, pt.y, pt.x ), type, coi );  }


inline double CvMAT::operator()( int row, int col, int coi ) const
{   return get( CV_MAT_ELEM_PTR( *this, row, col ), type, coi );    }


inline _CvMATElem_ CvMAT::operator ()( CvPoint pt )
{   return _CvMATElem_( CV_MAT_ELEM_PTR( *this, pt.y, pt.x ), type ); }


inline _CvMATConstElem_ CvMAT::operator ()( CvPoint pt ) const
{   return _CvMATConstElem_( CV_MAT_ELEM_PTR( *this, pt.y, pt.x ), type ); }


inline double CvMAT::operator()( CvPoint pt, int coi ) const
{   return get( CV_MAT_ELEM_PTR( *this, pt.y, pt.x ), type, coi );    }


inline void* CvMAT::ptr( int row )
{   return CV_MAT_ELEM_PTR( *this, row, 0 );    }


inline const void* CvMAT::ptr( int row ) const
{   return (const void*)CV_MAT_ELEM_PTR( *this, row, 0 );   }


inline void* CvMAT::ptr( int row, int col )
{   return CV_MAT_ELEM_PTR( *this, row, col );  }


inline const void* CvMAT::ptr( int row, int col ) const
{   return (const void*)CV_MAT_ELEM_PTR( *this, row, col ); }


inline void* CvMAT::ptr( CvPoint pt )
{   return CV_MAT_ELEM_PTR( *this, pt.y, pt.x );    }


inline const void* CvMAT::ptr( CvPoint pt ) const
{   return (const void*)CV_MAT_ELEM_PTR( *this, pt.y, pt.x ); }


inline _CvMAT_INV_ CvMAT::inv( int method ) const
{   return _CvMAT_INV_( this, method ); }


inline _CvMAT_T_ CvMAT::t() const
{   return _CvMAT_T_( this );   }


inline _CvMAT_COPY_ CvMAT::clone() const
{   return _CvMAT_COPY_( this ); }

inline _CvMAT_CVT_ CvMAT::cvt( int newdepth, double scale, double shift ) const
{   return _CvMAT_CVT_( this, newdepth, scale, shift ); }


inline CvMAT::CvMAT( const CvMat& mat, CvRect rect )
{   
    type = 0;
    cvGetSubArr( &mat, this, rect );
    cvIncRefData( this );
}


/* submatrix:
     k == 0 - i-th row
     k > 0 - i-th column
     k < 0 - i-th diagonal */
inline CvMAT::CvMAT( const CvMat& mat, int k, int i )
{
    type = 0;
    if( k == 0 )
        cvGetRow( &mat, this, i );
    else if( k > 0 )
        cvGetCol( &mat, this, i );
    else
        cvGetDiag( &mat, this, i );
    cvIncRefData( this );
}


inline CvMAT CvMAT::row( int r ) const
{   return CvMAT( *this, 0, r );  }


inline CvMAT CvMAT::col( int c ) const
{   return CvMAT( *this, 1, c );  }


inline CvMAT CvMAT::diag( int d ) const
{   return CvMAT( *this, -1, d );  }


inline CvMAT CvMAT::rect( CvRect rect ) const
{   return CvMAT( *this, rect );    }

inline CvMAT CvMAT::rowrange( int row1, int row2 ) const
{   
    assert( 0 <= row1 && row1 < row2 && row2 <= height );
    return CvMAT( *this, cvRect( 0, row1, width, row2 - row1 ));
}

inline CvMAT CvMAT::colrange( int col1, int col2 ) const
{   
    assert( 0 <= col1 && col1 < col2 && col2 <= width );
    return CvMAT( *this, cvRect( col1, 0, col2 - col1, height ));
}

inline _CvMAT_DOT_OP_ CvMAT::mul( const CvMAT& mat ) const
{   return _CvMAT_DOT_OP_( this, &mat, '*' );   }

inline _CvMAT_DOT_OP_ CvMAT::mul( const _CvMAT_SCALE_& mat ) const
{   return _CvMAT_DOT_OP_( this, mat.a, '*', mat.alpha );   }

inline _CvMAT_DOT_OP_ CvMAT::div( const CvMAT& mat ) const
{   return _CvMAT_DOT_OP_( this, &mat, '/' );  }

inline _CvMAT_DOT_OP_ CvMAT::div( const _CvMAT_SCALE_& mat ) const
{   return _CvMAT_DOT_OP_( this, mat.a, '/', 1./mat.alpha );    }

inline _CvMAT_DOT_OP_ CvMAT::min( const CvMAT& mat ) const
{   return _CvMAT_DOT_OP_( this, &mat, 'm' );   }

inline _CvMAT_DOT_OP_ CvMAT::max( const CvMAT& mat ) const
{   return _CvMAT_DOT_OP_( this, &mat, 'M' );   }

inline _CvMAT_DOT_OP_ CvMAT::min( double value ) const
{   return _CvMAT_DOT_OP_( this, 0, 'm', value );   }

inline _CvMAT_DOT_OP_ CvMAT::max( double value ) const
{   return _CvMAT_DOT_OP_( this, 0, 'M', value );   }

inline _CvMAT_DOT_OP_ CvMAT::abs() const
{   return _CvMAT_DOT_OP_( this, 0, 'a', 0 );   }

/****************************************************************************************\
*                               binary operations (+,-,*)                                *
\****************************************************************************************/

/*
* PART I. Scaling, shifting, addition/subtraction operations
*/

/* (mat2^t) = (mat1^t) * scalar */
inline _CvMAT_T_ operator * ( const _CvMAT_T_& a, double alpha )
{   return _CvMAT_T_( &a.a, a.alpha*alpha );  }

/* (mat2^t) = scalar * (mat1^t) */
inline _CvMAT_T_ operator * ( double alpha, const _CvMAT_T_& a )
{   return _CvMAT_T_( &a.a, a.alpha*alpha );  }

/* -(mat^t) */
inline _CvMAT_T_ operator - ( const _CvMAT_T_& a )
{   return _CvMAT_T_( &a.a, -a.alpha ); }

/* mat_scaled = mat * scalar */
inline _CvMAT_SCALE_ operator * ( const CvMAT& a, double alpha )
{   return _CvMAT_SCALE_( &a, alpha );  }

/* mat_scaled = scalar * mat */
inline _CvMAT_SCALE_ operator * ( double alpha, const CvMAT& a )
{   return _CvMAT_SCALE_( &a, alpha );  }

/* mat_scaled2 = mat_scaled1 * scalar */
inline _CvMAT_SCALE_ operator * ( const _CvMAT_SCALE_& a, double alpha )
{   return _CvMAT_SCALE_( a.a, a.alpha*alpha ); }

/* mat_scaled2 = scalar * mat_scaled1 */
inline _CvMAT_SCALE_ operator * ( double alpha, const _CvMAT_SCALE_& a )
{   return _CvMAT_SCALE_( a.a, a.alpha*alpha ); }

/* -mat_scaled */
inline _CvMAT_SCALE_ operator - ( const _CvMAT_SCALE_& a )
{   return _CvMAT_SCALE_( a.a, -a.alpha ); }


/* mat_scaled_shifted = mat + scalar */
inline _CvMAT_SCALE_SHIFT_ operator + ( const CvMAT& a, double beta )
{   return _CvMAT_SCALE_SHIFT_( &a, 1, beta );  }

/* mat_scaled_shifted = scalar + mat */
inline _CvMAT_SCALE_SHIFT_ operator + ( double beta, const CvMAT& a )
{   return _CvMAT_SCALE_SHIFT_( &a, 1, beta );  }

/* mat_scaled_shifted = mat - scalar */
inline _CvMAT_SCALE_SHIFT_ operator - ( const CvMAT& a, double beta )
{   return _CvMAT_SCALE_SHIFT_( &a, 1, -beta ); }

/* mat_scaled_shifted = scalar - mat */
inline _CvMAT_SCALE_SHIFT_ operator - ( double beta, const CvMAT& a )
{   return _CvMAT_SCALE_SHIFT_( &a, -1, beta ); }

/* mat_scaled_shifted = mat_scaled + scalar */
inline _CvMAT_SCALE_SHIFT_ operator + ( const _CvMAT_SCALE_& a, double beta )
{   return _CvMAT_SCALE_SHIFT_( a.a, a.alpha, beta );   }

/* mat_scaled_shifted = scalar + mat_scaled */
inline _CvMAT_SCALE_SHIFT_ operator + ( double beta, const _CvMAT_SCALE_& a )
{   return _CvMAT_SCALE_SHIFT_( a.a, a.alpha, beta );   }

/* mat_scaled_shifted = mat_scaled - scalar */
inline _CvMAT_SCALE_SHIFT_ operator - ( const _CvMAT_SCALE_& a, double beta )
{   return _CvMAT_SCALE_SHIFT_( a.a, a.alpha, -beta );  }

/* mat_scaled_shifted = scalar - mat_scaled */
inline _CvMAT_SCALE_SHIFT_ operator - ( double beta, const _CvMAT_SCALE_& a )
{   return _CvMAT_SCALE_SHIFT_( a.a, -a.alpha, beta );  }

/* mat_scaled_shifted2 = mat_scaled_shifted1 + scalar */
inline _CvMAT_SCALE_SHIFT_ operator + ( const _CvMAT_SCALE_SHIFT_& a, double beta )
{   return _CvMAT_SCALE_SHIFT_( a.a, a.alpha, a.beta + beta );  }

/* mat_scaled_shifted2 = scalar + mat_scaled_shifted1 */
inline _CvMAT_SCALE_SHIFT_ operator + ( double beta, const _CvMAT_SCALE_SHIFT_& a )
{   return _CvMAT_SCALE_SHIFT_( a.a, a.alpha, a.beta + beta );  }

/* mat_scaled_shifted2 = mat_scaled_shifted1 - scalar */
inline _CvMAT_SCALE_SHIFT_ operator - ( const _CvMAT_SCALE_SHIFT_& a, double beta )
{   return _CvMAT_SCALE_SHIFT_( a.a, a.alpha, a.beta - beta );  }

/* mat_scaled_shifted2 = scalar - mat_scaled_shifted1 */
inline _CvMAT_SCALE_SHIFT_ operator - ( double beta, const _CvMAT_SCALE_SHIFT_& a )
{   return _CvMAT_SCALE_SHIFT_( a.a, -a.alpha, beta - a.beta ); }

/* mat_scaled_shifted2 = mat_scaled_shifted1 * scalar */
inline _CvMAT_SCALE_SHIFT_ operator * ( const _CvMAT_SCALE_SHIFT_& a, double alpha )
{   return _CvMAT_SCALE_SHIFT_( a.a, a.alpha*alpha, a.beta*alpha ); }

/* mat_scaled_shifted2 = scalar * mat_scaled_shifted1 */
inline _CvMAT_SCALE_SHIFT_ operator * ( double alpha, const _CvMAT_SCALE_SHIFT_& a )
{   return _CvMAT_SCALE_SHIFT_( a.a, a.alpha*alpha, a.beta*alpha ); }

/* -mat_scaled_shifted */
inline _CvMAT_SCALE_SHIFT_ operator - ( const _CvMAT_SCALE_SHIFT_& a )
{   return _CvMAT_SCALE_SHIFT_( a.a, -a.alpha, -a.beta ); }


/* -mat1 */
inline _CvMAT_SCALE_ operator - ( const CvMAT& a )
{   return _CvMAT_SCALE_( &a, -1 );   }

/* mat_add = mat1 + mat2 */
inline _CvMAT_ADD_ operator + ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_ADD_( &a, &b );   }

/* mat_add = mat1 - mat2 */
inline _CvMAT_ADD_ operator - ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_ADD_( &a, &b, -1 );    }

/* mat_add = mat_scaled1 + mat2 */
inline _CvMAT_ADD_ operator + ( const _CvMAT_SCALE_& a, const CvMAT& b )
{   return _CvMAT_ADD_( &b, a.a, a.alpha );  }

/* mat_add = mat1 + mat_scaled2 */
inline _CvMAT_ADD_ operator + ( const CvMAT& b, const _CvMAT_SCALE_& a )
{   return _CvMAT_ADD_( &b, a.a, a.alpha );  }

/* -mat_add */
inline _CvMAT_ADD_EX_ operator - ( const _CvMAT_ADD_& a )
{   return _CvMAT_ADD_EX_( a.a, -1, a.b, -a.beta ); }

/* mat_add = mat_scaled1 - mat2 */
inline _CvMAT_ADD_EX_ operator - ( const _CvMAT_SCALE_& a, const CvMAT& b )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, &b, -1 ); }

/* mat_add = mat1 - mat_scaled2 */
inline _CvMAT_ADD_ operator - ( const CvMAT& b, const _CvMAT_SCALE_& a )
{   return _CvMAT_ADD_( &b, a.a, -a.alpha ); }

/* mat_add = mat_scaled_shifted1 + mat2 */
inline _CvMAT_ADD_EX_ operator + ( const _CvMAT_SCALE_SHIFT_& a, const CvMAT& b )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, &b, 1, a.beta ); }

/* mat_add = mat1 + mat_scaled_shifted2 */
inline _CvMAT_ADD_EX_ operator + ( const CvMAT& b, const _CvMAT_SCALE_SHIFT_& a )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, &b, 1, a.beta ); }

/* mat_add = mat_scaled_shifted1 - mat2 */
inline _CvMAT_ADD_EX_ operator - ( const _CvMAT_SCALE_SHIFT_& a, const CvMAT& b )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, &b, -1, a.beta ); }

/* mat_add = mat1 - mat_scaled_shifted2 */
inline _CvMAT_ADD_EX_ operator - ( const CvMAT& b, const _CvMAT_SCALE_SHIFT_& a )
{   return _CvMAT_ADD_EX_( a.a, -a.alpha, &b, 1, -a.beta ); }

/* mat_add = mat_scaled_shifted1 + mat_scaled2 */
inline _CvMAT_ADD_EX_ operator + ( const _CvMAT_SCALE_SHIFT_& a, const _CvMAT_SCALE_& b )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, b.a, b.alpha, a.beta ); }

/* mat_add = mat_scaled1 + mat_scaled_shifted2 */
inline _CvMAT_ADD_EX_ operator + ( const _CvMAT_SCALE_& b, const _CvMAT_SCALE_SHIFT_& a )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, b.a, b.alpha, a.beta ); }

/* mat_add = mat_scaled_shifted1 - mat_scaled2 */
inline _CvMAT_ADD_EX_ operator - ( const _CvMAT_SCALE_SHIFT_& a, const _CvMAT_SCALE_& b )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, b.a, -b.alpha, a.beta ); }

/* mat_add = mat_scaled1 - mat_scaled_shifted2 */
inline _CvMAT_ADD_EX_ operator - ( const _CvMAT_SCALE_& b, const _CvMAT_SCALE_SHIFT_& a )
{   return _CvMAT_ADD_EX_( a.a, -a.alpha, b.a, b.alpha, -a.beta ); }

/* mat_add = mat_scaled1 + mat_scaled2 */
inline _CvMAT_ADD_EX_ operator + ( const _CvMAT_SCALE_& a, const _CvMAT_SCALE_& b )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, b.a, b.alpha ); }

/* mat_add = mat_scaled1 - mat_scaled2 */
inline _CvMAT_ADD_EX_ operator - ( const _CvMAT_SCALE_& a, const _CvMAT_SCALE_& b )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, b.a, -b.alpha ); }

/* mat_add = mat_scaled_shifted1 + mat_scaled_shifted2 */
inline _CvMAT_ADD_EX_ operator + ( const _CvMAT_SCALE_SHIFT_& a,
                                const _CvMAT_SCALE_SHIFT_& b )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, b.a, b.alpha, a.beta + b.beta ); }

/* mat_add = mat_scaled_shifted1 - mat_scaled_shifted2 */
inline _CvMAT_ADD_EX_ operator - ( const _CvMAT_SCALE_SHIFT_& a,
                                const _CvMAT_SCALE_SHIFT_& b )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, b.a, -b.alpha, a.beta - b.beta ); }

/* mat_add2 = mat_add1 + scalar */
inline _CvMAT_ADD_EX_ operator + ( const _CvMAT_ADD_EX_& a, double gamma )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, a.b, a.beta, a.gamma + gamma ); }

/* mat_add2 = scalar + mat_add1 */
inline _CvMAT_ADD_EX_ operator + ( double gamma, const _CvMAT_ADD_EX_& a )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, a.b, a.beta, a.gamma + gamma ); }

/* mat_add2 = mat_add1 - scalar */
inline _CvMAT_ADD_EX_ operator - ( const _CvMAT_ADD_EX_& a, double gamma )
{   return _CvMAT_ADD_EX_( a.a, a.alpha, a.b, a.beta, a.gamma - gamma ); }

/* mat_add2 = scalar - mat_add1 */
inline _CvMAT_ADD_EX_ operator - ( double gamma, const _CvMAT_ADD_EX_& a )
{   return _CvMAT_ADD_EX_( a.a, -a.alpha, a.b, -a.beta, gamma - a.gamma ); }

/* mat_add2 = mat_add1 * scalar */
inline _CvMAT_ADD_EX_ operator * ( const _CvMAT_ADD_EX_& a, double alpha )
{   return _CvMAT_ADD_EX_( a.a, a.alpha*alpha, a.b, a.beta*alpha, a.gamma*alpha ); }

/* mat_add2 = scalar * mat_add1 */
inline _CvMAT_ADD_EX_ operator * ( double alpha, const _CvMAT_ADD_EX_& a )
{   return _CvMAT_ADD_EX_( a.a, a.alpha*alpha, a.b, a.beta*alpha, a.gamma*alpha ); }

/* mat_add2 = mat_add1 + scalar */
inline _CvMAT_ADD_EX_ operator + ( const _CvMAT_ADD_& a, double gamma )
{   return _CvMAT_ADD_EX_( a.a, 1, a.b, a.beta, gamma ); }

/* mat_add2 = scalar + mat_add1 */
inline _CvMAT_ADD_EX_ operator + ( double gamma, const _CvMAT_ADD_& a )
{   return _CvMAT_ADD_EX_( a.a, 1, a.b, a.beta, gamma ); }

/* mat_add2 = mat_add1 - scalar */
inline _CvMAT_ADD_EX_ operator - ( const _CvMAT_ADD_& a, double gamma )
{   return _CvMAT_ADD_EX_( a.a, 1, a.b, a.beta, -gamma ); }

/* mat_add2 = scalar - mat_add1 */
inline _CvMAT_ADD_EX_ operator - ( double gamma, const _CvMAT_ADD_& a )
{   return _CvMAT_ADD_EX_( a.a, -1, a.b, -a.beta, gamma ); }

/* mat_add2 = mat_add1 * scalar */
inline _CvMAT_ADD_EX_ operator * ( const _CvMAT_ADD_& a, double alpha )
{   return _CvMAT_ADD_EX_( a.a, alpha, a.b, a.beta*alpha, 0 ); }

/* mat_add2 = scalar * mat_add1 */
inline _CvMAT_ADD_EX_ operator * ( double alpha, const _CvMAT_ADD_& a )
{   return _CvMAT_ADD_EX_( a.a, alpha, a.b, a.beta*alpha, 0 ); }

/* -mat_add_ex */
inline _CvMAT_ADD_EX_ operator - ( const _CvMAT_ADD_EX_& a )
{   return _CvMAT_ADD_EX_( a.a, -a.alpha, a.b, -a.beta, -a.gamma ); }


/*
* PART II. Matrix multiplication.
*/

/* mmul = mat1 * mat2 */
inline _CvMAT_MUL_ operator * ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_MUL_( &a, &b, 0 );    }

/* mmul = (mat1^t) * mat2 */
inline _CvMAT_MUL_ operator * ( const _CvMAT_T_& a, const CvMAT& b )
{   return _CvMAT_MUL_( &a.a, &b, a.alpha, 1 );   }

/* mmul = mat1 * (mat2^t) */
inline _CvMAT_MUL_ operator * ( const CvMAT& b, const _CvMAT_T_& a )
{   return _CvMAT_MUL_( &b, &a.a, a.alpha, 2 );  }

/* mmul = (mat1^t) * (mat2^t) */
inline _CvMAT_MUL_ operator * ( const _CvMAT_T_& a, const _CvMAT_T_& b )
{   return _CvMAT_MUL_( &a.a, &b.a, a.alpha*b.alpha, 3 );  }

/* mmul = mat_scaled1 * mat2 */
inline _CvMAT_MUL_ operator * ( const _CvMAT_SCALE_& a, const CvMAT& b )
{   return _CvMAT_MUL_( a.a, &b, a.alpha, 0 ); }

/* mmul = mat1 * mat_scaled2 */
inline _CvMAT_MUL_ operator * ( const CvMAT& b, const _CvMAT_SCALE_& a )
{   return _CvMAT_MUL_( &b, a.a, a.alpha, 0 ); }

/* mmul = (mat1^t) * mat_scaled1 */
inline _CvMAT_MUL_ operator * ( const _CvMAT_T_& a, const _CvMAT_SCALE_& b )
{   return _CvMAT_MUL_( &a.a, b.a, a.alpha*b.alpha, 1 ); }

/* mmul = mat_scaled1 * (mat2^t) */
inline _CvMAT_MUL_ operator * ( const _CvMAT_SCALE_& b, const _CvMAT_T_& a )
{   return _CvMAT_MUL_( b.a, &a.a, a.alpha*b.alpha, 2 ); }

/* mmul = mat_scaled1 * mat_scaled2 */
inline _CvMAT_MUL_ operator * ( const _CvMAT_SCALE_& a, const _CvMAT_SCALE_& b )
{   return _CvMAT_MUL_( a.a, b.a, a.alpha*b.alpha, 0 ); }

/* mmul2 = mmul1 * scalar */
inline _CvMAT_MUL_ operator * ( const _CvMAT_MUL_& a, double alpha )
{   return _CvMAT_MUL_( a.a, a.b, a.alpha*alpha, a.t_ab ); }

/* mmul2 = scalar * mmul1 */
inline _CvMAT_MUL_ operator * ( double alpha, const _CvMAT_MUL_& a )
{   return _CvMAT_MUL_( a.a, a.b, a.alpha*alpha, a.t_ab ); }

/* -mmul */
inline _CvMAT_MUL_ operator - ( const _CvMAT_MUL_& a )
{   return _CvMAT_MUL_( a.a, a.b, -a.alpha, a.t_ab ); }

/* mmuladd = mmul + mat */
inline _CvMAT_MUL_ADD_ operator + ( const _CvMAT_MUL_& a, const CvMAT& b )
{   return _CvMAT_MUL_ADD_( a.a, a.b, a.alpha, &b, 1, a.t_ab ); }

/* !!! Comment this off because of ambigous conversion error !!!
   mmuladd = mat + mmul */
/* inline _CvMAT_MUL_ADD_ operator + ( const CvMAT& b, const _CvMAT_MUL_& a )
{   return _CvMAT_MUL_ADD_( a.a, a.b, a.alpha, &b, 1, a.t_ab ); }*/

/* mmuladd = mmul - mat */
inline _CvMAT_MUL_ADD_ operator - ( const _CvMAT_MUL_& a, const CvMAT& b )
{   return _CvMAT_MUL_ADD_( a.a, a.b, a.alpha, &b, -1, a.t_ab ); }

/* !!! Comment this off because of ambigous conversion error !!!
   mmuladd = mat - mmul */
/*inline _CvMAT_MUL_ADD_ operator - ( const CvMAT& b, const _CvMAT_MUL_& a )
{   return _CvMAT_MUL_ADD_( a.a, a.b, -a.alpha, &b, 1, a.t_ab ); }*/

/* mmuladd = mmul + mat_scaled */
inline _CvMAT_MUL_ADD_ operator + ( const _CvMAT_MUL_& a, const _CvMAT_SCALE_& b )
{   return _CvMAT_MUL_ADD_( a.a, a.b, a.alpha, b.a, b.alpha, a.t_ab ); }

/* mmuladd = mat_scaled + mmul */
inline _CvMAT_MUL_ADD_ operator + ( const _CvMAT_SCALE_& b, const _CvMAT_MUL_& a )
{   return _CvMAT_MUL_ADD_( a.a, a.b, a.alpha, b.a, b.alpha, a.t_ab ); }

/* mmuladd = mmul - mat_scaled */
inline _CvMAT_MUL_ADD_ operator - ( const _CvMAT_MUL_& a, const _CvMAT_SCALE_& b )
{   return _CvMAT_MUL_ADD_( a.a, a.b, a.alpha, b.a, -b.alpha, a.t_ab ); }

/* mmuladd = mat_scaled - mmul */
inline _CvMAT_MUL_ADD_ operator - ( const _CvMAT_SCALE_& b, const _CvMAT_MUL_& a )
{   return _CvMAT_MUL_ADD_( a.a, a.b, -a.alpha, b.a, b.alpha, a.t_ab ); }

/* mmuladd = mmul + (mat^t) */
inline _CvMAT_MUL_ADD_ operator + ( const _CvMAT_MUL_& a, const _CvMAT_T_& b )
{   return _CvMAT_MUL_ADD_( a.a, a.b, a.alpha, &b.a, b.alpha, a.t_ab + 4 );  }

/* mmuladd = (mat^t) + mmul */
inline _CvMAT_MUL_ADD_ operator + ( const _CvMAT_T_& b, const _CvMAT_MUL_& a )
{   return _CvMAT_MUL_ADD_( a.a, a.b, a.alpha, &b.a, b.alpha, a.t_ab + 4 );  }

/* mmuladd = mmul - (mat^t) */
inline _CvMAT_MUL_ADD_ operator - ( const _CvMAT_MUL_& a, const _CvMAT_T_& b )
{   return _CvMAT_MUL_ADD_( a.a, a.b, a.alpha, &b.a, -b.alpha, a.t_ab + 4 );  }

/* mmuladd = (mat^t) - mmul */
inline _CvMAT_MUL_ADD_ operator - ( const _CvMAT_T_& b, const _CvMAT_MUL_& a )
{   return _CvMAT_MUL_ADD_( a.a, a.b, -a.alpha, &b.a, b.alpha, a.t_ab + 4 );  }


/* mmuladd = mat_scaled_shifted * mat */
inline _CvMAT_MUL_ADD_ operator * ( const _CvMAT_SCALE_SHIFT_& a, const CvMAT& b )
{   return _CvMAT_MUL_ADD_( a.a, &b, a.alpha, &b, a.beta, 0 );  }

/* mmuladd = mat * mat_scaled_shifted */
inline _CvMAT_MUL_ADD_ operator * ( const CvMAT& b, const _CvMAT_SCALE_SHIFT_& a )
{   return _CvMAT_MUL_ADD_( &b, a.a, a.alpha, &b, a.beta, 0 );  }

/* mmuladd = mat_scaled_shifted * mat_scaled */
inline _CvMAT_MUL_ADD_ operator * ( const _CvMAT_SCALE_SHIFT_& a, const _CvMAT_SCALE_& b )
{   return _CvMAT_MUL_ADD_( a.a, b.a, a.alpha*b.alpha, b.a, a.beta*b.alpha, 0 );  }

/* mmuladd = mat_scaled * mat_scaled_shifted */
inline _CvMAT_MUL_ADD_ operator * ( const _CvMAT_SCALE_& b, const _CvMAT_SCALE_SHIFT_& a )
{   return _CvMAT_MUL_ADD_( b.a, a.a, a.alpha*b.alpha, b.a, a.beta*b.alpha, 0 );  }

/* mmuladd = mat_scaled_shifted * (mat^t) */
inline _CvMAT_MUL_ADD_ operator * ( const _CvMAT_SCALE_SHIFT_& a, const _CvMAT_T_& b )
{   return _CvMAT_MUL_ADD_( a.a, &b.a, a.alpha*b.alpha, &b.a, a.beta*b.alpha, 6 );  }

/* mmuladd = (mat^t) * mat_scaled_shifted */
inline _CvMAT_MUL_ADD_ operator * ( const _CvMAT_T_& b, const _CvMAT_SCALE_SHIFT_& a )
{   return _CvMAT_MUL_ADD_( &b.a, a.a, a.alpha*b.alpha, &b.a, a.beta*b.alpha, 5 );  }

/* mmuladd2 = mmuladd1 * scalar */
inline _CvMAT_MUL_ADD_ operator * ( const _CvMAT_MUL_ADD_& a, double alpha )
{   return _CvMAT_MUL_ADD_( a.a, a.b, a.alpha*alpha, a.c, a.beta*alpha, a.t_abc ); }

/* mmuladd2 = scalar * mmuladd1 */
inline _CvMAT_MUL_ADD_ operator * ( double alpha, const _CvMAT_MUL_ADD_& a )
{   return _CvMAT_MUL_ADD_( a.a, a.b, a.alpha*alpha, a.c, a.beta*alpha, a.t_abc ); }

/* -mmuladd */
inline _CvMAT_MUL_ADD_ operator - ( const _CvMAT_MUL_ADD_& a )
{   return _CvMAT_MUL_ADD_( a.a, a.b, -a.alpha, a.c, -a.beta, a.t_abc ); }

/* inv(a)*b, i.e. solve a*x = b */
inline _CvMAT_SOLVE_ operator * ( const _CvMAT_INV_& a, const CvMAT& b )
{   return _CvMAT_SOLVE_( &a.a, &b, a.method );    }


/*
* PART III. Logical operations
*/
inline _CvMAT_NOT_ operator ~ ( const CvMAT& a )
{   return _CvMAT_NOT_(&a); }

inline _CvMAT_LOGIC_ operator & ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_LOGIC_( &a, &b, _CvMAT_LOGIC_::AND, 0 ); }

inline _CvMAT_LOGIC_ operator & ( const _CvMAT_NOT_& a, const CvMAT& b )
{   return _CvMAT_LOGIC_( a.a, &b, _CvMAT_LOGIC_::AND, 1 ); }

inline _CvMAT_LOGIC_ operator & ( const CvMAT& a, const _CvMAT_NOT_& b )
{   return _CvMAT_LOGIC_( &a, b.a, _CvMAT_LOGIC_::AND, 2 ); }

inline _CvMAT_LOGIC_ operator & ( const _CvMAT_NOT_& a, const _CvMAT_NOT_& b )
{   return _CvMAT_LOGIC_( a.a, b.a, _CvMAT_LOGIC_::AND, 3 ); }


inline _CvMAT_LOGIC_ operator | ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_LOGIC_( &a, &b, _CvMAT_LOGIC_::OR, 0 ); }

inline _CvMAT_LOGIC_ operator | ( const _CvMAT_NOT_& a, const CvMAT& b )
{   return _CvMAT_LOGIC_( a.a, &b, _CvMAT_LOGIC_::OR, 1 ); }

inline _CvMAT_LOGIC_ operator | ( const CvMAT& a, const _CvMAT_NOT_& b )
{   return _CvMAT_LOGIC_( &a, b.a, _CvMAT_LOGIC_::OR, 2 ); }

inline _CvMAT_LOGIC_ operator | ( const _CvMAT_NOT_& a, const _CvMAT_NOT_& b )
{   return _CvMAT_LOGIC_( a.a, b.a, _CvMAT_LOGIC_::OR, 3 ); }


inline _CvMAT_LOGIC_ operator ^ ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_LOGIC_( &a, &b, _CvMAT_LOGIC_::XOR, 0 ); }

inline _CvMAT_LOGIC_ operator ^ ( const _CvMAT_NOT_& a, const CvMAT& b )
{   return _CvMAT_LOGIC_( a.a, &b, _CvMAT_LOGIC_::XOR, 1 ); }

inline _CvMAT_LOGIC_ operator ^ ( const CvMAT& a, const _CvMAT_NOT_& b )
{   return _CvMAT_LOGIC_( &a, b.a, _CvMAT_LOGIC_::XOR, 2 ); }

inline _CvMAT_LOGIC_ operator ^ ( const _CvMAT_NOT_& a, const _CvMAT_NOT_& b )
{   return _CvMAT_LOGIC_( a.a, b.a, _CvMAT_LOGIC_::XOR, 3 ); }


inline _CvMAT_UN_LOGIC_ operator & ( const CvMAT& a, double alpha )
{   return _CvMAT_UN_LOGIC_( &a, alpha, _CvMAT_LOGIC_::AND, 0 ); }

inline _CvMAT_UN_LOGIC_ operator & ( double alpha, const CvMAT& a )
{   return _CvMAT_UN_LOGIC_( &a, alpha, _CvMAT_LOGIC_::AND, 0 ); }

inline _CvMAT_UN_LOGIC_ operator & ( const _CvMAT_NOT_& a, double alpha )
{   return _CvMAT_UN_LOGIC_( a.a, alpha, _CvMAT_LOGIC_::AND, 1 ); }

inline _CvMAT_UN_LOGIC_ operator & ( double alpha, const _CvMAT_NOT_& a )
{   return _CvMAT_UN_LOGIC_( a.a, alpha, _CvMAT_LOGIC_::AND, 1 ); }


inline _CvMAT_UN_LOGIC_ operator | ( const CvMAT& a, double alpha )
{   return _CvMAT_UN_LOGIC_( &a, alpha, _CvMAT_LOGIC_::OR, 0 ); }

inline _CvMAT_UN_LOGIC_ operator | ( double alpha, const CvMAT& a )
{   return _CvMAT_UN_LOGIC_( &a, alpha, _CvMAT_LOGIC_::OR, 0 ); }

inline _CvMAT_UN_LOGIC_ operator | ( const _CvMAT_NOT_& a, double alpha )
{   return _CvMAT_UN_LOGIC_( a.a, alpha, _CvMAT_LOGIC_::OR, 1 ); }

inline _CvMAT_UN_LOGIC_ operator | ( double alpha, const _CvMAT_NOT_& a )
{   return _CvMAT_UN_LOGIC_( a.a, alpha, _CvMAT_LOGIC_::OR, 1 ); }


inline _CvMAT_UN_LOGIC_ operator ^ ( const CvMAT& a, double alpha )
{   return _CvMAT_UN_LOGIC_( &a, alpha, _CvMAT_LOGIC_::XOR, 0 ); }

inline _CvMAT_UN_LOGIC_ operator ^ ( double alpha, const CvMAT& a )
{   return _CvMAT_UN_LOGIC_( &a, alpha, _CvMAT_LOGIC_::XOR, 0 ); }

inline _CvMAT_UN_LOGIC_ operator ^ ( const _CvMAT_NOT_& a, double alpha )
{   return _CvMAT_UN_LOGIC_( a.a, alpha, _CvMAT_LOGIC_::XOR, 1 ); }

inline _CvMAT_UN_LOGIC_ operator ^ ( double alpha, const _CvMAT_NOT_& a )
{   return _CvMAT_UN_LOGIC_( a.a, alpha, _CvMAT_LOGIC_::XOR, 1 ); }


/*
* PART IV. Comparison operations
*/
inline _CvMAT_CMP_ operator > ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_CMP_( &a, &b, CV_CMP_GT ); }

inline _CvMAT_CMP_ operator >= ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_CMP_( &a, &b, CV_CMP_GE ); }

inline _CvMAT_CMP_ operator < ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_CMP_( &a, &b, CV_CMP_LT ); }

inline _CvMAT_CMP_ operator <= ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_CMP_( &a, &b, CV_CMP_LE ); }

inline _CvMAT_CMP_ operator == ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_CMP_( &a, &b, CV_CMP_EQ ); }

inline _CvMAT_CMP_ operator != ( const CvMAT& a, const CvMAT& b )
{   return _CvMAT_CMP_( &a, &b, CV_CMP_NE ); }


inline _CvMAT_CMP_ operator > ( const CvMAT& a, double alpha )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_GT ); }

inline _CvMAT_CMP_ operator > ( double alpha, const CvMAT& a )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_LT ); }

inline _CvMAT_CMP_ operator >= ( const CvMAT& a, double alpha )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_GE ); }

inline _CvMAT_CMP_ operator >= ( double alpha, const CvMAT& a )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_LE ); }

inline _CvMAT_CMP_ operator < ( const CvMAT& a, double alpha )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_LT ); }

inline _CvMAT_CMP_ operator < ( double alpha, const CvMAT& a )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_GT ); }

inline _CvMAT_CMP_ operator <= ( const CvMAT& a, double alpha )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_LE ); }

inline _CvMAT_CMP_ operator <= ( double alpha, const CvMAT& a )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_GE ); }

inline _CvMAT_CMP_ operator == ( const CvMAT& a, double alpha )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_EQ ); }

inline _CvMAT_CMP_ operator == ( double alpha, const CvMAT& a )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_EQ ); }

inline _CvMAT_CMP_ operator != ( const CvMAT& a, double alpha )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_NE ); }

inline _CvMAT_CMP_ operator != ( double alpha, const CvMAT& a )
{   return _CvMAT_CMP_( &a, alpha, CV_CMP_NE ); }


/*
* PART V. Speedup for some augmented assignments to CvMAT
*/

inline CvMAT& CvMAT::operator += ( const _CvMAT_SCALE_& scale_mat )
{   return (*this = *this + scale_mat); }

inline CvMAT& CvMAT::operator += ( const _CvMAT_SCALE_SHIFT_& scale_mat )
{   return (*this = *this + scale_mat); }

inline CvMAT& CvMAT::operator += ( const _CvMAT_MUL_& mmul )
{   return (*this = mmul + *this);  }

inline CvMAT& CvMAT::operator -= ( const _CvMAT_SCALE_& scale_mat )
{   return (*this = *this - scale_mat);  }

inline CvMAT& CvMAT::operator -= ( const _CvMAT_SCALE_SHIFT_& scale_mat )
{   return (*this = *this - scale_mat);  }

inline CvMAT& CvMAT::operator -= ( const _CvMAT_MUL_& mmul )
{   return (*this = -mmul + *this);  }

inline CvMAT& CvMAT::operator *= ( const _CvMAT_SCALE_& scale_mat )
{   return (*this = *this * scale_mat);  }

inline CvMAT& CvMAT::operator *= ( const _CvMAT_SCALE_SHIFT_& scale_mat )
{   return (*this = *this * scale_mat);  }

/****************************************************************************************\
*                        misc. operations on temporary matrices (+,-,*)                  *
\****************************************************************************************/

/*
* the base proxy class implementation
*/

/* a.*b */
inline _CvMAT_DOT_OP_ _CvMAT_BASE_OP_::mul( const CvMAT& a ) const
{   return ((CvMAT)*this).mul(a);  }

/* a.*b*alpha */
inline _CvMAT_DOT_OP_ _CvMAT_BASE_OP_::mul( const _CvMAT_SCALE_& a ) const
{   return ((CvMAT)*this).mul(a);  }

/* a./b */
inline _CvMAT_DOT_OP_ _CvMAT_BASE_OP_::div( const CvMAT& a ) const
{   return ((CvMAT)*this).div(a);  }

/* a./(b*alpha) */
inline _CvMAT_DOT_OP_ _CvMAT_BASE_OP_::div( const _CvMAT_SCALE_& a ) const
{   return ((CvMAT)*this).div(a);  }

/* a.max(b) */
inline _CvMAT_DOT_OP_ _CvMAT_BASE_OP_::min( const CvMAT& a ) const
{   return ((CvMAT)*this).min(a);  }

/* a.min(b) */
inline _CvMAT_DOT_OP_ _CvMAT_BASE_OP_::max( const CvMAT& a ) const
{   return ((CvMAT)*this).max(a);  }

/* a.max(alpha) */
inline _CvMAT_DOT_OP_ _CvMAT_BASE_OP_::min( double alpha ) const
{   return ((CvMAT)*this).min(alpha);  }

/* a.min(alpha) */
inline _CvMAT_DOT_OP_ _CvMAT_BASE_OP_::max( double alpha ) const
{   return ((CvMAT)*this).max(alpha);  }


inline _CvMAT_INV_  _CvMAT_BASE_OP_::inv( int method ) const
{   return ((CvMAT)*this).inv(method);  }

inline _CvMAT_T_  _CvMAT_BASE_OP_::t() const
{   return ((CvMAT)*this).t();  }

inline _CvMAT_CVT_ _CvMAT_BASE_OP_::cvt( int newdepth, double scale, double shift ) const
{   return ((CvMAT)*this).cvt( newdepth, scale, shift ); }

inline CvMAT  _CvMAT_BASE_OP_::row( int r ) const
{   return CvMAT((CvMAT)*this, 0, r ); }

inline CvMAT  _CvMAT_BASE_OP_::rowrange( int row1, int row2 ) const
{   
    CvMAT m = (CvMAT)*this;
    assert( 0 <= row1 && row1 < row2 && row2 <= m.height );
    return CvMAT( m, cvRect( 0, row1, m.width, row2 - row1 ));
}

inline CvMAT  _CvMAT_BASE_OP_::col( int c ) const
{   return CvMAT( (CvMAT)*this, 1, c ); }

inline CvMAT  _CvMAT_BASE_OP_::colrange( int col1, int col2 ) const
{   
    CvMAT m = (CvMAT)*this;
    assert( 0 <= col1 && col1 < col2 && col2 <= m.width );
    return CvMAT( m, cvRect( col1, 0, col2 - col1, m.height ));
}

inline CvMAT  _CvMAT_BASE_OP_::rect( CvRect r ) const
{   return CvMAT( (CvMAT)*this, r ); }

inline CvMAT  _CvMAT_BASE_OP_::diag( int d ) const
{   return CvMAT( (CvMAT)*this, -1, d ); }

inline double _CvMAT_BASE_OP_::det() const
{   return ((CvMAT)*this).det(); }

inline double _CvMAT_BASE_OP_::norm( int norm_type ) const
{   return ((CvMAT)*this).norm( norm_type ); }

inline CvScalar _CvMAT_BASE_OP_::sum() const
{   return ((CvMAT)*this).sum(); }

inline double _CvMAT_BASE_OP_::min( CvPoint* minloc ) const
{   return ((CvMAT)*this).min( minloc ); }

inline double _CvMAT_BASE_OP_::max( CvPoint* maxloc ) const
{   return ((CvMAT)*this).max( maxloc ); }


/****************************************************************************************/
/*                              proxy classes implementation.                           */
/*                              part I. constructors                                    */
/****************************************************************************************/

/* constructors */
inline _CvMAT_COPY_::_CvMAT_COPY_( const CvMAT* _a ) : a((CvMAT*)_a)  {}

inline _CvMAT_CVT_::_CvMAT_CVT_( const CvMAT* _a, int _newdepth,
                                 double _scale, double _shift ) :
    a(*(CvMAT*)_a), newdepth(_newdepth), scale(_scale), shift(_shift)  {}

inline _CvMAT_T_::_CvMAT_T_( const CvMAT* _a ) : a(*(CvMAT*)_a), alpha(1)  {}


inline _CvMAT_T_::_CvMAT_T_( const CvMAT* _a, double _alpha ) :
    a(*(CvMAT*)_a), alpha(_alpha)  {}


inline _CvMAT_INV_::_CvMAT_INV_( const CvMAT* _a, int _method ) :
    a(*(CvMAT*)_a), method(_method) {}


inline _CvMAT_MUL_::_CvMAT_MUL_( const CvMAT* _a, const CvMAT* _b, int _t_ab ) :
    a((CvMAT*)_a), b((CvMAT*)_b), alpha(1), t_ab(_t_ab) {}


inline _CvMAT_MUL_::_CvMAT_MUL_( const CvMAT* _a, const CvMAT* _b,
                                 double _alpha, int _t_ab ) :
    a((CvMAT*)_a), b((CvMAT*)_b), alpha(_alpha), t_ab(_t_ab) {}


inline _CvMAT_MUL_ADD_::_CvMAT_MUL_ADD_( const CvMAT* _a, const CvMAT* _b,
                                         const CvMAT* _c, int _t_abc ) :
    a((CvMAT*)_a), b((CvMAT*)_b), c((CvMAT*)_c), t_abc(_t_abc) {}


inline _CvMAT_MUL_ADD_::_CvMAT_MUL_ADD_( const CvMAT* _a, const CvMAT* _b, double _alpha,
                                         const CvMAT* _c, double _beta, int _t_abc ) :
    a((CvMAT*)_a), b((CvMAT*)_b), alpha(_alpha),
    c((CvMAT*)_c), beta(_beta), t_abc(_t_abc) {}


inline _CvMAT_ADD_::_CvMAT_ADD_( const CvMAT* _a, const CvMAT* _b, double _beta ) :
    a((CvMAT*)_a), b((CvMAT*)_b), beta(_beta) {}


inline _CvMAT_ADD_EX_::_CvMAT_ADD_EX_( const CvMAT* _a, double _alpha,
                                       const CvMAT* _b, double _beta, double _gamma ) :
    a((CvMAT*)_a), alpha(_alpha), b((CvMAT*)_b), beta(_beta), gamma(_gamma) {}


inline _CvMAT_SCALE_::_CvMAT_SCALE_( const CvMAT* _a, double _alpha ) :
    a((CvMAT*)_a), alpha(_alpha) {}


inline _CvMAT_SCALE_SHIFT_::_CvMAT_SCALE_SHIFT_( const CvMAT* _a,
                                                 double _alpha, double _beta ) :
    a((CvMAT*)_a), alpha(_alpha), beta(_beta) {}


inline _CvMAT_LOGIC_::_CvMAT_LOGIC_( const CvMAT* _a, const CvMAT* _b,
                                            _CvMAT_LOGIC_::Op _op, int _flags ) :
    a((CvMAT*)_a), b((CvMAT*)_b), op(_op), flags(_flags) {}


inline _CvMAT_UN_LOGIC_::_CvMAT_UN_LOGIC_( const CvMAT* _a, double _alpha,
                                           _CvMAT_LOGIC_::Op _op, int _flags ) :
    a((CvMAT*)_a), alpha(_alpha), op(_op), flags(_flags) {}


inline _CvMAT_NOT_::_CvMAT_NOT_( const CvMAT* _a ) :
    a((CvMAT*)_a) {}


inline _CvMAT_DOT_OP_::_CvMAT_DOT_OP_( const CvMAT* _a, const CvMAT* _b,
                                       int _op, double _alpha ) :
    a(*_a), b((CvMAT*)_b), op(_op), alpha(_alpha) {}


inline _CvMAT_SOLVE_::_CvMAT_SOLVE_( const CvMAT* _a, const CvMAT* _b, int _method ) :
    a((CvMAT*)_a), b((CvMAT*)_b), method(_method) {}

inline _CvMAT_CMP_::_CvMAT_CMP_( const CvMAT* _a, const CvMAT* _b, int _cmp_op ) :
    a((CvMAT*)_a), b((CvMAT*)_b), alpha(0), cmp_op(_cmp_op) {}

inline _CvMAT_CMP_::_CvMAT_CMP_( const CvMAT* _a, double _alpha, int _cmp_op ) :
    a((CvMAT*)_a), b(0), alpha(_alpha), cmp_op(_cmp_op) {}

/****************************************************************************************/
/*                              proxy classes implementation.                           */
/*                              part II. conversion to CvMAT                      */
/****************************************************************************************/

inline _CvMAT_T_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_INV_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_MUL_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_SCALE_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_SCALE_SHIFT_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_ADD_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_ADD_EX_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_MUL_ADD_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_LOGIC_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_UN_LOGIC_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_NOT_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_DOT_OP_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_SOLVE_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_CMP_::operator CvMAT() const
{   return CvMAT( *this );    }

inline _CvMAT_CVT_::operator CvMAT() const
{   return CvMAT(*this);   }

inline _CvMAT_COPY_::operator CvMAT() const
{   return *a;   }

/****************************************************************************************/
/*                              proxy classes implementation.                           */
/*                              part III. custom overrided methods                      */
/****************************************************************************************/

inline _CvMAT_DOT_OP_ _CvMAT_SCALE_::mul( const CvMAT& mat ) const
{   return _CvMAT_DOT_OP_( a, &mat, '*', alpha );   }

inline _CvMAT_DOT_OP_ _CvMAT_SCALE_::mul( const _CvMAT_SCALE_& mat ) const
{   return _CvMAT_DOT_OP_( a, mat.a, '*', alpha*mat.alpha );   }

inline _CvMAT_DOT_OP_ _CvMAT_SCALE_::div( const CvMAT& mat ) const
{   return _CvMAT_DOT_OP_( a, &mat, '/', alpha );   }

inline _CvMAT_DOT_OP_ _CvMAT_SCALE_::div( const _CvMAT_SCALE_& mat ) const
{   return _CvMAT_DOT_OP_( a, mat.a, '/', alpha/mat.alpha );   }

inline _CvMAT_DOT_OP_ operator * ( const _CvMAT_DOT_OP_& dot_op, double alpha )
{   return _CvMAT_DOT_OP_( &dot_op.a, dot_op.b, dot_op.op, dot_op.alpha * alpha );  }

inline _CvMAT_DOT_OP_ operator * ( double alpha, const _CvMAT_DOT_OP_& dot_op )
{   return _CvMAT_DOT_OP_( &dot_op.a, dot_op.b, dot_op.op, dot_op.alpha * alpha );  }

inline _CvMAT_DOT_OP_ operator / ( double alpha, const CvMAT& mat )
{   return _CvMAT_DOT_OP_( &mat, 0, '/', alpha );  }

inline _CvMAT_DOT_OP_ operator / ( double alpha, const _CvMAT_SCALE_& mat )
{   return _CvMAT_DOT_OP_( mat.a, 0, '/', alpha/mat.alpha );  }


inline double _CvMAT_T_::det() const
{   return a.det();     }

inline double _CvMAT_T_::norm( int norm_type ) const
{   return a.norm( norm_type );    }

inline double _CvMAT_ADD_::norm( int norm_type ) const
{
    if( beta == -1 )
        return cvNorm( a, b, norm_type );
    else
        return ((CvMAT)*this).norm( norm_type );
}

inline _CvMAT_DOT_OP_ _CvMAT_ADD_::abs() const
{
    if( beta == -1 )
        return _CvMAT_DOT_OP_( a, b, 'a', 0 );
    else
        return ((CvMAT)*this).abs();
}

inline _CvMAT_DOT_OP_ _CvMAT_SCALE_SHIFT_::abs() const
{
    if( alpha == 1 )
        return _CvMAT_DOT_OP_( a, 0, 'a', -beta );
    else
        return ((CvMAT)*this).abs();
}

#endif /* __cplusplus */

#endif /*_CVMAT_HPP_*/

