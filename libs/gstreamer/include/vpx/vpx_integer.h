/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


#ifndef VPX_INTEGER_H
#define VPX_INTEGER_H

/* get ptrdiff_t, size_t, wchar_t, NULL */
#include <stddef.h>

#if defined(_MSC_VER) || defined(VPX_EMULATE_INTTYPES)
typedef signed char  int8_t;
typedef signed short int16_t;
typedef signed int   int32_t;

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#if defined(_MSC_VER)
typedef signed __int64   int64_t;
typedef unsigned __int64 uint64_t;
#define PRId64 "I64d"
#endif

#ifdef HAVE_ARMV6
typedef unsigned int int_fast16_t;
#else
typedef signed short int_fast16_t;
#endif
typedef signed char int_fast8_t;
typedef unsigned char uint_fast8_t;

#ifndef _UINTPTR_T_DEFINED
typedef unsigned int   uintptr_t;
#endif

#else

/* Most platforms have the C99 standard integer types. */

#if defined(__cplusplus) && !defined(__STDC_FORMAT_MACROS)
#define __STDC_FORMAT_MACROS
#endif
#include <stdint.h>
#include <inttypes.h>

#endif

#endif
