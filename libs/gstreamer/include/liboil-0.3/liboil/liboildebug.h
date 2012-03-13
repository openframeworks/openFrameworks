/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
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

#ifndef _LIBOIL_DEBUG_H_
#define _LIBOIL_DEBUG_H_

#include <stdarg.h>
#include <liboil/liboilutils.h>

OIL_BEGIN_DECLS

#ifdef OIL_ENABLE_UNSTABLE_API

/**
 * OilDebugPrintFunc:
 * @level: the debug level
 * @file: name of the file where the debug message occurs
 * @func: name of the function where the debug message occurs
 * @line: line in the file where the debug message occurs
 * @format: a printf format
 * @varargs: varargs for the printf format
 *
 * Typedef describing functions that can be registered using
 * oil_debug_set_print_function() so that it is called to
 * print debugging messages.
 */
typedef void (*OilDebugPrintFunc) (int level, const char *file,
    const char *func, int line, const char *format, va_list varargs);

/**
 * OilDebugLevel:
 *
 * Enumeration describing debug levels in Liboil.
 */
typedef enum {
  OIL_DEBUG_NONE = 0,
  OIL_DEBUG_ERROR,
  OIL_DEBUG_WARNING,
  OIL_DEBUG_INFO,
  OIL_DEBUG_DEBUG,
  OIL_DEBUG_LOG
} OilDebugLevel;

/**
 * OIL_ERROR:
 *
 * Macro to call OIL_DEBUG_PRINT() with a level of #OIL_DEBUG_ERROR.
 */
#define OIL_ERROR(...) OIL_DEBUG_PRINT(OIL_DEBUG_ERROR, __VA_ARGS__)
/**
 * OIL_WARNING:
 *
 * Macro to call OIL_DEBUG_PRINT() with a level of #OIL_DEBUG_WARNING.
 */
#define OIL_WARNING(...) OIL_DEBUG_PRINT(OIL_DEBUG_WARNING, __VA_ARGS__)
/**
 * OIL_INFO:
 *
 * Macro to call OIL_DEBUG_PRINT() with a level of #OIL_DEBUG_INFO.
 */
#define OIL_INFO(...) OIL_DEBUG_PRINT(OIL_DEBUG_INFO, __VA_ARGS__)
/**
 * OIL_DEBUG:
 *
 * Macro to call OIL_DEBUG_PRINT() with a level of #OIL_DEBUG_DEBUG.
 */
#define OIL_DEBUG(...) OIL_DEBUG_PRINT(OIL_DEBUG_DEBUG, __VA_ARGS__)
/**
 * OIL_LOG:
 *
 * Macro to call OIL_DEBUG_PRINT() with a level of #OIL_DEBUG_LOG.
 */
#define OIL_LOG(...) OIL_DEBUG_PRINT(OIL_DEBUG_LOG, __VA_ARGS__)

/**
 * OIL_FUNCTION:
 *
 * Internal macro that points to __PRETTY_FUNCTION__ or __func__
 * if the former is not available.
 */
#if defined (__GNUC__) || defined (__PRETTY_FUNCTION__)
#define OIL_FUNCTION __PRETTY_FUNCTION__
#elif defined(__func__)
#define OIL_FUNCTION __func__
#else
#define OIL_FUNCTION ""
#endif

/**
 * OIL_DEBUG_PRINT:
 * @level:
 * @...:
 *
 * Macro to call oil_debug_print() with the correct values for
 * the name of the source file, line of source file, and function.
 */
#define OIL_DEBUG_PRINT(level, ...) do { \
  oil_debug_print((level), __FILE__, OIL_FUNCTION, __LINE__, __VA_ARGS__); \
}while(0)

void oil_debug_set_print_function (OilDebugPrintFunc func);
int oil_debug_get_level (void);
void oil_debug_set_level (int level);

void _oil_debug_init (void);

void oil_debug_print (int level, const char *file, const char *func,
    int line, const char *format, ...);

#endif

OIL_END_DECLS

#endif

