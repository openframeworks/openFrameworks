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

#ifndef _LIBOIL_PROTOTYPE_H_
#define _LIBOIL_PROTOTYPE_H_

#include <liboil/liboilutils.h>
#include <liboil/liboilfunction.h>
#include <liboil/liboilparameter.h>

OIL_BEGIN_DECLS

#ifdef OIL_ENABLE_UNSTABLE_API

typedef struct _OilPrototype OilPrototype;
/**
 * OilPrototype:
 *
 * An opaque structure describing the C function prototype of
 * an @OilFunctionClass.
 */
struct _OilPrototype {
  int n_params;
  OilParameter *params;
  OilFunctionClass *klass;
};


OilPrototype *oil_prototype_from_string (const char *s);
char *oil_prototype_to_string (OilPrototype *proto);
char *oil_prototype_to_arg_string (OilPrototype *proto);
void oil_prototype_free (OilPrototype *proto);
void oil_prototype_append_param (OilPrototype *proto, OilParameter *param);

int oil_type_sizeof (OilType type);
const char * oil_type_name (OilType type);
const char * oil_arg_type_name (OilArgType type);

#endif

OIL_END_DECLS

#endif

