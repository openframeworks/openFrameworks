



#include "ofMatrix4x4.h"

#include <limits>
#include <stdlib.h>
#include <iomanip>

#if (_MSC_VER)
#undef min
// see: http://stackoverflow.com/questions/1904635/warning-c4003-and-errors-c2589-and-c2059-on-x-stdnumericlimitsintmax
#endif

inline bool equivalent(double lhs,double rhs,double epsilon=1e-6)
{ double delta = rhs-lhs; return delta<0.0?delta>=-epsilon:delta<=epsilon; }
template<typename T>
inline T square(T v) { return v*v; }

#define SET_ROW(row, v1, v2, v3, v4 )    \
_mat[(row)][0] = (v1); \
_mat[(row)][1] = (v2); \
_mat[(row)][2] = (v3); \
_mat[(row)][3] = (v4);

#define INNER_PRODUCT(a,b,r,c) \
((a)._mat[r][0] * (b)._mat[0][c]) \
+((a)._mat[r][1] * (b)._mat[1][c]) \
+((a)._mat[r][2] * (b)._mat[2][c]) \
+((a)._mat[r][3] * (b)._mat[3][c])

ofMatrix4x4::ofMatrix4x4( float a00, float a01, float a02, float a03,
											 float a10, float a11, float a12, float a13,
											 float a20, float a21, float a22, float a23,
											 float a30, float a31, float a32, float a33)
{
    SET_ROW(0, a00, a01, a02, a03 )
    SET_ROW(1, a10, a11, a12, a13 )
    SET_ROW(2, a20, a21, a22, a23 )
    SET_ROW(3, a30, a31, a32, a33 )
}

void ofMatrix4x4::set( float a00, float a01, float a02, float a03,
								float a10, float a11, float a12, float a13,
								float a20, float a21, float a22, float a23,
								float a30, float a31, float a32, float a33)
{
    SET_ROW(0, a00, a01, a02, a03 )
    SET_ROW(1, a10, a11, a12, a13 )
    SET_ROW(2, a20, a21, a22, a23 )
    SET_ROW(3, a30, a31, a32, a33 )
}

#define QX  q._v[0]
#define QY  q._v[1]
#define QZ  q._v[2]
#define QW  q._v[3]

void ofMatrix4x4::setRotate(const ofQuaternion& q)
{
    double length2 = q.length2();
    if (fabs(length2) <= std::numeric_limits<double>::min())
    {
        _mat[0][0] = 1.0; _mat[1][0] = 0.0; _mat[2][0] = 0.0;
        _mat[0][1] = 0.0; _mat[1][1] = 1.0; _mat[2][1] = 0.0;
        _mat[0][2] = 0.0; _mat[1][2] = 0.0; _mat[2][2] = 1.0;
    }
    else
    {
        double rlength2;
        // normalize quat if required.
        // We can avoid the expensive sqrt in this case since all 'coefficients' below are products of two q components.
        // That is a square of a square root, so it is possible to avoid that
        if (length2 != 1.0)
        {
            rlength2 = 2.0/length2;
        }
        else
        {
            rlength2 = 2.0;
        }

        // Source: Gamasutra, Rotating Objects Using Quaternions
        //
        //http://www.gamasutra.com/features/19980703/quaternions_01.htm

        double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

        // calculate coefficients
        x2 = rlength2*QX;
        y2 = rlength2*QY;
        z2 = rlength2*QZ;

        xx = QX * x2;
        xy = QX * y2;
        xz = QX * z2;

        yy = QY * y2;
        yz = QY * z2;
        zz = QZ * z2;

        wx = QW * x2;
        wy = QW * y2;
        wz = QW * z2;

        // Note.  Gamasutra gets the matrix assignments inverted, resulting
        // in left-handed rotations, which is contrary to OpenGL and OSG's
        // methodology.  The matrix assignment has been altered in the next
        // few lines of code to do the right thing.
        // Don Burns - Oct 13, 2001
        _mat[0][0] = 1.0 - (yy + zz);
        _mat[1][0] = xy - wz;
        _mat[2][0] = xz + wy;


        _mat[0][1] = xy + wz;
        _mat[1][1] = 1.0 - (xx + zz);
        _mat[2][1] = yz - wx;

        _mat[0][2] = xz - wy;
        _mat[1][2] = yz + wx;
        _mat[2][2] = 1.0 - (xx + yy);
    }

#if 0
    _mat[0][3] = 0.0;
    _mat[1][3] = 0.0;
    _mat[2][3] = 0.0;

    _mat[3][0] = 0.0;
    _mat[3][1] = 0.0;
    _mat[3][2] = 0.0;
    _mat[3][3] = 1.0;
#endif
}
//
//ofQuaternion ofMatrix4x4::getRotate() const {
//	ofVec4f q;
//	float trace = _mat[0][0] + _mat[1][1] + _mat[2][2]; // I removed + 1.0f; see discussion with Ethan
//	if( trace > 0 ) {// I changed M_EPSILON to 0
//		float s = 0.5f / sqrtf(trace+ 1.0f);
//		q.w = 0.25f / s;
//		q.x = ( _mat[2][1] - _mat[1][2] ) * s;
//		q.y = ( _mat[0][2] - _mat[2][0] ) * s;
//		q.z = ( _mat[1][0] - _mat[0][1] ) * s;
//	} else {
//		if ( _mat[0][0] > _mat[1][1] && _mat[0][0] > _mat[2][2] ) {
//			float s = 2.0f * sqrtf( 1.0f + _mat[0][0] - _mat[1][1] - _mat[2][2]);
//			q.w = (_mat[2][1] - _mat[1][2] ) / s;
//			q.x = 0.25f * s;
//			q.y = (_mat[0][1] + _mat[1][0] ) / s;
//			q.z = (_mat[0][2] + _mat[2][0] ) / s;
//		} else if (_mat[1][1] > _mat[2][2]) {
//			float s = 2.0f * sqrtf( 1.0f + _mat[1][1] - _mat[0][0] - _mat[2][2]);
//			q.w = (_mat[0][2] - _mat[2][0] ) / s;
//			q.x = (_mat[0][1] + _mat[1][0] ) / s;
//			q.y = 0.25f * s;
//			q.z = (_mat[1][2] + _mat[2][1] ) / s;
//		} else {
//			float s = 2.0f * sqrtf( 1.0f + _mat[2][2] - _mat[0][0] - _mat[1][1] );
//			q.w = (_mat[1][0] - _mat[0][1] ) / s;
//			q.x = (_mat[0][2] + _mat[2][0] ) / s;
//			q.y = (_mat[1][2] + _mat[2][1] ) / s;
//			q.z = 0.25f * s;
//		}
//	}
//	return ofQuaternion(q.x, q.y, q.z, q.w);
//}

//#define COMPILE_getRotate_David_Spillings_Mk1
//#define COMPILE_getRotate_David_Spillings_Mk2
#define COMPILE_getRotate_Original

#ifdef COMPILE_getRotate_David_Spillings_Mk1
// David Spillings implementation Mk 1
ofQuaternion ofMatrix4x4::getRotate() const
{
	ofQuaternion q;

    float s;
    float tq[4];
    int    i, j;

    // Use tq to store the largest trace
    tq[0] = 1 + _mat[0][0]+_mat[1][1]+_mat[2][2];
    tq[1] = 1 + _mat[0][0]-_mat[1][1]-_mat[2][2];
    tq[2] = 1 - _mat[0][0]+_mat[1][1]-_mat[2][2];
    tq[3] = 1 - _mat[0][0]-_mat[1][1]+_mat[2][2];

    // Find the maximum (could also use stacked if's later)
    j = 0;
    for(i=1;i<4;i++) j = (tq[i]>tq[j])? i : j;

    // check the diagonal
    if (j==0)
    {
        /* perform instant calculation */
        QW = tq[0];
        QX = _mat[1][2]-_mat[2][1];
        QY = _mat[2][0]-_mat[0][2];
        QZ = _mat[0][1]-_mat[1][0];
    }
    else if (j==1)
    {
        QW = _mat[1][2]-_mat[2][1];
        QX = tq[1];
        QY = _mat[0][1]+_mat[1][0];
        QZ = _mat[2][0]+_mat[0][2];
    }
    else if (j==2)
    {
        QW = _mat[2][0]-_mat[0][2];
        QX = _mat[0][1]+_mat[1][0];
        QY = tq[2];
        QZ = _mat[1][2]+_mat[2][1];
    }
    else /* if (j==3) */
    {
        QW = _mat[0][1]-_mat[1][0];
        QX = _mat[2][0]+_mat[0][2];
        QY = _mat[1][2]+_mat[2][1];
        QZ = tq[3];
    }

    s = sqrt(0.25/tq[j]);
    QW *= s;
    QX *= s;
    QY *= s;
    QZ *= s;

    return q;

}
#endif


#ifdef COMPILE_getRotate_David_Spillings_Mk2
// David Spillings implementation Mk 2
ofQuaternion ofMatrix4x4::getRotate() const
{
    ofQuaternion q;

    // From http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
    QW = 0.5 * sqrt( osg::maximum( 0.0, 1.0 + _mat[0][0] + _mat[1][1] + _mat[2][2] ) );
    QX = 0.5 * sqrt( osg::maximum( 0.0, 1.0 + _mat[0][0] - _mat[1][1] - _mat[2][2] ) );
    QY = 0.5 * sqrt( osg::maximum( 0.0, 1.0 - _mat[0][0] + _mat[1][1] - _mat[2][2] ) );
    QZ = 0.5 * sqrt( osg::maximum( 0.0, 1.0 - _mat[0][0] - _mat[1][1] + _mat[2][2] ) );

#if 0
    // Robert Osfield, June 7th 2007, arggg this new implementation produces many many errors, so have to revert to sign(..) orignal below.
    QX = QX * osg::signOrZero(  _mat[1][2] - _mat[2][1]) ;
    QY = QY * osg::signOrZero(  _mat[2][0] - _mat[0][2]) ;
    QZ = QZ * osg::signOrZero(  _mat[0][1] - _mat[1][0]) ;
#else
    QX = QX * osg::sign(  _mat[1][2] - _mat[2][1]) ;
    QY = QY * osg::sign(  _mat[2][0] - _mat[0][2]) ;
    QZ = QZ * osg::sign(  _mat[0][1] - _mat[1][0]) ;
#endif

    return q;
}
#endif

#ifdef COMPILE_getRotate_Original
// Original implementation
ofQuaternion ofMatrix4x4::getRotate() const
{
    ofQuaternion q;

    ofMatrix4x4 mat = *this;
    ofVec3f vs = mat.getScale();
    mat.scale(1./vs.x,1./vs.y,1./vs.z);

    ofVec4f* m = mat._mat;

    // Source: Gamasutra, Rotating Objects Using Quaternions
    //
    //http://www.gamasutra.com/features/programming/19980703/quaternions_01.htm

    float  tr, s;
    float tq[4];
    int    i, j, k;

    int nxt[3] = {1, 2, 0};

    tr = m[0][0] + m[1][1] + m[2][2]+1.0;

    // check the diagonal
    if (tr > 0.0)
    {
        s = (float)sqrt (tr);
        QW = s / 2.0;
        s = 0.5 / s;
        QX = (m[1][2] - m[2][1]) * s;
        QY = (m[2][0] - m[0][2]) * s;
        QZ = (m[0][1] - m[1][0]) * s;
    }
    else
    {
        // diagonal is negative
        i = 0;
        if (m[1][1] > m[0][0])
            i = 1;
        if (m[2][2] > m[i][i])
            i = 2;
        j = nxt[i];
        k = nxt[j];

        s = (float)sqrt ((m[i][i] - (m[j][j] + m[k][k])) + 1.0);

        tq[i] = s * 0.5;

        if (s != 0.0)
            s = 0.5 / s;

        tq[3] = (m[j][k] - m[k][j]) * s;
        tq[j] = (m[i][j] + m[j][i]) * s;
        tq[k] = (m[i][k] + m[k][i]) * s;

        QX = tq[0];
        QY = tq[1];
        QZ = tq[2];
        QW = tq[3];
    }

    return q;
}
#endif


//int ofMatrix4x4::compare(const ofMatrix4x4& m) const
//{
//    const ofMatrix4x4::float* lhs = reinterpret_cast<const ofMatrix4x4::float*>(_mat);
//    const ofMatrix4x4::float* end_lhs = lhs+16;
//    const ofMatrix4x4::float* rhs = reinterpret_cast<const ofMatrix4x4::float*>(m._mat);
//    for(;lhs!=end_lhs;++lhs,++rhs)
//    {
//        if (*lhs < *rhs) return -1;
//        if (*rhs < *lhs) return 1;
//    }
//    return 0;
//}

void ofMatrix4x4::setTranslation( float tx, float ty, float tz )
{
    _mat[3][0] = tx;
    _mat[3][1] = ty;
    _mat[3][2] = tz;
}


void ofMatrix4x4::setTranslation( const ofVec3f& v )
{
    _mat[3][0] = v.getPtr()[0];
    _mat[3][1] = v.getPtr()[1];
    _mat[3][2] = v.getPtr()[2];
}

void ofMatrix4x4::makeIdentityMatrix()
{
    SET_ROW(0,    1, 0, 0, 0 )
    SET_ROW(1,    0, 1, 0, 0 )
    SET_ROW(2,    0, 0, 1, 0 )
    SET_ROW(3,    0, 0, 0, 1 )
}

void ofMatrix4x4::makeScaleMatrix( const ofVec3f& v )
{
    makeScaleMatrix(v.getPtr()[0], v.getPtr()[1], v.getPtr()[2] );
}

void ofMatrix4x4::makeScaleMatrix( float x, float y, float z )
{
    SET_ROW(0,    x, 0, 0, 0 )
    SET_ROW(1,    0, y, 0, 0 )
    SET_ROW(2,    0, 0, z, 0 )
    SET_ROW(3,    0, 0, 0, 1 )
}

void ofMatrix4x4::makeTranslationMatrix( const ofVec3f& v )
{
    makeTranslationMatrix( v.getPtr()[0], v.getPtr()[1], v.getPtr()[2] );
}
void ofMatrix4x4::makeTranslationMatrix( float x, float y, float z )
{
    SET_ROW(0,    1, 0, 0, 0 )
    SET_ROW(1,    0, 1, 0, 0 )
    SET_ROW(2,    0, 0, 1, 0 )
    SET_ROW(3,    x, y, z, 1 )
}

void ofMatrix4x4::makeRotationMatrix( const ofVec3f& from, const ofVec3f& to )
{
    makeIdentityMatrix();

    ofQuaternion quat;
    quat.makeRotate(from,to);
    setRotate(quat);
}
void ofMatrix4x4::makeRotationMatrix( float angle, const ofVec3f& axis )
{
    makeIdentityMatrix();

    ofQuaternion quat;
    quat.makeRotate( angle, axis);
    setRotate(quat);
}

void ofMatrix4x4::makeRotationMatrix( float angle, float x, float y, float z )
{
    makeIdentityMatrix();

    ofQuaternion quat;
    quat.makeRotate( angle, x, y, z);
    setRotate(quat);
}

void ofMatrix4x4::makeRotationMatrix( const ofQuaternion& quat )
{
    makeIdentityMatrix();

    setRotate(quat);
}

void ofMatrix4x4::makeRotationMatrix( float angle1, const ofVec3f& axis1,
									   float angle2, const ofVec3f& axis2,
									   float angle3, const ofVec3f& axis3)
{
    makeIdentityMatrix();

    ofQuaternion quat;
    quat.makeRotate(angle1, axis1,
                    angle2, axis2,
                    angle3, axis3);
    setRotate(quat);
}

void ofMatrix4x4::makeFromMultiplicationOf( const ofMatrix4x4& lhs, const ofMatrix4x4& rhs )
{
    if (&lhs==this)
    {
        postMult(rhs);
        return;
    }
    if (&rhs==this)
    {
        preMult(lhs);
        return;
    }

	// PRECONDITION: We assume neither &lhs nor &rhs == this
	// if it did, use preMult or postMult instead
    _mat[0][0] = INNER_PRODUCT(lhs, rhs, 0, 0);
    _mat[0][1] = INNER_PRODUCT(lhs, rhs, 0, 1);
    _mat[0][2] = INNER_PRODUCT(lhs, rhs, 0, 2);
    _mat[0][3] = INNER_PRODUCT(lhs, rhs, 0, 3);
    _mat[1][0] = INNER_PRODUCT(lhs, rhs, 1, 0);
    _mat[1][1] = INNER_PRODUCT(lhs, rhs, 1, 1);
    _mat[1][2] = INNER_PRODUCT(lhs, rhs, 1, 2);
    _mat[1][3] = INNER_PRODUCT(lhs, rhs, 1, 3);
    _mat[2][0] = INNER_PRODUCT(lhs, rhs, 2, 0);
    _mat[2][1] = INNER_PRODUCT(lhs, rhs, 2, 1);
    _mat[2][2] = INNER_PRODUCT(lhs, rhs, 2, 2);
    _mat[2][3] = INNER_PRODUCT(lhs, rhs, 2, 3);
    _mat[3][0] = INNER_PRODUCT(lhs, rhs, 3, 0);
    _mat[3][1] = INNER_PRODUCT(lhs, rhs, 3, 1);
    _mat[3][2] = INNER_PRODUCT(lhs, rhs, 3, 2);
    _mat[3][3] = INNER_PRODUCT(lhs, rhs, 3, 3);
}

void ofMatrix4x4::preMult( const ofMatrix4x4& other )
{
    // brute force method requiring a copy
    //ofMatrix4x4 tmp(other* *this);
    // *this = tmp;

    // more efficient method just use a float[4] for temporary storage.
    float t[4];
    for(int col=0; col<4; ++col) {
        t[0] = INNER_PRODUCT( other, *this, 0, col );
        t[1] = INNER_PRODUCT( other, *this, 1, col );
        t[2] = INNER_PRODUCT( other, *this, 2, col );
        t[3] = INNER_PRODUCT( other, *this, 3, col );
        _mat[0][col] = t[0];
        _mat[1][col] = t[1];
        _mat[2][col] = t[2];
        _mat[3][col] = t[3];
    }

}

void ofMatrix4x4::postMult( const ofMatrix4x4& other )
{
    // brute force method requiring a copy
    //ofMatrix4x4 tmp(*this * other);
    // *this = tmp;

    // more efficient method just use a float[4] for temporary storage.
    float t[4];
    for(int row=0; row<4; ++row)
    {
        t[0] = INNER_PRODUCT( *this, other, row, 0 );
        t[1] = INNER_PRODUCT( *this, other, row, 1 );
        t[2] = INNER_PRODUCT( *this, other, row, 2 );
        t[3] = INNER_PRODUCT( *this, other, row, 3 );
        SET_ROW(row, t[0], t[1], t[2], t[3] )
    }
}

#undef INNER_PRODUCT

// orthoNormalize the 3x3 rotation matrix
void ofMatrix4x4::makeOrthoNormalOf(const ofMatrix4x4& rhs)
{
    float x_colMag = (rhs._mat[0][0] * rhs._mat[0][0]) + (rhs._mat[1][0] * rhs._mat[1][0]) + (rhs._mat[2][0] * rhs._mat[2][0]);
    float y_colMag = (rhs._mat[0][1] * rhs._mat[0][1]) + (rhs._mat[1][1] * rhs._mat[1][1]) + (rhs._mat[2][1] * rhs._mat[2][1]);
    float z_colMag = (rhs._mat[0][2] * rhs._mat[0][2]) + (rhs._mat[1][2] * rhs._mat[1][2]) + (rhs._mat[2][2] * rhs._mat[2][2]);

    if(!equivalent((double)x_colMag, 1.0) && !equivalent((double)x_colMag, 0.0))
    {
		x_colMag = sqrt(x_colMag);
		_mat[0][0] = rhs._mat[0][0] / x_colMag;
		_mat[1][0] = rhs._mat[1][0] / x_colMag;
		_mat[2][0] = rhs._mat[2][0] / x_colMag;
    }
    else
    {
		_mat[0][0] = rhs._mat[0][0];
		_mat[1][0] = rhs._mat[1][0];
		_mat[2][0] = rhs._mat[2][0];
    }

    if(!equivalent((double)y_colMag, 1.0) && !equivalent((double)y_colMag, 0.0))
    {
		y_colMag = sqrt(y_colMag);
		_mat[0][1] = rhs._mat[0][1] / y_colMag;
		_mat[1][1] = rhs._mat[1][1] / y_colMag;
		_mat[2][1] = rhs._mat[2][1] / y_colMag;
    }
    else
    {
		_mat[0][1] = rhs._mat[0][1];
		_mat[1][1] = rhs._mat[1][1];
		_mat[2][1] = rhs._mat[2][1];
    }

    if(!equivalent((double)z_colMag, 1.0) && !equivalent((double)z_colMag, 0.0))
    {
		z_colMag = sqrt(z_colMag);
		_mat[0][2] = rhs._mat[0][2] / z_colMag;
		_mat[1][2] = rhs._mat[1][2] / z_colMag;
		_mat[2][2] = rhs._mat[2][2] / z_colMag;
    }
    else
    {
		_mat[0][2] = rhs._mat[0][2];
		_mat[1][2] = rhs._mat[1][2];
		_mat[2][2] = rhs._mat[2][2];
    }

    _mat[3][0] = rhs._mat[3][0];
    _mat[3][1] = rhs._mat[3][1];
    _mat[3][2] = rhs._mat[3][2];

    _mat[0][3] = rhs._mat[0][3];
    _mat[1][3] = rhs._mat[1][3];
    _mat[2][3] = rhs._mat[2][3];
    _mat[3][3] = rhs._mat[3][3];

}


/** 4x3 matrix invert, not right hand column is assumed to be 0,0,0,1. */
bool invert_4x3( const ofMatrix4x4& src, ofMatrix4x4 & dst);

/** full 4x4 matrix invert. */
bool invert_4x4( const ofMatrix4x4& rhs, ofMatrix4x4 & dst);

bool ofMatrix4x4::makeInvertOf(const ofMatrix4x4 & rhs){
	bool is_4x3 = (rhs._mat[0][3] == 0.0f && rhs._mat[1][3] == 0.0f &&  rhs._mat[2][3] == 0.0f && rhs._mat[3][3] == 1.0f);
	return is_4x3 ? invert_4x3(rhs,*this) :  invert_4x4(rhs,*this);
}

ofMatrix4x4 ofMatrix4x4::getInverse() const
{
    ofMatrix4x4 inverse;
    inverse.makeInvertOf(*this);
    return inverse;
}





/******************************************
 Matrix inversion technique:
 Given a matrix mat, we want to invert it.
 mat = [ r00 r01 r02 a
 r10 r11 r12 b
 r20 r21 r22 c
 tx  ty  tz  d ]
 We note that this matrix can be split into three matrices.
 mat = rot * trans * corr, where rot is rotation part, trans is translation part, and corr is the correction due to perspective (if any).
 rot = [ r00 r01 r02 0
 r10 r11 r12 0
 r20 r21 r22 0
 0   0   0   1 ]
 trans = [ 1  0  0  0
 0  1  0  0
 0  0  1  0
 tx ty tz 1 ]
 corr = [ 1 0 0 px
 0 1 0 py
 0 0 1 pz
 0 0 0 s ]
 where the elements of corr are obtained from linear combinations of the elements of rot, trans, and mat.
 So the inverse is mat' = (trans * corr)' * rot', where rot' must be computed the traditional way, which is easy since it is only a 3x3 matrix.
 This problem is simplified if [px py pz s] = [0 0 0 1], which will happen if mat was composed only of rotations, scales, and translations (which is common).  In this case, we can ignore corr entirely which saves on a lot of computations.
 ******************************************/

bool invert_4x3( const ofMatrix4x4& src, ofMatrix4x4 & dst )
{
    if (&src==&dst)
    {
		ofMatrix4x4 tm(src);
		return invert_4x3(tm,dst);
    }

    float r00, r01, r02,
	r10, r11, r12,
	r20, r21, r22;
	// Copy rotation components directly into registers for speed
    r00 = src._mat[0][0]; r01 = src._mat[0][1]; r02 = src._mat[0][2];
    r10 = src._mat[1][0]; r11 = src._mat[1][1]; r12 = src._mat[1][2];
    r20 = src._mat[2][0]; r21 = src._mat[2][1]; r22 = src._mat[2][2];

	// Partially compute inverse of rot
    dst._mat[0][0] = r11*r22 - r12*r21;
    dst._mat[0][1] = r02*r21 - r01*r22;
    dst._mat[0][2] = r01*r12 - r02*r11;

	// Compute determinant of rot from 3 elements just computed
    float one_over_det = 1.0/(r00*dst._mat[0][0] + r10*dst._mat[0][1] + r20*dst._mat[0][2]);
    r00 *= one_over_det; r10 *= one_over_det; r20 *= one_over_det;  // Saves on later computations

	// Finish computing inverse of rot
    dst._mat[0][0] *= one_over_det;
    dst._mat[0][1] *= one_over_det;
    dst._mat[0][2] *= one_over_det;
    dst._mat[0][3] = 0.0;
    dst._mat[1][0] = r12*r20 - r10*r22; // Have already been divided by det
    dst._mat[1][1] = r00*r22 - r02*r20; // same
    dst._mat[1][2] = r02*r10 - r00*r12; // same
    dst._mat[1][3] = 0.0;
    dst._mat[2][0] = r10*r21 - r11*r20; // Have already been divided by det
    dst._mat[2][1] = r01*r20 - r00*r21; // same
    dst._mat[2][2] = r00*r11 - r01*r10; // same
    dst._mat[2][3] = 0.0;
    dst._mat[3][3] = 1.0;

	// We no longer need the rxx or det variables anymore, so we can reuse them for whatever we want.  But we will still rename them for the sake of clarity.

#define d r22
    d  = src._mat[3][3];

    if( square(d-1.0) > 1.0e-6 )  // Involves perspective, so we must
    {                       // compute the full inverse

        ofMatrix4x4 TPinv;
        dst._mat[3][0] = dst._mat[3][1] = dst._mat[3][2] = 0.0;

#define px r00
#define py r01
#define pz r02
#define one_over_s  one_over_det
#define a  r10
#define b  r11
#define c  r12

        a  = src._mat[0][3]; b  = src._mat[1][3]; c  = src._mat[2][3];
        px = dst._mat[0][0]*a + dst._mat[0][1]*b + dst._mat[0][2]*c;
        py = dst._mat[1][0]*a + dst._mat[1][1]*b + dst._mat[1][2]*c;
        pz = dst._mat[2][0]*a + dst._mat[2][1]*b + dst._mat[2][2]*c;

#undef a
#undef b
#undef c
#define tx r10
#define ty r11
#define tz r12

        tx = src._mat[3][0]; ty = src._mat[3][1]; tz = src._mat[3][2];
        one_over_s  = 1.0/(d - (tx*px + ty*py + tz*pz));

        tx *= one_over_s; ty *= one_over_s; tz *= one_over_s;  // Reduces number of calculations later on

        // Compute inverse of trans*corr
        TPinv._mat[0][0] = tx*px + 1.0;
        TPinv._mat[0][1] = ty*px;
        TPinv._mat[0][2] = tz*px;
        TPinv._mat[0][3] = -px * one_over_s;
        TPinv._mat[1][0] = tx*py;
        TPinv._mat[1][1] = ty*py + 1.0;
        TPinv._mat[1][2] = tz*py;
        TPinv._mat[1][3] = -py * one_over_s;
        TPinv._mat[2][0] = tx*pz;
        TPinv._mat[2][1] = ty*pz;
        TPinv._mat[2][2] = tz*pz + 1.0;
        TPinv._mat[2][3] = -pz * one_over_s;
        TPinv._mat[3][0] = -tx;
        TPinv._mat[3][1] = -ty;
        TPinv._mat[3][2] = -tz;
        TPinv._mat[3][3] = one_over_s;

        dst.preMult(TPinv); // Finish computing full inverse of mat

#undef px
#undef py
#undef pz
#undef one_over_s
#undef d
    }
    else // Rightmost column is [0; 0; 0; 1] so it can be ignored
    {
        tx = src._mat[3][0]; ty = src._mat[3][1]; tz = src._mat[3][2];

        // Compute translation components of mat'
        dst._mat[3][0] = -(tx*dst._mat[0][0] + ty*dst._mat[1][0] + tz*dst._mat[2][0]);
        dst._mat[3][1] = -(tx*dst._mat[0][1] + ty*dst._mat[1][1] + tz*dst._mat[2][1]);
        dst._mat[3][2] = -(tx*dst._mat[0][2] + ty*dst._mat[1][2] + tz*dst._mat[2][2]);

#undef tx
#undef ty
#undef tz
    }

    return true;
}


template <class T>
inline T SGL_ABS(T a)
{
	return (a >= 0 ? a : -a);
}

#ifndef SGL_SWAP
#define SGL_SWAP(a,b,temp) ((temp)=(a),(a)=(b),(b)=(temp))
#endif

bool invert_4x4( const ofMatrix4x4& src, ofMatrix4x4&dst )
{
    if (&src==&dst) {
		ofMatrix4x4 tm(src);
		return invert_4x4(tm,dst);
    }

    unsigned int indxc[4], indxr[4], ipiv[4];
    unsigned int i,j,k,l,ll;
    unsigned int icol = 0;
    unsigned int irow = 0;
    double temp, pivinv, dum, big;

    // copy in place this may be unnecessary
    dst = src;

    for (j=0; j<4; j++) ipiv[j]=0;

    for(i=0;i<4;i++)
    {
		big=0.0;
		for (j=0; j<4; j++)
			if (ipiv[j] != 1)
				for (k=0; k<4; k++)
				{
					if (ipiv[k] == 0)
					{
						if (SGL_ABS(dst(j,k)) >= big)
						{
							big = SGL_ABS(dst(j,k));
							irow=j;
							icol=k;
						}
					}
					else if (ipiv[k] > 1)
						return false;
				}
		++(ipiv[icol]);
		if (irow != icol)
			for (l=0; l<4; l++) SGL_SWAP(dst(irow,l),
										 dst(icol,l),
										 temp);

		indxr[i]=irow;
		indxc[i]=icol;
		if (dst(icol,icol) == 0)
			return false;

		pivinv = 1.0/dst(icol,icol);
		dst(icol,icol) = 1;
		for (l=0; l<4; l++) dst(icol,l) *= pivinv;
		for (ll=0; ll<4; ll++)
			if (ll != icol)
			{
				dum=dst(ll,icol);
				dst(ll,icol) = 0;
				for (l=0; l<4; l++)dst(ll,l) -= dst(icol,l)*dum;
			}
    }
    for (int lx=4; lx>0; --lx)
    {
		if (indxr[lx-1] != indxc[lx-1])
			for (k=0; k<4; k++) SGL_SWAP(dst(k,indxr[lx-1]),
										 dst(k,indxc[lx-1]),temp);
    }

    return true;
}

void ofMatrix4x4::makeOrthoMatrix(double left, double right,
									  double bottom, double top,
									  double zNear, double zFar)
{
    // note transpose of ofMatrix4x4 wr.t OpenGL documentation, since the OSG use post multiplication rather than pre.
    double tx = -(right+left)/(right-left);
    double ty = -(top+bottom)/(top-bottom);
    double tz = -(zFar+zNear)/(zFar-zNear);
    SET_ROW(0, 2.0/(right-left),               0.0,               0.0, 0.0 )
    SET_ROW(1,              0.0,  2.0/(top-bottom),               0.0, 0.0 )
    SET_ROW(2,              0.0,               0.0,  -2.0/(zFar-zNear), 0.0 )
    SET_ROW(3,               tx,                ty,                 tz, 1.0 )
}

bool ofMatrix4x4::getOrtho(double& left, double& right,
									 double& bottom, double& top,
									 double& zNear, double& zFar) const
{
    if (_mat[0][3]!=0.0 || _mat[1][3]!=0.0 || _mat[2][3]!=0.0 || _mat[3][3]!=1.0) return false;

    zNear = (_mat[3][2]+1.0) / _mat[2][2];
    zFar = (_mat[3][2]-1.0) / _mat[2][2];

    left = -(1.0+_mat[3][0]) / _mat[0][0];
    right = (1.0-_mat[3][0]) / _mat[0][0];

    bottom = -(1.0+_mat[3][1]) / _mat[1][1];
    top = (1.0-_mat[3][1]) / _mat[1][1];

    return true;
}


void ofMatrix4x4::makeFrustumMatrix(double left, double right,
										double bottom, double top,
										double zNear, double zFar)
{
    // note transpose of ofMatrix4x4 wr.t OpenGL documentation, since the OSG use post multiplication rather than pre.
    double A = (right+left)/(right-left);
    double B = (top+bottom)/(top-bottom);
    double C = -(zFar+zNear)/(zFar-zNear);
    double D = -2.0*zFar*zNear/(zFar-zNear);
    SET_ROW(0, 2.0*zNear/(right-left),                    0.0, 0.0,  0.0 )
    SET_ROW(1,                    0.0, 2.0*zNear/(top-bottom), 0.0,  0.0 )
    SET_ROW(2,                      A,                      B,   C, -1.0 )
    SET_ROW(3,                    0.0,                    0.0,   D,  0.0 )
}

bool ofMatrix4x4::getFrustum(double& left, double& right,
                                       double& bottom, double& top,
                                       double& zNear, double& zFar) const
{
    if (_mat[0][3]!=0.0 || _mat[1][3]!=0.0 || _mat[2][3]!=-1.0 || _mat[3][3]!=0.0) return false;


    zNear = _mat[3][2] / (_mat[2][2]-1.0);
    zFar = _mat[3][2] / (1.0+_mat[2][2]);

    left = zNear * (_mat[2][0]-1.0) / _mat[0][0];
    right = zNear * (1.0+_mat[2][0]) / _mat[0][0];

    top = zNear * (1.0+_mat[2][1]) / _mat[1][1];
    bottom = zNear * (_mat[2][1]-1.0) / _mat[1][1];

    return true;
}


void ofMatrix4x4::makePerspectiveMatrix(double fovy,double aspectRatio,
                                            double zNear, double zFar)
{
    // calculate the appropriate left, right etc.
    double tan_fovy = tan(fovy*0.5*DEG_TO_RAD);
    double right  =  tan_fovy * aspectRatio * zNear;
    double left   = -right;
    double top    =  tan_fovy * zNear;
    double bottom =  -top;
    makeFrustumMatrix(left,right,bottom,top,zNear,zFar);
}

bool ofMatrix4x4::getPerspective(double& fovy,double& aspectRatio,
                                           double& zNear, double& zFar) const
{
    double right  =  0.0;
    double left   =  0.0;
    double top    =  0.0;
    double bottom =  0.0;
    if (getFrustum(left,right,bottom,top,zNear,zFar))
    {
        fovy = (atan(top/zNear)-atan(bottom/zNear))*RAD_TO_DEG;
        aspectRatio = (right-left)/(top-bottom);
        return true;
    }
    return false;
}

void ofMatrix4x4::makeLookAtViewMatrix(const ofVec3f& eye,const ofVec3f& center,const ofVec3f& up)
{
	ofVec3f zaxis = (eye - center).getNormalized();
	ofVec3f xaxis = up.getCrossed(zaxis).getNormalized();
	ofVec3f yaxis = zaxis.getCrossed(xaxis);

	_mat[0].set(xaxis.x, yaxis.x, zaxis.x, 0);
	_mat[1].set(xaxis.y, yaxis.y, zaxis.y, 0);
	_mat[2].set(xaxis.z, yaxis.z, zaxis.z, 0);
	_mat[3].set(-xaxis.dot(eye), -yaxis.dot(eye), -zaxis.dot(eye), 1);
}

void ofMatrix4x4::makeLookAtMatrix(const ofVec3f& eye,const ofVec3f& center,const ofVec3f& up)
{
	ofVec3f zaxis = (eye - center).getNormalized();
	ofVec3f xaxis = up.getCrossed(zaxis).getNormalized();
	ofVec3f yaxis = zaxis.getCrossed(xaxis);

	_mat[0].set(xaxis.x, xaxis.y, xaxis.z, 0);
	_mat[1].set(yaxis.x, yaxis.y, yaxis.z, 0);
	_mat[2].set(zaxis.x, zaxis.y, zaxis.z, 0);
	_mat[3].set(eye.x, eye.y, eye.z, 1);
}

void ofMatrix4x4::getLookAt(ofVec3f& eye,ofVec3f& center,ofVec3f& up,float lookDistance) const
{
    ofMatrix4x4 inv;
    inv.makeInvertOf(*this);
    eye = ofVec3f(0.0,0.0,0.0)*inv;
    up = transform3x3(*this,ofVec3f(0.0,1.0,0.0));
    center = transform3x3(*this,ofVec3f(0.0,0.0,-1));
    center.normalize();
    center = eye + center*lookDistance;
}



typedef ofQuaternion HVect;
typedef double _HMatrix[4][4];
typedef struct
{
	ofVec4f t;     // Translation Component;
	ofQuaternion q;           // Essential Rotation
	ofQuaternion u;          //Stretch rotation
	HVect k;          //Sign of determinant
	double f;          // Sign of determinant
} _affineParts;


enum QuatPart {X, Y, Z, W};

#define SQRTHALF (0.7071067811865475244)
static ofQuaternion qxtoz(0,SQRTHALF,0,SQRTHALF);
static ofQuaternion qytoz(SQRTHALF,0,0,SQRTHALF);
static ofQuaternion qppmm( 0.5, 0.5,-0.5,-0.5);
static ofQuaternion qpppp( 0.5, 0.5, 0.5, 0.5);
static ofQuaternion qmpmm(-0.5, 0.5,-0.5,-0.5);
static ofQuaternion qpppm( 0.5, 0.5, 0.5,-0.5);
static ofQuaternion q0001( 0.0, 0.0, 0.0, 1.0);
static ofQuaternion q1000( 1.0, 0.0, 0.0, 0.0);

/** Copy nxn matrix A to C using "gets" for assignment **/
#define matrixCopy(C, gets, A, n) {int i, j; for (i=0;i<n;i++) for (j=0;j<n;j++)\
    C[i][j] gets (A[i][j]);}

/** Copy transpose of nxn matrix A to C using "gets" for assignment **/
#define mat_tpose(AT,gets,A,n) {int i,j; for(i=0;i<n;i++) for(j=0;j<n;j++)\
    AT[i][j] gets (A[j][i]);}

/** Fill out 3x3 matrix to 4x4 **/
#define mat_pad(A) (A[W][X]=A[X][W]=A[W][Y]=A[Y][W]=A[W][Z]=A[Z][W]=0,A[W][W]=1)


/** Assign nxn matrix C the element-wise combination of A and B using "op" **/
#define matBinop(C,gets,A,op,B,n) {int i,j; for(i=0;i<n;i++) for(j=0;j<n;j++)\
    C[i][j] gets (A[i][j]) op (B[i][j]);}

/** Copy nxn matrix A to C using "gets" for assignment **/
#define mat_copy(C,gets,A,n) {int i,j; for(i=0;i<n;i++) for(j=0;j<n;j++)\
    C[i][j] gets (A[i][j]);}

/** Multiply the upper left 3x3 parts of A and B to get AB **/
void mat_mult(_HMatrix A, _HMatrix B, _HMatrix AB)
{
    int i, j;
    for (i=0; i<3; i++) for (j=0; j<3; j++)
        AB[i][j] = A[i][0]*B[0][j] + A[i][1]*B[1][j] + A[i][2]*B[2][j];
}

double mat_norm(_HMatrix M, int tpose)
{
    int i;
    double sum, max;
    max = 0.0;
    for (i=0; i<3; i++) {
        if (tpose) sum = fabs(M[0][i])+fabs(M[1][i])+fabs(M[2][i]);
        else       sum = fabs(M[i][0])+fabs(M[i][1])+fabs(M[i][2]);
        if (max<sum) max = sum;
    }
    return max;
}

double norm_inf(_HMatrix M) {return mat_norm(M, 0);}
double norm_one(_HMatrix M) {return mat_norm(M, 1);}

static _HMatrix mat_id = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

/** Return index of column of M containing maximum abs entry, or -1 if M=0 **/
int find_max_col(_HMatrix M)
{
    double abs, max;
    int i, j, col;
    max = 0.0; col = -1;
    for (i=0; i<3; i++) for (j=0; j<3; j++) {
        abs = M[i][j]; if (abs<0.0) abs = -abs;
        if (abs>max) {max = abs; col = j;}
    }
    return col;
}

void vcross(double *va, double *vb, double *v)
{
    v[0] = va[1]*vb[2] - va[2]*vb[1];
    v[1] = va[2]*vb[0] - va[0]*vb[2];
    v[2] = va[0]*vb[1] - va[1]*vb[0];
}

/** Return dot product of length 3 vectors va and vb **/
double vdot(double *va, double *vb)
{
    return (va[0]*vb[0] + va[1]*vb[1] + va[2]*vb[2]);
}


/** Set MadjT to transpose of inverse of M times determinant of M **/
void adjoint_transpose(_HMatrix M, _HMatrix MadjT)
{
    vcross(M[1], M[2], MadjT[0]);
    vcross(M[2], M[0], MadjT[1]);
    vcross(M[0], M[1], MadjT[2]);
}

/** Setup u for Household reflection to zero all v components but first **/
void make_reflector(double *v, double *u)
{
    double s = sqrt(vdot(v, v));
    u[0] = v[0]; u[1] = v[1];
    u[2] = v[2] + ((v[2]<0.0) ? -s : s);
    s = sqrt(2.0/vdot(u, u));
    u[0] = u[0]*s; u[1] = u[1]*s; u[2] = u[2]*s;
}

/** Apply Householder reflection represented by u to column vectors of M **/
void reflect_cols(_HMatrix M, double *u)
{
    int i, j;
    for (i=0; i<3; i++) {
        double s = u[0]*M[0][i] + u[1]*M[1][i] + u[2]*M[2][i];
        for (j=0; j<3; j++) M[j][i] -= u[j]*s;
    }
}

/** Apply Householder reflection represented by u to row vectors of M **/
void reflect_rows(_HMatrix M, double *u)
{
    int i, j;
    for (i=0; i<3; i++) {
        double s = vdot(u, M[i]);
        for (j=0; j<3; j++) M[i][j] -= u[j]*s;
    }
}

/** Find orthogonal factor Q of rank 1 (or less) M **/
void do_rank1(_HMatrix M, _HMatrix Q)
{
    double v1[3], v2[3], s;
    int col;
    mat_copy(Q,=,mat_id,4);
    /* If rank(M) is 1, we should find a non-zero column in M */
    col = find_max_col(M);
    if (col<0) return; /* Rank is 0 */
    v1[0] = M[0][col]; v1[1] = M[1][col]; v1[2] = M[2][col];
    make_reflector(v1, v1); reflect_cols(M, v1);
    v2[0] = M[2][0]; v2[1] = M[2][1]; v2[2] = M[2][2];
    make_reflector(v2, v2); reflect_rows(M, v2);
    s = M[2][2];
    if (s<0.0) Q[2][2] = -1.0;
    reflect_cols(Q, v1); reflect_rows(Q, v2);
}

/** Find orthogonal factor Q of rank 2 (or less) M using adjoint transpose **/
void do_rank2(_HMatrix M, _HMatrix MadjT, _HMatrix Q)
{
    double v1[3], v2[3];
    double w, x, y, z, c, s, d;
    int col;
    /* If rank(M) is 2, we should find a non-zero column in MadjT */
    col = find_max_col(MadjT);
    if (col<0) {do_rank1(M, Q); return;} /* Rank<2 */
    v1[0] = MadjT[0][col]; v1[1] = MadjT[1][col]; v1[2] = MadjT[2][col];
    make_reflector(v1, v1); reflect_cols(M, v1);
    vcross(M[0], M[1], v2);
    make_reflector(v2, v2); reflect_rows(M, v2);
    w = M[0][0]; x = M[0][1]; y = M[1][0]; z = M[1][1];
    if (w*z>x*y) {
        c = z+w; s = y-x; d = sqrt(c*c+s*s); c = c/d; s = s/d;
        Q[0][0] = Q[1][1] = c; Q[0][1] = -(Q[1][0] = s);
    } else {
        c = z-w; s = y+x; d = sqrt(c*c+s*s); c = c/d; s = s/d;
        Q[0][0] = -(Q[1][1] = c); Q[0][1] = Q[1][0] = s;
    }
    Q[0][2] = Q[2][0] = Q[1][2] = Q[2][1] = 0.0; Q[2][2] = 1.0;
    reflect_cols(Q, v1); reflect_rows(Q, v2);
}

/* Return product of quaternion q by scalar w. */
ofQuaternion Qt_Scale(ofQuaternion q, double w)
{
	ofQuaternion qq;
	qq.w() = q.w()*w;
	qq.x() = q.x()*w;
	qq.y() = q.y()*w;
	qq.z() = q.z()*w;
	return (qq);
}

/* Construct a unit quaternion from rotation matrix.  Assumes matrix is
* used to multiply column vector on the left: vnew = mat vold.  Works
* correctly for right-handed coordinate system and right-handed rotations.
* Translation and perspective components ignored. */

ofQuaternion quatFromMatrix(_HMatrix mat)
{
   /* This algorithm avoids near-zero divides by looking for a large component
	* - first w, then x, y, or z.  When the trace is greater than zero,
	* |w| is greater than 1/2, which is as small as a largest component can be.
	* Otherwise, the largest diagonal entry corresponds to the largest of |x|,
	* |y|, or |z|, one of which must be larger than |w|, and at least 1/2. */
   ofQuaternion qu = q0001;
   double tr, s;

   tr = mat[X][X] + mat[Y][Y]+ mat[Z][Z];
   if (tr >= 0.0)
   {
	   s = sqrt(tr + mat[W][W]);
	   qu.w() = s*0.5;
	   s = 0.5 / s;
	   qu.x() = (mat[Z][Y] - mat[Y][Z]) * s;
	   qu.y() = (mat[X][Z] - mat[Z][X]) * s;
	   qu.z() = (mat[Y][X] - mat[X][Y]) * s;
   }
   else
   {
	   int h = X;
	   if (mat[Y][Y] > mat[X][X]) h = Y;
	   if (mat[Z][Z] > mat[h][h]) h = Z;
	   switch (h) {
#define caseMacro(i,j,k,I,J,K) \
		   case I:\
				  s = sqrt( (mat[I][I] - (mat[J][J]+mat[K][K])) + mat[W][W] );\
		   qu.i() = s*0.5;\
		   s = 0.5 / s;\
		   qu.j() = (mat[I][J] + mat[J][I]) * s;\
		   qu.k() = (mat[K][I] + mat[I][K]) * s;\
		   qu.w() = (mat[K][J] - mat[J][K]) * s;\
		   break
		   caseMacro(x,y,z,X,Y,Z);
		   caseMacro(y,z,x,Y,Z,X);
		   caseMacro(z,x,y,Z,X,Y);
	   }
   }
   if (mat[W][W] != 1.0) qu = Qt_Scale(qu, 1/sqrt(mat[W][W]));
   return (qu);
}


/******* Polar Decomposition *******/
/* Polar Decomposition of 3x3 matrix in 4x4,
 * M = QS.  See Nicholas Higham and Robert S. Schreiber,
 * Fast Polar Decomposition of An Arbitrary Matrix,
 * Technical Report 88-942, October 1988,
 * Department of Computer Science, Cornell University.
 */

double polarDecomp( _HMatrix M, _HMatrix Q, _HMatrix S)
{

#define TOL 1.0e-6
	_HMatrix Mk, MadjTk, Ek;
	double det, M_one, M_inf, MadjT_one, MadjT_inf, E_one, gamma, g1, g2;
	int i, j;

	mat_tpose(Mk,=,M,3);
	M_one = norm_one(Mk);  M_inf = norm_inf(Mk);

	do
	{
		adjoint_transpose(Mk, MadjTk);
		det = vdot(Mk[0], MadjTk[0]);
		if (det==0.0)
		{
			do_rank2(Mk, MadjTk, Mk);
			break;
		}

		MadjT_one = norm_one(MadjTk);
		MadjT_inf = norm_inf(MadjTk);

		gamma = sqrt(sqrt((MadjT_one*MadjT_inf)/(M_one*M_inf))/fabs(det));
		g1 = gamma*0.5;
		g2 = 0.5/(gamma*det);
		matrixCopy(Ek,=,Mk,3);
		matBinop(Mk,=,g1*Mk,+,g2*MadjTk,3);
		mat_copy(Ek,-=,Mk,3);
		E_one = norm_one(Ek);
		M_one = norm_one(Mk);
		M_inf = norm_inf(Mk);

	} while(E_one>(M_one*TOL));

	mat_tpose(Q,=,Mk,3); mat_pad(Q);
	mat_mult(Mk, M, S);  mat_pad(S);

	for (i=0; i<3; i++)
		for (j=i; j<3; j++)
			S[i][j] = S[j][i] = 0.5*(S[i][j]+S[j][i]);
	return (det);
}

/******* Spectral Decomposition *******/
/* Compute the spectral decomposition of symmetric positive semi-definite S.
* Returns rotation in U and scale factors in result, so that if K is a diagonal
* matrix of the scale factors, then S = U K (U transpose). Uses Jacobi method.
* See Gene H. Golub and Charles F. Van Loan. Matrix Computations. Hopkins 1983.
*/
HVect spectDecomp(_HMatrix S, _HMatrix U)
{
   HVect kv;
   double Diag[3],OffD[3]; /* OffD is off-diag (by omitted index) */
   double g,h,fabsh,fabsOffDi,t,theta,c,s,tau,ta,OffDq,a,b;
   static char nxt[] = {Y,Z,X};
   int sweep, i, j;
   mat_copy(U,=,mat_id,4);
   Diag[X] = S[X][X]; Diag[Y] = S[Y][Y]; Diag[Z] = S[Z][Z];
   OffD[X] = S[Y][Z]; OffD[Y] = S[Z][X]; OffD[Z] = S[X][Y];
   for (sweep=20; sweep>0; sweep--) {
	   double sm = fabs(OffD[X])+fabs(OffD[Y])+fabs(OffD[Z]);
	   if (sm==0.0) break;
	   for (i=Z; i>=X; i--) {
		   int p = nxt[i]; int q = nxt[p];
		   fabsOffDi = fabs(OffD[i]);
		   g = 100.0*fabsOffDi;
		   if (fabsOffDi>0.0) {
			   h = Diag[q] - Diag[p];
			   fabsh = fabs(h);
			   if (fabsh+g==fabsh) {
				   t = OffD[i]/h;
			   } else {
				   theta = 0.5*h/OffD[i];
				   t = 1.0/(fabs(theta)+sqrt(theta*theta+1.0));
				   if (theta<0.0) t = -t;
			   }
			   c = 1.0/sqrt(t*t+1.0); s = t*c;
			   tau = s/(c+1.0);
			   ta = t*OffD[i]; OffD[i] = 0.0;
			   Diag[p] -= ta; Diag[q] += ta;
			   OffDq = OffD[q];
			   OffD[q] -= s*(OffD[p] + tau*OffD[q]);
			   OffD[p] += s*(OffDq   - tau*OffD[p]);
			   for (j=Z; j>=X; j--) {
				   a = U[j][p]; b = U[j][q];
				   U[j][p] -= s*(b + tau*a);
				   U[j][q] += s*(a - tau*b);
			   }
		   }
	   }
   }
   kv.x() = Diag[X]; kv.y() = Diag[Y]; kv.z() = Diag[Z]; kv.w() = 1.0;
   return (kv);
}

/* Return conjugate of quaternion. */
ofQuaternion Qt_Conj(ofQuaternion q)
{
	ofQuaternion qq;
    qq.x() = -q.x(); qq.y() = -q.y(); qq.z() = -q.z(); qq.w() = q.w();
    return (qq);
}

/* Return quaternion product qL * qR.  Note: order is important!
 * To combine rotations, use the product Mul(qSecond, qFirst),
 * which gives the effect of rotating by qFirst then qSecond. */
ofQuaternion Qt_Mul(ofQuaternion qL, ofQuaternion qR)
{
	ofQuaternion qq;
    qq.w() = qL.w()*qR.w() - qL.x()*qR.x() - qL.y()*qR.y() - qL.z()*qR.z();
    qq.x() = qL.w()*qR.x() + qL.x()*qR.w() + qL.y()*qR.z() - qL.z()*qR.y();
    qq.y() = qL.w()*qR.y() + qL.y()*qR.w() + qL.z()*qR.x() - qL.x()*qR.z();
    qq.z() = qL.w()*qR.z() + qL.z()*qR.w() + qL.x()*qR.y() - qL.y()*qR.x();
    return (qq);
}

/* Construct a (possibly non-unit) quaternion from real components. */
ofQuaternion Qt_(double x, double y, double z, double w)
{
	ofQuaternion qq;
    qq.x() = x; qq.y() = y; qq.z() = z; qq.w() = w;
    return (qq);
}

/******* Spectral Axis Adjustment *******/

/* Given a unit quaternion, q, and a scale vector, k, find a unit quaternion, p,
 * which permutes the axes and turns freely in the plane of duplicate scale
 * factors, such that q p has the largest possible w component, i.e. the
 * smallest possible angle. Permutes k's components to go with q p instead of q.
 * See Ken Shoemake and Tom Duff. Matrix Animation and Polar Decomposition.
 * Proceedings of Graphics Interface 1992. Details on p. 262-263.
 */
ofQuaternion snuggle(ofQuaternion q, HVect *k)
{
#define sgn(n,v)    ((n)?-(v):(v))
#define swap(a,i,j) {a[3]=a[i]; a[i]=a[j]; a[j]=a[3];}
#define cycle(a,p)  if (p) {a[3]=a[0]; a[0]=a[1]; a[1]=a[2]; a[2]=a[3];}\
	else   {a[3]=a[2]; a[2]=a[1]; a[1]=a[0]; a[0]=a[3];}

	ofQuaternion p = q0001;
	double ka[4];
	int i, turn = -1;
	ka[X] = k->x(); ka[Y] = k->y(); ka[Z] = k->z();

	if (ka[X]==ka[Y]) {
		if (ka[X]==ka[Z])
			turn = W;
		else turn = Z;
	}
	else {
		if (ka[X]==ka[Z])
			turn = Y;
		else if (ka[Y]==ka[Z])
			turn = X;
	}
	if (turn>=0) {
		ofQuaternion qtoz, qp;
		unsigned int  win;
		double mag[3], t;
		switch (turn) {
			default: return (Qt_Conj(q));
			case X: q = Qt_Mul(q, qtoz = qxtoz); swap(ka,X,Z) break;
			case Y: q = Qt_Mul(q, qtoz = qytoz); swap(ka,Y,Z) break;
			case Z: qtoz = q0001; break;
		}
		q = Qt_Conj(q);
		mag[0] = (double)q.z()*q.z()+(double)q.w()*q.w()-0.5;
		mag[1] = (double)q.x()*q.z()-(double)q.y()*q.w();
		mag[2] = (double)q.y()*q.z()+(double)q.x()*q.w();

		bool neg[3];
		for (i=0; i<3; i++)
		{
			neg[i] = (mag[i]<0.0);
			if (neg[i]) mag[i] = -mag[i];
		}

		if (mag[0]>mag[1]) {
			if (mag[0]>mag[2])
				win = 0;
			else win = 2;
		}
		else {
			if (mag[1]>mag[2]) win = 1;
			else win = 2;
		}

		switch (win) {
			case 0: if (neg[0]) p = q1000; else p = q0001; break;
			case 1: if (neg[1]) p = qppmm; else p = qpppp; cycle(ka,0) break;
			case 2: if (neg[2]) p = qmpmm; else p = qpppm; cycle(ka,1) break;
		}

		qp = Qt_Mul(q, p);
		t = sqrt(mag[win]+0.5);
		p = Qt_Mul(p, Qt_(0.0,0.0,-qp.z()/t,qp.w()/t));
		p = Qt_Mul(qtoz, Qt_Conj(p));
	}
	else {
		double qa[4], pa[4];
		unsigned int lo, hi;
		bool par = false;
		bool neg[4];
		double all, big, two;
		qa[0] = q.x(); qa[1] = q.y(); qa[2] = q.z(); qa[3] = q.w();
		for (i=0; i<4; i++) {
			pa[i] = 0.0;
			neg[i] = (qa[i]<0.0);
			if (neg[i]) qa[i] = -qa[i];
			par ^= neg[i];
		}

		/* Find two largest components, indices in hi and lo */
		if (qa[0]>qa[1]) lo = 0;
		else lo = 1;

		if (qa[2]>qa[3]) hi = 2;
		else hi = 3;

		if (qa[lo]>qa[hi]) {
			if (qa[lo^1]>qa[hi]) {
				hi = lo; lo ^= 1;
			}
			else {
				hi ^= lo; lo ^= hi; hi ^= lo;
			}
		}
		else {
			if (qa[hi^1]>qa[lo]) lo = hi^1;
		}

		all = (qa[0]+qa[1]+qa[2]+qa[3])*0.5;
		two = (qa[hi]+qa[lo])*SQRTHALF;
		big = qa[hi];
		if (all>two) {
			if (all>big) {/*all*/
				{int i; for (i=0; i<4; i++) pa[i] = sgn(neg[i], 0.5);}
				cycle(ka,par);
			}
			else {/*big*/ pa[hi] = sgn(neg[hi],1.0);}
		} else {
			if (two>big) { /*two*/
				pa[hi] = sgn(neg[hi],SQRTHALF);
				pa[lo] = sgn(neg[lo], SQRTHALF);
				if (lo>hi) {
					hi ^= lo; lo ^= hi; hi ^= lo;
				}
				if (hi==W) {
					hi = "\001\002\000"[lo];
					lo = 3-hi-lo;
				}
				swap(ka,hi,lo);
			}
			else {/*big*/
				pa[hi] = sgn(neg[hi],1.0);
			}
		}
		p.x() = -pa[0]; p.y() = -pa[1]; p.z() = -pa[2]; p.w() = pa[3];
	}
	k->x() = ka[X]; k->y() = ka[Y]; k->z() = ka[Z];
	return (p);
}

/******* Decompose Affine Matrix *******/

/* Decompose 4x4 affine matrix A as TFRUK(U transpose), where t contains the
 * translation components, q contains the rotation R, u contains U, k contains
 * scale factors, and f contains the sign of the determinant.
 * Assumes A transforms column vectors in right-handed coordinates.
 * See Ken Shoemake and Tom Duff. Matrix Animation and Polar Decomposition.
 * Proceedings of Graphics Interface 1992.
 */
void decompAffine(_HMatrix A, _affineParts * parts)
{
	_HMatrix Q, S, U;
	ofQuaternion p;

	//Translation component.
	parts->t = ofVec4f(A[X][W], A[Y][W], A[Z][W], 0);
	double det = polarDecomp(A, Q, S);
	if (det<0.0)
	{
		matrixCopy(Q, =, -Q, 3);
		parts->f = -1;
	}
	else
		parts->f = 1;

	parts->q = quatFromMatrix(Q);
	parts->k = spectDecomp(S, U);
	parts->u = quatFromMatrix(U);
	p = snuggle(parts->u, &parts->k);
	parts->u = Qt_Mul(parts->u, p);
}

void ofMatrix4x4::decompose( ofVec3f& t,
		   ofQuaternion& r,
		   ofVec3f& s,
		   ofQuaternion& so ) const{

	_affineParts parts;
    _HMatrix hmatrix;

    // Transpose copy of LTW
    for ( int i =0; i<4; i++)
    {
        for ( int j=0; j<4; j++)
        {
            hmatrix[i][j] = (*this)(j,i);
        }
    }

    decompAffine(hmatrix, &parts);

    double mul = 1.0;
    if (parts.t[W] != 0.0)
        mul = 1.0 / parts.t[W];

    t.x = parts.t[X] * mul;
    t.y = parts.t[Y] * mul;
    t.z = parts.t[Z] * mul;

    r.set(parts.q.x(), parts.q.y(), parts.q.z(), parts.q.w());

    mul = 1.0;
    if (parts.k.w() != 0.0)
        mul = 1.0 / parts.k.w();

    // mul be sign of determinant to support negative scales.
    mul *= parts.f;
    s.x= parts.k.x() * mul;
    s.y = parts.k.y() * mul;
    s.z = parts.k.z() * mul;

    so.set(parts.u.x(), parts.u.y(), parts.u.z(), parts.u.w());
}

#undef SET_ROW

std::ostream& operator<<(std::ostream& os, const ofMatrix4x4& M) {
	int w = 8;
	os	<< std::setw(w)
		<< M._mat[0][0] << ", " << std::setw(w)
		<< M._mat[0][1] << ", " << std::setw(w)
		<< M._mat[0][2] << ", " << std::setw(w)
		<< M._mat[0][3] << std::endl;

	os	<< std::setw(w)
		<< M._mat[1][0] << ", " << std::setw(w)
		<< M._mat[1][1] << ", " << std::setw(w)
		<< M._mat[1][2] << ", " << std::setw(w)
		<< M._mat[1][3] << std::endl;

	os	<< std::setw(w)
		<< M._mat[2][0] << ", " << std::setw(w)
		<< M._mat[2][1] << ", " << std::setw(w)
		<< M._mat[2][2] << ", " << std::setw(w)
		<< M._mat[2][3] << std::endl;

	os	<< std::setw(w)
		<< M._mat[3][0] << ", " << std::setw(w)
		<< M._mat[3][1] << ", " << std::setw(w)
		<< M._mat[3][2] << ", " << std::setw(w)
		<< M._mat[3][3];

	return os;
}

std::istream& operator>>(std::istream& is, ofMatrix4x4& M) {
	is >> M._mat[0][0]; is.ignore(2);
	is >> M._mat[0][1]; is.ignore(2);
	is >> M._mat[0][2]; is.ignore(2);
	is >> M._mat[0][3]; is.ignore(1);

	is >> M._mat[1][0]; is.ignore(2);
	is >> M._mat[1][1]; is.ignore(2);
	is >> M._mat[1][2]; is.ignore(2);
	is >> M._mat[1][3]; is.ignore(1);

	is >> M._mat[2][0]; is.ignore(2);
	is >> M._mat[2][1]; is.ignore(2);
	is >> M._mat[2][2]; is.ignore(2);
	is >> M._mat[2][3]; is.ignore(1);

	is >> M._mat[3][0]; is.ignore(2);
	is >> M._mat[3][1]; is.ignore(2);
	is >> M._mat[3][2]; is.ignore(2);
	is >> M._mat[3][3];
	return is;
}
