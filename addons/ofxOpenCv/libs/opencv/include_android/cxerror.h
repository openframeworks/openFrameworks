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

#ifndef __OPENCV_ERROR_H__
#define __OPENCV_ERROR_H__

/************Below is declaration of error handling stuff in PLSuite manner**/

typedef int CVStatus;

/* this part of CVStatus is compatible with IPLStatus 
  Some of below symbols are not [yet] used in OpenCV
*/
#define CV_StsOk                    0  /* everithing is ok                */
#define CV_StsBackTrace            -1  /* pseudo error for back trace     */
#define CV_StsError                -2  /* unknown /unspecified error      */
#define CV_StsInternal             -3  /* internal error (bad state)      */
#define CV_StsNoMem                -4  /* insufficient memory             */
#define CV_StsBadArg               -5  /* function arg/param is bad       */
#define CV_StsBadFunc              -6  /* unsupported function            */
#define CV_StsNoConv               -7  /* iter. didn't converge           */
#define CV_StsAutoTrace            -8  /* tracing                         */

#define CV_HeaderIsNull            -9  /* image header is NULL            */
#define CV_BadImageSize            -10 /* image size is invalid           */
#define CV_BadOffset               -11 /* offset is invalid               */
#define CV_BadDataPtr              -12 /**/
#define CV_BadStep                 -13 /**/
#define CV_BadModelOrChSeq         -14 /**/
#define CV_BadNumChannels          -15 /**/
#define CV_BadNumChannel1U         -16 /**/
#define CV_BadDepth                -17 /**/
#define CV_BadAlphaChannel         -18 /**/
#define CV_BadOrder                -19 /**/
#define CV_BadOrigin               -20 /**/
#define CV_BadAlign                -21 /**/
#define CV_BadCallBack             -22 /**/
#define CV_BadTileSize             -23 /**/
#define CV_BadCOI                  -24 /**/
#define CV_BadROISize              -25 /**/

#define CV_MaskIsTiled             -26 /**/

#define CV_StsNullPtr                -27 /* null pointer */
#define CV_StsVecLengthErr           -28 /* incorrect vector length */
#define CV_StsFilterStructContentErr -29 /* incorr. filter structure content */
#define CV_StsKernelStructContentErr -30 /* incorr. transform kernel content */
#define CV_StsFilterOffsetErr        -31 /* incorrect filter ofset value */

/*extra for CV */
#define CV_StsBadSize                -201 /* the input/output structure size is incorrect  */
#define CV_StsDivByZero              -202 /* division by zero */
#define CV_StsInplaceNotSupported    -203 /* in-place operation is not supported */
#define CV_StsObjectNotFound         -204 /* request can't be completed */
#define CV_StsUnmatchedFormats       -205 /* formats of input/output arrays differ */
#define CV_StsBadFlag                -206 /* flag is wrong or not supported */  
#define CV_StsBadPoint               -207 /* bad CvPoint */ 
#define CV_StsBadMask                -208 /* bad format of mask (neither 8uC1 nor 8sC1)*/
#define CV_StsUnmatchedSizes         -209 /* sizes of input/output structures do not match */
#define CV_StsUnsupportedFormat      -210 /* the data format/type is not supported by the function*/
#define CV_StsOutOfRange             -211 /* some of parameters are out of range */
#define CV_StsParseError             -212 /* invalid syntax/structure of the parsed file */
#define CV_StsNotImplemented         -213 /* the requested function/feature is not implemented */
#define CV_StsBadMemBlock            -214 /* an allocated block has been corrupted */
#define CV_StsAssert                 -215 /* assertion failed */

/********************************* Error handling Macros ********************************/

#define OPENCV_ERROR(status,func,context)                           \
                cvError((status),(func),(context),__FILE__,__LINE__)

#define OPENCV_ERRCHK(func,context)                                 \
                {if (cvGetErrStatus() >= 0)                         \
                 {OPENCV_ERROR(CV_StsBackTrace,(func),(context));}}

#define OPENCV_ASSERT(expr,func,context)                            \
                {if (! (expr))                                      \
                 {OPENCV_ERROR(CV_StsInternal,(func),(context));}}

#define OPENCV_RSTERR() (cvSetErrStatus(CV_StsOk))

#define OPENCV_CALL( Func )                                         \
{                                                                   \
    Func;                                                           \
} 


/**************************** OpenCV-style error handling *******************************/

/* CV_FUNCNAME macro defines icvFuncName constant which is used by CV_ERROR macro */
#ifdef CV_NO_FUNC_NAMES
    #define CV_FUNCNAME( Name )
    #define cvFuncName ""
#else    
    #define CV_FUNCNAME( Name )  \
    static char cvFuncName[] = Name
#endif


/*
  CV_ERROR macro unconditionally raises error with passed code and message.
  After raising error, control will be transferred to the exit label.
*/
#define CV_ERROR( Code, Msg )                                       \
{                                                                   \
     cvError( (Code), cvFuncName, Msg, __FILE__, __LINE__ );        \
     __CV_EXIT__;                                                   \
}

/* Simplified form of CV_ERROR */
#define CV_ERROR_FROM_CODE( code )   \
    CV_ERROR( code, "" )

/*
 CV_CHECK macro checks error status after CV (or IPL)
 function call. If error detected, control will be transferred to the exit
 label.
*/
#define CV_CHECK()                                                  \
{                                                                   \
    if( cvGetErrStatus() < 0 )                                      \
        CV_ERROR( CV_StsBackTrace, "Inner function failed." );      \
}


/*
 CV_CALL macro calls CV (or IPL) function, checks error status and
 signals a error if the function failed. Useful in "parent node"
 error procesing mode
*/
#define CV_CALL( Func )                                             \
{                                                                   \
    Func;                                                           \
    CV_CHECK();                                                     \
}


/* Runtime assertion macro */
#define CV_ASSERT( Condition )                                          \
{                                                                       \
    if( !(Condition) )                                                  \
        CV_ERROR( CV_StsInternal, "Assertion: " #Condition " failed" ); \
}

#define __CV_BEGIN__       {
#define __CV_END__         goto exit; exit: ; }
#define __CV_EXIT__        goto exit

#endif /* _CXCORE_ERROR_H_ */

/* End of file. */
