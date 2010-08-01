/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
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

#ifndef __OPENCV_CV_H__
#define __OPENCV_CV_H__

#ifdef __IPL_H__
#define HAVE_IPL
#endif

#ifndef SKIP_INCLUDES
  #if defined(_CH_)
    #pragma package <chopencv>
    #include <chdl.h>
    LOAD_CHDL(cv)
  #endif
#endif

#include "cxcore.h"
#include "cvtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************\
*                                    Image Processing                                    *
\****************************************************************************************/

/* Copies source 2D array inside of the larger destination array and
   makes a border of the specified type (IPL_BORDER_*) around the copied area. */
CVAPI(void) cvCopyMakeBorder( const CvArr* src, CvArr* dst, CvPoint offset,
                              int bordertype, CvScalar value CV_DEFAULT(cvScalarAll(0)));

#define CV_BLUR_NO_SCALE 0
#define CV_BLUR  1
#define CV_GAUSSIAN  2
#define CV_MEDIAN 3
#define CV_BILATERAL 4

/* Smoothes array (removes noise) */
CVAPI(void) cvSmooth( const CvArr* src, CvArr* dst,
                      int smoothtype CV_DEFAULT(CV_GAUSSIAN),
                      int size1 CV_DEFAULT(3),
                      int size2 CV_DEFAULT(0),
                      double sigma1 CV_DEFAULT(0),
                      double sigma2 CV_DEFAULT(0));

/* Convolves the image with the kernel */
CVAPI(void) cvFilter2D( const CvArr* src, CvArr* dst, const CvMat* kernel,
                        CvPoint anchor CV_DEFAULT(cvPoint(-1,-1)));

/* Finds integral image: SUM(X,Y) = sum(x<X,y<Y)I(x,y) */
CVAPI(void) cvIntegral( const CvArr* image, CvArr* sum,
                       CvArr* sqsum CV_DEFAULT(NULL),
                       CvArr* tilted_sum CV_DEFAULT(NULL));

/*
   Smoothes the input image with gaussian kernel and then down-samples it.
   dst_width = floor(src_width/2)[+1],
   dst_height = floor(src_height/2)[+1]
*/
CVAPI(void)  cvPyrDown( const CvArr* src, CvArr* dst,
                        int filter CV_DEFAULT(CV_GAUSSIAN_5x5) );

/*
   Up-samples image and smoothes the result with gaussian kernel.
   dst_width = src_width*2,
   dst_height = src_height*2
*/
CVAPI(void)  cvPyrUp( const CvArr* src, CvArr* dst,
                      int filter CV_DEFAULT(CV_GAUSSIAN_5x5) );

/* Builds pyramid for an image */
CVAPI(CvMat**) cvCreatePyramid( const CvArr* img, int extra_layers, double rate,
                                const CvSize* layer_sizes CV_DEFAULT(0),
                                CvArr* bufarr CV_DEFAULT(0),
                                int calc CV_DEFAULT(1),
                                int filter CV_DEFAULT(CV_GAUSSIAN_5x5) );

/* Releases pyramid */
CVAPI(void)  cvReleasePyramid( CvMat*** pyramid, int extra_layers );


/* Splits color or grayscale image into multiple connected components
   of nearly the same color/brightness using modification of Burt algorithm.
   comp with contain a pointer to sequence (CvSeq)
   of connected components (CvConnectedComp) */
CVAPI(void) cvPyrSegmentation( IplImage* src, IplImage* dst,
                              CvMemStorage* storage, CvSeq** comp,
                              int level, double threshold1,
                              double threshold2 );

/* Filters image using meanshift algorithm */
CVAPI(void) cvPyrMeanShiftFiltering( const CvArr* src, CvArr* dst,
    double sp, double sr, int max_level CV_DEFAULT(1),
    CvTermCriteria termcrit CV_DEFAULT(cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,5,1)));

/* Segments image using seed "markers" */
CVAPI(void) cvWatershed( const CvArr* image, CvArr* markers );

#define CV_INPAINT_NS      0
#define CV_INPAINT_TELEA   1

/* Inpaints the selected region in the image */
CVAPI(void) cvInpaint( const CvArr* src, const CvArr* inpaint_mask,
                       CvArr* dst, double inpaintRange, int flags );

#define CV_SCHARR -1
#define CV_MAX_SOBEL_KSIZE 7

/* Calculates an image derivative using generalized Sobel
   (aperture_size = 1,3,5,7) or Scharr (aperture_size = -1) operator.
   Scharr can be used only for the first dx or dy derivative */
CVAPI(void) cvSobel( const CvArr* src, CvArr* dst,
                    int xorder, int yorder,
                    int aperture_size CV_DEFAULT(3));

/* Calculates the image Laplacian: (d2/dx + d2/dy)I */
CVAPI(void) cvLaplace( const CvArr* src, CvArr* dst,
                      int aperture_size CV_DEFAULT(3) );

/* Constants for color conversion */
#define  CV_BGR2BGRA    0
#define  CV_RGB2RGBA    CV_BGR2BGRA

#define  CV_BGRA2BGR    1
#define  CV_RGBA2RGB    CV_BGRA2BGR

#define  CV_BGR2RGBA    2
#define  CV_RGB2BGRA    CV_BGR2RGBA

#define  CV_RGBA2BGR    3
#define  CV_BGRA2RGB    CV_RGBA2BGR

#define  CV_BGR2RGB     4
#define  CV_RGB2BGR     CV_BGR2RGB

#define  CV_BGRA2RGBA   5
#define  CV_RGBA2BGRA   CV_BGRA2RGBA

#define  CV_BGR2GRAY    6
#define  CV_RGB2GRAY    7
#define  CV_GRAY2BGR    8
#define  CV_GRAY2RGB    CV_GRAY2BGR
#define  CV_GRAY2BGRA   9
#define  CV_GRAY2RGBA   CV_GRAY2BGRA
#define  CV_BGRA2GRAY   10
#define  CV_RGBA2GRAY   11

#define  CV_BGR2BGR565  12
#define  CV_RGB2BGR565  13
#define  CV_BGR5652BGR  14
#define  CV_BGR5652RGB  15
#define  CV_BGRA2BGR565 16
#define  CV_RGBA2BGR565 17
#define  CV_BGR5652BGRA 18
#define  CV_BGR5652RGBA 19

#define  CV_GRAY2BGR565 20
#define  CV_BGR5652GRAY 21

#define  CV_BGR2BGR555  22
#define  CV_RGB2BGR555  23
#define  CV_BGR5552BGR  24
#define  CV_BGR5552RGB  25
#define  CV_BGRA2BGR555 26
#define  CV_RGBA2BGR555 27
#define  CV_BGR5552BGRA 28
#define  CV_BGR5552RGBA 29

#define  CV_GRAY2BGR555 30
#define  CV_BGR5552GRAY 31

#define  CV_BGR2XYZ     32
#define  CV_RGB2XYZ     33
#define  CV_XYZ2BGR     34
#define  CV_XYZ2RGB     35

#define  CV_BGR2YCrCb   36
#define  CV_RGB2YCrCb   37
#define  CV_YCrCb2BGR   38
#define  CV_YCrCb2RGB   39

#define  CV_BGR2HSV     40
#define  CV_RGB2HSV     41

#define  CV_BGR2Lab     44
#define  CV_RGB2Lab     45

#define  CV_BayerBG2BGR 46
#define  CV_BayerGB2BGR 47
#define  CV_BayerRG2BGR 48
#define  CV_BayerGR2BGR 49

#define  CV_BayerBG2RGB CV_BayerRG2BGR
#define  CV_BayerGB2RGB CV_BayerGR2BGR
#define  CV_BayerRG2RGB CV_BayerBG2BGR
#define  CV_BayerGR2RGB CV_BayerGB2BGR

#define  CV_BGR2Luv     50
#define  CV_RGB2Luv     51
#define  CV_BGR2HLS     52
#define  CV_RGB2HLS     53

#define  CV_HSV2BGR     54
#define  CV_HSV2RGB     55

#define  CV_Lab2BGR     56
#define  CV_Lab2RGB     57
#define  CV_Luv2BGR     58
#define  CV_Luv2RGB     59
#define  CV_HLS2BGR     60
#define  CV_HLS2RGB     61

#define  CV_COLORCVT_MAX  100

/* Converts input array pixels from one color space to another */
CVAPI(void)  cvCvtColor( const CvArr* src, CvArr* dst, int code );

#define  CV_INTER_NN        0
#define  CV_INTER_LINEAR    1
#define  CV_INTER_CUBIC     2
#define  CV_INTER_AREA      3

#define  CV_WARP_FILL_OUTLIERS 8
#define  CV_WARP_INVERSE_MAP  16

/* Resizes image (input array is resized to fit the destination array) */
CVAPI(void)  cvResize( const CvArr* src, CvArr* dst,
                       int interpolation CV_DEFAULT( CV_INTER_LINEAR ));

/* Warps image with affine transform */
CVAPI(void)  cvWarpAffine( const CvArr* src, CvArr* dst, const CvMat* map_matrix,
                           int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS),
                           CvScalar fillval CV_DEFAULT(cvScalarAll(0)) );

/* Computes affine transform matrix for mapping src[i] to dst[i] (i=0,1,2) */
CVAPI(CvMat*) cvGetAffineTransform( const CvPoint2D32f * src,
                                    const CvPoint2D32f * dst,
                                    CvMat * map_matrix );

/* Computes rotation_matrix matrix */
CVAPI(CvMat*)  cv2DRotationMatrix( CvPoint2D32f center, double angle,
                                   double scale, CvMat* map_matrix );

/* Warps image with perspective (projective) transform */
CVAPI(void)  cvWarpPerspective( const CvArr* src, CvArr* dst, const CvMat* map_matrix,
                                int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS),
                                CvScalar fillval CV_DEFAULT(cvScalarAll(0)) );

/* Computes perspective transform matrix for mapping src[i] to dst[i] (i=0,1,2,3) */
CVAPI(CvMat*) cvGetPerspectiveTransform( const CvPoint2D32f* src,
                                         const CvPoint2D32f* dst,
                                         CvMat* map_matrix );

/* Performs generic geometric transformation using the specified coordinate maps */
CVAPI(void)  cvRemap( const CvArr* src, CvArr* dst,
                      const CvArr* mapx, const CvArr* mapy,
                      int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS),
                      CvScalar fillval CV_DEFAULT(cvScalarAll(0)) );

/* Converts mapx & mapy from floating-point to integer formats for cvRemap */
CVAPI(void)  cvConvertMaps( const CvArr* mapx, const CvArr* mapy,
                            CvArr* mapxy, CvArr* mapalpha );

/* Performs forward or inverse log-polar image transform */
CVAPI(void)  cvLogPolar( const CvArr* src, CvArr* dst,
                         CvPoint2D32f center, double M,
                         int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS));

/* Performs forward or inverse linear-polar image transform */
CVAPI(void)  cvLinearPolar( const CvArr* src, CvArr* dst,
                         CvPoint2D32f center, double maxRadius,
                         int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS));

#define  CV_SHAPE_RECT      0
#define  CV_SHAPE_CROSS     1
#define  CV_SHAPE_ELLIPSE   2
#define  CV_SHAPE_CUSTOM    100

/* creates structuring element used for morphological operations */
CVAPI(IplConvKernel*)  cvCreateStructuringElementEx(
            int cols, int  rows, int  anchor_x, int  anchor_y,
            int shape, int* values CV_DEFAULT(NULL) );

/* releases structuring element */
CVAPI(void)  cvReleaseStructuringElement( IplConvKernel** element );

/* erodes input image (applies minimum filter) one or more times.
   If element pointer is NULL, 3x3 rectangular element is used */
CVAPI(void)  cvErode( const CvArr* src, CvArr* dst,
                      IplConvKernel* element CV_DEFAULT(NULL),
                      int iterations CV_DEFAULT(1) );

/* dilates input image (applies maximum filter) one or more times.
   If element pointer is NULL, 3x3 rectangular element is used */
CVAPI(void)  cvDilate( const CvArr* src, CvArr* dst,
                       IplConvKernel* element CV_DEFAULT(NULL),
                       int iterations CV_DEFAULT(1) );

#define CV_MOP_ERODE        0
#define CV_MOP_DILATE       1
#define CV_MOP_OPEN         2
#define CV_MOP_CLOSE        3
#define CV_MOP_GRADIENT     4
#define CV_MOP_TOPHAT       5
#define CV_MOP_BLACKHAT     6

/* Performs complex morphological transformation */
CVAPI(void)  cvMorphologyEx( const CvArr* src, CvArr* dst,
                             CvArr* temp, IplConvKernel* element,
                             int operation, int iterations CV_DEFAULT(1) );

/* Calculates all spatial and central moments up to the 3rd order */
CVAPI(void) cvMoments( const CvArr* arr, CvMoments* moments, int binary CV_DEFAULT(0));

/* Retrieve particular spatial, central or normalized central moments */
CVAPI(double)  cvGetSpatialMoment( CvMoments* moments, int x_order, int y_order );
CVAPI(double)  cvGetCentralMoment( CvMoments* moments, int x_order, int y_order );
CVAPI(double)  cvGetNormalizedCentralMoment( CvMoments* moments,
                                             int x_order, int y_order );

/* Calculates 7 Hu's invariants from precalculated spatial and central moments */
CVAPI(void) cvGetHuMoments( CvMoments*  moments, CvHuMoments*  hu_moments );

/*********************************** data sampling **************************************/

/* Fetches pixels that belong to the specified line segment and stores them to the buffer.
   Returns the number of retrieved points. */
CVAPI(int)  cvSampleLine( const CvArr* image, CvPoint pt1, CvPoint pt2, void* buffer,
                          int connectivity CV_DEFAULT(8));

/* Retrieves the rectangular image region with specified center from the input array.
 dst(x,y) <- src(x + center.x - dst_width/2, y + center.y - dst_height/2).
 Values of pixels with fractional coordinates are retrieved using bilinear interpolation*/
CVAPI(void)  cvGetRectSubPix( const CvArr* src, CvArr* dst, CvPoint2D32f center );


/* Retrieves quadrangle from the input array.
    matrixarr = ( a11  a12 | b1 )   dst(x,y) <- src(A[x y]' + b)
                ( a21  a22 | b2 )   (bilinear interpolation is used to retrieve pixels
                                     with fractional coordinates)
*/
CVAPI(void)  cvGetQuadrangleSubPix( const CvArr* src, CvArr* dst,
                                    const CvMat* map_matrix );

/* Methods for comparing two array */
#define  CV_TM_SQDIFF        0
#define  CV_TM_SQDIFF_NORMED 1
#define  CV_TM_CCORR         2
#define  CV_TM_CCORR_NORMED  3
#define  CV_TM_CCOEFF        4
#define  CV_TM_CCOEFF_NORMED 5

/* Measures similarity between template and overlapped windows in the source image
   and fills the resultant image with the measurements */
CVAPI(void)  cvMatchTemplate( const CvArr* image, const CvArr* templ,
                              CvArr* result, int method );

/* Computes earth mover distance between
   two weighted point sets (called signatures) */
CVAPI(float)  cvCalcEMD2( const CvArr* signature1,
                          const CvArr* signature2,
                          int distance_type,
                          CvDistanceFunction distance_func CV_DEFAULT(NULL),
                          const CvArr* cost_matrix CV_DEFAULT(NULL),
                          CvArr* flow CV_DEFAULT(NULL),
                          float* lower_bound CV_DEFAULT(NULL),
                          void* userdata CV_DEFAULT(NULL));

/****************************************************************************************\
*                              Contours retrieving                                       *
\****************************************************************************************/

/* Retrieves outer and optionally inner boundaries of white (non-zero) connected
   components in the black (zero) background */
CVAPI(int)  cvFindContours( CvArr* image, CvMemStorage* storage, CvSeq** first_contour,
                            int header_size CV_DEFAULT(sizeof(CvContour)),
                            int mode CV_DEFAULT(CV_RETR_LIST),
                            int method CV_DEFAULT(CV_CHAIN_APPROX_SIMPLE),
                            CvPoint offset CV_DEFAULT(cvPoint(0,0)));


/* Initalizes contour retrieving process.
   Calls cvStartFindContours.
   Calls cvFindNextContour until null pointer is returned
   or some other condition becomes true.
   Calls cvEndFindContours at the end. */
CVAPI(CvContourScanner)  cvStartFindContours( CvArr* image, CvMemStorage* storage,
                            int header_size CV_DEFAULT(sizeof(CvContour)),
                            int mode CV_DEFAULT(CV_RETR_LIST),
                            int method CV_DEFAULT(CV_CHAIN_APPROX_SIMPLE),
                            CvPoint offset CV_DEFAULT(cvPoint(0,0)));

/* Retrieves next contour */
CVAPI(CvSeq*)  cvFindNextContour( CvContourScanner scanner );


/* Substitutes the last retrieved contour with the new one
   (if the substitutor is null, the last retrieved contour is removed from the tree) */
CVAPI(void)   cvSubstituteContour( CvContourScanner scanner, CvSeq* new_contour );


/* Releases contour scanner and returns pointer to the first outer contour */
CVAPI(CvSeq*)  cvEndFindContours( CvContourScanner* scanner );

/* Approximates a single Freeman chain or a tree of chains to polygonal curves */
CVAPI(CvSeq*) cvApproxChains( CvSeq* src_seq, CvMemStorage* storage,
                            int method CV_DEFAULT(CV_CHAIN_APPROX_SIMPLE),
                            double parameter CV_DEFAULT(0),
                            int  minimal_perimeter CV_DEFAULT(0),
                            int  recursive CV_DEFAULT(0));


/* Initalizes Freeman chain reader.
   The reader is used to iteratively get coordinates of all the chain points.
   If the Freeman codes should be read as is, a simple sequence reader should be used */
CVAPI(void) cvStartReadChainPoints( CvChain* chain, CvChainPtReader* reader );

/* Retrieves the next chain point */
CVAPI(CvPoint) cvReadChainPoint( CvChainPtReader* reader );


/****************************************************************************************\
*                                  Motion Analysis                                       *
\****************************************************************************************/

/************************************ optical flow ***************************************/

/* Calculates optical flow for 2 images using classical Lucas & Kanade algorithm */
CVAPI(void)  cvCalcOpticalFlowLK( const CvArr* prev, const CvArr* curr,
                                  CvSize win_size, CvArr* velx, CvArr* vely );

/* Calculates optical flow for 2 images using block matching algorithm */
CVAPI(void)  cvCalcOpticalFlowBM( const CvArr* prev, const CvArr* curr,
                                  CvSize block_size, CvSize shift_size,
                                  CvSize max_range, int use_previous,
                                  CvArr* velx, CvArr* vely );

/* Calculates Optical flow for 2 images using Horn & Schunck algorithm */
CVAPI(void)  cvCalcOpticalFlowHS( const CvArr* prev, const CvArr* curr,
                                  int use_previous, CvArr* velx, CvArr* vely,
                                  double lambda, CvTermCriteria criteria );

#define  CV_LKFLOW_PYR_A_READY       1
#define  CV_LKFLOW_PYR_B_READY       2
#define  CV_LKFLOW_INITIAL_GUESSES   4
#define  CV_LKFLOW_GET_MIN_EIGENVALS 8

/* It is Lucas & Kanade method, modified to use pyramids.
   Also it does several iterations to get optical flow for
   every point at every pyramid level.
   Calculates optical flow between two images for certain set of points (i.e.
   it is a "sparse" optical flow, which is opposite to the previous 3 methods) */
CVAPI(void)  cvCalcOpticalFlowPyrLK( const CvArr*  prev, const CvArr*  curr,
                                     CvArr*  prev_pyr, CvArr*  curr_pyr,
                                     const CvPoint2D32f* prev_features,
                                     CvPoint2D32f* curr_features,
                                     int       count,
                                     CvSize    win_size,
                                     int       level,
                                     char*     status,
                                     float*    track_error,
                                     CvTermCriteria criteria,
                                     int       flags );


/* Modification of a previous sparse optical flow algorithm to calculate
   affine flow */
CVAPI(void)  cvCalcAffineFlowPyrLK( const CvArr*  prev, const CvArr*  curr,
                                    CvArr*  prev_pyr, CvArr*  curr_pyr,
                                    const CvPoint2D32f* prev_features,
                                    CvPoint2D32f* curr_features,
                                    float* matrices, int  count,
                                    CvSize win_size, int  level,
                                    char* status, float* track_error,
                                    CvTermCriteria criteria, int flags );

/* Estimate rigid transformation between 2 images or 2 point sets */
CVAPI(int)  cvEstimateRigidTransform( const CvArr* A, const CvArr* B,
                                      CvMat* M, int full_affine );

/* Estimate optical flow for each pixel using the two-frame G. Farneback algorithm */
CVAPI(void) cvCalcOpticalFlowFarneback( const CvArr* prev, const CvArr* next,
                                        CvArr* flow, double pyr_scale, int levels,
                                        int winsize, int iterations, int poly_n,
                                        double poly_sigma, int flags );

/********************************* motion templates *************************************/

/****************************************************************************************\
*        All the motion template functions work only with single channel images.         *
*        Silhouette image must have depth IPL_DEPTH_8U or IPL_DEPTH_8S                   *
*        Motion history image must have depth IPL_DEPTH_32F,                             *
*        Gradient mask - IPL_DEPTH_8U or IPL_DEPTH_8S,                                   *
*        Motion orientation image - IPL_DEPTH_32F                                        *
*        Segmentation mask - IPL_DEPTH_32F                                               *
*        All the angles are in degrees, all the times are in milliseconds                *
\****************************************************************************************/

/* Updates motion history image given motion silhouette */
CVAPI(void)    cvUpdateMotionHistory( const CvArr* silhouette, CvArr* mhi,
                                      double timestamp, double duration );

/* Calculates gradient of the motion history image and fills
   a mask indicating where the gradient is valid */
CVAPI(void)    cvCalcMotionGradient( const CvArr* mhi, CvArr* mask, CvArr* orientation,
                                     double delta1, double delta2,
                                     int aperture_size CV_DEFAULT(3));

/* Calculates average motion direction within a selected motion region
   (region can be selected by setting ROIs and/or by composing a valid gradient mask
   with the region mask) */
CVAPI(double)  cvCalcGlobalOrientation( const CvArr* orientation, const CvArr* mask,
                                        const CvArr* mhi, double timestamp,
                                        double duration );

/* Splits a motion history image into a few parts corresponding to separate independent motions
   (e.g. left hand, right hand) */
CVAPI(CvSeq*)  cvSegmentMotion( const CvArr* mhi, CvArr* seg_mask,
                                CvMemStorage* storage,
                                double timestamp, double seg_thresh );

/*********************** Background statistics accumulation *****************************/

/* Adds image to accumulator */
CVAPI(void)  cvAcc( const CvArr* image, CvArr* sum,
                    const CvArr* mask CV_DEFAULT(NULL) );

/* Adds squared image to accumulator */
CVAPI(void)  cvSquareAcc( const CvArr* image, CvArr* sqsum,
                          const CvArr* mask CV_DEFAULT(NULL) );

/* Adds a product of two images to accumulator */
CVAPI(void)  cvMultiplyAcc( const CvArr* image1, const CvArr* image2, CvArr* acc,
                            const CvArr* mask CV_DEFAULT(NULL) );

/* Adds image to accumulator with weights: acc = acc*(1-alpha) + image*alpha */
CVAPI(void)  cvRunningAvg( const CvArr* image, CvArr* acc, double alpha,
                           const CvArr* mask CV_DEFAULT(NULL) );


/****************************************************************************************\
*                                       Tracking                                         *
\****************************************************************************************/

/* Implements CAMSHIFT algorithm - determines object position, size and orientation
   from the object histogram back project (extension of meanshift) */
CVAPI(int)  cvCamShift( const CvArr* prob_image, CvRect  window,
                       CvTermCriteria criteria, CvConnectedComp* comp,
                       CvBox2D* box CV_DEFAULT(NULL) );

/* Implements MeanShift algorithm - determines object position
   from the object histogram back project */
CVAPI(int)  cvMeanShift( const CvArr* prob_image, CvRect  window,
                        CvTermCriteria criteria, CvConnectedComp* comp );

/* Creates Kalman filter and sets A, B, Q, R and state to some initial values */
CVAPI(CvKalman*) cvCreateKalman( int dynam_params, int measure_params,
                                int control_params CV_DEFAULT(0));

/* Releases Kalman filter state */
CVAPI(void)  cvReleaseKalman( CvKalman** kalman);

/* Updates Kalman filter by time (predicts future state of the system) */
CVAPI(const CvMat*)  cvKalmanPredict( CvKalman* kalman,
                                     const CvMat* control CV_DEFAULT(NULL));

/* Updates Kalman filter by measurement
   (corrects state of the system and internal matrices) */
CVAPI(const CvMat*)  cvKalmanCorrect( CvKalman* kalman, const CvMat* measurement );

/****************************************************************************************\
*                              Planar subdivisions                                       *
\****************************************************************************************/

/* Initializes Delaunay triangulation */
CVAPI(void)  cvInitSubdivDelaunay2D( CvSubdiv2D* subdiv, CvRect rect );

/* Creates new subdivision */
CVAPI(CvSubdiv2D*)  cvCreateSubdiv2D( int subdiv_type, int header_size,
                                      int vtx_size, int quadedge_size,
                                      CvMemStorage* storage );

/************************* high-level subdivision functions ***************************/

/* Simplified Delaunay diagram creation */
CV_INLINE  CvSubdiv2D* cvCreateSubdivDelaunay2D( CvRect rect, CvMemStorage* storage )
{
    CvSubdiv2D* subdiv = cvCreateSubdiv2D( CV_SEQ_KIND_SUBDIV2D, sizeof(*subdiv),
                         sizeof(CvSubdiv2DPoint), sizeof(CvQuadEdge2D), storage );

    cvInitSubdivDelaunay2D( subdiv, rect );
    return subdiv;
}


/* Inserts new point to the Delaunay triangulation */
CVAPI(CvSubdiv2DPoint*)  cvSubdivDelaunay2DInsert( CvSubdiv2D* subdiv, CvPoint2D32f pt);

/* Locates a point within the Delaunay triangulation (finds the edge
   the point is left to or belongs to, or the triangulation point the given
   point coinsides with */
CVAPI(CvSubdiv2DPointLocation)  cvSubdiv2DLocate(
                               CvSubdiv2D* subdiv, CvPoint2D32f pt,
                               CvSubdiv2DEdge* edge,
                               CvSubdiv2DPoint** vertex CV_DEFAULT(NULL) );

/* Calculates Voronoi tesselation (i.e. coordinates of Voronoi points) */
CVAPI(void)  cvCalcSubdivVoronoi2D( CvSubdiv2D* subdiv );


/* Removes all Voronoi points from the tesselation */
CVAPI(void)  cvClearSubdivVoronoi2D( CvSubdiv2D* subdiv );


/* Finds the nearest to the given point vertex in subdivision. */
CVAPI(CvSubdiv2DPoint*) cvFindNearestPoint2D( CvSubdiv2D* subdiv, CvPoint2D32f pt );


/************ Basic quad-edge navigation and operations ************/

CV_INLINE  CvSubdiv2DEdge  cvSubdiv2DNextEdge( CvSubdiv2DEdge edge )
{
    return  CV_SUBDIV2D_NEXT_EDGE(edge);
}


CV_INLINE  CvSubdiv2DEdge  cvSubdiv2DRotateEdge( CvSubdiv2DEdge edge, int rotate )
{
    return  (edge & ~3) + ((edge + rotate) & 3);
}

CV_INLINE  CvSubdiv2DEdge  cvSubdiv2DSymEdge( CvSubdiv2DEdge edge )
{
    return edge ^ 2;
}

CV_INLINE  CvSubdiv2DEdge  cvSubdiv2DGetEdge( CvSubdiv2DEdge edge, CvNextEdgeType type )
{
    CvQuadEdge2D* e = (CvQuadEdge2D*)(edge & ~3);
    edge = e->next[(edge + (int)type) & 3];
    return  (edge & ~3) + ((edge + ((int)type >> 4)) & 3);
}


CV_INLINE  CvSubdiv2DPoint*  cvSubdiv2DEdgeOrg( CvSubdiv2DEdge edge )
{
    CvQuadEdge2D* e = (CvQuadEdge2D*)(edge & ~3);
    return (CvSubdiv2DPoint*)e->pt[edge & 3];
}


CV_INLINE  CvSubdiv2DPoint*  cvSubdiv2DEdgeDst( CvSubdiv2DEdge edge )
{
    CvQuadEdge2D* e = (CvQuadEdge2D*)(edge & ~3);
    return (CvSubdiv2DPoint*)e->pt[(edge + 2) & 3];
}


CV_INLINE  double  cvTriangleArea( CvPoint2D32f a, CvPoint2D32f b, CvPoint2D32f c )
{
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}


/****************************************************************************************\
*                            Contour Processing and Shape Analysis                       *
\****************************************************************************************/

#define CV_POLY_APPROX_DP 0

/* Approximates a single polygonal curve (contour) or
   a tree of polygonal curves (contours) */
CVAPI(CvSeq*)  cvApproxPoly( const void* src_seq,
                             int header_size, CvMemStorage* storage,
                             int method, double parameter,
                             int parameter2 CV_DEFAULT(0));

/* Calculates perimeter of a contour or length of a part of contour */
CVAPI(double)  cvArcLength( const void* curve,
                            CvSlice slice CV_DEFAULT(CV_WHOLE_SEQ),
                            int is_closed CV_DEFAULT(-1));
#define cvContourPerimeter( contour ) cvArcLength( contour, CV_WHOLE_SEQ, 1 )

/* Calculates contour boundning rectangle (update=1) or
   just retrieves pre-calculated rectangle (update=0) */
CVAPI(CvRect)  cvBoundingRect( CvArr* points, int update CV_DEFAULT(0) );

/* Calculates area of a contour or contour segment */
CVAPI(double)  cvContourArea( const CvArr* contour,
                              CvSlice slice CV_DEFAULT(CV_WHOLE_SEQ),
                              int oriented CV_DEFAULT(0));

/* Finds minimum area rotated rectangle bounding a set of points */
CVAPI(CvBox2D)  cvMinAreaRect2( const CvArr* points,
                                CvMemStorage* storage CV_DEFAULT(NULL));

/* Finds minimum enclosing circle for a set of points */
CVAPI(int)  cvMinEnclosingCircle( const CvArr* points,
                                  CvPoint2D32f* center, float* radius );

#define CV_CONTOURS_MATCH_I1  1
#define CV_CONTOURS_MATCH_I2  2
#define CV_CONTOURS_MATCH_I3  3

/* Compares two contours by matching their moments */
CVAPI(double)  cvMatchShapes( const void* object1, const void* object2,
                              int method, double parameter CV_DEFAULT(0));

/* Builds hierarhical representation of a contour */
CVAPI(CvContourTree*)  cvCreateContourTree( const CvSeq* contour,
                                            CvMemStorage* storage,
                                            double threshold );

/* Reconstruct (completelly or partially) contour a from contour tree */
CVAPI(CvSeq*)  cvContourFromContourTree( const CvContourTree* tree,
                                         CvMemStorage* storage,
                                         CvTermCriteria criteria );

/* Compares two contour trees */
#define  CV_CONTOUR_TREES_MATCH_I1  1

CVAPI(double)  cvMatchContourTrees( const CvContourTree* tree1,
                                    const CvContourTree* tree2,
                                    int method, double threshold );

#define CV_CLOCKWISE         1
#define CV_COUNTER_CLOCKWISE 2

/* Calculates exact convex hull of 2d point set */
CVAPI(CvSeq*) cvConvexHull2( const CvArr* input,
                             void* hull_storage CV_DEFAULT(NULL),
                             int orientation CV_DEFAULT(CV_CLOCKWISE),
                             int return_points CV_DEFAULT(0));

/* Checks whether the contour is convex or not (returns 1 if convex, 0 if not) */
CVAPI(int)  cvCheckContourConvexity( const CvArr* contour );

/* Finds convexity defects for the contour */
CVAPI(CvSeq*)  cvConvexityDefects( const CvArr* contour, const CvArr* convexhull,
                                   CvMemStorage* storage CV_DEFAULT(NULL));

/* Fits ellipse into a set of 2d points */
CVAPI(CvBox2D) cvFitEllipse2( const CvArr* points );

/* Finds minimum rectangle containing two given rectangles */
CVAPI(CvRect)  cvMaxRect( const CvRect* rect1, const CvRect* rect2 );

/* Finds coordinates of the box vertices */
CVAPI(void) cvBoxPoints( CvBox2D box, CvPoint2D32f pt[4] );

/* Initializes sequence header for a matrix (column or row vector) of points -
   a wrapper for cvMakeSeqHeaderForArray (it does not initialize bounding rectangle!!!) */
CVAPI(CvSeq*) cvPointSeqFromMat( int seq_kind, const CvArr* mat,
                                 CvContour* contour_header,
                                 CvSeqBlock* block );

/* Checks whether the point is inside polygon, outside, on an edge (at a vertex).
   Returns positive, negative or zero value, correspondingly.
   Optionally, measures a signed distance between
   the point and the nearest polygon edge (measure_dist=1) */
CVAPI(double) cvPointPolygonTest( const CvArr* contour,
                                  CvPoint2D32f pt, int measure_dist );

/****************************************************************************************\
*                                  Histogram functions                                   *
\****************************************************************************************/

/* Creates new histogram */
CVAPI(CvHistogram*)  cvCreateHist( int dims, int* sizes, int type,
                                   float** ranges CV_DEFAULT(NULL),
                                   int uniform CV_DEFAULT(1));

/* Assignes histogram bin ranges */
CVAPI(void)  cvSetHistBinRanges( CvHistogram* hist, float** ranges,
                                int uniform CV_DEFAULT(1));

/* Creates histogram header for array */
CVAPI(CvHistogram*)  cvMakeHistHeaderForArray(
                            int  dims, int* sizes, CvHistogram* hist,
                            float* data, float** ranges CV_DEFAULT(NULL),
                            int uniform CV_DEFAULT(1));

/* Releases histogram */
CVAPI(void)  cvReleaseHist( CvHistogram** hist );

/* Clears all the histogram bins */
CVAPI(void)  cvClearHist( CvHistogram* hist );

/* Finds indices and values of minimum and maximum histogram bins */
CVAPI(void)  cvGetMinMaxHistValue( const CvHistogram* hist,
                                   float* min_value, float* max_value,
                                   int* min_idx CV_DEFAULT(NULL),
                                   int* max_idx CV_DEFAULT(NULL));


/* Normalizes histogram by dividing all bins by sum of the bins, multiplied by <factor>.
   After that sum of histogram bins is equal to <factor> */
CVAPI(void)  cvNormalizeHist( CvHistogram* hist, double factor );


/* Clear all histogram bins that are below the threshold */
CVAPI(void)  cvThreshHist( CvHistogram* hist, double threshold );

#define CV_COMP_CORREL        0
#define CV_COMP_CHISQR        1
#define CV_COMP_INTERSECT     2
#define CV_COMP_BHATTACHARYYA 3

/* Compares two histogram */
CVAPI(double)  cvCompareHist( const CvHistogram* hist1,
                              const CvHistogram* hist2,
                              int method);

/* Copies one histogram to another. Destination histogram is created if
   the destination pointer is NULL */
CVAPI(void)  cvCopyHist( const CvHistogram* src, CvHistogram** dst );


/* Calculates bayesian probabilistic histograms
   (each or src and dst is an array of <number> histograms */
CVAPI(void)  cvCalcBayesianProb( CvHistogram** src, int number,
                                CvHistogram** dst);

/* Calculates array histogram */
CVAPI(void)  cvCalcArrHist( CvArr** arr, CvHistogram* hist,
                            int accumulate CV_DEFAULT(0),
                            const CvArr* mask CV_DEFAULT(NULL) );

CV_INLINE  void  cvCalcHist( IplImage** image, CvHistogram* hist,
                             int accumulate CV_DEFAULT(0),
                             const CvArr* mask CV_DEFAULT(NULL) )
{
    cvCalcArrHist( (CvArr**)image, hist, accumulate, mask );
}

/* Calculates back project */
CVAPI(void)  cvCalcArrBackProject( CvArr** image, CvArr* dst,
                                   const CvHistogram* hist );
#define  cvCalcBackProject(image, dst, hist) cvCalcArrBackProject((CvArr**)image, dst, hist)


/* Does some sort of template matching but compares histograms of
   template and each window location */
CVAPI(void)  cvCalcArrBackProjectPatch( CvArr** image, CvArr* dst, CvSize range,
                                        CvHistogram* hist, int method,
                                        double factor );
#define  cvCalcBackProjectPatch( image, dst, range, hist, method, factor ) \
     cvCalcArrBackProjectPatch( (CvArr**)image, dst, range, hist, method, factor )


/* calculates probabilistic density (divides one histogram by another) */
CVAPI(void)  cvCalcProbDensity( const CvHistogram* hist1, const CvHistogram* hist2,
                                CvHistogram* dst_hist, double scale CV_DEFAULT(255) );

/* equalizes histogram of 8-bit single-channel image */
CVAPI(void)  cvEqualizeHist( const CvArr* src, CvArr* dst );


#define  CV_VALUE  1
#define  CV_ARRAY  2
/* Updates active contour in order to minimize its cummulative
   (internal and external) energy. */
CVAPI(void)  cvSnakeImage( const IplImage* image, CvPoint* points,
                           int  length, float* alpha,
                           float* beta, float* gamma,
                           int coeff_usage, CvSize  win,
                           CvTermCriteria criteria, int calc_gradient CV_DEFAULT(1));

#define CV_DIST_MASK_3   3
#define CV_DIST_MASK_5   5
#define CV_DIST_MASK_PRECISE 0

/* Applies distance transform to binary image */
CVAPI(void)  cvDistTransform( const CvArr* src, CvArr* dst,
                              int distance_type CV_DEFAULT(CV_DIST_L2),
                              int mask_size CV_DEFAULT(3),
                              const float* mask CV_DEFAULT(NULL),
                              CvArr* labels CV_DEFAULT(NULL));


/* Types of thresholding */
#define CV_THRESH_BINARY      0  /* value = value > threshold ? max_value : 0       */
#define CV_THRESH_BINARY_INV  1  /* value = value > threshold ? 0 : max_value       */
#define CV_THRESH_TRUNC       2  /* value = value > threshold ? threshold : value   */
#define CV_THRESH_TOZERO      3  /* value = value > threshold ? value : 0           */
#define CV_THRESH_TOZERO_INV  4  /* value = value > threshold ? 0 : value           */
#define CV_THRESH_MASK        7

#define CV_THRESH_OTSU        8  /* use Otsu algorithm to choose the optimal threshold value;
                                    combine the flag with one of the above CV_THRESH_* values */

/* Applies fixed-level threshold to grayscale image.
   This is a basic operation applied before retrieving contours */
CVAPI(double)  cvThreshold( const CvArr*  src, CvArr*  dst,
                            double  threshold, double  max_value,
                            int threshold_type );

#define CV_ADAPTIVE_THRESH_MEAN_C  0
#define CV_ADAPTIVE_THRESH_GAUSSIAN_C  1

/* Applies adaptive threshold to grayscale image.
   The two parameters for methods CV_ADAPTIVE_THRESH_MEAN_C and
   CV_ADAPTIVE_THRESH_GAUSSIAN_C are:
   neighborhood size (3, 5, 7 etc.),
   and a constant subtracted from mean (...,-3,-2,-1,0,1,2,3,...) */
CVAPI(void)  cvAdaptiveThreshold( const CvArr* src, CvArr* dst, double max_value,
                                  int adaptive_method CV_DEFAULT(CV_ADAPTIVE_THRESH_MEAN_C),
                                  int threshold_type CV_DEFAULT(CV_THRESH_BINARY),
                                  int block_size CV_DEFAULT(3),
                                  double param1 CV_DEFAULT(5));

#define CV_FLOODFILL_FIXED_RANGE (1 << 16)
#define CV_FLOODFILL_MASK_ONLY   (1 << 17)

/* Fills the connected component until the color difference gets large enough */
CVAPI(void)  cvFloodFill( CvArr* image, CvPoint seed_point,
                          CvScalar new_val, CvScalar lo_diff CV_DEFAULT(cvScalarAll(0)),
                          CvScalar up_diff CV_DEFAULT(cvScalarAll(0)),
                          CvConnectedComp* comp CV_DEFAULT(NULL),
                          int flags CV_DEFAULT(4),
                          CvArr* mask CV_DEFAULT(NULL));

/****************************************************************************************\
*                                  Feature detection                                     *
\****************************************************************************************/

#define CV_CANNY_L2_GRADIENT  (1 << 31)

/* Runs canny edge detector */
CVAPI(void)  cvCanny( const CvArr* image, CvArr* edges, double threshold1,
                      double threshold2, int  aperture_size CV_DEFAULT(3) );

/* Calculates constraint image for corner detection
   Dx^2 * Dyy + Dxx * Dy^2 - 2 * Dx * Dy * Dxy.
   Applying threshold to the result gives coordinates of corners */
CVAPI(void) cvPreCornerDetect( const CvArr* image, CvArr* corners,
                              int aperture_size CV_DEFAULT(3) );

/* Calculates eigen values and vectors of 2x2
   gradient covariation matrix at every image pixel */
CVAPI(void)  cvCornerEigenValsAndVecs( const CvArr* image, CvArr* eigenvv,
                                      int block_size, int aperture_size CV_DEFAULT(3) );

/* Calculates minimal eigenvalue for 2x2 gradient covariation matrix at
   every image pixel */
CVAPI(void)  cvCornerMinEigenVal( const CvArr* image, CvArr* eigenval,
                                 int block_size, int aperture_size CV_DEFAULT(3) );

/* Harris corner detector:
   Calculates det(M) - k*(trace(M)^2), where M is 2x2 gradient covariation matrix for each pixel */
CVAPI(void)  cvCornerHarris( const CvArr* image, CvArr* harris_responce,
                             int block_size, int aperture_size CV_DEFAULT(3),
                             double k CV_DEFAULT(0.04) );

/* Adjust corner position using some sort of gradient search */
CVAPI(void)  cvFindCornerSubPix( const CvArr* image, CvPoint2D32f* corners,
                                 int count, CvSize win, CvSize zero_zone,
                                 CvTermCriteria  criteria );

/* Finds a sparse set of points within the selected region
   that seem to be easy to track */
CVAPI(void)  cvGoodFeaturesToTrack( const CvArr* image, CvArr* eig_image,
                                   CvArr* temp_image, CvPoint2D32f* corners,
                                   int* corner_count, double  quality_level,
                                   double  min_distance,
                                   const CvArr* mask CV_DEFAULT(NULL),
                                   int block_size CV_DEFAULT(3),
                                   int use_harris CV_DEFAULT(0),
                                   double k CV_DEFAULT(0.04) );

#define CV_HOUGH_STANDARD 0
#define CV_HOUGH_PROBABILISTIC 1
#define CV_HOUGH_MULTI_SCALE 2
#define CV_HOUGH_GRADIENT 3

/* Finds lines on binary image using one of several methods.
   line_storage is either memory storage or 1 x <max number of lines> CvMat, its
   number of columns is changed by the function.
   method is one of CV_HOUGH_*;
   rho, theta and threshold are used for each of those methods;
   param1 ~ line length, param2 ~ line gap - for probabilistic,
   param1 ~ srn, param2 ~ stn - for multi-scale */
CVAPI(CvSeq*)  cvHoughLines2( CvArr* image, void* line_storage, int method,
                              double rho, double theta, int threshold,
                              double param1 CV_DEFAULT(0), double param2 CV_DEFAULT(0));

/* Finds circles in the image */
CVAPI(CvSeq*) cvHoughCircles( CvArr* image, void* circle_storage,
                              int method, double dp, double min_dist,
                              double param1 CV_DEFAULT(100),
                              double param2 CV_DEFAULT(100),
                              int min_radius CV_DEFAULT(0),
                              int max_radius CV_DEFAULT(0));

/* Fits a line into set of 2d or 3d points in a robust way (M-estimator technique) */
CVAPI(void)  cvFitLine( const CvArr* points, int dist_type, double param,
                        double reps, double aeps, float* line );



struct CvFeatureTree;

/* Constructs kd-tree from set of feature descriptors */
CVAPI(struct CvFeatureTree*) cvCreateKDTree(CvMat* desc);

/* Constructs spill-tree from set of feature descriptors */
CVAPI(struct CvFeatureTree*) cvCreateSpillTree( const CvMat* raw_data,
                                    const int naive CV_DEFAULT(50),
                                    const double rho CV_DEFAULT(.7),
                                    const double tau CV_DEFAULT(.1) );

/* Release feature tree */
CVAPI(void) cvReleaseFeatureTree(struct CvFeatureTree* tr);

/* Searches feature tree for k nearest neighbors of given reference points,
   searching (in case of kd-tree/bbf) at most emax leaves. */
CVAPI(void) cvFindFeatures(struct CvFeatureTree* tr, const CvMat* query_points,
                           CvMat* indices, CvMat* dist, int k, int emax CV_DEFAULT(20));

/* Search feature tree for all points that are inlier to given rect region.
   Only implemented for kd trees */
CVAPI(int) cvFindFeaturesBoxed(struct CvFeatureTree* tr,
                               CvMat* bounds_min, CvMat* bounds_max,
                               CvMat* out_indices);


struct CvLSH;
struct CvLSHOperations;

/* Construct a Locality Sensitive Hash (LSH) table, for indexing d-dimensional vectors of
   given type. Vectors will be hashed L times with k-dimensional p-stable (p=2) functions. */
CVAPI(struct CvLSH*) cvCreateLSH(struct CvLSHOperations* ops, int d,
                                 int L CV_DEFAULT(10), int k CV_DEFAULT(10),
                                 int type CV_DEFAULT(CV_64FC1), double r CV_DEFAULT(4),
                                 int64 seed CV_DEFAULT(-1));

/* Construct in-memory LSH table, with n bins. */
CVAPI(struct CvLSH*) cvCreateMemoryLSH(int d, int n, int L CV_DEFAULT(10), int k CV_DEFAULT(10),
                                       int type CV_DEFAULT(CV_64FC1), double r CV_DEFAULT(4),
                                       int64 seed CV_DEFAULT(-1));

/* Free the given LSH structure. */
CVAPI(void) cvReleaseLSH(struct CvLSH** lsh);

/* Return the number of vectors in the LSH. */
CVAPI(unsigned int) LSHSize(struct CvLSH* lsh);

/* Add vectors to the LSH structure, optionally returning indices. */
CVAPI(void) cvLSHAdd(struct CvLSH* lsh, const CvMat* data, CvMat* indices CV_DEFAULT(0));

/* Remove vectors from LSH, as addressed by given indices. */
CVAPI(void) cvLSHRemove(struct CvLSH* lsh, const CvMat* indices);

/* Query the LSH n times for at most k nearest points; data is n x d,
   indices and dist are n x k. At most emax stored points will be accessed. */
CVAPI(void) cvLSHQuery(struct CvLSH* lsh, const CvMat* query_points,
                       CvMat* indices, CvMat* dist, int k, int emax);


typedef struct CvSURFPoint
{
    CvPoint2D32f pt;
    int laplacian;
    int size;
    float dir;
    float hessian;
} CvSURFPoint;

CV_INLINE CvSURFPoint cvSURFPoint( CvPoint2D32f pt, int laplacian,
                                   int size, float dir CV_DEFAULT(0),
                                   float hessian CV_DEFAULT(0))
{
    CvSURFPoint kp;
    kp.pt = pt;
    kp.laplacian = laplacian;
    kp.size = size;
    kp.dir = dir;
    kp.hessian = hessian;
    return kp;
}

typedef struct CvSURFParams
{
    int extended;
    double hessianThreshold;

    int nOctaves;
    int nOctaveLayers;
}
CvSURFParams;

CVAPI(CvSURFParams) cvSURFParams( double hessianThreshold, int extended CV_DEFAULT(0) );

// If useProvidedKeyPts!=0, keypoints are not detected, but descriptors are computed
//  at the locations provided in keypoints (a CvSeq of CvSURFPoint).
CVAPI(void) cvExtractSURF( const CvArr* img, const CvArr* mask,
                           CvSeq** keypoints, CvSeq** descriptors,
                           CvMemStorage* storage, CvSURFParams params, int useProvidedKeyPts CV_DEFAULT(0)  );

typedef struct CvMSERParams
{
    // delta, in the code, it compares (size_{i}-size_{i-delta})/size_{i-delta}
    int delta;
    // prune the area which bigger/smaller than max_area/min_area
    int maxArea;
    int minArea;
    // prune the area have simliar size to its children
    float maxVariation;
    // trace back to cut off mser with diversity < min_diversity
    float minDiversity;
    /* the next few params for MSER of color image */
    // for color image, the evolution steps
    int maxEvolution;
    // the area threshold to cause re-initialize
    double areaThreshold;
    // ignore too small margin
    double minMargin;
    // the aperture size for edge blur
    int edgeBlurSize;
}
CvMSERParams;

CVAPI(CvMSERParams) cvMSERParams( int delta CV_DEFAULT(5), int min_area CV_DEFAULT(60),
                           int max_area CV_DEFAULT(14400), float max_variation CV_DEFAULT(.25f),
                           float min_diversity CV_DEFAULT(.2f), int max_evolution CV_DEFAULT(200),
                           double area_threshold CV_DEFAULT(1.01),
                           double min_margin CV_DEFAULT(.003),
                           int edge_blur_size CV_DEFAULT(5) );

// Extracts the contours of Maximally Stable Extremal Regions
CVAPI(void) cvExtractMSER( CvArr* _img, CvArr* _mask, CvSeq** contours, CvMemStorage* storage, CvMSERParams params );


typedef struct CvStarKeypoint
{
    CvPoint pt;
    int size;
    float response;
}
CvStarKeypoint;

CV_INLINE CvStarKeypoint cvStarKeypoint(CvPoint pt, int size, float response)
{
    CvStarKeypoint kpt;
    kpt.pt = pt;
    kpt.size = size;
    kpt.response = response;
    return kpt;
}

typedef struct CvStarDetectorParams
{
    int maxSize;
    int responseThreshold;
    int lineThresholdProjected;
    int lineThresholdBinarized;
    int suppressNonmaxSize;
}
CvStarDetectorParams;

CV_INLINE CvStarDetectorParams cvStarDetectorParams(
    int maxSize CV_DEFAULT(45),
    int responseThreshold CV_DEFAULT(30),
    int lineThresholdProjected CV_DEFAULT(10),
    int lineThresholdBinarized CV_DEFAULT(8),
    int suppressNonmaxSize CV_DEFAULT(5))
{
    CvStarDetectorParams params;
    params.maxSize = maxSize;
    params.responseThreshold = responseThreshold;
    params.lineThresholdProjected = lineThresholdProjected;
    params.lineThresholdBinarized = lineThresholdBinarized;
    params.suppressNonmaxSize = suppressNonmaxSize;

    return params;
}

CVAPI(CvSeq*) cvGetStarKeypoints( const CvArr* img, CvMemStorage* storage,
        CvStarDetectorParams params CV_DEFAULT(cvStarDetectorParams()));

/****************************************************************************************\
*                         Haar-like Object Detection functions                           *
\****************************************************************************************/

/* Loads haar classifier cascade from a directory.
   It is obsolete: convert your cascade to xml and use cvLoad instead */
CVAPI(CvHaarClassifierCascade*) cvLoadHaarClassifierCascade(
                    const char* directory, CvSize orig_window_size);

CVAPI(void) cvReleaseHaarClassifierCascade( CvHaarClassifierCascade** cascade );

#define CV_HAAR_DO_CANNY_PRUNING    1
#define CV_HAAR_SCALE_IMAGE         2
#define CV_HAAR_FIND_BIGGEST_OBJECT 4
#define CV_HAAR_DO_ROUGH_SEARCH     8

CVAPI(CvSeq*) cvHaarDetectObjects( const CvArr* image,
                     CvHaarClassifierCascade* cascade,
                     CvMemStorage* storage, double scale_factor CV_DEFAULT(1.1),
                     int min_neighbors CV_DEFAULT(3), int flags CV_DEFAULT(0),
                     CvSize min_size CV_DEFAULT(cvSize(0,0)));

/* sets images for haar classifier cascade */
CVAPI(void) cvSetImagesForHaarClassifierCascade( CvHaarClassifierCascade* cascade,
                                                const CvArr* sum, const CvArr* sqsum,
                                                const CvArr* tilted_sum, double scale );

/* runs the cascade on the specified window */
CVAPI(int) cvRunHaarClassifierCascade( const CvHaarClassifierCascade* cascade,
                                       CvPoint pt, int start_stage CV_DEFAULT(0));

/****************************************************************************************\
*                      Camera Calibration, Pose Estimation and Stereo                    *
\****************************************************************************************/

/* Transforms the input image to compensate lens distortion */
CVAPI(void) cvUndistort2( const CvArr* src, CvArr* dst,
                          const CvMat* camera_matrix,
                          const CvMat* distortion_coeffs,
                          const CvMat* new_camera_matrix CV_DEFAULT(0) );

/* Computes transformation map from intrinsic camera parameters
   that can used by cvRemap */
CVAPI(void) cvInitUndistortMap( const CvMat* camera_matrix,
                                const CvMat* distortion_coeffs,
                                CvArr* mapx, CvArr* mapy );

/* Computes undistortion+rectification map for a head of stereo camera */
CVAPI(void) cvInitUndistortRectifyMap( const CvMat* camera_matrix,
                                       const CvMat* dist_coeffs,
                                       const CvMat *R, const CvMat* new_camera_matrix,
                                       CvArr* mapx, CvArr* mapy );

/* Computes the original (undistorted) feature coordinates
   from the observed (distorted) coordinates */
CVAPI(void) cvUndistortPoints( const CvMat* src, CvMat* dst,
                               const CvMat* camera_matrix,
                               const CvMat* dist_coeffs,
                               const CvMat* R CV_DEFAULT(0),
                               const CvMat* P CV_DEFAULT(0));
    
/* Computes the optimal new camera matrix according to the free scaling parameter alpha:
   alpha=0 - only valid pixels will be retained in the undistorted image
   alpha=1 - all the source image pixels will be retained in the undistorted image
*/
CVAPI(void) cvGetOptimalNewCameraMatrix( const CvMat* camera_matrix,
                                         const CvMat* dist_coeffs,
                                         CvSize image_size, double alpha,
                                         CvMat* new_camera_matrix,
                                         CvSize new_imag_size CV_DEFAULT(cvSize(0,0)),
                                         CvRect* valid_pixel_ROI CV_DEFAULT(0) );

/* Converts rotation vector to rotation matrix or vice versa */
CVAPI(int) cvRodrigues2( const CvMat* src, CvMat* dst,
                         CvMat* jacobian CV_DEFAULT(0) );

#define CV_LMEDS 4
#define CV_RANSAC 8

/* Finds perspective transformation between the object plane and image (view) plane */
CVAPI(int) cvFindHomography( const CvMat* src_points,
                             const CvMat* dst_points,
                             CvMat* homography,
                             int method CV_DEFAULT(0),
                             double ransacReprojThreshold CV_DEFAULT(0),
                             CvMat* mask CV_DEFAULT(0));

/* Computes RQ decomposition for 3x3 matrices */
CVAPI(void) cvRQDecomp3x3( const CvMat *matrixM, CvMat *matrixR, CvMat *matrixQ,
                           CvMat *matrixQx CV_DEFAULT(NULL),
                           CvMat *matrixQy CV_DEFAULT(NULL),
                           CvMat *matrixQz CV_DEFAULT(NULL),
                           CvPoint3D64f *eulerAngles CV_DEFAULT(NULL));

/* Computes projection matrix decomposition */
CVAPI(void) cvDecomposeProjectionMatrix( const CvMat *projMatr, CvMat *calibMatr,
                                         CvMat *rotMatr, CvMat *posVect,
                                         CvMat *rotMatrX CV_DEFAULT(NULL),
                                         CvMat *rotMatrY CV_DEFAULT(NULL),
                                         CvMat *rotMatrZ CV_DEFAULT(NULL),
                                         CvPoint3D64f *eulerAngles CV_DEFAULT(NULL));

/* Computes d(AB)/dA and d(AB)/dB */
CVAPI(void) cvCalcMatMulDeriv( const CvMat* A, const CvMat* B, CvMat* dABdA, CvMat* dABdB );

/* Computes r3 = rodrigues(rodrigues(r2)*rodrigues(r1)),
   t3 = rodrigues(r2)*t1 + t2 and the respective derivatives */
CVAPI(void) cvComposeRT( const CvMat* _rvec1, const CvMat* _tvec1,
                         const CvMat* _rvec2, const CvMat* _tvec2,
                         CvMat* _rvec3, CvMat* _tvec3,
                         CvMat* dr3dr1 CV_DEFAULT(0), CvMat* dr3dt1 CV_DEFAULT(0),
                         CvMat* dr3dr2 CV_DEFAULT(0), CvMat* dr3dt2 CV_DEFAULT(0),
                         CvMat* dt3dr1 CV_DEFAULT(0), CvMat* dt3dt1 CV_DEFAULT(0),
                         CvMat* dt3dr2 CV_DEFAULT(0), CvMat* dt3dt2 CV_DEFAULT(0) );

/* Projects object points to the view plane using
   the specified extrinsic and intrinsic camera parameters */
CVAPI(void) cvProjectPoints2( const CvMat* object_points, const CvMat* rotation_vector,
                              const CvMat* translation_vector, const CvMat* camera_matrix,
                              const CvMat* distortion_coeffs, CvMat* image_points,
                              CvMat* dpdrot CV_DEFAULT(NULL), CvMat* dpdt CV_DEFAULT(NULL),
                              CvMat* dpdf CV_DEFAULT(NULL), CvMat* dpdc CV_DEFAULT(NULL),
                              CvMat* dpddist CV_DEFAULT(NULL),
                              double aspect_ratio CV_DEFAULT(0));

/* Finds extrinsic camera parameters from
   a few known corresponding point pairs and intrinsic parameters */
CVAPI(void) cvFindExtrinsicCameraParams2( const CvMat* object_points,
                                          const CvMat* image_points,
                                          const CvMat* camera_matrix,
                                          const CvMat* distortion_coeffs,
                                          CvMat* rotation_vector,
                                          CvMat* translation_vector,
                                          int use_extrinsic_guess CV_DEFAULT(0) );

/* Computes initial estimate of the intrinsic camera parameters
   in case of planar calibration target (e.g. chessboard) */
CVAPI(void) cvInitIntrinsicParams2D( const CvMat* object_points,
                                     const CvMat* image_points,
                                     const CvMat* npoints, CvSize image_size,
                                     CvMat* camera_matrix,
                                     double aspect_ratio CV_DEFAULT(1.) );

#define CV_CALIB_CB_ADAPTIVE_THRESH  1
#define CV_CALIB_CB_NORMALIZE_IMAGE  2
#define CV_CALIB_CB_FILTER_QUADS     4
#define CV_CALIB_CB_FAST_CHECK       8

// Performs a fast check if a chessboard is in the input image. This is a workaround to 
// a problem of cvFindChessboardCorners being slow on images with no chessboard
// - src: input image
// - size: chessboard size
// Returns 1 if a chessboard can be in this image and findChessboardCorners should be called, 
// 0 if there is no chessboard, -1 in case of error
CVAPI(int) cvCheckChessboard(IplImage* src, CvSize size);
    
    /* Detects corners on a chessboard calibration pattern */
CVAPI(int) cvFindChessboardCorners( const void* image, CvSize pattern_size,
                                    CvPoint2D32f* corners,
                                    int* corner_count CV_DEFAULT(NULL),
                                    int flags CV_DEFAULT(CV_CALIB_CB_ADAPTIVE_THRESH+
                                        CV_CALIB_CB_NORMALIZE_IMAGE) );

/* Draws individual chessboard corners or the whole chessboard detected */
CVAPI(void) cvDrawChessboardCorners( CvArr* image, CvSize pattern_size,
                                     CvPoint2D32f* corners,
                                     int count, int pattern_was_found );

#define CV_CALIB_USE_INTRINSIC_GUESS  1
#define CV_CALIB_FIX_ASPECT_RATIO     2
#define CV_CALIB_FIX_PRINCIPAL_POINT  4
#define CV_CALIB_ZERO_TANGENT_DIST    8
#define CV_CALIB_FIX_FOCAL_LENGTH 16
#define CV_CALIB_FIX_K1  32
#define CV_CALIB_FIX_K2  64
#define CV_CALIB_FIX_K3  128

/* Finds intrinsic and extrinsic camera parameters
   from a few views of known calibration pattern */
CVAPI(double) cvCalibrateCamera2( const CvMat* object_points,
                                const CvMat* image_points,
                                const CvMat* point_counts,
                                CvSize image_size,
                                CvMat* camera_matrix,
                                CvMat* distortion_coeffs,
                                CvMat* rotation_vectors CV_DEFAULT(NULL),
                                CvMat* translation_vectors CV_DEFAULT(NULL),
                                int flags CV_DEFAULT(0) );

/* Computes various useful characteristics of the camera from the data computed by
   cvCalibrateCamera2 */
CVAPI(void) cvCalibrationMatrixValues( const CvMat *camera_matrix,
                                CvSize image_size,
                                double aperture_width CV_DEFAULT(0),
                                double aperture_height CV_DEFAULT(0),
                                double *fovx CV_DEFAULT(NULL),
                                double *fovy CV_DEFAULT(NULL),
                                double *focal_length CV_DEFAULT(NULL),
                                CvPoint2D64f *principal_point CV_DEFAULT(NULL),
                                double *pixel_aspect_ratio CV_DEFAULT(NULL));

#define CV_CALIB_FIX_INTRINSIC  256
#define CV_CALIB_SAME_FOCAL_LENGTH 512

/* Computes the transformation from one camera coordinate system to another one
   from a few correspondent views of the same calibration target. Optionally, calibrates
   both cameras */
CVAPI(double) cvStereoCalibrate( const CvMat* object_points, const CvMat* image_points1,
                               const CvMat* image_points2, const CvMat* npoints,
                               CvMat* camera_matrix1, CvMat* dist_coeffs1,
                               CvMat* camera_matrix2, CvMat* dist_coeffs2,
                               CvSize image_size, CvMat* R, CvMat* T,
                               CvMat* E CV_DEFAULT(0), CvMat* F CV_DEFAULT(0),
                               CvTermCriteria term_crit CV_DEFAULT(cvTermCriteria(
                                   CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,30,1e-6)),
                               int flags CV_DEFAULT(CV_CALIB_FIX_INTRINSIC));

#define CV_CALIB_ZERO_DISPARITY 1024

/* Computes 3D rotations (+ optional shift) for each camera coordinate system to make both
   views parallel (=> to make all the epipolar lines horizontal or vertical) */
CVAPI(void) cvStereoRectify( const CvMat* camera_matrix1, const CvMat* camera_matrix2,
                             const CvMat* dist_coeffs1, const CvMat* dist_coeffs2,
                             CvSize image_size, const CvMat* R, const CvMat* T,
                             CvMat* R1, CvMat* R2, CvMat* P1, CvMat* P2,
                             CvMat* Q CV_DEFAULT(0),
                             int flags CV_DEFAULT(CV_CALIB_ZERO_DISPARITY),
                             double alpha CV_DEFAULT(-1),
                             CvSize new_image_size CV_DEFAULT(cvSize(0,0)),
                             CvRect* valid_pix_ROI1 CV_DEFAULT(0),
                             CvRect* valid_pix_ROI2 CV_DEFAULT(0));

/* Computes rectification transformations for uncalibrated pair of images using a set
   of point correspondences */
CVAPI(int) cvStereoRectifyUncalibrated( const CvMat* points1, const CvMat* points2,
                                        const CvMat* F, CvSize img_size,
                                        CvMat* H1, CvMat* H2,
                                        double threshold CV_DEFAULT(5));

typedef struct CvPOSITObject CvPOSITObject;

/* Allocates and initializes CvPOSITObject structure before doing cvPOSIT */
CVAPI(CvPOSITObject*)  cvCreatePOSITObject( CvPoint3D32f* points, int point_count );


/* Runs POSIT (POSe from ITeration) algorithm for determining 3d position of
   an object given its model and projection in a weak-perspective case */
CVAPI(void)  cvPOSIT(  CvPOSITObject* posit_object, CvPoint2D32f* image_points,
                       double focal_length, CvTermCriteria criteria,
                       CvMatr32f rotation_matrix, CvVect32f translation_vector);

/* Releases CvPOSITObject structure */
CVAPI(void)  cvReleasePOSITObject( CvPOSITObject**  posit_object );

/* updates the number of RANSAC iterations */
CVAPI(int) cvRANSACUpdateNumIters( double p, double err_prob,
                                   int model_points, int max_iters );

CVAPI(void) cvConvertPointsHomogeneous( const CvMat* src, CvMat* dst );

/* Calculates fundamental matrix given a set of corresponding points */
#define CV_FM_7POINT 1
#define CV_FM_8POINT 2
#define CV_FM_LMEDS_ONLY  CV_LMEDS
#define CV_FM_RANSAC_ONLY CV_RANSAC
#define CV_FM_LMEDS CV_LMEDS
#define CV_FM_RANSAC CV_RANSAC
CVAPI(int) cvFindFundamentalMat( const CvMat* points1, const CvMat* points2,
                                 CvMat* fundamental_matrix,
                                 int method CV_DEFAULT(CV_FM_RANSAC),
                                 double param1 CV_DEFAULT(3.), double param2 CV_DEFAULT(0.99),
                                 CvMat* status CV_DEFAULT(NULL) );

/* For each input point on one of images
   computes parameters of the corresponding
   epipolar line on the other image */
CVAPI(void) cvComputeCorrespondEpilines( const CvMat* points,
                                         int which_image,
                                         const CvMat* fundamental_matrix,
                                         CvMat* correspondent_lines );

/* Triangulation functions */

CVAPI(void) cvTriangulatePoints(CvMat* projMatr1, CvMat* projMatr2,
                                CvMat* projPoints1, CvMat* projPoints2,
                                CvMat* points4D);

CVAPI(void) cvCorrectMatches(CvMat* F, CvMat* points1, CvMat* points2,
                             CvMat* new_points1, CvMat* new_points2);

/* stereo correspondence parameters and functions */

#define CV_STEREO_BM_NORMALIZED_RESPONSE  0
#define CV_STEREO_BM_XSOBEL               1

/* Block matching algorithm structure */
typedef struct CvStereoBMState
{
    // pre-filtering (normalization of input images)
    int preFilterType; // =CV_STEREO_BM_NORMALIZED_RESPONSE now
    int preFilterSize; // averaging window size: ~5x5..21x21
    int preFilterCap; // the output of pre-filtering is clipped by [-preFilterCap,preFilterCap]

    // correspondence using Sum of Absolute Difference (SAD)
    int SADWindowSize; // ~5x5..21x21
    int minDisparity;  // minimum disparity (can be negative)
    int numberOfDisparities; // maximum disparity - minimum disparity (> 0)

    // post-filtering
    int textureThreshold;  // the disparity is only computed for pixels
                           // with textured enough neighborhood
    int uniquenessRatio;   // accept the computed disparity d* only if
                           // SAD(d) >= SAD(d*)*(1 + uniquenessRatio/100.)
                           // for any d != d*+/-1 within the search range.
    int speckleWindowSize; // disparity variation window
    int speckleRange; // acceptable range of variation in window

    int trySmallerWindows; // if 1, the results may be more accurate,
                           // at the expense of slower processing 
    CvRect roi1, roi2;
    int disp12MaxDiff;

    // temporary buffers
    CvMat* preFilteredImg0;
    CvMat* preFilteredImg1;
    CvMat* slidingSumBuf;
    CvMat* cost;
    CvMat* disp;
}
CvStereoBMState;

#define CV_STEREO_BM_BASIC 0
#define CV_STEREO_BM_FISH_EYE 1
#define CV_STEREO_BM_NARROW 2

CVAPI(CvStereoBMState*) cvCreateStereoBMState(int preset CV_DEFAULT(CV_STEREO_BM_BASIC),
                                              int numberOfDisparities CV_DEFAULT(0));

CVAPI(void) cvReleaseStereoBMState( CvStereoBMState** state );

CVAPI(void) cvFindStereoCorrespondenceBM( const CvArr* left, const CvArr* right,
                                          CvArr* disparity, CvStereoBMState* state );
    
CVAPI(CvRect) cvGetValidDisparityROI( CvRect roi1, CvRect roi2, int minDisparity,
                                      int numberOfDisparities, int SADWindowSize );
    
CVAPI(void) cvValidateDisparity( CvArr* disparity, const CvArr* cost,
                                 int minDisparity, int numberOfDisparities,
                                 int disp12MaxDiff CV_DEFAULT(1) );  

/* Kolmogorov-Zabin stereo-correspondence algorithm (a.k.a. KZ1) */
#define CV_STEREO_GC_OCCLUDED  SHRT_MAX

typedef struct CvStereoGCState
{
    int Ithreshold;
    int interactionRadius;
    float K, lambda, lambda1, lambda2;
    int occlusionCost;
    int minDisparity;
    int numberOfDisparities;
    int maxIters;

    CvMat* left;
    CvMat* right;
    CvMat* dispLeft;
    CvMat* dispRight;
    CvMat* ptrLeft;
    CvMat* ptrRight;
    CvMat* vtxBuf;
    CvMat* edgeBuf;
}
CvStereoGCState;

CVAPI(CvStereoGCState*) cvCreateStereoGCState( int numberOfDisparities, int maxIters );
CVAPI(void) cvReleaseStereoGCState( CvStereoGCState** state );

CVAPI(void) cvFindStereoCorrespondenceGC( const CvArr* left, const CvArr* right,
                                          CvArr* disparityLeft, CvArr* disparityRight,
                                          CvStereoGCState* state,
                                          int useDisparityGuess CV_DEFAULT(0) );

/* Reprojects the computed disparity image to the 3D space using the specified 4x4 matrix */
CVAPI(void)  cvReprojectImageTo3D( const CvArr* disparityImage,
                                   CvArr* _3dImage, const CvMat* Q,
                                   int handleMissingValues CV_DEFAULT(0) );

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#ifndef SKIP_INCLUDES // for now only expose old interface to swig
#include "cv.hpp"
#endif // SKIP_INCLUDES
#endif

/****************************************************************************************\
*                                 Backward compatibility                                 *
\****************************************************************************************/

#ifndef CV_NO_BACKWARD_COMPATIBILITY
#include "cvcompat.h"
#endif

#endif
