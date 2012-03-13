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

#ifndef _LIBOIL_PROFILE_H_
#define _LIBOIL_PROFILE_H_

#include <liboil/liboilutils.h>

OIL_BEGIN_DECLS

#ifdef OIL_ENABLE_UNSTABLE_API

/**
 * OIL_PROFILE_HIST_LENGTH
 *
 * Internal definition of the number of histogram entries in #OilProfile.
 */
#define OIL_PROFILE_HIST_LENGTH 10

typedef struct _OilProfile OilProfile;
/**
 * OilProfile:
 *
 * An opaque structure representing profiling information.
 */
struct _OilProfile {
  /*< private >*/
  unsigned long start;
  unsigned long stop;
  unsigned long min;
  unsigned long last;
  unsigned long total;
  int n;

  int hist_n;
  unsigned long hist_time[OIL_PROFILE_HIST_LENGTH];
  int hist_count[OIL_PROFILE_HIST_LENGTH];
};

unsigned long oil_profile_stamp(void);
void oil_profile_init(OilProfile *prof);
void oil_profile_stop_handle(OilProfile *prof);
void oil_profile_get_ave_std (OilProfile *prof, double *ave_p, double *std_p);

/**
 * oil_profile_start:
 * @x: a pointer to an OilProfile structure
 *
 * Starts a profiling run by obtaining a timestamp via oil_profile_stamp()
 * and writing it into @x.
 */
#define oil_profile_start(x) do{ \
	(x)->start = oil_profile_stamp(); \
}while(0)
/**
 * oil_profile_stop:
 * @x: a pointer to an OilProfile structure
 *
 * Stops a profiling run by obtaining a timestamp via oil_profile_stamp()
 * and writing it into @x.  It then calls oil_profile_stop_handle() to
 * handle post-processing of the profiling run.
 */
#define oil_profile_stop(x) do{ \
	(x)->stop = oil_profile_stamp(); \
        oil_profile_stop_handle(x); \
}while(0)

#endif

OIL_END_DECLS


#endif

