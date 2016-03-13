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

#ifndef __OPENCV_CORE_PRIVATE_HPP__
#define __OPENCV_CORE_PRIVATE_HPP__

#ifndef __OPENCV_BUILD
#  error this is a private header which should not be used from outside of the OpenCV library
#endif

#include "opencv2/core.hpp"
#include "cvconfig.h"

#ifdef HAVE_EIGEN
#  if defined __GNUC__ && defined __APPLE__
#    pragma GCC diagnostic ignored "-Wshadow"
#  endif
#  include <Eigen/Core>
#  include "opencv2/core/eigen.hpp"
#endif

#ifdef HAVE_TBB
#  include "tbb/tbb_stddef.h"
#  if TBB_VERSION_MAJOR*100 + TBB_VERSION_MINOR >= 202
#    include "tbb/tbb.h"
#    include "tbb/task.h"
#    undef min
#    undef max
#  else
#    undef HAVE_TBB
#  endif
#endif

//! @cond IGNORED

namespace cv
{
#ifdef HAVE_TBB

    typedef tbb::blocked_range<int> BlockedRange;

    template<typename Body> static inline
    void parallel_for( const BlockedRange& range, const Body& body )
    {
        tbb::parallel_for(range, body);
    }

    typedef tbb::split Split;

    template<typename Body> static inline
    void parallel_reduce( const BlockedRange& range, Body& body )
    {
        tbb::parallel_reduce(range, body);
    }

    typedef tbb::concurrent_vector<Rect> ConcurrentRectVector;
#else
    class BlockedRange
    {
    public:
        BlockedRange() : _begin(0), _end(0), _grainsize(0) {}
        BlockedRange(int b, int e, int g=1) : _begin(b), _end(e), _grainsize(g) {}
        int begin() const { return _begin; }
        int end() const { return _end; }
        int grainsize() const { return _grainsize; }

    protected:
        int _begin, _end, _grainsize;
    };

    template<typename Body> static inline
    void parallel_for( const BlockedRange& range, const Body& body )
    {
        body(range);
    }
    typedef std::vector<Rect> ConcurrentRectVector;

    class Split {};

    template<typename Body> static inline
    void parallel_reduce( const BlockedRange& range, Body& body )
    {
        body(range);
    }
#endif

    // Returns a static string if there is a parallel framework,
    // NULL otherwise.
    CV_EXPORTS const char* currentParallelFramework();
} //namespace cv

/****************************************************************************************\
*                                  Common declarations                                   *
\****************************************************************************************/

/* the alignment of all the allocated buffers */
#define  CV_MALLOC_ALIGN    16

/* IEEE754 constants and macros */
#define  CV_TOGGLE_FLT(x) ((x)^((int)(x) < 0 ? 0x7fffffff : 0))
#define  CV_TOGGLE_DBL(x) ((x)^((int64)(x) < 0 ? CV_BIG_INT(0x7fffffffffffffff) : 0))

static inline void* cvAlignPtr( const void* ptr, int align = 32 )
{
    CV_DbgAssert ( (align & (align-1)) == 0 );
    return (void*)( ((size_t)ptr + align - 1) & ~(size_t)(align-1) );
}

static inline int cvAlign( int size, int align )
{
    CV_DbgAssert( (align & (align-1)) == 0 && size < INT_MAX );
    return (size + align - 1) & -align;
}

#ifdef IPL_DEPTH_8U
static inline cv::Size cvGetMatSize( const CvMat* mat )
{
    return cv::Size(mat->cols, mat->rows);
}
#endif

namespace cv
{
CV_EXPORTS void scalarToRawData(const cv::Scalar& s, void* buf, int type, int unroll_to = 0);
}

// property implementation macros

#define CV_IMPL_PROPERTY_RO(type, name, member) \
    inline type get##name() const { return member; }

#define CV_HELP_IMPL_PROPERTY(r_type, w_type, name, member) \
    CV_IMPL_PROPERTY_RO(r_type, name, member) \
    inline void set##name(w_type val) { member = val; }

#define CV_HELP_WRAP_PROPERTY(r_type, w_type, name, internal_name, internal_obj) \
    r_type get##name() const { return internal_obj.get##internal_name(); } \
    void set##name(w_type val) { internal_obj.set##internal_name(val); }

#define CV_IMPL_PROPERTY(type, name, member) CV_HELP_IMPL_PROPERTY(type, type, name, member)
#define CV_IMPL_PROPERTY_S(type, name, member) CV_HELP_IMPL_PROPERTY(type, const type &, name, member)

#define CV_WRAP_PROPERTY(type, name, internal_name, internal_obj)  CV_HELP_WRAP_PROPERTY(type, type, name, internal_name, internal_obj)
#define CV_WRAP_PROPERTY_S(type, name, internal_name, internal_obj) CV_HELP_WRAP_PROPERTY(type, const type &, name, internal_name, internal_obj)

#define CV_WRAP_SAME_PROPERTY(type, name, internal_obj) CV_WRAP_PROPERTY(type, name, name, internal_obj)
#define CV_WRAP_SAME_PROPERTY_S(type, name, internal_obj) CV_WRAP_PROPERTY_S(type, name, name, internal_obj)

/****************************************************************************************\
*                     Structures and macros for integration with IPP                     *
\****************************************************************************************/

#ifdef HAVE_IPP
#include "ipp.h"

#ifndef IPP_VERSION_UPDATE // prior to 7.1
#define IPP_VERSION_UPDATE 0
#endif

#define IPP_VERSION_X100 (IPP_VERSION_MAJOR * 100 + IPP_VERSION_MINOR*10 + IPP_VERSION_UPDATE)

// General define for ipp function disabling
#define IPP_DISABLE_BLOCK 0

#ifdef CV_MALLOC_ALIGN
#undef CV_MALLOC_ALIGN
#endif
#define CV_MALLOC_ALIGN 32 // required for AVX optimization

#define setIppErrorStatus() cv::ipp::setIppStatus(-1, CV_Func, __FILE__, __LINE__)

static inline IppiSize ippiSize(int width, int height)
{
    IppiSize size = { width, height };
    return size;
}

static inline IppiSize ippiSize(const cv::Size & _size)
{
    IppiSize size = { _size.width, _size.height };
    return size;
}

static inline IppiBorderType ippiGetBorderType(int borderTypeNI)
{
    return borderTypeNI == cv::BORDER_CONSTANT ? ippBorderConst :
        borderTypeNI == cv::BORDER_WRAP ? ippBorderWrap :
        borderTypeNI == cv::BORDER_REPLICATE ? ippBorderRepl :
        borderTypeNI == cv::BORDER_REFLECT_101 ? ippBorderMirror :
        borderTypeNI == cv::BORDER_REFLECT ? ippBorderMirrorR : (IppiBorderType)-1;
}

static inline IppDataType ippiGetDataType(int depth)
{
    return depth == CV_8U ? ipp8u :
        depth == CV_8S ? ipp8s :
        depth == CV_16U ? ipp16u :
        depth == CV_16S ? ipp16s :
        depth == CV_32S ? ipp32s :
        depth == CV_32F ? ipp32f :
        depth == CV_64F ? ipp64f : (IppDataType)-1;
}

// IPP temporary buffer hepler
template<typename T>
class IppAutoBuffer
{
public:
    IppAutoBuffer() { m_pBuffer = NULL; }
    IppAutoBuffer(int size) { Alloc(size); }
    ~IppAutoBuffer() { Release(); }
    T* Alloc(int size) { m_pBuffer = (T*)ippMalloc(size); return m_pBuffer; }
    void Release() { if(m_pBuffer) ippFree(m_pBuffer); }
    inline operator T* () { return (T*)m_pBuffer;}
    inline operator const T* () const { return (const T*)m_pBuffer;}
private:
    // Disable copy operations
    IppAutoBuffer(IppAutoBuffer &) {};
    IppAutoBuffer& operator =(const IppAutoBuffer &) {return *this;};

    T* m_pBuffer;
};

#else
#define IPP_VERSION_X100 0
#endif

// There shoud be no API difference in OpenCV between ICV and IPP since 9.0
#if (defined HAVE_IPP_ICV_ONLY) && IPP_VERSION_X100 >= 900
#undef HAVE_IPP_ICV_ONLY
#endif

#ifdef HAVE_IPP_ICV_ONLY
#define HAVE_ICV 1
#else
#define HAVE_ICV 0
#endif

#if defined HAVE_IPP
#if IPP_VERSION_X100 >= 900
#define IPP_INITIALIZER(FEAT)                           \
{                                                       \
    if(FEAT)                                            \
        ippSetCpuFeatures(FEAT);                        \
    else                                                \
        ippInit();                                      \
}
#elif IPP_VERSION_X100 >= 800
#define IPP_INITIALIZER(FEAT)                           \
{                                                       \
    ippInit();                                          \
}
#else
#define IPP_INITIALIZER(FEAT)                           \
{                                                       \
    ippStaticInit();                                    \
}
#endif

#ifdef CVAPI_EXPORTS
#define IPP_INITIALIZER_AUTO                            \
struct __IppInitializer__                               \
{                                                       \
    __IppInitializer__()                                \
    {IPP_INITIALIZER(cv::ipp::getIppFeatures())}        \
};                                                      \
static struct __IppInitializer__ __ipp_initializer__;
#else
#define IPP_INITIALIZER_AUTO
#endif
#else
#define IPP_INITIALIZER
#define IPP_INITIALIZER_AUTO
#endif

#define CV_IPP_CHECK_COND (cv::ipp::useIPP())
#define CV_IPP_CHECK() if(CV_IPP_CHECK_COND)

#ifdef HAVE_IPP

#ifdef CV_IPP_RUN_VERBOSE
#define CV_IPP_RUN_(condition, func, ...)                                   \
    {                                                                       \
        if (cv::ipp::useIPP() && (condition) && func)                       \
        {                                                                   \
            printf("%s: IPP implementation is running\n", CV_Func);         \
            fflush(stdout);                                                 \
            CV_IMPL_ADD(CV_IMPL_IPP);                                       \
            return __VA_ARGS__;                                             \
        }                                                                   \
        else                                                                \
        {                                                                   \
            printf("%s: Plain implementation is running\n", CV_Func);       \
            fflush(stdout);                                                 \
        }                                                                   \
    }
#elif defined CV_IPP_RUN_ASSERT
#define CV_IPP_RUN_(condition, func, ...)                                   \
    {                                                                       \
        if (cv::ipp::useIPP() && (condition))                               \
        {                                                                   \
            if(func)                                                        \
            {                                                               \
                CV_IMPL_ADD(CV_IMPL_IPP);                                   \
            }                                                               \
            else                                                            \
            {                                                               \
                setIppErrorStatus();                                        \
                CV_Error(cv::Error::StsAssert, #func);                      \
            }                                                               \
            return __VA_ARGS__;                                             \
        }                                                                   \
    }
#else
#define CV_IPP_RUN_(condition, func, ...)                                   \
    if (cv::ipp::useIPP() && (condition) && func)                           \
    {                                                                       \
        CV_IMPL_ADD(CV_IMPL_IPP);                                           \
        return __VA_ARGS__;                                                 \
    }
#endif

#else
#define CV_IPP_RUN_(condition, func, ...)
#endif

#define CV_IPP_RUN(condition, func, ...) CV_IPP_RUN_(condition, func, __VA_ARGS__)


#ifndef IPPI_CALL
#  define IPPI_CALL(func) CV_Assert((func) >= 0)
#endif

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

    CV_BADARG_ERR               = -49,  //ipp comp
    CV_NOTDEFINED_ERR           = -48,  //ipp comp

    CV_BADCHANNELS_ERR          = -47,  //ipp comp
    CV_BADRANGE_ERR             = -44,  //ipp comp
    CV_BADSTEP_ERR              = -29,  //ipp comp

    CV_BADFLAG_ERR              =  -12,
    CV_DIV_BY_ZERO_ERR          =  -11, //ipp comp
    CV_BADCOEF_ERR              =  -10,

    CV_BADFACTOR_ERR            =  -7,
    CV_BADPOINT_ERR             =  -6,
    CV_BADSCALE_ERR             =  -4,
    CV_OUTOFMEM_ERR             =  -3,
    CV_NULLPTR_ERR              =  -2,
    CV_BADSIZE_ERR              =  -1,
    CV_NO_ERR                   =   0,
    CV_OK                       =   CV_NO_ERR
}
CvStatus;

#ifdef HAVE_TEGRA_OPTIMIZATION
namespace tegra {

CV_EXPORTS bool useTegra();
CV_EXPORTS void setUseTegra(bool flag);

}
#endif

//! @endcond

#endif // __OPENCV_CORE_PRIVATE_HPP__
