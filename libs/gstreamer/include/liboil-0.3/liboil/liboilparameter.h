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

#ifndef _LIBOIL_PARAMETER_H_
#define _LIBOIL_PARAMETER_H_

#include <liboil/liboiltypes.h>
#include <liboil/liboilutils.h>

OIL_BEGIN_DECLS

#ifdef OIL_ENABLE_UNSTABLE_API

/**
 * OilType:
 *
 * Enumeration containing the data types understood by Liboil.
 */
typedef enum {
  OIL_TYPE_UNKNOWN = 0,
  OIL_TYPE_INT,
  OIL_TYPE_s8,
  OIL_TYPE_u8,
  OIL_TYPE_s16,
  OIL_TYPE_u16,
  OIL_TYPE_s32,
  OIL_TYPE_u32,
  OIL_TYPE_s64,
  OIL_TYPE_u64,
  OIL_TYPE_f32,
  OIL_TYPE_f64,
  OIL_TYPE_s8p,
  OIL_TYPE_u8p,
  OIL_TYPE_s16p,
  OIL_TYPE_u16p,
  OIL_TYPE_s32p,
  OIL_TYPE_u32p,
  OIL_TYPE_s64p,
  OIL_TYPE_u64p,
  OIL_TYPE_f32p,
  OIL_TYPE_f64p,
} OilType;

/**
 * OilArgType:
 *
 * Enumeration containing the types of parameter types understood
 * by Liboil.
 */
typedef enum {
  OIL_ARG_UNKNOWN = 0,
  OIL_ARG_N,
  OIL_ARG_M,
  OIL_ARG_DEST1,
  OIL_ARG_DSTR1,
  OIL_ARG_DEST2,
  OIL_ARG_DSTR2,
  OIL_ARG_DEST3,
  OIL_ARG_DSTR3,
  OIL_ARG_SRC1,
  OIL_ARG_SSTR1,
  OIL_ARG_SRC2,
  OIL_ARG_SSTR2,
  OIL_ARG_SRC3,
  OIL_ARG_SSTR3,
  OIL_ARG_SRC4,
  OIL_ARG_SSTR4,
  OIL_ARG_SRC5,
  OIL_ARG_SSTR5,
  OIL_ARG_INPLACE1,
  OIL_ARG_ISTR1,
  OIL_ARG_INPLACE2,
  OIL_ARG_ISTR2,

  OIL_ARG_LAST
} OilArgType;

/**
 * OilParameter:
 * 
 * An opaque structure representing a single parameter in the
 * function prototype of an OilFunctionClass.
 */
struct _OilParameter {
  /*< private >*/
  char *type_name;
  char *parameter_name;

  int order;
  OilType type;

  int direction;
  int is_pointer;
  int is_stride;
  int index;
  int prestride_length;
  int prestride_var;
  int poststride_length;
  int poststride_var;

  OilArgType parameter_type;

  uint8_t *src_data;
  uint8_t *ref_data;
  uint8_t *test_data;
  unsigned long value;

  int pre_n;
  int post_n;
  int stride;
  int size;
  int guard;
  int test_header;
  int test_footer;
};

void *oil_param_get_source_data (OilParameter *param);
int oil_param_from_string (OilParameter *p, char *s);

#define oil_type_is_floating_point(type) \
  (((type) == OIL_TYPE_f64p) || ((type) == OIL_TYPE_f32p))

#endif

OIL_END_DECLS

#endif

