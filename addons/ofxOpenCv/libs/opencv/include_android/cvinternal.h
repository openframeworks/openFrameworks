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

/* The header is for internal use and it is likely to change.
   It contains some macro definitions that are used in cxcore, cv, cvaux
   and, probably, other libraries. If you need some of this functionality,
   the safe way is to copy it into your code and rename the macros.
*/
#ifndef __OPENCV_INTERNAL_H__
#define __OPENCV_INTERNAL_H__

#include <vector>

#ifdef HAVE_CONFIG_H
    #include <cvconfig.h>
#endif

#if defined WIN32 || defined _WIN32
#  ifndef WIN32
#    define WIN32
#  endif
#  ifndef _WIN32
#    define _WIN32
#  endif
#endif

#if defined WIN32 || defined WINCE
#ifndef _WIN32_WINNT         // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
#define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#include <windows.h>
#undef small
#undef min
#undef max
#else
#include <pthread.h>
#include <sys/mman.h>
#endif

#ifdef __BORLANDC__
#ifndef WIN32
    #define     WIN32
#endif
#ifndef _WIN32
    #define     _WIN32
#endif
    #define     CV_DLL
    #undef      _CV_ALWAYS_PROFILE_
    #define     _CV_ALWAYS_NO_PROFILE_
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#define __BEGIN__ __CV_BEGIN__
#define __END__ __CV_END__
#define EXIT __CV_EXIT__

#ifdef HAVE_IPP
#include "ipp.h"

CV_INLINE IppiSize ippiSize(int width, int height)
{
    IppiSize size = { width, height };
    return size;
}
#endif

#if defined __SSE2__ || _MSC_VER >= 1300
#include "emmintrin.h"
#define CV_SSE 1
#define CV_SSE2 1
#if defined __SSE3__ || _MSC_VER >= 1400
#include "pmmintrin.h"
#define CV_SSE3 1
#endif
#else
#define CV_SSE 0
#define CV_SSE2 0
#define CV_SSE3 0
#endif

#ifndef IPPI_CALL
#define IPPI_CALL(func) CV_Assert((func) >= 0)
#endif

#ifdef HAVE_TBB
    #include "tbb/tbb_stddef.h"
    #if TBB_VERSION_MAJOR*100 + TBB_VERSION_MINOR >= 202
        #include "tbb/tbb.h"
        #undef min
        #undef max
    #else
        #undef HAVE_TBB
    #endif
#endif

#ifdef HAVE_TBB
    namespace cv
    {
        typedef tbb::blocked_range<int> BlockedRange;
        
        template<typename Body> static inline
        void parallel_for( const BlockedRange& range, const Body& body )
        {
            tbb::parallel_for(range, body);
        }
        
        template<typename Iterator, typename Body> static inline
        void parallel_do( Iterator first, Iterator last, const Body& body )
        {
            tbb::parallel_do(first, last, body);
        }
        
        typedef tbb::split Split;
        
        template<typename Body> static inline
        void parallel_reduce( const BlockedRange& range, Body& body )
        {
            tbb::parallel_reduce(range, body);
        }
        
        typedef tbb::concurrent_vector<Rect> ConcurrentRectVector;
    }
#else
    namespace cv
    {
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
        
        template<typename Iterator, typename Body> static inline
        void parallel_do( Iterator first, Iterator last, const Body& body )
        {
            for( ; first != last; ++first )
                body(*first);
        }
        
        class Split {};
        
        template<typename Body> static inline
        void parallel_reduce( const BlockedRange& range, Body& body )
        {
            body(range);
        }
        
        typedef std::vector<Rect> ConcurrentRectVector;
    }
#endif

#endif
