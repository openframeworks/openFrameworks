/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2007 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LIBOIL_UTILS_H_
#define _LIBOIL_UTILS_H_

#include <liboil/liboil-stdint.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned int oil_bool;

#ifdef OIL_ENABLE_UNSTABLE_API

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define DIVIDE_ROUND_UP(a,b) (((a) + (b) - 1)/(b))
#ifndef MIN
#define MIN(a,b) ((a)<(b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a)>(b) ? (a) : (b))
#endif
#ifndef CLAMP
#define CLAMP(x,a,b) ((x)<(a) ? (a) : ((x)>(b) ? (b) : (x)))
#endif
#define ROUND_UP_SHIFT(x,y) (((x) + (1<<(y)) - 1)>>(y))
#define ROUND_UP_POW2(x,y) (((x) + (1<<(y)) - 1)&((~0)<<(y)))
#define ROUND_UP_2(x) ROUND_UP_POW2(x,1)
#define ROUND_UP_4(x) ROUND_UP_POW2(x,2)
#define ROUND_UP_8(x) ROUND_UP_POW2(x,3)
#define ROUND_SHIFT(x,y) (((x) + (1<<((y)-1)))>>(y))

#endif

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#define OIL_GNUC_PREREQ(maj, min) \
  ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
#define OIL_GNUC_PREREQ(maj, min) 0
#endif
  
#if OIL_GNUC_PREREQ(3,3) && defined(__ELF__)
#define OIL_INTERNAL __attribute__ ((visibility ("internal")))
#else
#define OIL_INTERNAL
#endif

#ifdef __cplusplus
#define OIL_BEGIN_DECLS extern "C" {
#define OIL_END_DECLS }
#else
#define OIL_BEGIN_DECLS
#define OIL_END_DECLS
#endif

#ifdef _MSC_VER
#ifdef OIL_EXPORTS
#define OIL_EXPORT __declspec(dllexport) extern
#else
#define OIL_EXPORT __declspec(dllimport) extern
#endif
#else /* not _MSC_VER */
#define OIL_EXPORT extern
#endif

OIL_BEGIN_DECLS

#ifdef OIL_ENABLE_UNSTABLE_API

int get_file_int (const char *file, int *value);
char * get_file (const char *file);
char * get_cpuinfo_line (char *cpuinfo, const char *tag);
char * _strndup (const char *s, int n);
char ** strsplit (char *s);
char * get_tag_value (char *s, const char *tag);

#endif

OIL_END_DECLS

#endif

