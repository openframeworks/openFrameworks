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
// Copyright( C) 2000, Intel Corporation, all rights reserved.
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
//(including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort(including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

/*
   A few macros and definitions for backward compatibility
   with the previous versions of OpenCV. They are obsolete and
   are likely to be removed in future. To check whether your code
   uses any of these, define CV_NO_BACKWARD_COMPATIBILITY before
   including cv.h.
*/

#ifndef _CVCOMPAT_H_
#define _CVCOMPAT_H_

#include <string.h>

#ifdef __cplusplus
    #define CV_UNREFERENCED(arg)
#else
    #define CV_UNREFERENCED(arg) arg
#endif

#define CvMatType int
#define CvDisMaskType int
#define CvMatArray CvMat

#define CvThreshType int
#define CvAdaptiveThreshMethod int
#define CvCompareMethod int
#define CvFontFace int
#define CvPolyApproxMethod int
#define CvContoursMatchMethod int
#define CvContourTreesMatchMethod int
#define CvCoeffType int
#define CvRodriguesType int
#define CvElementShape int
#define CvMorphOp int
#define CvTemplMatchMethod int

#define CvPoint2D64d CvPoint2D64f
#define CvPoint3D64d CvPoint3D64f

#define  CV_MAT32F      CV_32FC1
#define  CV_MAT3x1_32F  CV_32FC1
#define  CV_MAT4x1_32F  CV_32FC1
#define  CV_MAT3x3_32F  CV_32FC1
#define  CV_MAT4x4_32F  CV_32FC1

#define  CV_MAT64D      CV_64FC1
#define  CV_MAT3x1_64D  CV_64FC1
#define  CV_MAT4x1_64D  CV_64FC1
#define  CV_MAT3x3_64D  CV_64FC1
#define  CV_MAT4x4_64D  CV_64FC1

#define  IPL_GAUSSIAN_5x5   7
#define  CvBox2D32f     CvBox2D

/* allocation/deallocation macros */
#define cvCreateImageData   cvCreateData
#define cvReleaseImageData  cvReleaseData
#define cvSetImageData      cvSetData
#define cvGetImageRawData   cvGetRawData

#define cvmAlloc            cvCreateData
#define cvmFree             cvReleaseData
#define cvmAllocArray       cvCreateData
#define cvmFreeArray        cvReleaseData

#define cvIntegralImage     cvIntegral
#define cvMatchContours     cvMatchShapes

CV_INLINE CvMat cvMatArray( int rows, int cols, int type,
                            int count, void* data CV_DEFAULT(0))
{
    return cvMat( rows*count, cols, type, data );    
}

#define cvUpdateMHIByTime  cvUpdateMotionHistory

#define cvAccMask cvAcc
#define cvSquareAccMask cvSquareAcc
#define cvMultiplyAccMask cvMultiplyAcc
#define cvRunningAvgMask(imgY, imgU, mask, alpha) cvRunningAvg(imgY, imgU, alpha, mask)

#define cvSetHistThresh  cvSetHistBinRanges
#define cvCalcHistMask(img, mask, hist, doNotClear) cvCalcHist(img, hist, doNotClear, mask)

CV_INLINE double cvMean( const CvArr* image, const CvArr* mask CV_DEFAULT(0))
{
    CvScalar mean = cvAvg( image, mask );
    return mean.val[0];
}


CV_INLINE double  cvSumPixels( const CvArr* image )
{
    CvScalar scalar = cvSum( image );
    return scalar.val[0];
}

CV_INLINE void  cvMean_StdDev( const CvArr* image, double* mean, double* sdv,
                               const CvArr* mask CV_DEFAULT(0))
{
    CvScalar _mean, _sdv;
    cvAvgSdv( image, &_mean, &_sdv, mask );

    if( mean )
        *mean = _mean.val[0];

    if( sdv )
        *sdv = _sdv.val[0];
}


CV_INLINE void cvmPerspectiveProject( const CvMat* mat, const CvArr* src, CvArr* dst )
{
    CvMat tsrc, tdst;

    cvReshape( src, &tsrc, 3, 0 );
    cvReshape( dst, &tdst, 3, 0 );

    cvPerspectiveTransform( &tsrc, &tdst, mat );
}


CV_INLINE void cvFillImage( CvArr* mat, double color )
{
    cvSet( mat, cvColorToScalar(color, cvGetElemType(mat)), 0 );
}


#define cvCvtPixToPlane cvSplit
#define cvCvtPlaneToPix cvMerge

typedef struct CvRandState
{
    CvRNG     state;    /* RNG state (the current seed and carry)*/
    int       disttype; /* distribution type */
    CvScalar  param[2]; /* parameters of RNG */
}
CvRandState;


/* Changes RNG range while preserving RNG state */
CV_INLINE  void  cvRandSetRange( CvRandState* state, double param1,
                                 double param2, int index CV_DEFAULT(-1))
{
    if( !state )
    {
        cvError( CV_StsNullPtr, "cvRandSetRange", "Null pointer to RNG state", "cvcompat.h", 0 );
        return;
    }

    if( (unsigned)(index + 1) > 4 )
    {
        cvError( CV_StsOutOfRange, "cvRandSetRange", "index is not in -1..3", "cvcompat.h", 0 );
        return;
    }

    if( index < 0 )
    {
        state->param[0].val[0] = state->param[0].val[1] =
        state->param[0].val[2] = state->param[0].val[3] = param1;
        state->param[1].val[0] = state->param[1].val[1] = 
        state->param[1].val[2] = state->param[1].val[3] = param2;
    }
    else
    {
        state->param[0].val[index] = param1;
        state->param[1].val[index] = param2;
    }
}


CV_INLINE  void  cvRandInit( CvRandState* state, double param1,
                             double param2, int seed,
                             int disttype CV_DEFAULT(CV_RAND_UNI))
{
    if( !state )
    {
        cvError( CV_StsNullPtr, "cvRandInit", "Null pointer to RNG state", "cvcompat.h", 0 );
        return;
    }

    if( disttype != CV_RAND_UNI && disttype != CV_RAND_NORMAL )
    {
        cvError( CV_StsBadFlag, "cvRandInit", "Unknown distribution type", "cvcompat.h", 0 );
        return;
    }

    state->state = (uint64)(seed ? seed : -1);
    state->disttype = disttype;
    cvRandSetRange( state, param1, param2, -1 );
}


/* Fills array with random numbers */
CV_INLINE void cvRand( CvRandState* state, CvArr* arr )
{
    if( !state )
    {
        cvError( CV_StsNullPtr, "cvRand", "Null pointer to RNG state", "cvcompat.h", 0 );
        return;
    }
    cvRandArr( &state->state, arr, state->disttype, state->param[0], state->param[1] ); 
}

#define cvRandNext( _state ) cvRandInt( &(_state)->state )

CV_INLINE void cvbRand( CvRandState* state, float* dst, int len )
{
    CvMat mat = cvMat( 1, len, CV_32F, (void*)dst );
    cvRand( state, &mat );
}


CV_INLINE void  cvbCartToPolar( const float* y, const float* x,
                                float* magnitude, float* angle, int len )
{
    CvMat mx = cvMat( 1, len, CV_32F, (void*)x );
    CvMat my = mx;
    CvMat mm = mx;
    CvMat ma = mx;

    my.data.fl = (float*)y;
    mm.data.fl = (float*)magnitude;
    ma.data.fl = (float*)angle;

    cvCartToPolar( &mx, &my, &mm, angle ? &ma : NULL, 1 );
}


CV_INLINE void  cvbFastArctan( const float* y, const float* x,
                               float* angle, int len )
{
    CvMat mx = cvMat( 1, len, CV_32F, (void*)x );
    CvMat my = mx;
    CvMat ma = mx;

    my.data.fl = (float*)y;
    ma.data.fl = (float*)angle;

    cvCartToPolar( &mx, &my, NULL, &ma, 1 );
}


CV_INLINE  void  cvbSqrt( const float* x, float* y, int len )
{
    CvMat mx = cvMat( 1, len, CV_32F, (void*)x );
    CvMat my = mx;
    my.data.fl = (float*)y;

    cvPow( &mx, &my, 0.5 );
}


CV_INLINE  void  cvbInvSqrt( const float* x, float* y, int len )
{
    CvMat mx = cvMat( 1, len, CV_32F, (void*)x );
    CvMat my = mx;
    my.data.fl = (float*)y;

    cvPow( &mx, &my, -0.5 );
}


CV_INLINE  void  cvbReciprocal( const float* x, float* y, int len )
{
    CvMat mx = cvMat( 1, len, CV_32F, (void*)x );
    CvMat my = mx;
    my.data.fl = (float*)y;

    cvPow( &mx, &my, -1 );
}


CV_INLINE  void  cvbFastExp( const float* x, double* y, int len )
{
    CvMat mx = cvMat( 1, len, CV_32F, (void*)x );
    CvMat my = cvMat( 1, len, CV_64F, y );
    cvExp( &mx, &my );
}


CV_INLINE  void  cvbFastLog( const double* x, float* y, int len )
{
    CvMat mx = cvMat( 1, len, CV_64F, (void*)x );
    CvMat my = cvMat( 1, len, CV_32F, y );
    cvLog( &mx, &my );
}


CV_INLINE  CvRect  cvContourBoundingRect( void* point_set, int update CV_DEFAULT(0))
{
    return cvBoundingRect( point_set, update );
}


CV_INLINE double cvPseudoInverse( const CvArr* src, CvArr* dst )
{
    return cvInvert( src, dst, CV_SVD );
}

#define cvPseudoInv cvPseudoInverse

#define cvContourMoments( contour, moments ) \
    cvMoments( contour, moments, 0 )

#define cvGetPtrAt              cvPtr2D
#define cvGetAt                 cvGet2D
#define cvSetAt(arr,val,y,x)    cvSet2D((arr),(y),(x),(val))

#define cvMeanMask  cvMean
#define cvMean_StdDevMask(img,mask,mean,sdv) cvMean_StdDev(img,mean,sdv,mask)

#define cvNormMask(imgA,imgB,mask,normType) cvNorm(imgA,imgB,normType,mask)

#define cvMinMaxLocMask(img, mask, min_val, max_val, min_loc, max_loc) \
        cvMinMaxLoc(img, min_val, max_val, min_loc, max_loc, mask)

#define cvRemoveMemoryManager  cvSetMemoryManager

#define cvmSetZero( mat )               cvSetZero( mat )
#define cvmSetIdentity( mat )           cvSetIdentity( mat )
#define cvmAdd( src1, src2, dst )       cvAdd( src1, src2, dst, 0 )
#define cvmSub( src1, src2, dst )       cvSub( src1, src2, dst, 0 )
#define cvmCopy( src, dst )             cvCopy( src, dst, 0 )
#define cvmMul( src1, src2, dst )       cvMatMulAdd( src1, src2, 0, dst )
#define cvmTranspose( src, dst )        cvT( src, dst )
#define cvmInvert( src, dst )           cvInv( src, dst )
#define cvmMahalanobis(vec1, vec2, mat) cvMahalanobis( vec1, vec2, mat )
#define cvmDotProduct( vec1, vec2 )     cvDotProduct( vec1, vec2 )
#define cvmCrossProduct(vec1, vec2,dst) cvCrossProduct( vec1, vec2, dst )
#define cvmTrace( mat )                 (cvTrace( mat )).val[0]
#define cvmMulTransposed( src, dst, order ) cvMulTransposed( src, dst, order )
#define cvmEigenVV( mat, evec, eval, eps)   cvEigenVV( mat, evec, eval, eps )
#define cvmDet( mat )                   cvDet( mat )
#define cvmScale( src, dst, scale )     cvScale( src, dst, scale )

#define cvCopyImage( src, dst )         cvCopy( src, dst, 0 )
#define cvReleaseMatHeader              cvReleaseMat

/* Calculates exact convex hull of 2d point set */
CV_INLINE void cvConvexHull( CvPoint* points, int num_points,
                             CvRect* CV_UNREFERENCED(bound_rect),
                             int orientation, int* hull, int* hullsize )
{
    CvMat points1 = cvMat( 1, num_points, CV_32SC2, points );
    CvMat hull1 = cvMat( 1, num_points, CV_32SC1, hull );
    
    cvConvexHull2( &points1, &hull1, orientation, 0 );
    *hullsize = hull1.cols;
}

/* Calculates exact convex hull of 2d point set stored in a sequence */
#define cvContourConvexHull( contour, orientation, storage ) \
    cvConvexHull2( contour, storage, orientation )

/* Calculates approximate convex hull of 2d point set */
#define cvConvexHullApprox( points, num_points, bound_rect, bandwidth,      \
                            orientation, hull, hullsize )                   \
cvConvexHull( points, num_points, bound_rect, orientation, hull, hullsize )

/* Calculates approximate convex hull of 2d point set stored in a sequence */
#define cvContourConvexHullApprox( contour, bandwidth, orientation, storage )   \
    cvConvexHull2( contour, storage, orientation )


CV_INLINE void cvMinAreaRect( CvPoint* points, int n,
                              int CV_UNREFERENCED(left), int CV_UNREFERENCED(bottom),
                              int CV_UNREFERENCED(right), int CV_UNREFERENCED(top),
                              CvPoint2D32f* anchor,
                              CvPoint2D32f* vect1,
                              CvPoint2D32f* vect2 )
{
    CvMat mat = cvMat( 1, n, CV_32SC2, points );
    CvBox2D box = cvMinAreaRect2( &mat, 0 );
    CvPoint2D32f pt[4];

    cvBoxPoints( box, pt );
    *anchor = pt[0];
    vect1->x = pt[1].x - pt[0].x;
    vect1->y = pt[1].y - pt[0].y;
    vect2->x = pt[3].x - pt[0].x;
    vect2->y = pt[3].y - pt[0].y;

    CV_UNREFERENCED( (left, bottom, right, top) );
}

typedef int CvDisType;
typedef int CvChainApproxMethod;
typedef int CvContourRetrievalMode;

CV_INLINE  void  cvFitLine3D( CvPoint3D32f* points, int count, int dist,
                              void *param, float reps, float aeps, float* line )
{
    CvMat mat = cvMat( 1, count, CV_32FC3, points );
    float _param = param != NULL ? *(float*)param : 0.f;
    assert( dist != CV_DIST_USER );
    cvFitLine( &mat, dist, _param, reps, aeps, line );
}

/* Fits a line into set of 2d points in a robust way (M-estimator technique) */ 
CV_INLINE  void  cvFitLine2D( CvPoint2D32f* points, int count, int dist,
                              void *param, float reps, float aeps, float* line )
{
    CvMat mat = cvMat( 1, count, CV_32FC2, points );
    float _param = param != NULL ? *(float*)param : 0.f;
    assert( dist != CV_DIST_USER );
    cvFitLine( &mat, dist, _param, reps, aeps, line );
}


CV_INLINE  void cvFitEllipse( const CvPoint2D32f* points, int count, CvBox2D* box )
{
    CvMat mat = cvMat( 1, count, CV_32FC2, (void*)points );
    *box = cvFitEllipse2( &mat );
}

/* Projects 2d points to one of standard coordinate planes
   (i.e. removes one of coordinates) */
CV_INLINE  void  cvProject3D( CvPoint3D32f* points3D, int count,
                              CvPoint2D32f* points2D,
                              int xIndx CV_DEFAULT(0),
                              int yIndx CV_DEFAULT(1))
{
    CvMat src = cvMat( 1, count, CV_32FC3, points3D );
    CvMat dst = cvMat( 1, count, CV_32FC2, points2D );
    float m[6] = {0,0,0,0,0,0};
    CvMat M = cvMat( 2, 3, CV_32F, m );
    
    assert( (unsigned)xIndx < 3 && (unsigned)yIndx < 3 );
    m[xIndx] = m[yIndx+3] = 1.f;

    cvTransform( &src, &dst, &M, NULL );
}


/* Retrieves value of the particular bin
   of x-dimensional (x=1,2,3,...) histogram */
#define cvQueryHistValue_1D( hist, idx0 ) \
    ((float)cvGetReal1D( (hist)->bins, (idx0)))
#define cvQueryHistValue_2D( hist, idx0, idx1 ) \
    ((float)cvGetReal2D( (hist)->bins, (idx0), (idx1)))
#define cvQueryHistValue_3D( hist, idx0, idx1, idx2 ) \
    ((float)cvGetReal3D( (hist)->bins, (idx0), (idx1), (idx2)))
#define cvQueryHistValue_nD( hist, idx ) \
    ((float)cvGetRealND( (hist)->bins, (idx)))

/* Returns pointer to the particular bin of x-dimesional histogram.
   For sparse histogram the bin is created if it didn't exist before */
#define cvGetHistValue_1D( hist, idx0 ) \
    ((float*)cvPtr1D( (hist)->bins, (idx0), 0))
#define cvGetHistValue_2D( hist, idx0, idx1 ) \
    ((float*)cvPtr2D( (hist)->bins, (idx0), (idx1), 0))
#define cvGetHistValue_3D( hist, idx0, idx1, idx2 ) \
    ((float*)cvPtr3D( (hist)->bins, (idx0), (idx1), (idx2), 0))
#define cvGetHistValue_nD( hist, idx ) \
    ((float*)cvPtrND( (hist)->bins, (idx), 0))


#define CV_IS_SET_ELEM_EXISTS CV_IS_SET_ELEM


CV_INLINE  int  cvHoughLines( CvArr* image, double rho,
                              double theta, int threshold,
                              float* lines, int linesNumber )
{
    CvMat linesMat = cvMat( 1, linesNumber, CV_32FC2, lines ); 
    cvHoughLines2( image, &linesMat, CV_HOUGH_STANDARD,
                   rho, theta, threshold, 0, 0 );

    return linesMat.cols;
}


CV_INLINE  int  cvHoughLinesP( CvArr* image, double rho,
                               double theta, int threshold,
                               int lineLength, int lineGap,
                               int* lines, int linesNumber )
{
    CvMat linesMat = cvMat( 1, linesNumber, CV_32SC4, lines ); 
    cvHoughLines2( image, &linesMat, CV_HOUGH_PROBABILISTIC,
                   rho, theta, threshold, lineLength, lineGap );

    return linesMat.cols;
}


CV_INLINE  int  cvHoughLinesSDiv( CvArr* image, double rho, int srn,
                                  double theta, int stn, int threshold,
                                  float* lines, int linesNumber )
{
    CvMat linesMat = cvMat( 1, linesNumber, CV_32FC2, lines ); 
    cvHoughLines2( image, &linesMat, CV_HOUGH_MULTI_SCALE,
                   rho, theta, threshold, srn, stn );

    return linesMat.cols;
}


/* Find fundamental matrix */
CV_INLINE  void  cvFindFundamentalMatrix( int* points1, int* points2,
                            int numpoints, int CV_UNREFERENCED(method), float* matrix )
{
    CvMat* pointsMat1;
    CvMat* pointsMat2;
    CvMat fundMatr = cvMat(3,3,CV_32F,matrix);
    int i, curr = 0;

    pointsMat1 = cvCreateMat(3,numpoints,CV_64F);
    pointsMat2 = cvCreateMat(3,numpoints,CV_64F);

    for( i = 0; i < numpoints; i++ )
    {
        cvmSet(pointsMat1,0,i,points1[curr]);//x
        cvmSet(pointsMat1,1,i,points1[curr+1]);//y
        cvmSet(pointsMat1,2,i,1.0);

        cvmSet(pointsMat2,0,i,points2[curr]);//x
        cvmSet(pointsMat2,1,i,points2[curr+1]);//y
        cvmSet(pointsMat2,2,i,1.0);
        curr += 2;
    }

    cvFindFundamentalMat(pointsMat1,pointsMat2,&fundMatr,CV_FM_RANSAC,1,0.99,0);

    cvReleaseMat(&pointsMat1);
    cvReleaseMat(&pointsMat2);
}



CV_INLINE int
cvFindChessBoardCornerGuesses( const void* arr, void* CV_UNREFERENCED(thresharr),
                               CvMemStorage * CV_UNREFERENCED(storage),
                               CvSize pattern_size, CvPoint2D32f * corners,
                               int *corner_count )
{
    return cvFindChessboardCorners( arr, pattern_size, corners,
                                    corner_count, CV_CALIB_CB_ADAPTIVE_THRESH );
}


/* Calibrates camera using multiple views of calibration pattern */
CV_INLINE void cvCalibrateCamera( int image_count, int* _point_counts,
    CvSize image_size, CvPoint2D32f* _image_points, CvPoint3D32f* _object_points,
    float* _distortion_coeffs, float* _camera_matrix, float* _translation_vectors,
    float* _rotation_matrices, int flags )
{
    int i, total = 0;
    CvMat point_counts = cvMat( image_count, 1, CV_32SC1, _point_counts );
    CvMat image_points, object_points;
    CvMat dist_coeffs = cvMat( 4, 1, CV_32FC1, _distortion_coeffs );
    CvMat camera_matrix = cvMat( 3, 3, CV_32FC1, _camera_matrix );
    CvMat rotation_matrices = cvMat( image_count, 9, CV_32FC1, _rotation_matrices );
    CvMat translation_vectors = cvMat( image_count, 3, CV_32FC1, _translation_vectors );

    for( i = 0; i < image_count; i++ )
        total += _point_counts[i];

    image_points = cvMat( total, 1, CV_32FC2, _image_points );
    object_points = cvMat( total, 1, CV_32FC3, _object_points );

    cvCalibrateCamera2( &object_points, &image_points, &point_counts, image_size,
        &camera_matrix, &dist_coeffs, &rotation_matrices, &translation_vectors,
        flags );
}


CV_INLINE void cvCalibrateCamera_64d( int image_count, int* _point_counts,
    CvSize image_size, CvPoint2D64f* _image_points, CvPoint3D64f* _object_points,
    double* _distortion_coeffs, double* _camera_matrix, double* _translation_vectors,
    double* _rotation_matrices, int flags )
{
    int i, total = 0;
    CvMat point_counts = cvMat( image_count, 1, CV_32SC1, _point_counts );
    CvMat image_points, object_points;
    CvMat dist_coeffs = cvMat( 4, 1, CV_64FC1, _distortion_coeffs );
    CvMat camera_matrix = cvMat( 3, 3, CV_64FC1, _camera_matrix );
    CvMat rotation_matrices = cvMat( image_count, 9, CV_64FC1, _rotation_matrices );
    CvMat translation_vectors = cvMat( image_count, 3, CV_64FC1, _translation_vectors );

    for( i = 0; i < image_count; i++ )
        total += _point_counts[i];

    image_points = cvMat( total, 1, CV_64FC2, _image_points );
    object_points = cvMat( total, 1, CV_64FC3, _object_points );

    cvCalibrateCamera2( &object_points, &image_points, &point_counts, image_size,
        &camera_matrix, &dist_coeffs, &rotation_matrices, &translation_vectors,
        flags );
}



/* Find 3d position of object given intrinsic camera parameters,
   3d model of the object and projection of the object into view plane */
CV_INLINE void cvFindExtrinsicCameraParams( int point_count,
    CvSize CV_UNREFERENCED(image_size), CvPoint2D32f* _image_points,
    CvPoint3D32f* _object_points, float* focal_length,
    CvPoint2D32f principal_point, float* _distortion_coeffs,
    float* _rotation_vector, float* _translation_vector )
{
    CvMat image_points = cvMat( point_count, 1, CV_32FC2, _image_points );
    CvMat object_points = cvMat( point_count, 1, CV_32FC3, _object_points );
    CvMat dist_coeffs = cvMat( 4, 1, CV_32FC1, _distortion_coeffs );
    float a[9];
    CvMat camera_matrix = cvMat( 3, 3, CV_32FC1, a );
    CvMat rotation_vector = cvMat( 1, 1, CV_32FC3, _rotation_vector );
    CvMat translation_vector = cvMat( 1, 1, CV_32FC3, _translation_vector );

    a[0] = focal_length[0]; a[4] = focal_length[1];
    a[2] = principal_point.x; a[5] = principal_point.y;
    a[1] = a[3] = a[6] = a[7] = 0.f;
    a[8] = 1.f;

    cvFindExtrinsicCameraParams2( &object_points, &image_points, &camera_matrix,
        &dist_coeffs, &rotation_vector, &translation_vector );
}


/* Variant of the previous function that takes double-precision parameters */
CV_INLINE void cvFindExtrinsicCameraParams_64d( int point_count,
    CvSize CV_UNREFERENCED(image_size), CvPoint2D64f* _image_points,
    CvPoint3D64f* _object_points, double* focal_length,
    CvPoint2D64f principal_point, double* _distortion_coeffs,
    double* _rotation_vector, double* _translation_vector )
{
    CvMat image_points = cvMat( point_count, 1, CV_64FC2, _image_points );
    CvMat object_points = cvMat( point_count, 1, CV_64FC3, _object_points );
    CvMat dist_coeffs = cvMat( 4, 1, CV_64FC1, _distortion_coeffs );
    double a[9];
    CvMat camera_matrix = cvMat( 3, 3, CV_64FC1, a );
    CvMat rotation_vector = cvMat( 1, 1, CV_64FC3, _rotation_vector );
    CvMat translation_vector = cvMat( 1, 1, CV_64FC3, _translation_vector );

    a[0] = focal_length[0]; a[4] = focal_length[1];
    a[2] = principal_point.x; a[5] = principal_point.y;
    a[1] = a[3] = a[6] = a[7] = 0.;
    a[8] = 1.;

    cvFindExtrinsicCameraParams2( &object_points, &image_points, &camera_matrix,
        &dist_coeffs, &rotation_vector, &translation_vector );
}


/* Rodrigues transform */
#define CV_RODRIGUES_M2V  0
#define CV_RODRIGUES_V2M  1

/* Converts rotation_matrix matrix to rotation_matrix vector or vice versa */
CV_INLINE void  cvRodrigues( CvMat* rotation_matrix, CvMat* rotation_vector,
                             CvMat* jacobian, int conv_type )
{
    if( conv_type == CV_RODRIGUES_V2M )
        cvRodrigues2( rotation_vector, rotation_matrix, jacobian );
    else
        cvRodrigues2( rotation_matrix, rotation_vector, jacobian );
}


/* Does reprojection of 3d object points to the view plane */
CV_INLINE void  cvProjectPoints( int point_count, CvPoint3D64f* _object_points,
    double* _rotation_vector, double*  _translation_vector,
    double* focal_length, CvPoint2D64f principal_point,
    double* _distortion, CvPoint2D64f* _image_points,
    double* _deriv_points_rotation_matrix,
    double* _deriv_points_translation_vect,
    double* _deriv_points_focal,
    double* _deriv_points_principal_point,
    double* _deriv_points_distortion_coeffs )
{
    CvMat object_points = cvMat( point_count, 1, CV_64FC3, _object_points );
    CvMat image_points = cvMat( point_count, 1, CV_64FC2, _image_points );
    CvMat rotation_vector = cvMat( 3, 1, CV_64FC1, _rotation_vector );
    CvMat translation_vector = cvMat( 3, 1, CV_64FC1, _translation_vector );
    double a[9];
    CvMat camera_matrix = cvMat( 3, 3, CV_64FC1, a );
    CvMat dist_coeffs = cvMat( 4, 1, CV_64FC1, _distortion );
    CvMat dpdr = cvMat( 2*point_count, 3, CV_64FC1, _deriv_points_rotation_matrix );
    CvMat dpdt = cvMat( 2*point_count, 3, CV_64FC1, _deriv_points_translation_vect );
    CvMat dpdf = cvMat( 2*point_count, 2, CV_64FC1, _deriv_points_focal );
    CvMat dpdc = cvMat( 2*point_count, 2, CV_64FC1, _deriv_points_principal_point );
    CvMat dpdk = cvMat( 2*point_count, 4, CV_64FC1, _deriv_points_distortion_coeffs );

    a[0] = focal_length[0]; a[4] = focal_length[1];
    a[2] = principal_point.x; a[5] = principal_point.y;
    a[1] = a[3] = a[6] = a[7] = 0.;
    a[8] = 1.;

    cvProjectPoints2( &object_points, &rotation_vector, &translation_vector,
                      &camera_matrix, &dist_coeffs, &image_points,
                      &dpdr, &dpdt, &dpdf, &dpdc, &dpdk );
}


/* Simpler version of the previous function */
CV_INLINE void  cvProjectPointsSimple( int point_count, CvPoint3D64f* _object_points,
    double* _rotation_matrix, double*  _translation_vector,
    double* _camera_matrix, double* _distortion, CvPoint2D64f* _image_points )
{
    CvMat object_points = cvMat( point_count, 1, CV_64FC3, _object_points );
    CvMat image_points = cvMat( point_count, 1, CV_64FC2, _image_points );
    CvMat rotation_matrix = cvMat( 3, 3, CV_64FC1, _rotation_matrix );
    CvMat translation_vector = cvMat( 3, 1, CV_64FC1, _translation_vector );
    CvMat camera_matrix = cvMat( 3, 3, CV_64FC1, _camera_matrix );
    CvMat dist_coeffs = cvMat( 4, 1, CV_64FC1, _distortion );

    cvProjectPoints2( &object_points, &rotation_matrix, &translation_vector,
                      &camera_matrix, &dist_coeffs, &image_points,
                      0, 0, 0, 0, 0 );
}


CV_INLINE void cvUnDistortOnce( const CvArr* src, CvArr* dst,
                                const float* intrinsic_matrix,
                                const float* distortion_coeffs,
                                int CV_UNREFERENCED(interpolate) )
{
    CvMat _a = cvMat( 3, 3, CV_32F, (void*)intrinsic_matrix );
    CvMat _k = cvMat( 4, 1, CV_32F, (void*)distortion_coeffs );
    cvUndistort2( src, dst, &_a, &_k );
}


/* the two functions below have quite hackerish implementations, use with care
   (or, which is better, switch to cvUndistortInitMap and cvRemap instead */ 
CV_INLINE void cvUnDistortInit( const CvArr* CV_UNREFERENCED(src),
                                CvArr* undistortion_map,
                                const float* A, const float* k,
                                int CV_UNREFERENCED(interpolate) )
{
    union { uchar* ptr; float* fl; } data;
    CvSize sz;
    cvGetRawData( undistortion_map, &data.ptr, 0, &sz );
    assert( sz.width >= 8 );
    /* just save the intrinsic parameters to the map */
    data.fl[0] = A[0]; data.fl[1] = A[4];
    data.fl[2] = A[2]; data.fl[3] = A[5];
    data.fl[4] = k[0]; data.fl[5] = k[1];
    data.fl[6] = k[2]; data.fl[7] = k[3];
}

CV_INLINE void  cvUnDistort( const CvArr* src, CvArr* dst,
                             const CvArr* undistortion_map,
                             int CV_UNREFERENCED(interpolate) )
{
    union { uchar* ptr; float* fl; } data;
    float a[] = {0,0,0,0,0,0,0,0,1};
    CvSize sz;
    cvGetRawData( undistortion_map, &data.ptr, 0, &sz );
    assert( sz.width >= 8 );
    a[0] = data.fl[0]; a[4] = data.fl[1];
    a[2] = data.fl[2]; a[5] = data.fl[3];
    cvUnDistortOnce( src, dst, a, data.fl + 4, 1 ); 
}


CV_INLINE  float  cvCalcEMD( const float* signature1, int size1,
                             const float* signature2, int size2,
                             int dims, int dist_type CV_DEFAULT(CV_DIST_L2),
                             CvDistanceFunction dist_func CV_DEFAULT(0),
                             float* lower_bound CV_DEFAULT(0),
                             void* user_param CV_DEFAULT(0))
{
    CvMat sign1 = cvMat( size1, dims + 1, CV_32FC1, (void*)signature1 );
    CvMat sign2 = cvMat( size2, dims + 1, CV_32FC1, (void*)signature2 );

    return cvCalcEMD2( &sign1, &sign2, dist_type, dist_func, 0, 0, lower_bound, user_param ); 
}


CV_INLINE  void  cvKMeans( int num_clusters, float** samples,
                           int num_samples, int vec_size,
                           CvTermCriteria termcrit, int* cluster_idx )
{
    CvMat* samples_mat = cvCreateMat( num_samples, vec_size, CV_32FC1 );
    CvMat cluster_idx_mat = cvMat( num_samples, 1, CV_32SC1, cluster_idx );
    int i;
    for( i = 0; i < num_samples; i++ )
        memcpy( samples_mat->data.fl + i*vec_size, samples[i], vec_size*sizeof(float));
    cvKMeans2( samples_mat, num_clusters, &cluster_idx_mat, termcrit );
    cvReleaseMat( &samples_mat );
}


CV_INLINE void  cvStartScanGraph( CvGraph* graph, CvGraphScanner* scanner,
                                  CvGraphVtx* vtx CV_DEFAULT(NULL),
                                  int mask CV_DEFAULT(CV_GRAPH_ALL_ITEMS))
{
    CvGraphScanner* temp_scanner;
    
    if( !scanner )
        cvError( CV_StsNullPtr, "cvStartScanGraph", "Null scanner pointer", "cvcompat.h", 0 );

    temp_scanner = cvCreateGraphScanner( graph, vtx, mask );
    *scanner = *temp_scanner;
    cvFree( &temp_scanner );
}


CV_INLINE  void  cvEndScanGraph( CvGraphScanner* scanner )
{
    if( !scanner )
        cvError( CV_StsNullPtr, "cvEndScanGraph", "Null scanner pointer", "cvcompat.h", 0 );

    if( scanner->stack )
    {
        CvGraphScanner* temp_scanner = (CvGraphScanner*)cvAlloc( sizeof(*temp_scanner) );
        *temp_scanner = *scanner;
        cvReleaseGraphScanner( &temp_scanner );
        memset( scanner, 0, sizeof(*scanner) );
    }
}


#define cvKalmanUpdateByTime  cvKalmanPredict
#define cvKalmanUpdateByMeasurement cvKalmanCorrect

/* old drawing functions */
CV_INLINE  void  cvLineAA( CvArr* img, CvPoint pt1, CvPoint pt2,
                           double color, int scale CV_DEFAULT(0))
{
    cvLine( img, pt1, pt2, cvColorToScalar(color, cvGetElemType(img)), 1, CV_AA, scale );
}

CV_INLINE  void  cvCircleAA( CvArr* img, CvPoint center, int radius,
                             double color, int scale CV_DEFAULT(0) )
{
    cvCircle( img, center, radius, cvColorToScalar(color, cvGetElemType(img)), 1, CV_AA, scale );
}

CV_INLINE  void  cvEllipseAA( CvArr* img, CvPoint center, CvSize axes,
                              double angle, double start_angle,
                              double end_angle, double color,
                              int scale CV_DEFAULT(0) )
{
    cvEllipse( img, center, axes, angle, start_angle, end_angle,
               cvColorToScalar(color, cvGetElemType(img)), 1, CV_AA, scale );
}

CV_INLINE  void  cvPolyLineAA( CvArr* img, CvPoint** pts, int* npts, int contours,
                               int is_closed, double color, int scale CV_DEFAULT(0) )
{
    cvPolyLine( img, pts, npts, contours, is_closed,
                cvColorToScalar(color, cvGetElemType(img)),
                1, CV_AA, scale );
}


#define cvMake2DPoints cvConvertPointsHomogenious
#define cvMake3DPoints cvConvertPointsHomogenious

#define cvWarpPerspectiveQMatrix cvGetPerspectiveTransform

/****************************************************************************************\
*                                   Pixel Access Macros                                  *
\****************************************************************************************/

typedef struct _CvPixelPosition8u
{
    unsigned char*   currline;      /* pointer to the start of the current pixel line   */
    unsigned char*   topline;       /* pointer to the start of the top pixel line       */
    unsigned char*   bottomline;    /* pointer to the start of the first line           */
                                    /* which is below the image                         */
    int     x;                      /* current x coordinate ( in pixels )               */
    int     width;                  /* width of the image  ( in pixels )                */
    int     height;                 /* height of the image  ( in pixels )               */
    int     step;                   /* distance between lines ( in elements of single   */
                                    /* plane )                                          */
    int     step_arr[3];            /* array: ( 0, -step, step ). It is used for        */
                                    /* vertical moving                                  */
} CvPixelPosition8u;

/* this structure differs from the above only in data type */
typedef struct _CvPixelPosition8s
{
    char*   currline;
    char*   topline;
    char*   bottomline;
    int     x;
    int     width;
    int     height;
    int     step;
    int     step_arr[3];
} CvPixelPosition8s;

/* this structure differs from the CvPixelPosition8u only in data type */
typedef struct _CvPixelPosition32f
{
    float*  currline;
    float*  topline;
    float*  bottomline;
    int     x;
    int     width;
    int     height;
    int     step;
    int     step_arr[3];
} CvPixelPosition32f;


/* Initialize one of the CvPixelPosition structures.   */
/*  pos    - initialized structure                     */
/*  origin - pointer to the left-top corner of the ROI */
/*  step   - width of the whole image in bytes         */
/*  roi    - width & height of the ROI                 */
/*  x, y   - initial position                          */
#define CV_INIT_PIXEL_POS(pos, origin, _step, roi, _x, _y, orientation)    \
    (                                                                        \
    (pos).step = (_step)/sizeof((pos).currline[0]) * (orientation ? -1 : 1), \
    (pos).width = (roi).width,                                               \
    (pos).height = (roi).height,                                             \
    (pos).bottomline = (origin) + (pos).step*(pos).height,                   \
    (pos).topline = (origin) - (pos).step,                                   \
    (pos).step_arr[0] = 0,                                                   \
    (pos).step_arr[1] = -(pos).step,                                         \
    (pos).step_arr[2] = (pos).step,                                          \
    (pos).x = (_x),                                                          \
    (pos).currline = (origin) + (pos).step*(_y) )


/* Move to specified point ( absolute shift ) */
/*  pos    - position structure               */
/*  x, y   - coordinates of the new position  */
/*  cs     - number of the image channels     */
#define CV_MOVE_TO( pos, _x, _y, cs )                                                   \
((pos).currline = (_y) >= 0 && (_y) < (pos).height ? (pos).topline + ((_y)+1)*(pos).step : 0, \
 (pos).x = (_x) >= 0 && (_x) < (pos).width ? (_x) : 0, (pos).currline + (_x) * (cs) )

/* Get current coordinates                    */
/*  pos    - position structure               */
/*  x, y   - coordinates of the new position  */
/*  cs     - number of the image channels     */
#define CV_GET_CURRENT( pos, cs )  ((pos).currline + (pos).x * (cs))
 
/* Move by one pixel relatively to current position */
/*  pos    - position structure                     */
/*  cs     - number of the image channels           */
 
/* left */
#define CV_MOVE_LEFT( pos, cs ) \
 ( --(pos).x >= 0 ? (pos).currline + (pos).x*(cs) : 0 )
 
/* right */
#define CV_MOVE_RIGHT( pos, cs ) \
 ( ++(pos).x < (pos).width ? (pos).currline + (pos).x*(cs) : 0 )
 
/* up */
#define CV_MOVE_UP( pos, cs ) \
 (((pos).currline -= (pos).step) != (pos).topline ? (pos).currline + (pos).x*(cs) : 0 )
 
/* down */
#define CV_MOVE_DOWN( pos, cs ) \
 (((pos).currline += (pos).step) != (pos).bottomline ? (pos).currline + (pos).x*(cs) : 0 )
 
/* left up */
#define CV_MOVE_LU( pos, cs ) ( CV_MOVE_LEFT(pos, cs), CV_MOVE_UP(pos, cs))
 
/* right up */
#define CV_MOVE_RU( pos, cs ) ( CV_MOVE_RIGHT(pos, cs), CV_MOVE_UP(pos, cs))
 
/* left down */
#define CV_MOVE_LD( pos, cs ) ( CV_MOVE_LEFT(pos, cs), CV_MOVE_DOWN(pos, cs))
 
/* right down */
#define CV_MOVE_RD( pos, cs ) ( CV_MOVE_RIGHT(pos, cs), CV_MOVE_DOWN(pos, cs))
 
 
 
/* Move by one pixel relatively to current position with wrapping when the position     */
/* achieves image boundary                                                              */
/*  pos    - position structure                                                         */
/*  cs     - number of the image channels                                               */
 
/* left */
#define CV_MOVE_LEFT_WRAP( pos, cs ) \
 ((pos).currline + ( --(pos).x >= 0 ? (pos).x : ((pos).x = (pos).width-1))*(cs))
 
/* right */
#define CV_MOVE_RIGHT_WRAP( pos, cs ) \
 ((pos).currline + ( ++(pos).x < (pos).width ? (pos).x : ((pos).x = 0))*(cs) )
 
/* up */
#define CV_MOVE_UP_WRAP( pos, cs ) \
    ((((pos).currline -= (pos).step) != (pos).topline ? \
    (pos).currline : ((pos).currline = (pos).bottomline - (pos).step)) + (pos).x*(cs) )
 
/* down */
#define CV_MOVE_DOWN_WRAP( pos, cs ) \
    ((((pos).currline += (pos).step) != (pos).bottomline ? \
    (pos).currline : ((pos).currline = (pos).topline + (pos).step)) + (pos).x*(cs) )
 
/* left up */
#define CV_MOVE_LU_WRAP( pos, cs ) ( CV_MOVE_LEFT_WRAP(pos, cs), CV_MOVE_UP_WRAP(pos, cs))
/* right up */
#define CV_MOVE_RU_WRAP( pos, cs ) ( CV_MOVE_RIGHT_WRAP(pos, cs), CV_MOVE_UP_WRAP(pos, cs))
/* left down */
#define CV_MOVE_LD_WRAP( pos, cs ) ( CV_MOVE_LEFT_WRAP(pos, cs), CV_MOVE_DOWN_WRAP(pos, cs))
/* right down */
#define CV_MOVE_RD_WRAP( pos, cs ) ( CV_MOVE_RIGHT_WRAP(pos, cs), CV_MOVE_DOWN_WRAP(pos, cs))
 
/* Numeric constants which used for moving in arbitrary direction  */
#define CV_SHIFT_NONE   2
#define CV_SHIFT_LEFT   1
#define CV_SHIFT_RIGHT  3
#define CV_SHIFT_UP     6
#define CV_SHIFT_DOWN  10
#define CV_SHIFT_LU     5
#define CV_SHIFT_RU     7
#define CV_SHIFT_LD     9
#define CV_SHIFT_RD    11
 
/* Move by one pixel in specified direction                                     */
/*  pos    - position structure                                                 */
/*  shift  - direction ( it's value must be one of the CV_SHIFT_… constants ) */
/*  cs     - number of the image channels                                       */
#define CV_MOVE_PARAM( pos, shift, cs )                                             \
    ( (pos).currline += (pos).step_arr[(shift)>>2], (pos).x += ((shift)&3)-2,       \
    ((pos).currline != (pos).topline && (pos).currline != (pos).bottomline &&       \
    (pos).x >= 0 && (pos).x < (pos).width) ? (pos).currline + (pos).x*(cs) : 0 )
 
/* Move by one pixel in specified direction with wrapping when the               */
/* position achieves image boundary                                              */
/*  pos    - position structure                                                  */
/*  shift  - direction ( it's value must be one of the CV_SHIFT_… constants )  */
/*  cs     - number of the image channels                                        */
#define CV_MOVE_PARAM_WRAP( pos, shift, cs )                                        \
    ( (pos).currline += (pos).step_arr[(shift)>>2],                                 \
    (pos).currline = ((pos).currline == (pos).topline ?                             \
    (pos).bottomline - (pos).step :                                                 \
    (pos).currline == (pos).bottomline ?                                            \
    (pos).topline + (pos).step : (pos).currline),                                   \
                                                                                    \
    (pos).x += ((shift)&3)-2,                                                       \
    (pos).x = ((pos).x < 0 ? (pos).width-1 : (pos).x >= (pos).width ? 0 : (pos).x), \
                                                                                    \
    (pos).currline + (pos).x*(cs) )

#endif/*_CVCOMPAT_H_*/
