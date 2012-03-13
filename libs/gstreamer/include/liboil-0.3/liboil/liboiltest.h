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

#ifndef _LIBOIL_TEST_H_
#define _LIBOIL_TEST_H_

#include <liboil/liboil.h>
#include <liboil/liboilutils.h>
#include <liboil/liboilprototype.h>
#include <liboil/liboilprofile.h>

OIL_BEGIN_DECLS

#ifdef OIL_ENABLE_UNSTABLE_API

/**
 * OilTest:
 * 
 * An opaque structure describing how to test an OilFunctionImpl
 * for an OilFunctionClass.
 */
struct _OilTest {
  /*< private >*/
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  OilPrototype *proto;
  OilParameter params[OIL_ARG_LAST];
  OilProfile prof;

  int iterations;
  int n;
  int m;
  
  int inited;
  int tested_ref;

  double sum_abs_diff;
  int n_points;

  double profile_ave;
  double profile_std;

  double tolerance;
};

/**
 * OilTestFunction:
 * @test: the @OilTest structure
 *
 * Typedef for functions that initialize special values
 * in source arrays for a particular function class.
 */
//typedef void (*OilTestFunction) (OilTest *test);

/**
 * OIL_TEST_HEADER:
 *
 * Default number of bytes that are prepended to the array test area.
 */
#define OIL_TEST_HEADER 256
/**
 * OIL_TEST_FOOTER:
 *
 * Default number of bytes that are appended to the array test area.
 */
#define OIL_TEST_FOOTER 256

OilTest *oil_test_new (OilFunctionClass *klass);
void oil_test_free (OilTest *test);

void oil_test_set_iterations (OilTest *test, int iterations);

void oil_test_check_ref (OilTest *test);
int oil_test_check_impl (OilTest *test, OilFunctionImpl *impl);

void oil_test_cleanup (OilTest *test);
void oil_test_init (OilTest *test);

void oil_test_set_test_header (OilTest *test, OilParameter *p, int test_header);
void oil_test_set_test_footer (OilTest *test, OilParameter *p, int test_footer);

void _oil_test_marshal_function (void *func, unsigned long *args, int n_args,
    unsigned int pointer_mask, OilProfile *prof);

void *oil_test_get_source_data (OilTest *test, OilArgType arg_type);
int oil_test_get_arg_pre_n (OilTest *test, OilArgType arg_type);
int oil_test_get_arg_post_n (OilTest *test, OilArgType arg_type);
int oil_test_get_arg_stride (OilTest *test, OilArgType arg_type);
int oil_test_get_value (OilTest *test, OilArgType arg_type);

#endif

OIL_END_DECLS

#endif

